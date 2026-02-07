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
volatile char inputBuffer[256];
volatile uint32_t inputLength = 0;



NathanRenaudShell::NathanRenaudShell()
{
    //commandHistoryEntries[32] = nullptr;
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

void NathanRenaudShell::SaveToHistory(char* command)
{
    if (!commandHistoryEntries) panic(0x03);
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

    //const char* cmd = GetToken(input);
    char* args[32];
    uint32_t argsCount = Tokenise(input, args, 32);

    SaveToHistory(args[0]);

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
    size_t historyIndexIndicator = -1;
    switch (key.code)
    {
        case KEY_CHAR:
            HandleChar(key.character);
            break;
        
        case KEY_UP:
            printf("\r> ");

            inputBuffer = ;
            GrabFromHistory(historyIndexIndicator++);
            break;
        
        case KEY_DOWN:
            printf("\r> ");
            GrabFromHistory(historyIndexIndicator--);
            break;

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

        ParseCommand((char*)inputBuffer);
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