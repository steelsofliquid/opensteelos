
#ifndef __OSOS__KERNEL__HWCOM__INTERRUPTS_H
#define __OSOS__KERNEL__HWCOM__INTERRUPTS_H

#include <common/types.h>
#include <common/lib/libio.h>
#include <kernel/hwcom/port.h>
#include <kernel/crashHandler.h>
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
                uint8_t interruptNumber;
                InterruptManager* interruptManager;

                InterruptHandler(InterruptManager* interruptManager, uint8_t interruptNumber);
                virtual ~InterruptHandler();

                public:
                virtual uint32_t HandleInterrupt(uint32_t esp);
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

                static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);
                uint32_t DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp);



                public:
                InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* gdt, TaskManager* taskManager);
                ~InterruptManager();
                uint16_t hardwareInterruptOffset();

                bool handlerExists(uint8_t interruptNumber);
                static bool interruptsEnabled();

                void Activate();
                void Deactivate();

            };
        }
    }
}

#endif
