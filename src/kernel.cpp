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
#include <drivers/drv/ata.h>
#include <drivers/snd/speaker.h>
#include <drivers/cmos.h>
// #include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/pit.h>
#include <drivers/rs232.h>
#include <drivers/vga.h>
#include <kernel/hwcom/driverModel.h>
#include <kernel/hwcom/driverManager.h>
#include <kernel/hwcom/interrupts.h>
#include <kernel/hwcom/pci.h>
#include <userland/lib/libcpu.h>
#include <userland/lib/libmem.h>
#include <userland/lib/libstr.h>
#include <userland/nrsh/cli.h>
#include <kernel/mem/dmm.h>
#include <kernel/gdt.h>
#include <globalfuncs.h>
#include <kernel/multitasking.h>

// GCC headers
#include <stdarg.h>

// namespaces
using namespace osos;
using namespace osos::common;
using namespace osos::drivers;
using namespace osos::drivers::drives;
using namespace osos::kernel;
using namespace osos::kernel::hwcom;
using namespace osos::libs;
// using namespace osos::gui;

// external variables and whatnot
extern volatile uint32_t tickCount;
extern volatile char lastChar;
extern volatile KeystrokeMode keymode;
extern volatile bool isShellInitialised;
extern volatile char inputBuffer[256];
extern volatile uint32_t inputLength;





int32_t testInteger1 = 5;
int32_t testInteger2 = 15;
int32_t testInteger3 = 327;
int32_t testInteger4 = 7629;
int32_t testInteger5 = 0;
uint16_t curX = 0;
uint16_t curY = 0;

//uint8_t userAgentSafe = 'OpenSteelOS_0.22_Denver';
//uint8_t userAgent = 'OpenSteel/OS 0.22 \"Denver\"';

volatile InterfaceModes currentInterface;

namespace osos
{
    namespace kernel
    {
        //TaskManager* taskManager = nullptr;
    }
}

