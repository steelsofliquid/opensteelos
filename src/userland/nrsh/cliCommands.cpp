#include <userland/nrsh/cliCommands.h>

using namespace osos;
using namespace osos::drivers;
using namespace osos::kernel;

extern volatile uint32_t tickCount;

void shutdown();

//static Speaker speakerNrshComs;
//static RecommendedStandard232Driver serialNrshComs;





void osos::cmdCpudata(uint32_t argc, char** argv)
{
    bool helpFlag;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
    }

    if (helpFlag) printf("%Rcpuinfo     %R: Provides CPU info, such as brand and vendor.\nExample usage: cpuinfo\n", 0x0E, 0x0F);
    else printf("Vendor: %s\nCPU: %s\n", cpuven(), cpubrd());
}

void osos::cmdUptime(uint32_t argc, char** argv)
{
    bool helpFlag;
    
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
    }

    if (helpFlag) printf("%Ruptime      %R: Provides the system uptime in ticks.\nExample usage: uptime\n", 0x0E, 0x0F);
    else printf("tick speed: 100 Hz\nuptime in ticks: %d\n", tickCount);
}

void osos::cmdShutdown(uint32_t argc, char** argv)
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

void osos::cmdPacinae(uint32_t argc, char** argv)
{
    bool helpFlag;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
    }

    if (helpFlag) printf("%Rpacinae     %R: Placeholder command for eventual OpenSteel/OS Package Manager.\nNot of use right now.\n", 0x0E, 0x0F);
    else
    {
        printf("OpenSteel/OS Package Manager (Pacinae)\nThe command %Rpacinae%R will be utilised by the, as of this build, unimplemented\npackage manager that will be added at a later date.\n", 0x0E, 0x0F);
    }
}

void osos::cmdSetup(uint32_t argc, char** argv)
{
    bool helpFlag;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
    }
    if (helpFlag) printf("%Rsetup       %R: Placeholder for the eventual installer for OpenSteel/OS.\nNot of use right now.\n", 0x0E, 0x0F);
    else
    {
        printf("OpenSteel/OS does not yet have an installer. While implementing it is planned,\na lot of work still needs to be done before it can be added.\n");
    }
}


void osos::cmdEcho(uint32_t argc, char** argv)
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
        printf("%Recho        %R: Allows you to print text onto the screen.\nExample usage: echo Hello World!\nFlags: -rs: Send to serial\n", 0x0E, 0x0F);
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

void osos::cmdCls(uint32_t argc, char** argv)
{
    bool helpFlag;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
    }

    if (helpFlag) printf("%Rcls        %R: Clears all contents from the screen.\nExample usage: cls\n", 0x0E, 0x0F);
    else printf("\a");
}

/*void osos::cmdClock(uint32_t argc, char** argv)
{}

void osos::cmdBeep(uint32_t argc, char** argv)
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


void osos::cmdVer(uint32_t argc, char** argv)
{
    bool helpFlag = false;
    bool dateFlag = false;
    bool buildFlag = false;
    bool copyrightFlag = false;
    bool pNameFlag = false;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag      = true;
        else if (strcmp(argv[i], "-f") == 0)
        {
            buildFlag     = true;
            dateFlag      = true;
            copyrightFlag = true;
            pNameFlag     = true;
        }
        else
        {
            if (strcmp(argv[i], "-b") == 0) buildFlag     = true;
            if (strcmp(argv[i], "-d") == 0) dateFlag      = true;
            if (strcmp(argv[i], "-c") == 0) copyrightFlag = true;
            if (strcmp(argv[i], "-n") == 0) pNameFlag     = true;
        }
    }

    if (helpFlag) printf("%Rversion\nver         %R: Provides the system version number.\nExample usage: ver\nFlags:\n -b: Display build number + codename.    -d: Display build date.                 -c: Display copyright info.             -n: Display formal product naming.      -f: Invoke all flags.\n", 0x0E, 0x0F);
    else 
    {
        printf(" OpenSteel/OS ");
        if(pNameFlag) printf("100\n");
        printf("version %d.%d", verMajor, verMinor);
        if (buildFlag) printf(".%d \"Denver\"", verBuild);
        if (dateFlag) printf(" (compiled on %d-%d-%d)", buildDay, buildMon, buildYr);
        if (copyrightFlag) printf("\nCopyright (C) 2021-2026 SteelsOfLiquid. This software comes with no warranty    whatsoever. Licenced under the General Public Licence 3.0.");
        printf("\n");
    }
}

void osos::cmdHelp(uint32_t argc, char** argv)
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
        printf(" about        cls          cpuinfo      credits      echo         help          ");
        printf(" licensing    uptime       shutdown     version      ver\n");
        printf("These additional commands are placeholders for later-date applications:         ");
        printf(" pacinae      setup");
        printf("\n");
    }
}

void osos::cmdLicensing(uint32_t argc, char** argv)
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

void osos::cmdCredits(uint32_t argc, char** argv)
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

void osos::cmdAbout(uint32_t argc, char** argv)
{
    bool helpFlag = false;
    bool mozillaFlag = false;
    bool inclVerFlag = false;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-?") == 0) helpFlag = true;
        if (strcmp(argv[i], "-moz") == 0) mozillaFlag = true;
        if (strcmp(argv[i], "-v") == 0) inclVerFlag = true;
    }

    if (helpFlag)
    {
        if (mozillaFlag) panic(0x1A); // intentional behaviour
        else printf("%Rabout       %R: General command for OpenSteel/OS information.\nExample usage: about\nFlags:\n -v: Include version info.\n\nSee also:\n%R credits      help         licensing\n", 0x0E, 0x0F, 0x0E, 0x0F);
    }
    else if (mozillaFlag)
    {
        // Poorly hidden easter egg. This is a reference to The Book of Mozilla, a parody (i think?) bible found in Netscape, Firefox and other
        // Mozilla browsers
        printf("%R                          The Book of OpenSteel - 1:29                          ", 0x1F);
        printf("    As the flightless bird begins to defy gravity, and wage war against the     ");
        printf("  decomposing, one of its intellectual allies follows in its creator\'s steps,   ");
        printf("  seeking unity, neutrality, and liberty. She wanted to incorporate some of its ");
        printf("   advancements, trim its flaws, and apply its concepts to her own project to   ");
        printf("    create a greater project, yet not displacing it. This project: A system.    \n");
        printf(" This system would be forged from steel, its creator providing her ideas and the");
        printf(" likes, in a simple, friendly and obtainable way, and protecting her system with");
        printf("   the horns of a gnu, which dictate those who copy the project\'s likeness.    %R\n", 0x0F);
    }
    else
    {
        printf("OpenSteel/OS is an x86 desktop operating system in development. It aims to one  ");
        printf("day stand alongside Microsoft Windows, macOS, and (GNU) Linux, with its own OS  ");
        printf("design (i.e. not a Unix-like), while not intentionally displacing any of those  ");
        printf("OSes (as to avoid antitrust laws that should be applied to Windows).            \n");
        if (inclVerFlag) printf("You are using version %d.%d.%d of OpenSteel/OS.\n", verMajor, verMinor, verBuild);
        printf("Goals of OpenSteel/OS include the supply of in-house OS components instead of   ");
        printf("already-existing components like the Linux kernel, GRUB or GNU coreutils, the   ");
        printf("continued support of legacy hardware (i.e. Intel i486 and later) and to provide ");
        printf("a free and open source OS that builds on the innovations of other OSes, like    ");
        printf("MINIX, (GNU) Linux, and ironically Microsoft Windows. Also to provide more      ");
        printf("readable and comprehensive source code.                                         ");
    }
}