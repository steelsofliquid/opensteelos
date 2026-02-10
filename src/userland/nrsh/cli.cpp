#include <userland/nrsh/cli.h>
#include <userland/nrsh/cliCommands.h>

using namespace osos;
using namespace osos::drivers;
using namespace osos::kernel;

extern volatile InterfaceModes currentInterface;

void FlushShell();

static Speaker speakCLI;





volatile bool isShellAvailable;
volatile bool isShellInitialised = false;
char inputBuffer[256];
volatile uint32_t inputLength = 0;



NathanRenaudShell::NathanRenaudShell()
{
    for (int i = 0; i < 32; i++) commandHistoryEntries[i] = nullptr;
    cmdHstCount = 0;
}

NathanRenaudShell::~NathanRenaudShell()
{
}

command_t commandsTable[] =
{
    {"cpuinfo", cmdCpudata},
    {"uptime", cmdUptime},
    {"shutdown", cmdShutdown},
    {"pacinae", cmdPacinae},
    {"setup", cmdSetup},
    {"cls", cmdCls},
    {"echo", cmdEcho},
    {"help", cmdHelp},
    {"ver", cmdVer},
    {"version", cmdVer},
    {"licensing", cmdLicensing},
    {"credits", cmdCredits},
    {"about", cmdAbout},
    {nullptr, nullptr}
};



void NathanRenaudShell::Initialise()
{
    currentInterface = TextShell;
    printf("%R> %R", 0x0B, 0x0F);
}



void NathanRenaudShell::RedrawInputLine()
{
    printf("\r%R> %R%s", 0x0B, 0x0F, inputBuffer);
    FlushShell();
}

void NathanRenaudShell::SaveToHistory(char* command)
{
    if ((cmdHstCount > 0) && (strcmp(commandHistoryEntries[0], command) == 0)) return;

    size_t found = 0;
    bool foundCommand = false;
    for (int i = 0; i < cmdHstCount; i++)
    {
        if (strcmp(command, commandHistoryEntries[i]) == 0)
        {
            found = i;
            foundCommand = true;
            break;
        }
    }

    if (foundCommand)
    {
        char* reusedCmd = commandHistoryEntries[found];
        for (size_t i = found; i > 0; i--) commandHistoryEntries[i] = commandHistoryEntries [i - 1];

        commandHistoryEntries[0] = reusedCmd;
        return;
    }

    if (cmdHstCount == 32)
    {
        osos::MemoryManager::activeMemoryManager->free(commandHistoryEntries[31]);
        cmdHstCount--;
    }

    for (size_t i = cmdHstCount; i > 0; i--) commandHistoryEntries[i] = commandHistoryEntries [i - 1];

    commandHistoryEntries[0] = strdup(command);
    cmdHstCount++;
}

char* NathanRenaudShell::GrabFromHistory(size_t index)
{
    return commandHistoryEntries[index];
}

void NathanRenaudShell::LoadHistoryEntry(size_t index)
{
    if (index < 0 || index >= cmdHstCount) return;

    char* entry = commandHistoryEntries[index];
    if (!entry || entry[0] == '\0') return;

    size_t len = strlen(entry);
    if (len > 255) len = 255;

    memcpy(inputBuffer, entry, len);
    inputBuffer[len] = '\0';
    inputLength = len;

    RedrawInputLine();
}


uint32_t NathanRenaudShell::Tokenise(char* input, char* argv[], uint32_t max)
{
    uint32_t count = 0;
    char* p = input;

    while (*p && count < max)
    {
        while (*p == ' ') p++;
        if (*p == 0) break;

        argv[count++] = p;

        while (*p && *p != ' ') p++;
        if (*p)
        {
            *p = 0;
            p++;
        }
    }

    return count;
}

void NathanRenaudShell::Trim(char* str)
{
    int32_t start = 0;
    while (str[start] == ' ') start++;

    if (start > 0)
    {
        int i = 0;
        while (str[start + i])
        {
            str[i] = str[start + i];
            i++;
        }

        str[i] = 0;
    }

    int32_t end = strlen(str) - 1;
    while (end >= 0 && (str[end] == ' '))
    {
        str[end] = '\0';
        end--;
    }
}

const char* NathanRenaudShell::GetToken(char* str)
{
    int32_t i = 0;
    while (str[i] != 0 && str[i] != ' ') i++;
    str[i] = 0;

    return str;
}

void NathanRenaudShell::ParseCommand(char* input)
{
    Trim(input);
    
    if(input[0] == 0) return;

    SaveToHistory(inputBuffer);

    char* args[32];
    uint32_t argsCount = Tokenise(input, args, 32);

    for (int i = 0; commandsTable[i].name != 0; i++)
    {
        if (strcmp(args[0], commandsTable[i].name) == 0)
        {
            commandsTable[i].func(argsCount, args);
            return;
        }
    }

    printf("\"%R%s%R\" isn't a known command or program. Stuck? Type %Rhelp%R.\n", 0x0E, args[0], 0x0F, 0x0E, 0x0F);
}



void NathanRenaudShell::HandleInput(keyEvent key)
{
    static int historyIndexIndicator = -1;
    static bool navKeyHeld = false;

    if ((key.code == KEY_UP) || (key.code == KEY_DOWN))
    {
        if (navKeyHeld == true)
        {
            return;
        }

        navKeyHeld = true;
    }
    else navKeyHeld = false;

    switch (key.code)
    {
        case KEY_CHAR:
            navKeyHeld = false;
            HandleChar(key.character);
            if (key.character == '\n') historyIndexIndicator = -1;
            break;
        
        case KEY_UP:
            if (cmdHstCount == 0) break;
            if (historyIndexIndicator == (cmdHstCount - 1)) break;
            else historyIndexIndicator++;
            LoadHistoryEntry(historyIndexIndicator);
            break;
        
        case KEY_DOWN:
            if (historyIndexIndicator <= -1) break;
            historyIndexIndicator--;

            if (historyIndexIndicator <= -1)
            {
                historyIndexIndicator = -1;
                printf("\r%R> %R", 0x0B, 0x0F);
                inputBuffer[0] = '\0';
                inputLength = 0;
                FlushShell();
            }
            else
            {
                LoadHistoryEntry(historyIndexIndicator);
            }
            break;

        case KEY_UP_REL:
        {
            navKeyHeld = false;
            break;
        }

        case KEY_DOWN_REL:
        {
            navKeyHeld = false;
            break;
        }

    }
}

void NathanRenaudShell::HandleChar(char c)
{
    if (c == '\b')
    {
        if (inputLength > 0)
        {
            inputLength--;
            inputBuffer[inputLength] = '\0';
            printf("\b");
            FlushShell();

            return;
        }
        else
        {
            //speakCLI.BeepVariable(741, 1);
        }
    }
    else if (c == '\n')
    {
        inputBuffer[inputLength] = '\0';

        ParseCommand(inputBuffer);
        inputLength = 0;
        printf("%R> %R", 0x0B, 0x0F);
        FlushShell();

        return;
    }
    else
    {
        if (inputLength < 255)
        {
            inputBuffer[inputLength++] = c;
            inputBuffer[inputLength] = '\0';
            FlushShell();
        }
    }
}
