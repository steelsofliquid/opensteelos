#ifndef __GLOBALFUNCS_H
#define __GLOBALFUNCS_H

#include <common/types.h>
#include <common/version.h>
// #include <drivers/pit.h>
#include <common/lib/libstr.h>

namespace osos
{
    // void InitialiseHardSleep();


    // --- system APIs ---
    // GrabSys APIs
    // DO NOT USE!!
    int8_t* GrabSysVer(int8_t sector);

    // hardware materials
    void HardSleep(uint32_t interval);
}

#endif