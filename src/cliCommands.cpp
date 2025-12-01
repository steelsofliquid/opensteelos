#include <cliCommands.h>

using namespace osos;
using namespace osos::common;
using namespace osos::libs;

extern volatile uint32_t tickCount;

void printf(char* str, ...);
void shutdown();





void osos::cmdCpudata(const char* args)
{
    printf("Vendor: %s\nCPU: %s\n", cpuven(), cpubrd());
}

void osos::cmdUptime(const char* args)
{
    if (args == "-?") printf("%Ruptime        %R: Provides the system uptime in ticks.\nExample usage: uptime", 0x0E, 0x0F);
    else printf("tick speed: 100 Hz\nuptime in ticks: %d\n", tickCount);
}

void osos::cmdShutdown(const char* args)
{
    shutdown();
}


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
    printf("List of available commands ___________\n");
    printf("(i) A more convenient way to get a description of a command is in development.  ");
    printf(" cls      | Clears the screen.\n");
    printf(" echo     | Prints text fed to it. Example: echo Hello!\n");
    printf(" shutdown | Shut down OpenSteel/OS.\n");
    printf(" Utility ______________________________\n");
    printf(" cpudata  | Get some information about the CPU.\n");
    printf(" uptime   | Get current system uptime.\n");
    printf(" Special ______________________________\n");
    printf(" credits  | Provides credits info for the OpenSteel/OS team.\n");
    printf(" help     | Provides a help menu of available commands.\n");
    printf(" licensing| Provides generalised license info about OpenSteel/OS.\n");
    printf(" ver      | Provides version info about this copy of OpenSteel/OS.\n");
    printf("\n");
}

void osos::cmdLicensing(const char* args)
{
    printf("OpenSteel/OS is licensed under the %RGNU General Public License 3.0%R.\n\n", 0x09, 0x0F);
    printf(" This copyleft license requires the complete availability of source code of\n");
    printf("works licensed under it.\n");
    printf("Colour Legend: %RPermitted %RLimited %RConditions\n", 0x0A, 0x0C, 0x0B);
    printf("%R - Commercial Use      %R - Liability           %R - License + Copyright Notice\n", 0x0A, 0x0C, 0x0B);
    printf("%R - Modification        %R - Warranty            %R - State changes\n", 0x0A, 0x0C, 0x0B);
    printf("%R - Distribution                               %R - Disclose source\n", 0x0A, 0x0B);
    printf("%R - Patent use                                 %R - Same license\n", 0x0A, 0x0B);
    printf("%R - Private use\n\n", 0x0A);
    printf("%R Source: GitHub. This is not legal advice.\n", 0x0F);
}

void osos::cmdCredits(const char* args)
{
    printf("The OpenSteel/OS Team Credits - This project wouldn\'t be possible without:\n");
    printf(" Director: %RNeisa Sapphira-Aera\n", 0x0D);
    printf("%R Developers: - Nathan Renaud\n\n", 0x0F);
    printf("The team behind OpenSteel/OS expresses their gratitude towards these projects,\n");
    printf("and resources, for their accessibility and usefulness as references at some time");
    printf("or another in this project\'s development, even if no longer used/never used:   ");
    printf(" - Write Your Own Operating System       - OSDev.org Wiki and forums            ");
    printf(" - osakaOS                               - Nanami/OS                            ");
    printf(" - Linux kernel and Linus Torvalds       - MINIX 3                              ");
    printf(" - Operating Systems: Three Easy Pieces  - Documentation from many companies. \n");
}