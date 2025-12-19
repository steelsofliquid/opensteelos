
#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include <common/types.h>
#include <hwcom/port.h>
#include <gdt.h>
#include <multitasking.h>


namespace osos
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
            ~InterruptHandler();

            public:
            virtual osos::common::uint32_t HandleInterrupt(osos::common::uint32_t esp);
            void attachToInterruptManager(InterruptManager* interruptManager);

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

#endif
