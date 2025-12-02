#ifndef __GLOBALFUNCS_H
#define __GLOBALFUNCS_H

#include <common/types.h>
#include <common/version.h>
#include <drivers/cmos.h>
#include <drivers/pit.h>
#include <lib/libstr.h>

namespace osos
{
    // void InitialiseHardSleep();


    // --- system APIs ---
    // GrabSys APIs
    // DO NOT USE!!
    osos::common::int8_t* GrabSysVer(osos::common::int8_t sector);

    // hardware materials
    void HardSleep(osos::common::uint32_t interval);


    // inline assembly carriers
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
    static inline void cpuid(int code, 
        osos::common::uint32_t* a, 
        osos::common::uint32_t* b, 
        osos::common::uint32_t* c, 
        osos::common::uint32_t* d)
    {
        asm volatile("cpuid" : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d) : "a"(code), "c"(0));
    }
}

#endif