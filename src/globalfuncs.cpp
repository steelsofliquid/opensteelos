#include <globalfuncs.h>

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;
using namespace osos::libs;

extern volatile uint32_t tickCount;





int8_t* osos::GrabSysVer(int8_t sector)
{
    static int8_t verstring[32];
    verstring[0] = '\0';

    int8_t strMajor[4];
    int8_t strMinor[4];
    int8_t strBuild[4];

    itoa(verMajor, strMajor, 10);
    itoa(verMinor, strMinor, 10);
    itoa(verBuild, strBuild, 10);

    switch(sector)
    {
        case 'p': // partial - major + minor
        {
            strcpy (verstring, strMajor);
            strcat (verstring, ".");
            strcat (verstring, strMinor);

            break;
        }

        case 'm': // major
        {
            strcpy (verstring, strMajor);
            break;
        }

        case 'n': // minor
        {
            strcpy (verstring, strMinor);
            break;
        }

        case 'b': // build
        {
            strcpy (verstring, strBuild);
            break;
        }

        case 'f': // full - major + minor + build
        default:
        {
            strcpy (verstring, strMajor);
            strcat (verstring, ".");
            strcat (verstring, strMinor);
            strcat (verstring, ".");
            strcat (verstring, strBuild);

            break;
        }
    }

    return verstring;
}



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