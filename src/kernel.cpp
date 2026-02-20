/*
  OpenSteel/OS kernel, written based off of the WYOOS tutorial.
  Some parts are actually code I wrote myself, and I'd expect that percent to expand in the later 2020's if I get into Oakland University
  since their computer science course touches up at least a little bit on operating systems

  Also for anyone wondering, this project was previously called NanamiOS (later stylized as Nanami/OS). The project's namesake was Nanami Madobe,
  the famous Windows 7 OS-tan, and that is why the project is not called Nanami/OS anymore - As OS-tans have somewhat socially unacceptable themes
  here in the Western world and so I don't want my OS project outright endorsing an OS-tan through its name. Imagine if I started the project in
  mid-2025...

  As for why I renamed it to OpenSteel/OS in particular...
  In 2021, I created SteelOS, a PowerPoint-based OS thing that didn't really do much. I adapted it into Steel OS in late 2022, and this project
  lasted a few years into mid-to-late 2024, when it was succeeded by Project Nova Carbonium and OpenSteelEnvy. "Open" conveys that it is open-source
  (GNU GPL, yo) and eventually I want it to resemble Steel OS in a strong way. That may never happen (sad >_<) but I wish it does happen, even if
  I have to vibe code to get it done (honestly my dad seems fine with AI, and his job involves AI systems and automotive engineering).

  But, this is essentially to be an open-source Steel OS. A la OpenSolaris, etc., etc. Why there's the / in there, it's because it's used with
  many historical OSes. And because OpenSteel/OS just looks more badass than a wimpy OpenSteel OS or OpenSteelOS. And because it's my project,
  my rules, even if it's mostly tutorial code I didn't write, damnit!
*/

// OpenSteel/OS headers
#include <common/types.h>
#include <common/sysHelpers.h>
#include <common/version.h>
#include <common/lib/libasm.h>
#include <common/lib/libcpu.h>
#include <common/lib/libio.h>
#include <common/lib/libmem.h>
#include <common/lib/libstr.h>
#include <drivers/drv/ata.h>
#include <drivers/snd/speaker.h>
#include <drivers/cmos.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/pit.h>
#include <drivers/rs232.h>
#include <drivers/rtcIrq8.h>
#include <drivers/vga.h>
#include <kernel/hwcom/driverModel.h>
#include <kernel/hwcom/driverManager.h>
#include <kernel/hwcom/interrupts.h>
#include <kernel/hwcom/pci.h>
#include <userland/nrsh/cli.h>
#include <kernel/mem/dmm.h>
#include <kernel/crashHandler.h>
#include <kernel/gdt.h>
#include <globalfuncs.h>
#include <kernel/multitasking.h>

// GCC headers


// namespaces
using namespace osos;
using namespace osos::drivers;
using namespace osos::drivers::drives;
using namespace osos::kernel;
using namespace osos::kernel::hwcom;
// using namespace osos::gui;

// external variables and whatnot
extern volatile uint32_t tickCount;
extern volatile keyEvent lastChar;
extern volatile KeystrokeMode keymode;
extern volatile bool isShellInitialised;
//extern volatile char inputBuffer[256];
//extern volatile uint32_t inputLength;





int32_t testInteger1 = 5;
int32_t testInteger2 = 15;
int32_t testInteger3 = 327;
int32_t testInteger4 = 7629;
int32_t testInteger5 = 0;

//uint8_t userAgentSafe = 'OpenSteelOS_0.22_Denver';
//uint8_t userAgent = 'OpenSteel/OS 0.22 \"Denver\"';

volatile InterfaceModes currentInterface;

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}

class MouseToConsole : public MouseEventHandler // This moves the sometimes very useless mouse cursor.
{
    int8_t x, y;

    public:
    MouseToConsole()
    {
        uint16_t* videoMemory = (uint16_t*)0xB8000;
        x = 40;
        y = 12;
        videoMemory[80*y+x] = (videoMemory[80*y+x] & 0x0F00) << 4
                            | (videoMemory[80*y+x] & 0xF000) >> 4
                            | (videoMemory[80*y+x] & 0x00FF);
    }

    void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* videoMemory = (uint16_t*)0xb8000;

        videoMemory[80*y+x] = (videoMemory[80*y+x] & 0x0F00) << 4
                            | (videoMemory[80*y+x] & 0xF000) >> 4
                            | (videoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x < 0) x = 0;
        if(x >= 80) x = 79;

        y += yoffset;
        if(y < 0) y = 0;
        if(y >= 25) y = 24;

        videoMemory[80*y+x] = (videoMemory[80*y+x] & 0x0F00) << 4
                            | (videoMemory[80*y+x] & 0xF000) >> 4
                            | (videoMemory[80*y+x] & 0x00FF);
    }

};

