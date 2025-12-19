#include <lib/libcpu.h>

using namespace osos;
using namespace osos::common;
using namespace osos::libs;





static char vendor[13];
static char brand[49];

namespace osos
{
    namespace libs
    {
        void cpudet()
        {
            uint32_t a, b, c, d;
            cpuid(0, &a, &b, &c, &d);

            ((uint32_t*) vendor)[0] = b;
            ((uint32_t*) vendor)[1] = d;
            ((uint32_t*) vendor)[2] = c;
            vendor[12] = 0;

            uint32_t* p = (uint32_t*)brand;
            for (int i = 0; i < 3; i++) cpuid(0x80000002 + i, &p[i*4], &p[i*4+1], &p[i*4+2], &p[i*4+3]);

            brand[48] = 0;
        }



        const char* cpuven()
        {
            return vendor;
        }

        const char* cpubrd()
        {
            return brand;
        }
    }
}
