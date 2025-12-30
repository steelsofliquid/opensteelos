#include <userland/nrsh/cliCommands.h>

using namespace osos;
using namespace osos::drivers;

extern volatile uint32_t tickCount;

void shutdown();

//static Speaker speakerNrshComs;
//static RecommendedStandard232Driver serialNrshComs;





void osos::cmdCpudata(uint32_t argc, const char** argv)
{
    bool helpFlag;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
    }

    if (helpFlag) printf("%Rcpuinfo     %R: Provides CPU info, such as brand and vendor.\nExample usage: cpuinfo\n", 0x0E, 0x0F);
    else printf("Vendor: %s\nCPU: %s\n", cpuven(), cpubrd());
}

void osos::cmdUptime(uint32_t argc, const char** argv)
{
    bool helpFlag;
    
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
    }

    if (helpFlag) printf("%Ruptime      %R: Provides the system uptime in ticks.\nExample usage: uptime\n", 0x0E, 0x0F);
    else printf("tick speed: 100 Hz\nuptime in ticks: %d\n", tickCount);
}

void osos::cmdShutdown(uint32_t argc, const char** argv)
{
    bool helpFlag;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
    }

    if (helpFlag) printf("%Rshutdown    %R: Shuts down OpenSteel/OS.\nExample usage: shutdown\n", 
        0x0E, 0x0F);
    else 
    {
        //serialNrshComs.WriteString("Ending session; OpenSteel/OS is shutting down...");
        //speakerNrshComs.RestChime();
        shutdown();
    }
}

void osos::cmdPacinae(uint32_t argc, const char** argv)
{
    bool helpFlag;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
    }

    if (helpFlag) printf("%Rpacinae     %R: Placeholder command for eventual OpenSteel/OS Package Manager.\nNot of use right now.\n", 0x0E, 0x0F);
    else
    {
        printf("OpenSteel/OS Package Manager (Pacinae)\nThe command %Rpacinae%R will be utilised by the, as of this build, unimplemented\npackage manager that will be added at a later date.", 0x0E, 0x0F);
    }
}


void osos::cmdEcho(uint32_t argc, const char** argv)
{
    bool helpFlag = false;
    bool serialFlag = false;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-rs") == 0) serialFlag = true;
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
    }

    if (helpFlag) 
    {
        printf("%Recho        %R: Allows you to print text onto the screen.\nExample usage: echo\nFlags: -rs: Send to serial\n", 0x0E, 0x0F);
        return;
    }
    
    if (argc <= 1) 
    {
        printf("\n");
        return;
    }

    if (serialFlag)
    {
        printf("Serial Port Functionality is disabled for now.\n");
        for (int i = 2; i < argc; i++)
        {
            //serialNrshComs.WriteString(argv[i]);
            //if (i + 1 < argc) serialNrshComs.WriteString(" ");
        }

        //serialNrshComs.WriteString("\n");
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            printf("%s", argv[i]);
            if (i + 1 < argc) printf(" ");
        }

        printf("\n");
    }
}

void osos::cmdCls(uint32_t argc, const char** argv)
{
    bool helpFlag;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
    }

    if (helpFlag) printf("%Rcls        %R: Clears all contents from the screen.\nExample usage: cls\n", 0x0E, 0x0F);
    else printf("\a");
}

/*void osos::cmdClock(uint32_t argc, const char** argv)
{}

void osos::cmdBeep(uint32_t argc, const char** argv)
{
    bool helpFlag;
    uint32_t argFreq = 0;
    uint32_t argLen = 0;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
    }

    if (helpFlag) printf("%Rbeep        %R: Plays a tone from the PC speaker for a specified duration.");
}*/


void osos::cmdVer(uint32_t argc, const char** argv)
{
    bool helpFlag;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
    }

    if (helpFlag) printf("%Rversion\nver         %R: Provides the system version number.\nExample usage: ver\n", 0x0E, 0x0F);
    else printf(" OpenSteel/OS %d.%d.%d \"Denver\"\n", verMajor, verMinor, verBuild);
}

void osos::cmdHelp(uint32_t argc, const char** argv)
{
    bool helpFlag;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
    }

    if (helpFlag) printf("%Rhelp        %R: Provides a coherent list of commands.\nExample usage: help\n", 0x0E, 0x0F);
    else
    {
        printf("                       --- List of available commands ---                       ");
        printf("To get specific information on a command, type that command, followed by only -?");
        printf("For example, ver -? or uptime -?.\n");
        printf(" cls          cpuinfo      credits      echo         help         licensing     ");
        printf(" uptime       shutdown     version      ver");
        printf("\n");
    }
}

void osos::cmdLicensing(uint32_t argc, const char** argv)
{
    bool helpFlag;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
    }

    if (helpFlag) printf("%Rlicensing   %R: Provides a basic rundown of the GPL, OpenSteel/OS' license.\nExample usage: licensing\n", 0x0E, 0x0F);
    else
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
}

void osos::cmdCredits(uint32_t argc, const char** argv)
{
    bool helpFlag;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
    }

    if (helpFlag) printf("%Rcredits     %R: Provides credit to those who worked on or influenced the project.\nExample usage: credits\n", 0x0E, 0x0F);
    else
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
}