
#ifndef __OSOS__LIB__LIBMEM_H
#define __OSOS__LIB__LIBMEM_H

    #include <common/types.h>

    namespace osos
    {
        namespace libs
        {
            class MemoryLibrary
            {

            public:
            MemoryLibrary();
            ~MemoryLibrary();

            void* memcpy(void* destination, const void* source, osos::common::size_t number);
            void* memmove(void* destination, const void* source, osos::common::size_t number);
            void* memset(void* destination, osos::common::int32_t value, osos::common::size_t number);
            osos::common::uint32_t memcmp(const void* size1, const void* size2, osos::common::size_t number);
            };
        }
    }

#endif