
#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include <common/types.h>
#include <hwcom/port.h>
#include <multitasking.h>
#include <gdt.h>
// #include <globalfuncs.h>


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

            /*
            Up until 2025-06-27, "Lead" was called "Master"; Follow was called "Slave" but people weren't as aware of the games they played in 1904 or so,
            and I know well enough from the history videos I watch that the historical terms here are just messed up. Shouldn't be too hard to rename each
            use in the code.
            */



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