void printf(char* str, ...) // the main screen output function.
{
    va_list params;
    va_start (params, str);

    static uint16_t* videoMemory = (uint16_t*)0xb8000;
    static uint8_t x = 0 , y = 0;
    static uint8_t currentColour = 0x0F;

    for(int i = 0; str[i] != '\0'; ++i)
    {

        switch(str[i])
        {

            /*
                Just a simple rundown on what each thing here does:

                 - \n means new line. That is the case for most languages.

                 - \b means backspace. I wrote this bit myself as I had to figure it out one morning in government class (what does an operating system have to do with politics T_T)

                 - \a means "clear the screen". There's something around that is messing with it and including a bullet point, though.

                 - Anything else on the keyboard placed in printf() will just input an ordinary IBM Code Page 437 character.
                 - Use \' or \" if you need to insert an apostrophe or quotation mark as you'll break it otherwise!
                 - Don't input anything not in the keyboard unless you want to break the graphics, I tried it and that's what happened!
            */

            case '\n':
            {
                y++; curY++;
                x = 0; curX = 0;
                break;
            }

            case '\b':
            {
                if(x == 0)
                {
                    if(y != 0)
                    {
                        y--; curY--;
                        int backX = 79;
                        while (backX >= 0 && (videoMemory[80*y+backX] & 0x00FF) == ' ') backX--;
                        if (backX < 0)
                        {
                            x = 0;
                            curX = 0;
                        }
                        else
                        {
                            x = backX;
                            curX = backX;
                        }

                        videoMemory[80*y+x] = (currentColour << 8) | ' ';
                    }
                }
                else
                {
                    x--; curX--;
                    videoMemory[80*y+x] = (currentColour << 8) | ' ';
                }
                break;
            }
      
            case '\a':
            {
                for (y = 0; y < 25; y++)
                    for (x = 0; x < 80; x++)
                        videoMemory[80*y+x] = (currentColour << 8) | ' ';
                x = 0; curX = 0;
                y = 0; curY = 0;
                break;
            }

            case '%':
                i++;
                switch (str[i])
                {
                    case 'i': case 'd':
                    {
                        int intval = va_arg(params, int);
                        char buffer[12];

                        itoa(intval, buffer, 10);

                        for (int j = 0; buffer[j] != '\0'; j++)
                        {
                            videoMemory[80*y+x] = (currentColour << 8) | buffer[j];
                            x++; curX++;
                        }

                        break;
                    }

                    case 'c':
                    {
                        char charVal = va_arg(params, int);
                        videoMemory[80*y+x] = (currentColour << 8) | charVal;
                        x++; curX++;

                        break;
                    }

                    case 'x':
                    {
                        uint8_t key = va_arg(params, int);

                        char* foo = "00";
                        char* hex = "0123456789ABCDEF";
                        foo[0] = hex[(key >> 4) & 0xF];
                        foo[1] = hex[key & 0xF];
            
                        videoMemory[80*y+x] = (currentColour << 8) | foo[0]; x++; curX++;
                        videoMemory[80*y+x] = (currentColour << 8) | foo[1]; x++; curX++;

                        break;
                    }

                    case 's':
                    {
                        const char* s = va_arg(params, const char*);
                        if (!s) s = "0x14_invalid";

                        while (*s != '\0')
                        {
                            videoMemory[80*y+x] = (currentColour << 8) | *s;
                            x++; curX++; s++;
                        }
                        break;
                    }

                    case 'R':
                    {
                        currentColour = (uint8_t)va_arg(params, int);
                        break;
                    }

                    default:
                    {
                        i--;
                        videoMemory[80*y+x] = (currentColour << 8) | str[i];
                        x++; curX++;
                        break;
                    }
                }

            continue;

            default:
            {
                videoMemory[80*y+x] = (currentColour << 8) | str[i];
                x++; curX++;
                break;
            }
        }

        if(x >= 80)
        {
            x = 0; curX = 0;
            y++; curY++;
        }

        if(y >= 25)
        {
            for(int r = 1; r < 25; r++)
                for(int c = 0; c < 80; c++)
                {
                    if(r == 0)
                    {
                    }
                    else
                    {
                        videoMemory[80 * (r - 1) + c] = videoMemory[80 * r + c];

                        // it ain't perfect (or close to, *for now*) but it works. will need to make it not print cursor spaghetti in a later build
                    }
                }
      
            for(x = 0; x < 80; x++)
            videoMemory[80 * 24 + x] = (currentColour << 8);

            x = 0; curX = 0;
            y = 24; curY = 24;
        }
    }

    va_end(params);
}

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

void EnableCursor(uint8_t start, uint8_t end)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | start);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | end);
}

