#include <userland/nrsh/cli.h>
#include <userland/nrsh/cliCommands.h>

using namespace osos;
using namespace osos::drivers;

extern volatile InterfaceModes currentInterface;

void FlushShell();

static Speaker speakCLI;





volatile bool isShellAvailable;
volatile bool isShellInitialised = false;
volatile char inputBuffer[256];
volatile uint32_t inputLength = 0;



NathanRenaudShell::NathanRenaudShell()
{
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
    const char* token = strtok(input, " ");
    const char* args[32];
    uint32_t argsCount = 0;

    while (token != nullptr && argsCount < 32)
    {
        args[argsCount++] = token;
        token = strtok(nullptr, " ");
    }

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



void NathanRenaudShell::HandleInput(char c)
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