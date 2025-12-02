#include <cli.h>
#include <cliCommands.h>

using namespace osos;
using namespace osos::common;
using namespace osos::libs;

extern volatile InterfaceModes currentInterface;

void printf(char* str, ...);





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
    {"cls", cmdCls},
    {"echo", cmdEcho},
    {"help", cmdHelp},
    {"ver", cmdVer},
    {"version", cmdVer},
    {"licensing", cmdLicensing},
    {"credits", cmdCredits},
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
    const char* args[8];
    uint32_t argsCount = 0;

    while (token != nullptr && argsCount < 8)
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

    printf(" <!> This is not a valid command. Type \"help\" for a coherent list of commands.\n");
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

            return;
        }
    }
    else if (c == '\n')
    {
        inputBuffer[inputLength] = '\0';

        ParseCommand((char*)inputBuffer);
        inputLength = 0;
        printf("%R> %R", 0x0B, 0x0F);

        return;
    }
    else
    {
        if (inputLength < 255)
        {
            inputBuffer[inputLength++] = c;
            inputBuffer[inputLength] = '\0';
        }
    }
}