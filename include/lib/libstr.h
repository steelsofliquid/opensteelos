
#ifndef __OSOS__LIB__LIBSTR_H
#define __OSOS__LIB__LIBSTR_H

#include <common/types.h>

namespace osos
{
    namespace libs
    {
        osos::common::size_t strlen(const char* string);
        osos::common::int32_t strcmp(const char* string1, const char* string2);
        osos::common::int32_t strncmp(const char* string1, const char* string2, osos::common::size_t n);

        osos::common::int8_t* strcpy(char* destination, const char* source);
        osos::common::int8_t* strncpy(char* destination, const char* source, osos::common::size_t n);
        osos::common::int8_t* strcat(char* destination, const char* source);

        osos::common::int8_t* strchr(const char* string, osos::common::int32_t character);
        osos::common::int8_t* strrchr(const char* string, osos::common::int32_t character);
        osos::common::int8_t* strstr(const char* haystack, const char* needle);

        osos::common::int8_t* itoa(osos::common::int32_t value, char* string, osos::common::int32_t base);

        osos::common::int8_t* strtok(char* newstr, const char* delim);
    }
}

#endif