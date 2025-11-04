#include <globalfuncs.h>

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;

extern volatile uint32_t tickcount;



void osos::HardSleep(uint32_t interval)
{
        uint32_t initial = tickcount; // don't need to set the pit count, i presume

        while ((tickcount - initial) < interval)
        {
        }
}