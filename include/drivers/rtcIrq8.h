// OpenSteel/OS IRQ 8 and RTC Interrupt Assistant Driver
//
// This driver exists to handle IRQ 8 (IDT entry 0x28) due to the
// design of the OpenSteel/OS driver model strictly requiring non-IRQ
// drivers to not have an IRQ, even if they may have functions that can
// fire an IRQ. This driver model is somewhat awkward, like with hard
// drives, but in that case it can hopefully be managed with competent
// design.
//
// IRQ 8, though, unlike hard drives' IRQs (IRQs 14 and 15), is mapped
// to the RTC (Real-Time Clock). The RTC is something that is usually
// handled by your local CMOS driver. And the CMOS driver may need to
// handle other things, like legacy hard drive settings and other
// config options. And most of the RTC magic can be handled inside the
// CMOS driver without it firing IRQs in many tested cases.
//
// As such, this driver exists solely for handling IRQ 8 and its adjacent
// RTC bits. To a degree, it is kinda like the way drivers work with
// IRQs on Unix and Unix-likes, where the IRQ handling is separate from
// the driver, and a driver has to request an IRQ. OpenSteel/OS has
// a driver model that usually unifies IRQ handling and device functions,
// but there's always going to be these awkward cases like the RTC.
//
// Oh, PS if this file looks huge and you are pissed about this large
// comment I'll tell you I wrote it, I'm just autistic and will go in
// to even the most minute details.

#ifndef __OSOS__DRIVERS__RTCIRQ8_H
#define __OSOS__DRIVERS__RTCIRQ8_H

#include <common/types.h>
#include <common/lib/libasm.h>
#include <drivers/cmos.h>       // just in case we need both
#include <kernel/hwcom/driverModel.h>
#include <kernel/hwcom/interrupts.h>

namespace osos
{
    namespace drivers
    {
        class InterruptRequest8AssistantDriver : public osos::kernel::hwcom::DriverModel, public osos::kernel::hwcom::InterruptHandler
        {
            public:
            InterruptRequest8AssistantDriver(osos::kernel::hwcom::InterruptManager* manager);
            ~InterruptRequest8AssistantDriver();

            // As you can see, this IRQ assistant driver is simple on purpose: It's only
            // supposed to handle an IRQ. We really only need two functions.

            virtual void StartDriver();
            virtual uint32_t HandleInterrupt(uint32_t esp);
        };
    }
}

#endif