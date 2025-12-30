#ifndef __OSOS__KERNEL__HWCOM__PIC_H
#define __OSOS__KERNEL__HWCOM__PIC_H

#include <common/types.h>
#include <common/lib/libasm.h>
#include <kernel/hwcom/port.h>

namespace osos
{
    namespace kernel
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

                osos::kernel::hwcom::Port8BitSlow picLeadCommand;
                osos::kernel::hwcom::Port8BitSlow picLeadData;
                osos::kernel::hwcom::Port8BitSlow picFollowCommand;
                osos::kernel::hwcom::Port8BitSlow picFollowData;

                ProgrammableInterruptController();
                ~ProgrammableInterruptController();

                void Initialise();
                void Disable();

                void MaskIRQ(uint8_t interruptReq);
                void UnmaskIRQ(uint8_t interruptReq);

                void SendEOI(uint8_t interruptRequest);
            };
        }
    }
}

#endif