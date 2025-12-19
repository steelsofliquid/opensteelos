#ifndef __OSOS__LIB__LIBCPU_H
#define __OSOS__LIB__LIBCPU_H

#include <common/types.h>
#include <globalfuncs.h>

namespace osos
{
    namespace libs
    {
        void cpudet();

        const char* cpuven();
        const char* cpubrd();
    }
}

#endif
