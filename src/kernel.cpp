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

#include <common/types.h>
#include <gdt.h>
#include <dmm.h>
#include <hwcom/interrupts.h>
#include <hwcom/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/pit.h>
#include <drivers/speaker.h>
#include <drivers/vga.h>
#include <multitasking.h>

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;
using namespace osos::hwcom;
// using namespace osos::gui;

/*
  The three int32_t values below denote the version number. It is some degree of "version control" or version indicator.
  They are essentially useless at the moment as no function can properly use them.
*/

uint32_t VersionMajor = 0;
uint32_t VersionMinor = 22;
uint32_t VersionBuild = 41;

void printf(char* str) // the main screen output function.
{
  static uint16_t* VideoMemory = (uint16_t*)0xb8000;
  
  static uint8_t x = 0 , y = 0;

  for(int i = 0; str[i] != '\0'; ++i)
  {

    switch(str[i])
    {

      /*
        Just a simple rundown on what each thing here does:

         - \n means new line. That is the case for most languages.

         - \b means backspace. I wrote this bit myself as I had to figure it out one morning in government class (what does an operating system have to do with politics T_T)

         - Anything else on the keyboard placed in printf() will just input an ordinary IBM Code Page 437 character.
          - Use \' or \" if you need to insert an apostrophe or quotation mark as you'll break it otherwise!
          - Don't input anything not in the keyboard unless you want to break the graphics, I tried it and that's what happened!
      */

      case '\n':
        y++;
        x = 0;
        break;

      case '\b':
        if(x == 0)
        {
          if(y != 0) // i swear to god, no, i swear to neisa-sama if you somehow get a number below zero, there's something seriously wrong with your pc. mind you, this is an int!
          {
            y--;
            x = 79;
          }
        } // Safeguard to ensure a negative number isn't... you know.
        else
        {
          x--;
        }
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
        break;
      
      case '\a':
        for (y = 0; y < 25; y++)
          for (x = 0; x < 80; x++)
            VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
        x = 0;
        y = 0;

      default:
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
        x++;
        break;
    }

    if(x >= 80)
    {
      x = 0;
      y++;
    }

    if(y >= 25)
    {
      for(y = 0; y < 25; y++)
        for(x = 0; x < 80; x++)
        {
          if(y == 0) // read the comment below
          {
          }
          else
          {
            VideoMemory[80 * (y - 1) + x] = VideoMemory[80 * y + x]; // now that i think, what the fuck did i just say about the y-integer and -1!?
            // i know uint8_t is a char, but it's supposed to be treated as an integer number here. simple. 25 rows, including zero, no lower than zero.
            // don't make uint8_t x and y uint32_t, the code will break and i know that without even testing it to see.

            // it ain't perfect (or close to, *for now*) but it works. will need to make it not print cursor spaghetti in a later build
          }
        }
      
      for(x = 0; x < 80; x++)
        VideoMemory[80 * 24 + x] = (VideoMemory[80*24+x] & 0xFF00) | ' ';

      x = 0;
      y = 24;
    }
  }
}

void sleep(uint32_t interval)
{
  ProgrammableIntervalTimer ProgrammableIntervalTimer;

  for (uint32_t i = 0; i < interval; i++)
  {
    ProgrammableIntervalTimer.SetPITCount(1193182 / 1000);
    uint32_t timing = ProgrammableIntervalTimer.ReadPIT();

    while ((timing - ProgrammableIntervalTimer.ReadPIT()) < 1000)
    {
    }
  }
}

void printfHex(uint8_t key)
{
  char* foo = "00";
  char* hex = "0123456789ABCDEF";
  foo[0] = hex[(key >> 4) & 0xF];
  foo[1] = hex[key & 0xF];
  printf(foo);
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
  void OnKeyDown(char c)
  {
    char* foo = " ";
    foo[0] = c;
    printf(foo);
  }

};

class MouseToConsole : public MouseEventHandler // This moves the sometimes very useless mouse cursor.
{
  int8_t x, y;
public:
  MouseToConsole()
  {
    uint16_t* VideoMemory = (uint16_t*)0xB8000;
    x = 40;
    y = 12;
    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                        | (VideoMemory[80*y+x] & 0xF000) >> 4
                        | (VideoMemory[80*y+x] & 0x00FF);
  }

  void OnMouseMove(int xoffset, int yoffset)
  {
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                        | (VideoMemory[80*y+x] & 0xF000) >> 4
                        | (VideoMemory[80*y+x] & 0x00FF);

    x += xoffset;
    if(x < 0) x = 0;
    if(x >= 80) x = 79;

    y += yoffset;
    if(y < 0) y = 0;
    if(y >= 25) y = 24;

    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                        | (VideoMemory[80*y+x] & 0xF000) >> 4
                        | (VideoMemory[80*y+x] & 0x00FF);
  }

};

