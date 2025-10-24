
// The first library to be developed for OpenSteel/OS. It contains memory functions.
// If you're looking for malloc and free, those are already defined in dmm.cpp and its header.

#include <lib/libmem.h>

using namespace osos;
using namespace osos::common;
using namespace osos::libs;


namespace osos{
    namespace libs {
        
        void* memcpy(void* destination, const void* source, size_t number)
        {
            // this shit obviously checks out easily - if you copy something, you just take a bit and set the destination to the source

            uint8_t* dest = (uint8_t*) destination;
            const uint8_t* sourc = (const uint8_t*) source;

            for (size_t i = 0; i < number; i++)
                dest[i] = sourc[i];

            return destination;
        }

        void* memmove(void* destination, const void* source, size_t number)
        {
            uint8_t* dest = (uint8_t*) destination;
            const uint8_t* sourc = (const uint8_t*) source;

            if (dest < sourc)
            {
                for (size_t i = 0; i < number; i++) // accidentally put i = number here
                    dest[i] = sourc[i];
            } else
            {
                for (size_t i = number; i != 0; i--)
                    dest[i - 1] = sourc[i - 1];
            }

            return destination;
        }

        void* memset(void* destination, int32_t value, size_t number)
        {
            // this one also checks out just as well as memcpy
            uint8_t* buffer = (uint8_t*) destination;

            for (size_t i = 0; i < number; i++)
                buffer[i] = (uint8_t) value;
            
            return destination;
        }

        uint32_t memcmp(const void* size1, const void* size2, size_t number)
        {
            const uint8_t* a = (const uint8_t*) size1;
            const uint8_t* b = (const uint8_t*) size2;

            for (size_t i = 0; i < number; i++)
            {
                if (a[i] < b[i])
                    return -1;
                else if (b[i] < a[i])
                    return 1;
            }

            return 0;
        }
    }

}