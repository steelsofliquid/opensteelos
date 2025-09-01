
#ifndef __OSOS__LIB__LIBSTR_H
#define __OSOS__LIB__LIBSTR_H

    #include <common/types.h>

    namespace osos
    {
        namespace libs
        {
            class StringLibrary
            {

            public:
            StringLibrary();
            ~StringLibrary();

            osos::common::size_t strlen(const osos::common::uint8_t* string);
            osos::common::int32_t strcmp(const osos::common::uint8_t* string1, const osos::common::uint8_t* string2);
            osos::common::int32_t strncmp(const osos::common::uint8_t* string1, const osos::common::uint8_t* string2, osos::common::size_t n);

            osos::common::uint8_t* strcpy(osos::common::uint8_t* destination, const osos::common::uint8_t* source);
            osos::common::uint8_t* strncpy(osos::common::uint8_t* destination, const osos::common::uint8_t* source, osos::common::size_t n);
            osos::common::uint8_t* strcat(osos::common::uint8_t* destination, const osos::common::uint8_t* source);

            osos::common::uint8_t* strchr(const osos::common::uint8_t* string, osos::common::uint32_t character);
            osos::common::uint8_t* strrchr(const osos::common::uint8_t* string, osos::common::uint32_t character);
            osos::common::uint8_t* strstr(const osos::common::uint8_t* haystack, const osos::common::uint8_t* needle);
            };
        }
    }

#endif