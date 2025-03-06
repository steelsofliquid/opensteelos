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

void WriteCharacter(unsigned char c, unsigned char forecolour, unsigned char backcolour, uint8_t x, uint8_t y)
{
     uint16_t attrib = (backcolour << 4) | (forecolour & 0x0F);
     volatile uint16_t * where;
     where = (volatile uint16_t *)0xB8000 + (y * 80 + x) ;
     *where = c | (attrib << 8);
}

void printf(char* str)
{
  static uint16_t* VideoMemory = (uint16_t*)0xb8000;
  
  static uint8_t x =0 , y=0;

  for(int i = 0; str[i] != '\0'; ++i)
  {

    switch(str[i])
    {

      case '\n':
        y++;
        x = 0;
        break;

      case '\b': // Interim solution for the no backspace problem. It works!!
        if(x == 0)
        {
        } // Safeguard to ensure a negative number isn't... you know.
        else
        {
          x--;
          VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
        }
        break;

      default:
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i]; // And, now, for a more serious comment, this system takes the current line, and moves the text """""placer""""" until it's at a new line.
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
          VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' '; // Video memory!! Immediately in the kernel! Yayyy! ^_^

      x = 0;
      y = 0;
    }
  }
}

void sleep(uint32_t interval)
{
  // Code in this thing is based off of the equivalent in osakaOS.
  // If interval = 1, that is 1 ms. Probably.

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

class MouseToConsole : public MouseEventHandler
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
    static uint16_t* VideoMemory = (uint16_t*)0xb8000; // The video memory is the same for all GPUs for now. It doesn't matter if your PC has a flashy brand-new RTX or an ancient Voodoo. NanamiOS treats both cards equally.

    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                        | (VideoMemory[80*y+x] & 0xF000) >> 4
                        | (VideoMemory[80*y+x] & 0x00FF);

    x += xoffset;
    if(x < 0) x = 0;
    if(x >= 80) x = 79;

    y += yoffset; // Can we patch an inverted y-axis with minimal code?
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
  printf(" OpenSteel/OS version 0.22.38 \"Hakurei\"\n");
}

void cmdTest()
{
  printf("Test command. If you see this, then hello!");
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

  /* r_nanamitsukasa at 13:08 toronto on 2024 july 2:
  I'm going to try something new, that being to add a number of printf things at the top to refresh the screen.*/

  // printf("                                                                                ");

  // Version headers have to be replaced manually. This hopefully shouldn't be too much!
  printf("  ___                                                                           ");
  printf(" /  /    SteelsOfLiquid OpenSteel/OS version 0.22.38 \"Hakurei\"                  ");
  printf(" \\__\\    By SteelsOfLiquid, based on WYOOS. Licensed under GNU-GPL 3.0          ");
  printf("  \\  \\   steelsofliquid@hotmail.com ~ https://steelsofliquid.neocities.org/     ");
  printf("  /__/                                                                          ");
  printf("                                                                                ");

  GlobalDescriptorTable gdt;

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

  TaskManager taskManager;
  Task task1(&gdt, TestTask1);
  Task task2(&gdt, TestTask2);
  Task taskcmd1(&gdt, cmdVersion);
  Task taskcmd2(&gdt, cmdTest);
  // taskManager.AddTask(&task1);
  // taskManager.AddTask(&task2);

  InterruptManager interrupts(0x20, &gdt, &taskManager);

  DriverManager drvManager;
    printf("[msg] A boot sequence has been initiated.\n[msg] Starting drivers...");

    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler); // Oopsies! I forgot a semicolon! >_< (a bit back)
    drvManager.AddDriver(&keyboard);

    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler); // drivers loading
    drvManager.AddDriver(&mouse);
    // printf("[DRV] Mouse Driver  "); // Oops... I did it again... * tapping chin/mouth *
    // That comment was a reference to the Windows 7 anime commercial, where Nanami Madobe forgot a screw in the PC
    // she built, causing the front panel to fall forward onto her desk. Do I know what she actually said there? No.
    // Do I have an idea of what she said? Maybe. I forgot a semi-colon here...

    // gotta keep that one in here despite the project re-organization

    Speaker speaker;

    printf("\n[msg] Drivers started. Obtaining PCI info...\n");

    PCIController PCIController;
    PCIController.SelectDrivers(&drvManager, &interrupts);

    // VideoGraphicsArray vga;

    drvManager.ActivateAll();

  interrupts.Activate();

  sleep(1);
  speaker.beep();
  printf("\n[msg] System online.\n");
  /* vga.SetMode(320, 200, 8);
  for(int32_t y = 0; y < 200; y++)
    for(int32_t x = 0; x < 320; x++)
      vga.PutPixel(x, y, 0x00, 0x00, 0xA8);
    */

  while(1); // Nothing happens! ...For now.
  
}

