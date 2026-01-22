#include <drivers/rtcIrq8.h>

using namespace osos;
using namespace osos::drivers;
using namespace osos::kernel;
using namespace osos::kernel::hwcom;





InterruptRequest8AssistantDriver::InterruptRequest8AssistantDriver(InterruptManager* manager) :
 InterruptHandler(manager, 0x28)
{
    driverAttributes.name      = "IRQ 8 Handler Driver Assistant";
    driverAttributes.publisher = "SteelsOfLiquid";
    driverAttributes.type      = "assistants";

    driverAttributes.isInitialised = false;
    driverAttributes.isActive      = false;

    driverAttributes.hasInterruptRequest  = true;
    driverAttributes.interruptRequestLine = 0x08;
    driverAttributes.vectorOffset         = interruptNumber;
}

InterruptRequest8AssistantDriver::~InterruptRequest8AssistantDriver()
{
}


void InterruptRequest8AssistantDriver::StartDriver()
{
}

uint32_t InterruptRequest8AssistantDriver::HandleInterrupt(uint32_t esp)
{
    outb(0x70, 0x0C);
    inb(0x71);

    return esp;
}