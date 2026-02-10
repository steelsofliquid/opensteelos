
#ifndef __OSOS__COMMON__LIB__LIBSTR_H
#define __OSOS__COMMON__LIB__LIBSTR_H

#include <common/types.h>
#include <common/lib/libmem.h>
#include <kernel/mem/dmm.h>

size_t strlen(const char* string);
size_t strnlen(const char* string, int limit);
int32_t strcmp(const char* string1, const char* string2);
int32_t strncmp(const char* string1, const char* string2, size_t n);

char* strcpy(char* destination, const char* source);
char* strncpy(char* destination, const char* source, size_t n);
char* strcat(char* destination, const char* source);
char* strdup(const char* source); // The function that invokes strdup also must invoke free()

char* strchr(const char* string, int32_t character);
char* strrchr(const char* string, int32_t character);
char* strstr(const char* haystack, const char* needle);

char* itoa(int32_t value, char* string, int32_t base);

char* strtok(char* newstr, const char* delim); // use is not recommended for kernel-level software

#endif