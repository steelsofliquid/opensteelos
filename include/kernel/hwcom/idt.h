#ifndef __OSOS__KERNEL__HWCOM__IDT_H
#define __OSOS__KERNEL__HWCOM__IDT_H

#include <common/types.h>
#include <common/lib/libio.h>
#include <kernel/gdt.h>

namespace osos
{
    namespace kernel
    {
        namespace hwcom
        {
            class InterruptDescriptorTable
            {
                protected:
                struct interruptDescriptorTablePointer
                {
                    uint16_t size;
                    uint32_t base;
                } __attribute__((packed));

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


                public:
                struct GateDescriptor
                {
                    uint16_t handlerAddressLowBits;
                    uint16_t gdtCodeSegmentSelector;
                    uint8_t reserved;
                    uint8_t access;
                    uint16_t handlerAddressHighBits;

                } __attribute__((packed)); // 2024-10-31 ah yes, intellisense is totally fine and not drunk as this code compiled a-ok in the original ubuntu install
                // also why tf am i coding this on windows 10 garbage edition (poor timing for that joke -_-) when i could be doing this on linux

                InterruptDescriptorTable();
                ~InterruptDescriptorTable();

                static GateDescriptor interruptDescriptorTable[256];

                static void SetInterruptDescriptorTableEntry(
                    uint8_t interruptNumber,
                    uint16_t codeSegmentSelectorOffset,
                    void (*handler)(),
                    uint8_t descriptorPrivilegeLevel,
                    uint8_t descriptorType
                );

                void SetExceptions(GlobalDescriptorTable* gdt);
                void SetInterruptRequests(GlobalDescriptorTable* gdt);
                void SetSystemCalls(GlobalDescriptorTable* gdt);
                void Initialise();

                // debug checks
                static void CheckIDTVector(uint8_t vector);
                static void CheckIDTAttribAndSlctr(uint8_t vector);
                //static void DebugStage5();

                struct idtR
                {
                        uint16_t limit;
                        uint32_t base;
                } __attribute__((packed));
            };
        }
    }
}

#endif