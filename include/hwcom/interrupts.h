
#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include <common/types.h>
#include <hwcom/port.h>
#include <multitasking.h>
#include <gdt.h>


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

            struct GateDescriptor
            {
                osos::common::uint16_t handlerAddressLowBits;
                osos::common::uint16_t gdt_codeSegmentSelector;
                osos::common::uint8_t reserved;
                osos::common::uint8_t access;
                osos::common::uint16_t handlerAddressHighBits;

            } __attribute__((packed)); // 2024-10-31 ah yes, intellisense is totally fine and not drunk as this code compiled a-ok in the original ubuntu install
            // also why tf am i coding this on windows 10 garbage edition (poor timing for that joke -_-) when i could be doing this on linux

            static GateDescriptor interruptDescriptorTable[256];

            struct interruptDescriptorTablePointer
            {
                osos::common::uint16_t size;
                osos::common::uint32_t base;
            } __attribute__((packed));

            static void SetInterruptDescriptorTableEntry(
                osos::common::uint8_t interruptNumber,
                osos::common::uint16_t codeSegmentSelectorOffset,
                void (*handler)(),
                osos::common::uint8_t DescriptorPrivilegeLevel,
                osos::common::uint8_t DescriptorType
            );

            static void IgnoreInterruptRequest();

            static void HandleInterruptRequest0x00();
            static void HandleInterruptRequest0x01();
            static void HandleInterruptRequest0x02();
            static void HandleInterruptRequest0x03();
            static void HandleInterruptRequest0x04();
            static void HandleInterruptRequest0x05();
            static void HandleInterruptRequest0x06();
            static void HandleInterruptRequest0x07();
            static void HandleInterruptRequest0x08();
            static void HandleInterruptRequest0x09();
            static void HandleInterruptRequest0x0A();
            static void HandleInterruptRequest0x0B();
            static void HandleInterruptRequest0x0C();
            static void HandleInterruptRequest0x0D();
            static void HandleInterruptRequest0x0E();
            static void HandleInterruptRequest0x0F();
            static void HandleInterruptRequest0x31();


            static void HandleException0x00();
            static void HandleException0x01();
            static void HandleException0x02();
            static void HandleException0x03();
            static void HandleException0x04();
            static void HandleException0x05();
            static void HandleException0x06();
            static void HandleException0x07();
            static void HandleException0x08();
            static void HandleException0x09();
            static void HandleException0x0A();
            static void HandleException0x0B();
            static void HandleException0x0C();
            static void HandleException0x0D();
            static void HandleException0x0E();
            static void HandleException0x0F();
            static void HandleException0x10();
            static void HandleException0x11();
            static void HandleException0x12();
            static void HandleException0x13();


            static osos::common::uint32_t handleInterrupt(osos::common::uint8_t interruptNumber, osos::common::uint32_t esp);
            osos::common::uint32_t DoHandleInterrupt(osos::common::uint8_t interruptNumber, osos::common::uint32_t esp);

            /*
            Up until 2025-06-27, "Lead" was called "Master"; Follow was called "Slave" but people weren't as aware of the games they played in 1904 or so,
            and I know well enough from the history videos I watch that the historical terms here are just messed up. Shouldn't be too hard to rename each
            use in the code.
            */

            osos::hwcom::Port8BitSlow picLeadCommand;
            osos::hwcom::Port8BitSlow picLeadData;
            osos::hwcom::Port8BitSlow picFollowCommand;
            osos::hwcom::Port8BitSlow picFollowData;


        public:
            InterruptManager(osos::common::uint16_t HardwareInterruptOffset, GlobalDescriptorTable* gdt, TaskManager* taskManager);
            ~InterruptManager();
            osos::common::uint16_t HardwareInterruptOffset();

            bool handlerExists(osos::common::uint8_t interruptNumber);
            static bool interruptsEnabled();
            static void CheckIDTVector(osos::common::uint8_t vector);

            struct idtR
            {
                osos::common::uint16_t limit;
                osos::common::uint32_t base;
            } __attribute__((packed));

            void Activate();
            void Deactivate();

        };
    }
}

#endif
