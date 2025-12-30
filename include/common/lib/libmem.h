
#ifndef __OSOS__COMMON__LIB__LIBMEM_H
#define __OSOS__COMMON__LIB__LIBMEM_H

#include <common/types.h>


void* memcpy(void* destination, const void* source, size_t number);
void* memmove(void* destination, const void* source, size_t number);
void* memset(void* destination, int32_t value, size_t number);
uint32_t memcmp(const void* size1, const void* size2, size_t number);

#endif