// First non-WYOOS component. If it looks similar to osakaOS (which, I need to make a confession, I've been peeking at its source code often), well, I used the OSDev Wiki and that code is similar to that of osakaOS.

#include <drivers/pit.h>
#include <multitasking.h>

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;
using namespace osos::hwcom;

void printf(char* str, ...);

volatile uint32_t tickcount = 0; // global variable

uint32_t ticksecs = 0;
uint32_t prevSecTick = 0;
bool IsExactlySecond;

        ProgrammableIntervalTimer::ProgrammableIntervalTimer(InterruptManager* manager) :
            InterruptHandler(manager, 0x20),
            Channel0(0x40),
            Channel1(0x41),
            Channel2(0x42),
            PITComPort(0x43),
            ProgIC(0x20)
        {
        }

		ProgrammableIntervalTimer::~ProgrammableIntervalTimer()
        {
        }

        uint32_t ProgrammableIntervalTimer::ReadPIT()
        {
            uint32_t count = 0;

            asm volatile("cli");

            count = Channel0.Read(); // Low
            count |= Channel0.Read() << 8; // High

            asm volatile("sti");
            return count;
        }

        void ProgrammableIntervalTimer::SetPITCount(uint32_t count)
        {
            asm volatile("cli");

            PITComPort.Write(0x36);
            Channel0.Write(count & 0xFF);
            Channel0.Write((count >> 8) & 0xFF);

            asm volatile("sti");
        }

        /* void ProgrammableIntervalTimer::HardSleep(uint32_t interval)
        { */
            /* This is the old busy-wait mechanism previously defined in kernel.cpp.
               It is maintained for two reasons: (1) As a "fallback" for nobody but me writing code,
               and (2) for special use cases, such as brief one-off delays in the kernel or another
               program, but primarily drivers.
               
               In the Denver phase I realised in the case of a userlandless program of sorts,
               what's now HardSleep is a piece of shit when it comes to task management, and
               so it was an early "this has to go" ordeal. Normal Sleep() is used on the
               software end, HardSleep() is hardware.

               HardSleep uses a very crude busy-wait system that if incorrectly used,
               will screw you over and make your beautiful code hang. At least I hope that
               OpenSteel/OS devs are, or will be, using the platform competently.

               So if your program hangs, go tell me to "fuck off" all you want but that's self-induced
               error. Use the normal Sleep mechanism in that sense -_-
               */
            
        /*    for (uint32_t i = 0; i < interval; i++)
            {
                uint32_t timing = ReadPIT(); // don't need to set the pit count, i presume

                while ((timing - ReadPIT()) < 100)
                {
                }
            }
        }*/


        void ProgrammableIntervalTimer::Activate()
        {
            SetPITCount(1193180 / 100); // 100 Hz
        }

        uint32_t ProgrammableIntervalTimer::HandleInterrupt(uint32_t esp)
        {
            /* So, I have a confession to make... When I first added the PIT driver to OpenSteel/OS
               during the days of its Nanami/OS guise, I added it assuming I wouldn't need to add
               either Activate() or HandleInterrupt(uint32_t esp). It was a fairly reasonable thing
               for my brain to assume, knowing I was calling Windows 98 on an Athlon XP a magical
               thing that was impossible at the same time.

               When I actually dug a bit deeper into this, I realised that was not the case. And
               I had MacGyvered a PIT driver that somehow worked despite a lack of interrupts.

               So, this was supposed to be added from the start. I'm sorry. And holy shit, it was
               hard to implement this.
            */

            tickcount++;

            if (tickcount % 100 == 0)
            {
                // this isn't the most necessary thing but it could be useful nonetheless.
                IsExactlySecond = true;
                prevSecTick = tickcount;
                ticksecs++;
            }
            else
            {
                IsExactlySecond = false;
            }
            esp = (uint32_t)taskManager.Schedule((CPUState*)esp);
            taskManager.WakeTask(tickcount);
            return esp;
        }

        // i need to do better with these fucking comments