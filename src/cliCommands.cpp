#include <cliCommands.h>

using namespace osos;
using namespace osos::common;

void printf(char* str, ...);
void shutdown();



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
    printf(" List of available commands ___________\n");
    printf(" General ______________________________\n");
    printf(" cls      | Clears the screen.\n");
    printf(" echo     | Prints text fed to it. Example: echo Hello!\n");
    printf(" shutdown | Shut down OpenSteel/OS.\n");
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
    printf("\n");
}