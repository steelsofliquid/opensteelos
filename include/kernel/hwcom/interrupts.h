
#ifndef __OSOS__KERNEL__HWCOM__INTERRUPTS_H
#define __OSOS__KERNEL__HWCOM__INTERRUPTS_H

#include <common/types.h>
#include <kernel/hwcom/port.h>
#include <kernel/gdt.h>
#include <kernel/multitasking.h>


namespace osos
{
    namespace kernel
    {
        namespace hwcom
        {
            class InterruptManager;

            class InterruptHandler
            {
                protected:
                osos::common::uint8_t interruptNumber;
                InterruptManager* interruptManager;

                InterruptHandler(InterruptManager* interruptManager, osos::common::uint8_t interruptNumber);
                virtual ~InterruptHandler();

                public:
                virtual osos::common::uint32_t HandleInterrupt(osos::common::uint32_t esp);
                void attachToInterruptManager(InterruptManager* interruptManager);
                //void RegisterRequestHandler();
                //void DeregisterRequestHandler();

            };

            class InterruptManager
            {
                friend class InterruptHandler;

                protected:
                static InterruptManager* ActiveInterruptManager;
                InterruptHandler* handlers[256];
                TaskManager *taskManager;

                static void IgnoreInterruptRequest();

                static osos::common::uint32_t handleInterrupt(osos::common::uint8_t interruptNumber, osos::common::uint32_t esp);
                osos::common::uint32_t DoHandleInterrupt(osos::common::uint8_t interruptNumber, osos::common::uint32_t esp);



                public:
                InterruptManager(osos::common::uint16_t hardwareInterruptOffset, GlobalDescriptorTable* gdt, TaskManager* taskManager);
                ~InterruptManager();
                osos::common::uint16_t hardwareInterruptOffset();

                bool handlerExists(osos::common::uint8_t interruptNumber);
                static bool interruptsEnabled();

                void Activate();
                void Deactivate();

            };
        }
    }
}

#endif
