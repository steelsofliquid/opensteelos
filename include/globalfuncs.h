#ifndef __GLOBALFUNCS_H
#define __GLOBALFUNCS_H

#include <common/types.h>
#include <drivers/pit.h>

namespace osos
{
        // void InitialiseHardSleep();

        void HardSleep(osos::common::uint32_t interval);


        static inline void outb(osos::common::uint16_t port, osos::common::uint8_t value)
        {
             asm volatile("outb %0, %1" : : "a" (value), "Nd" (port));
        }
        static inline osos::common::uint8_t inb(osos::common::uint16_t port)
        {
            osos::common::uint8_t result;
            asm volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
            return result;
        }
}

#endif