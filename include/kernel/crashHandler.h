#include <common/types.h>
#include <common/lib/libio.h>
#include <common/version.h>

namespace osos
{
    namespace kernel
    {
        // Crash handler logic and function
        void panic(uint32_t errorId);
    }
}