#ifndef __OSOS__HWCOM__PIC_H
#define __OSOS__HWCOM__PIC_H

#include <common/types.h>
#include <hwcom/port.h>
#include <globalfuncs.h>

namespace osos
{
    namespace hwcom
    {
        class ProgrammableInterruptController
        {
            public:
            /*
            Up until 2025-06-27, "Lead" was called "Master"; Follow was called "Slave" but people weren't as aware of the games they played in 1904 or so,
            and I know well enough from the history videos I watch that the historical terms here are just messed up. Shouldn't be too hard to rename each
            use in the code.
            */

            osos::hwcom::Port8BitSlow picLeadCommand;
            osos::hwcom::Port8BitSlow picLeadData;
            osos::hwcom::Port8BitSlow picFollowCommand;
            osos::hwcom::Port8BitSlow picFollowData;

            ProgrammableInterruptController();
            ~ProgrammableInterruptController();

            void Initialise();
            void Disable();

            void SendEOI(osos::common::uint8_t interruptRequest);
        };
    }
}

#endif