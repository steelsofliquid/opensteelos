#include <globalfuncs.h>

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;
using namespace osos::libs;

extern volatile uint32_t tickCount;

extern int32_t verMajor;
extern int32_t verMinor;
extern int32_t verBuild;



int8_t* osos::GrabSysVer(int8_t sector)
{
        int8_t* verstring;
        verstring[0] = '\0';

        switch(sector)
        {
                case 'p': // partial - major + minor
                {
                        int8_t* strMajor;
                        int8_t* strMinor;

                        itoa(verMajor, strMajor, 10);
                        itoa(verMinor, strMinor, 10);

                        int8_t* strResult[2] = {strMajor, strMinor};

                        for (int i = 0; i != 2; i++)
                        {
                                verstring = strcat(verstring, strResult[i]);
                                if (i != 2) verstring = strcat(verstring, ".");
                        }

                        break;
                }

                case 'm': // major
                {
                        int8_t* strMajor;
                        itoa(verMajor, strMajor, 10);

                        verstring = strMajor;
                        break;
                }

                case 'n': // minor
                {
                        int8_t* strMinor;
                        itoa(verMinor, strMinor, 10);

                        verstring = strMinor;
                        break;
                }

                case 'b': // build
                {
                        int8_t* strBuild;
                        itoa(verBuild, strBuild, 10);

                        verstring = strBuild;
                        break;
                }

                case 'f': // full - major + minor + build
                default:
                {
                        int8_t* strMajor;
                        int8_t* strMinor;
                        int8_t* strBuild;

                        itoa(verMajor, strMajor, 10);
                        itoa(verMinor, strMinor, 10);
                        itoa(verBuild, strBuild, 10);

                        int8_t* strResult[3] = {strMajor, strMinor, strBuild};

                        for (int i = 0; i != 3; i++)
                        {
                                verstring = strcat(verstring, strResult[i]);
                                if (i != 3) verstring = strcat(verstring, ".");
                        }

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