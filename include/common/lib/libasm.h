#ifndef __OSOS__COMMON__LIB__LIBASM_H
#define __OSOS__COMMON__LIB__LIBASM_H // inline assembly carriers

#include <common/types.h>

static inline void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %0, %1" : : "a" (value), "Nd" (port));
}
static inline uint8_t inb(uint16_t port)
{
    uint8_t result;
    asm volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}
static inline void cpuid(int code, 
    uint32_t* a, 
    uint32_t* b, 
    uint32_t* c, 
    uint32_t* d)
{
    asm volatile("cpuid" : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d) : "a"(code), "c"(0));
}

#endif