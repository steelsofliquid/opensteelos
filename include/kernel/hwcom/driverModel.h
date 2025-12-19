// The OpenSteel/OS driver model. Work on this began in December 2025
// following the revelation that the at-the-time driver and device
// management subsystem was just completely broken. For reference:
//  - Drivers started themselves (and interrupts)
//  - Some drivers just acted completely independently.
//  - Others had include things outside of the kernel.
// This new model aims to rectify those issues. Sadly, there isn't much
// online reference for implementing driver models, and so I have to do
// a lot of the heavy lifting... on a topic that is not very well taught
// about by anyone. Where's the Indian guy on YouTube when you need him
// the most?

#ifndef __OSOS__KERNEL__HWCOM__DRIVERMODEL_H
#define __OSOS__KERNEL__HWCOM__DRIVERMODEL_H

#include <common/types.h>
#include <kernel/hwcom/interrupts.h>
#include <kernel/hwcom/pic.h>
#include <kernel/hwcom/port.h>
#include <kernel/mem/dmm.h>

namespace osos
{
    namespace kernel
    {
        namespace hwcom
        {
            struct DriverStatus
            {
                const char* name;
                const char* publisher;
                const char* type;
                bool isInitialised;
                bool isActive;
                bool hasInterruptRequest;
                osos::common::uint8_t interruptRequestLine;
                osos::common::uint8_t vectorOffset;
            };

            class DriverModel
            {
                public:
                DriverModel();
                ~DriverModel();

                DriverStatus driverAttributes;
                virtual osos::kernel::hwcom::InterruptHandler* InterruptHandlerForme();

                virtual void StartDriver();

                void Initialise();
                void Activate();
                void Deactivate();
                void Reset();

                virtual const char* GetDriverName();
                virtual const char* GetDriverType();

                virtual bool IsDriverInitialised();
                virtual bool IsDriverActive();
            };

            /* 
            class InterruptDriver : public osos::kernel::hwcom::InterruptHandler
            {
                public:
                InterruptDriver();
                ~InterruptDriver();

                virtual osos::common::uint32_t HandleInterrupt(osos::common::uint32_t esp);
                virtual osos::common::uint8_t GetInterruptRequest();
            }; */
        }
    }
}

#endif