// First non-WYOOS component. Initially referencing the OSDev Wiki and osakaOS, although this driver has developed past that point.

#include <drivers/pit.h>
#include <kernel/multitasking.h>

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;
using namespace osos::kernel;
using namespace osos::kernel::hwcom;

void printf(char* str, ...);

volatile uint32_t tickCount = 0; // global variable

uint32_t tickSecs = 0;
uint32_t prevSecTick = 0;
bool isExactlySecond;

ProgrammableIntervalTimer::ProgrammableIntervalTimer(InterruptManager* manager) :
    DriverModel(),
    InterruptHandler(manager, 0x20),
    Channel0(0x40),
    Channel1(0x41),
    Channel2(0x42),
    PITComPort(0x43),
    ProgIC(0x20)
{
    driverAttributes.name      = "Intel i8254 PIT Driver";
    driverAttributes.publisher = "SteelsOfLiquid";
    driverAttributes.type      = "timers";

    driverAttributes.isInitialised = false;
    driverAttributes.isActive      = false;

    driverAttributes.hasInterruptRequest  = true;
    driverAttributes.interruptRequestLine = 0x00;
    driverAttributes.vectorOffset         = interruptNumber;
}

ProgrammableIntervalTimer::~ProgrammableIntervalTimer()
{
}

InterruptHandler* ProgrammableIntervalTimer::InterruptHandlerForme()
{
    return this;
}

uint32_t ProgrammableIntervalTimer::ReadPIT()
{
    uint32_t count = 0;

    //asm volatile("cli");

    count = Channel0.Read(); // Low
    count |= Channel0.Read() << 8; // High

    //asm volatile("sti");
    return count;
}

void ProgrammableIntervalTimer::SetPITCount(uint32_t count)
{
    //asm volatile("cli");

    PITComPort.Write(0x36);
    Channel0.Write(count & 0xFF);
    Channel0.Write((count >> 8) & 0xFF);

    //asm volatile("sti");
}



void ProgrammableIntervalTimer::StartDriver()
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

    tickCount++;
    
    if (tickCount % 100 == 0)
    {
        // this isn't the most necessary thing but it could be useful nonetheless.
        isExactlySecond = true;
        prevSecTick = tickCount;
        tickSecs++;
    }
    else
    {
        isExactlySecond = false;
    }
    esp = (uint32_t)taskManager.Schedule((CPUState*)esp);
    taskManager.WakeTask(tickCount);
    return esp;
}

// i need to do better with these fucking comments