void DisableCursor()
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void UpdateCursor()
{
    uint16_t pos = curY * 80 + curX;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void FlushShell()
{
    UpdateCursor();
}

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

void TestTask1()
{
    while(true)
        printf("A");
}

void TestTask2()
{
    while(true)
        printf("TT2B");
}

void TestTask3()
{
    while(true)
    {
        printf("n3isa");
        taskManager.sleep(3000);
        printf(" TCC");
        taskManager.sleep(50);
    }
}

// Crash handler logic and function
void panic(uint32_t errorId)
{
    static volatile bool inStateOfPanic = false;

    // go right here if already in panic
    if (inStateOfPanic) asm volatile ("cli; hlt");

    inStateOfPanic = true;
    asm volatile ("cli");
    const char* errorTextID[24] =
    {
        "DIVIDE_BY_ZERO", "DEBUG_TRAP", "NON_MASKABLE_INTERRUPT", "BREAKPOINT",
        "OVERFLOW_INCIDENT", "BOUND_RANGE_EXCEEDED", "INVALID_OPCODE", "DEVICE_UNAVAILABLE",
        "DOUBLE_FAULT", "COPROCCESSOR_OVERRUN", "INVALID_TASK_STATE_SEGMENT", "SEGMENT_NOT_PRESENT",
        "STACK_SEGMENT_FAULT", "GENERAL_PROTECTION_FAULT", "PAGE_FAULT", "0x0F",
        "X87_FLOATING_POINT_EXCEPTION", "MISALIGNED_MEMORY", "HARDWARE_ERROR", "SIMD_FLOATING_POINT_EXCEPTION",
        "VIRTUALISATION_EXCEPTION", "CONTROL_PROTECTION_EXCEPTION", "DRIVER_VIOLATION", "KILLSWITCH_INVOKED"
    };

    const char* errorName = "UNKNOWN_EXCEPTION";
    if (errorId < 24)
        errorName = errorTextID[errorId];

    // red screen of death
    printf("%R\a", 0x4F);
    printf(" <!> STOP                                                                   >_< ");
    printf("                                                                                ");
    printf(" OpenSteel/OS %d.%d.%d\n", verMajor, verMinor, verBuild);
    printf(" Exception ID 0x%x - %s\n", errorId, errorName); // keep it simple, only tell the end user what program/process broke it and why
    printf("                                                                                ");
    printf(" A problem has occurred and OpenSteel/OS has shut down.                         ");

    switch (errorId)
    {
        case 0x00:
            printf(" Attempt to divide by zero in the kernel or a kernel-level task caused a        ");
            printf(" system crash. Please contact the vendor of the faulting task to resolve this   ");
            printf(" problem.                                                                       ");
            break;

        case 0x09:
            printf(" Your system invoked a legacy error that has not been used since the Intel i486 ");
            printf(" series of processors. If you are running OpenSteel/OS on a computer with an    ");
            printf(" Intel Pentium (1993) or newer, you should not be seeing this error. If you are ");
            printf(" running OpenSteel/OS on a computer with an Intel i486 series processor or      ");
            printf(" older, please turn off the computer and restart it.                            ");
            break;

        case 0x12:
            printf(" This error, also called a machine check, indicates that there was a serious    ");
            printf(" hardware problem and that the CPU aborted itself. Please contact your hardware ");
            printf(" vendor(s) or distributor(s) for technical support.                             ");
            break;

        case 0x16:
            printf(" A task attempted to access a driver function without an appropriate system     ");
            printf(" call or API or construct its own driver outside of the driver manager.         ");
            break;

        case 0x17:
            printf(" A killswitch has been flipped to intentionally crash OpenSteel/OS. This could  ");
            printf(" be for a variety of reasons, ranging from you just want to have fun to you     ");
            printf(" could not access a shutdown interface safely and have a killswitch keybind.    ");
            printf(" Regardless, please try to avoid this error and shut the PC down normally, so   ");
            printf(" that you are not as numb to a serious error.                                   ");
            break;
        
        default:
            printf(" A description for this error is unavailable.                                   ");
            break;
    }

    printf("                                                                                ");
    printf("     system halted. please press the power button, it is now safe to do so.     ");
    while(1)
    {
        asm volatile ("cli; hlt");
    };
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
    printf("%RSteelsOfLiquid OpenSteel/OS %d.%d.%d \"Denver\" Beta 2 Circuit 5\n", 0x0F, verMajor, verMinor, verBuild);

    currentInterface = BootUI;
    GlobalDescriptorTable gdt;

    // Get memory heap, allocate ram, etc. v

    uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
    size_t heap = 10*1024*1024;
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

    // since a thing with this build is scheduler refinement, feel free to play around with my infinite-looping commands.
    // register tasks v

    //taskManager = new TaskManager();
    Task task1(&gdt, TestTask1);
    Task task2(&gdt, TestTask2);
    Task task3(&gdt, TestTask3);
    //taskManager.AddTask(&task1);
    //taskManager.AddTask(&task2);
    //taskManager.AddTask(&task3);

    // interrupts and drivers v

    InterruptManager interrupts(0x20, &gdt, &taskManager);


    DriverManager drvManager;
    // drivers loading
    printf("initialising drivers...                                                    ");

    RecommendedStandard232Driver serialPort;
    //serialPort.InitialiseSerial();
    serialPort.WriteString("OpenSteel/OS 0.22 \"Denver\" [OpenSteel/OS RS232 COM+Serial Driver]\nYou are seeing this because a successful COM1 connection was made.\n\n");

    KeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    drvManager.AddDriver(&keyboard);

    ProgrammableIntervalTimer programmableIntervalTimer(&interrupts);
    drvManager.AddDriver(&programmableIntervalTimer);

    //MouseToConsole mousehandler;
    //MouseDriver mouse(&interrupts, &mousehandler);
    //drvManager.AddDriver(&mouse);

    // ^ mouse driver sucked. let's not use it, if possible.

    Speaker speaker; drvManager.AddDriver(&speaker);
    ClockBatteryDriver cmos;
    char rtcSec[3], rtcMin[3];

    printf("%R[ok!]", 0x0A);
    printf("%Rfinding and selecting PCI devices and drivers...                           ", 0x0F);

    PCIController PCIController;
    PCIController.SelectDrivers(&drvManager, &interrupts);
    printf("%R[ok!]", 0x0A);

    // VideoGraphicsArray vga;

    printf("%Rstarting drivers and interrupts...                                         ", 0x0F);
    drvManager.ActivateAll(&interrupts);

/*     AdvancedTechnologyAttachment ata0l(0x1F0, true);  // IRQ 14
    serialPort.WriteString("ATA Pri. Lead: "); printf("%RATA Pri. Lead: ", 0x0F); ata0l.IdentifyDrive();
    AdvancedTechnologyAttachment ata0f(0x1F0, false);
    serialPort.WriteString("ATA Pri. Follow: "); printf("ATA Pri. Follow: "); ata0f.IdentifyDrive();

    char* testDriveString = "Testing hard drive stuff";
    ata0l.Write28Bit(0, testDriveString, 25);
    ata0l.FlushDrive();

    ata0l.Read28Bit(0, testDriveString, 25);

    AdvancedTechnologyAttachment ata1l(0x170, true);  // IRQ 15
    serialPort.WriteString("\nATA Sec. Lead: "); printf("\nATA Sec. Lead: "); ata1l.IdentifyDrive();
    AdvancedTechnologyAttachment ata1f(0x170, false);
    serialPort.WriteString("ATA Sec. Follow: "); printf("ATA Sec. Follow: "); ata1f.IdentifyDrive();
 */
    // the others are 0x1E8 and 0x168

    interrupts.Activate();
    RealTimeClockRegisters time = cmos.ReadRTC();
    cmos.PadRTCInteger(rtcSec, time.second);
    cmos.PadRTCInteger(rtcMin, time.minute);
    printf("%R[ok!]", 0x0A);

    NathanRenaudShell nrsh;

    cpudet();
    // booting is at the home stretch ^v

    // programmableIntervalTimer.HardSleep(30);
    speaker.LifeChime();
    serialPort.WriteString("Welcome to OpenSteel/OS.\n");
    printf("\n%RGreetings, and welcome to OpenSteel/OS. It is %d:%s:%s, %d %s, %d.\nType help for help.\n", 0x0F,
    time.hour, rtcMin, rtcSec,
    time.day, monthNames[time.month - 1], time.year);
    // Denotes end of booting process  ^

    // the code below is supposed to try to determine if the OS is in real mode or not. spoiler: it's not.
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


    //printf("Integer and printf Output Test _________________________________________________");
    //printf("Int 1: %d | Int 2: %d | Int 3: %d | Int 4: %d |Int 5: %d", testInteger1, testInteger2, testInteger3, testInteger4, testInteger5);
/* 
    if (interrupts.handlerExists(0x24)) printf("\nIRQ 4 Handler Exists."); else printf("\nIRQ 4 Handler doesn\'t exist.");
    if (interrupts.handlerExists(0x2E)) printf("\nIRQ 14 Handler Exists."); else printf("\nIRQ 14 Handler doesn\'t exist.");
    if (interrupts.handlerExists(0x21)) printf("\nIRQ 1 Handler Exists"); else printf("\nIRQ 1 handler doesn\'t exist.");
    if (interrupts.handlerExists(0x2C)) printf("\nIRQ 12 Handler exists."); else printf("\nIRQ 12 Handler doesn\'t exist.");
 */
    EnableCursor(13, 15);
    FlushShell();
    nrsh.Initialise();

    while(1)
    {
        if (lastChar != 0)
        {
            nrsh.HandleInput(lastChar);
            lastChar = 0;
        }
        asm volatile ("hlt");
    }
  
}

// honestly half of this code is documentation or disabled code