static const char* monthNames[12] =
{
    "January", "February", "March", "April",
    "May", "June", "July", "August",
    "September", "October", "November", "December"
};

static const char* monthAbrev[12] =
{
    "jan", "feb", "mar", "apr",
    "may", "jun", "jul", "aug",
    "sep", "oct", "nov", "dec"
};

void TestTask1(void* context)
{
    (void)context;
    while(true)
        printf("A");
}

void TestTask2(void* context)
{
    (void)context;
    while(true)
        printf("TT2B");
}

void TestTask3(void* context)
{
    (void)context;
    while(true)
    {
        printf("n3isa");
        sleep(3000);
        printf(" TCC");
        sleep(50);
    }
}

void NRShTaskSystem(void* context)
{
    (void)context;
    NathanRenaudShell nrsh;
    nrsh.Initialise();

    while(1)
    {
        if (lastChar.code != KEY_NONE)
        {
            keyEvent kbEvent;
            kbEvent.code = lastChar.code;
            kbEvent.character = lastChar.character;

            nrsh.HandleInput(kbEvent);
            lastChar.code = KEY_NONE;
        }
        asm volatile("hlt");
    }
}

struct ClockContext
{
    ClockBatteryDriver* rtcThing;
    RealTimeClockRegisters rtcTarget;
};

void ClockUpdatingTask(void* context)
{
    char rtcMinT[3], rtcSecT[3];
    ClockContext* contx = (ClockContext*)context;

    contx->rtcThing->PadRTCInteger(rtcSecT, contx->rtcTarget.second);
    contx->rtcThing->PadRTCInteger(rtcMinT, contx->rtcTarget.minute);

    sleep (100);
}

void shutdown()
{
    printf("OpenSteel/OS is shutting down...");
    DisableCursor();
    asm volatile ("cli");

    printf("%R\a", 0x09);
    printf("                                                                                ");
    printf("                                                                                ");
    printf("          .                   .                    .                      C     ");
    printf("                      .                 .                                       ");
    printf("      .                            .                   .                        ");
    printf("        .                                        .              .               ");
    printf("                  .           .                         .                       ");
    printf("                                                                                ");
    printf("                                                                                ");
    printf("                                                                                ");
    printf("                                                                                ");
    printf("                                                                                ");
    printf("                                                                                ");
    printf("                                                                                ");
    printf("                                                                                ");
    printf("                                                                                ");
    printf("    OpenSteel/OS has now halted and shut down. However, ACPI is unavailable.    ");
    printf("               As such, it is now safe to turn off your computer.               ");
    printf("                                                                                ");
    printf("                                                                           ___  ");
    printf("                                                                          /  /  ");
    printf("                                                                          \\__\\  ");
    printf("                                                                           \\  \\ ");
    printf("                                                                           /__/ ");

    while(1)
    {
        asm volatile ("cli; hlt");
    };
}



typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
  for(constructor* i = &start_ctors; i != &end_ctors; i++)
    (*i)();
}

// extern "C" void ExStringDemoProgramMain(); // register exec for testing

extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber)
{
    /*
        So, to give you the rundown on what this is, most of the kernel code is the boot process. PCI, interrupts, drivers and whatnot
        are mostly handled in their own worlds or don't have proper function otherwise as there isn't a shell. And you don't need too
        much power from a task manager or a memory manager just to type, which is literally all you can currently do with this OS.

        But, to put it simply, the boot process goes a little like this:

          1. GRUB does its thing (for now, because I would like to make my own bootloader, even if basic or a fork of GRUB I can pack with the OS)

          2. loader.s calls the constructors and then kernelMain (this huge ahh void function)

          3. OpenSteel/OS logo and version/copyright info is spat onto the screen

          4. GDT is invoked, the memory heap is spat and so is allocated memory once it is allocated

          5. Any tasks left unpinned are "registered"

          6. Interrupts invoked

          7. Drivers are initialized

          8. PCI devices detected, spat onto screen and initialized

          9. Drivers and interrupts activated proper

          10. System online.
    
        A shell would then be loaded and whatnot and then you'd use the system.
    */
  
    // Start of boot process v

    // reset video memory
    printf("%R\a", 0x0F);

    // bootsplash header (ASCII)
    printf("%R  ___                %R_ ___ __   _ _  __ __   _ _ ________________________ 0.22  ", 0x0B, 0x08);
    printf("%R /  /                           %R              __ _ _   _ ____ _ _______________ ", 0x0B, 0x08);
    printf("%R \\__\\     steelsofliquid        %R                          _ _   ___ _ _________ ", 0x0B, 0x08);
    printf("%R  \\  \\       OpenSteel/OS       %R                                    _  __ _____ ", 0x0B, 0x08);
    printf("%R  /__/                          %R                                           ____ ", 0x0B, 0x08);
    printf("                                                                                ");

    // sysagent header
    printf("%RSteelsOfLiquid OpenSteel/OS 100 - ver %d.%d.%d \"Denver\" Beta 2 [%d-%d-%d]\n", 0x0F, verMajor, verMinor, verBuild, buildDay, buildMon, buildYr);

    currentInterface = BootUI;
    GlobalDescriptorTable gdt;

    // Get memory heap, allocate ram, etc. v

    uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
    size_t heap = 10*1024*1024; // This should be 10 MiB
    MemoryManager MemoryManager(heap, (*memupper)*1024 - heap - 10*1024);

    printf("memory heap: %R0x%x%x%x%x%R, ",
        0x0E,
        ((heap >> 24) & 0xFF),
        ((heap >> 16) & 0xFF),
        ((heap >> 8 ) & 0xFF),
        ((heap      ) & 0xFF),
        0x0F
    );
    void* allocated = MemoryManager.malloc(1024);
  
    printf("%R0x%x%x%x%x %Rallocated by dmm.",
        0x0E,
        (((size_t)allocated >> 24) & 0xFF),
        (((size_t)allocated >> 16) & 0xFF),
        (((size_t)allocated >> 8 ) & 0xFF),
        (((size_t)allocated      ) & 0xFF),
        0x0F
    );
  
    printf("\n");

    // register tasks v

    TaskManager taskManager;
    Task task1(&gdt, TestTask1, nullptr);
    Task task2(&gdt, TestTask2, nullptr);
    Task task3(&gdt, TestTask3, nullptr);
    //taskManager.AddTask(&task1);
    //taskManager.AddTask(&task2);
    //taskManager.AddTask(&task3);

    // interrupts and drivers v

    InterruptManager interrupts(0x20, &gdt, &taskManager);

    const char* welcomeMessage;

    DriverManager drvManager;
    // drivers loading
    printf("initialising drivers...                                                    ");

    KeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    drvManager.AddDriver(&keyboard);

    ProgrammableIntervalTimer programmableIntervalTimer(&interrupts, &taskManager); drvManager.AddDriver(&programmableIntervalTimer);
    RecommendedStandard232Driver serialPort(&interrupts); drvManager.AddDriver(&serialPort);

    //MouseToConsole mousehandler;
    //MouseDriver mouse(&interrupts, &mousehandler);
    //drvManager.AddDriver(&mouse);

    // ^ mouse driver sucked. let's not use it, if possible.

    Speaker speaker; drvManager.AddDriver(&speaker);
    ClockBatteryDriver cmos; drvManager.AddDriver(&cmos);
    InterruptRequest8AssistantDriver irq8(&interrupts); drvManager.AddDriver(&irq8);
    char rtcSec[3], rtcMin[3];

    printf("%R[ok!]", 0x0A);
    printf("%Rfinding and selecting PCI devices and drivers...                           ", 0x0F);

    PCIController PCIController;
    PCIController.SelectDrivers(&drvManager, &interrupts);
    printf("%R[ok!]", 0x0A);

    // VideoGraphicsArray vga;

    printf("%Rstarting drivers and interrupts...                                         ", 0x0F);
    drvManager.ActivateAll(&interrupts);
    serialPort.WriteString("OpenSteel/OS version 0.22: COM1 connection has been made; the serial port driver has been started.\n");

    AdvancedTechnologyAttachment ata0l(0x1F0, true, 0x2E, &interrupts);  // IRQ 14
    serialPort.WriteString("ATA Pri. Lead: "); ata0l.IdentifyDrive();
    AdvancedTechnologyAttachment ata0f(0x1F0, false, 0x2E, &interrupts);
    serialPort.WriteString("ATA Pri. Follow: "); ata0f.IdentifyDrive();

    char testDriveString[] = "Testing hard drive stuff";
    char secondTestDriveStr[] = "OpenSteel/OS 0.22 \"Denver\" - Sample Test thing for HDD driver";
    //ata0l.Write28Bit(0, testDriveString, 25);
    //ata0l.Write28Bit(26, secondTestDriveStr, 62);

    //ata0l.Read28Bit(0, testDriveString, 25);
    //ata0l.Read28Bit(26, secondTestDriveStr, 62);

    AdvancedTechnologyAttachment ata1l(0x170, true, 0x2F, &interrupts);  // IRQ 15
    serialPort.WriteString("\nATA Sec. Lead: "); ata1l.IdentifyDrive();
    AdvancedTechnologyAttachment ata1f(0x170, false, 0x2F, &interrupts);
    serialPort.WriteString("ATA Sec. Follow: "); ata1f.IdentifyDrive();
    // the others are 0x1E8 and 0x168

    interrupts.Activate();
    RealTimeClockRegisters time = cmos.ReadRTC();
    cmos.PadRTCInteger(rtcSec, time.second);
    cmos.PadRTCInteger(rtcMin, time.minute);

    // Determine, based on the time of day, what to say during the startup message.
    // We can test this easily on an old Dell Dimension with a dead CMOS battery and Windows 2000,
    // improving productivity and reducing all-nighters of just, well, TESTING.
    // Oh, huh, I forgot that I've got 86Box installed.
    // Point is, morning (defined as after 4 and before 11 AM) = good morning, etc.
    if ((time.hour <= 10) && (time.hour >= 5)) welcomeMessage = "Good morning";
    else if ((time.hour <= 1) || (time.hour >= 22)) welcomeMessage = "Up late, eh? Anyways, greetings"; // maple syrup and poutine. oh, uh yeah the original message here produced a grammatical error. "Greetings. Up late, huh? Anyways, and welcome to OpenSteel/OS."
    else if ((time.hour <= 21) && (time.hour >= 18)) welcomeMessage = "Good evening";
    else welcomeMessage = "Greetings"; // ...and welcome to an LGR thing. (sorry, i had to. clint is a brilliant youtuber)

    printf("%R[ok!]", 0x0A);

    if (time.year < buildYr) printf("%RThe system date may not be set correctly or the CMOS battery has failed.   %R[wrn]", 0x0F, 0x0E);

    printf("%RStarting services...                                                       ", 0x0F);

    // As of 0.22.289, there nominally aren't any services yet. This stage is in a placeholder
    // state, as service infastructure is being put together. The following code is only a
    // placeholder that is not final. And you'll get errors trying to activate the code.
    //
    // As of 0.22.374, plans to implement a services-like thing via tasks is planned.
    // But still.

    // ServiceManager svcManager;
    // AudioService audioSvc(&speaker); svcManager.AddService(&audioSvc);
    // CommunicationService
    // svcManager.ActivateAll();

    printf("%R[ok!]", 0x0A);

    cpudet();
    // booting is at the home stretch ^v

    speaker.LifeChime();
    serialPort.WriteString("Welcome to OpenSteel/OS.\n");
    serialPort.WriteString("OpenSteel/OS 100 version 0.22 \"Denver\" [OpenSteel/OS RS232 COM+Serial Driver]\nYou are seeing this because a successful COM1 connection was made.\n\n");
    printf("\n%R%s, and welcome to OpenSteel/OS. It is %d:%s:%s, %d %s, %d.\nType help for help.\n", 0x0F, welcomeMessage,
    time.hour, rtcMin, rtcSec,
    time.day, monthNames[time.month - 1], time.year);
    // Denotes end of booting process  ^

    // the code below is supposed to try to determine if the OS is in real mode or not. spoiler: it's not.
    // it might be useful later, when neisaboot is developed, so we'll keep it in here for now, just unactivated
    /*
    uint32_t cr0;
    printf("Conducting test to determine if we\'re in realmode, will crash if so...");
    asm volatile("cli");
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    asm volatile("sti");*/

    // this code is for those who are using the VGA driver.
    /* vga.SetMode(320, 200, 8);
    for(int32_t y = 0; y < 200; y++)
        for(int32_t x = 0; x < 320; x++)
            vga.PutPixel(x, y, 0x00, 0x00, 0xA8);
    */

/* 
    if (interrupts.handlerExists(0x24)) printf("\nIRQ 4 Handler Exists."); else printf("\nIRQ 4 Handler doesn\'t exist.");
    if (interrupts.handlerExists(0x2E)) printf("\nIRQ 14 Handler Exists."); else printf("\nIRQ 14 Handler doesn\'t exist.");
    if (interrupts.handlerExists(0x21)) printf("\nIRQ 1 Handler Exists"); else printf("\nIRQ 1 handler doesn\'t exist.");
    if (interrupts.handlerExists(0x2C)) printf("\nIRQ 12 Handler exists."); else printf("\nIRQ 12 Handler doesn\'t exist.");
 */

    EnableCursor(13, 15);
    FlushShell();

    Task NRSh(&gdt, NRShTaskSystem, nullptr); taskManager.AddTask(&NRSh);

    ClockContext clockCtx;
    clockCtx.rtcThing = &cmos;
    clockCtx.rtcTarget = time;
    Task ClockUpdate(&gdt, ClockUpdatingTask, &clockCtx);
    while(1)
    {
        asm volatile ("hlt");
    }
  
}

// honestly half of this code is documentation or disabled code
