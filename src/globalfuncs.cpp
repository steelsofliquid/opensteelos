#include <globalfuncs.h>

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;

extern volatile uint32_t tickCount;



void osos::HardSleep(uint32_t interval)
{
        // This function is a busy-wait loop that is only provided for specific use cases.
        // It should not be used in constant, heavy use cases unless you want to hang the
        // system.

        uint32_t initial = tickCount; // don't need to set the pit count, i presume

        while ((tickCount - initial) < interval)
        {
        }
}