
#ifndef __OSOS__COMMON__LIB__LIBSTR_H
#define __OSOS__COMMON__LIB__LIBSTR_H

#include <common/types.h>

size_t strlen(const char* string);
int32_t strcmp(const char* string1, const char* string2);
int32_t strncmp(const char* string1, const char* string2, size_t n);

int8_t* strcpy(char* destination, const char* source);
int8_t* strncpy(char* destination, const char* source, size_t n);
int8_t* strcat(char* destination, const char* source);

int8_t* strchr(const char* string, int32_t character);
int8_t* strrchr(const char* string, int32_t character);
int8_t* strstr(const char* haystack, const char* needle);

int8_t* itoa(int32_t value, char* string, int32_t base);

int8_t* strtok(char* newstr, const char* delim);

#endif