void TestTask1()
{
  while(true)
    printf("[i] This is a testing task. It will display this message continously. (TestTask1");
}

void TestTask2()
{
  while(true)
    printf("[i] This is a testing task. It will display this message continously. (TestTask2");
}

void cmdVersion()
{
  printf(" OpenSteel/OS version 0.22.41 \"Hakurei\"\n");
}

void cmdTest()
{
  printf("Test command. If you see this, then hello!");
}

void panic()
{
  printf(" >_<   systempanic\n");

  printf(" OpenSteel/OS version 0.22.41");
  printf(" Offending Material: NULL"); // keep it simple, only tell the end user what program/process broke it and why
  printf(" Trigger: NULL"); // "NULL" is to be replaced with legitimate reasons, such as a memory leak or a buffer overflow.
  printf("                                                                                ");
  printf(" A problem has occurred and OpenSteel/OS needs to shut down or restart. Any     ");
  printf(" unsaved work has been lost and we apologise for the inconvenience. If you see  ");
  printf(" this message multiple times, it may be a potential hardware failure or malware ");
  printf(" may be present on your system. For help, contact steelsofliquid@hotmail.com    ");

  while(1); // use this in the kernel to try to stop someone from continuing to use an unstable system.
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
  for(constructor* i = &start_ctors; i != &end_ctors; i++)
    (*i)();
}

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

  printf("  ___                                                                           ");
  printf(" /  /    SteelsOfLiquid OpenSteel/OS version 0.22.41 \"Hakurei\"                  ");
  printf(" \\__\\    By SteelsOfLiquid, based on WYOOS. Licensed under GNU-GPL 3.0          ");
  printf("  \\  \\   steelsofliquid@hotmail.com ~ https://steelsofliquid.neocities.org/     ");
  printf("  /__/                                                                          ");
  printf("                                                                                ");

  GlobalDescriptorTable gdt;

  // Get memory heap, allocate ram, etc. v

  uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
  size_t heap = 10*1024*1024;
  MemoryManager MemoryManager(heap, (*memupper)*1024 - heap - 10*1024);

  printf("[dmm] Memory Heap: 0x");
  printfHex((heap >> 24) & 0xFF);
  printfHex((heap >> 16) & 0xFF);
  printfHex((heap >> 8 ) & 0xFF);
  printfHex((heap      ) & 0xFF);

  void* allocated = MemoryManager.malloc(1024);
  
  printf(" - Allocated: 0x");
  printfHex(((size_t)allocated >> 24) & 0xFF);
  printfHex(((size_t)allocated >> 16) & 0xFF);
  printfHex(((size_t)allocated >> 8 ) & 0xFF);
  printfHex(((size_t)allocated      ) & 0xFF);
  
  printf("\n");

  // register tasks v

  TaskManager taskManager;
  Task task1(&gdt, TestTask1);
  Task task2(&gdt, TestTask2);
  Task taskcmd1(&gdt, cmdVersion);
  Task taskcmd2(&gdt, cmdTest);
  // taskManager.AddTask(&task1);
  // taskManager.AddTask(&task2);

  // interrupts and drivers v

  InterruptManager interrupts(0x20, &gdt, &taskManager);

  DriverManager drvManager;
    printf("[msg] A boot sequence has been initiated.\n[msg] Starting drivers...");

    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    drvManager.AddDriver(&keyboard);

    // MouseToConsole mousehandler;
    // MouseDriver mouse(&interrupts, &mousehandler); // drivers loading
    // drvManager.AddDriver(&mouse);

    // ^ mouse driver sucked. let's not use it, if possible.

    Speaker speaker;

    printf("\n[msg] Drivers started. Obtaining PCI info...");

    PCIController PCIController;
    PCIController.SelectDrivers(&drvManager, &interrupts);

    // VideoGraphicsArray vga;

    drvManager.ActivateAll();

  interrupts.Activate();

  // booting is at the home stretch ^v

  sleep(1);
  speaker.beep();
  printf("\n[msg] System online.\n"); // Denotes end of booting process <

  /* vga.SetMode(320, 200, 8);
  for(int32_t y = 0; y < 200; y++)
    for(int32_t x = 0; x < 320; x++)
      vga.PutPixel(x, y, 0x00, 0x00, 0xA8);
    */

  sleep(5);

  while(1);
  
}

// honestly half of this code is documentation or disabled code
