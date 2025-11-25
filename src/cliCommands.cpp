#include <cliCommands.h>

using namespace osos;
using namespace osos::common;

void printf(char* str, ...);



void osos::cmdEcho(const char* args)
{
    if (args) printf("%s", args);
    printf("\n");
}

void osos::cmdCls(const char* args)
{
    printf("\a");
}

void osos::cmdVer(const char* args)
{
    int8_t f = 'f';
    printf(" OpenSteel/OS %d.%d.%d \"Denver\"\n", verMajor, verMinor, verBuild);
}

void osos::cmdHelp(const char* args)
{
    printf(" List of available commands ___________\n");
    printf(" General ______________________________\n");
    printf(" cls    | Clears the screen.\n");
    printf(" echo   | Prints text fed to it. Example: echo Hello!\n\n");
    printf(" Special ______________________________\n");
    printf(" help   | Provides a help menu of available commands.\n");
    printf(" ver    | Provides version info about this copy of OpenSteel/OS.\n");
    printf("\n");
}