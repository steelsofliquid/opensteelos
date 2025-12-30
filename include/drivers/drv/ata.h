#ifndef __OSOS__DRIVERS__DRV__ATA_H
#define __OSOS__DRIVERS__DRV__ATA_H

#include <common/types.h>
#include <common/lib/libio.h>
#include <drivers/rs232.h>
#include <kernel/hwcom/port.h>

namespace osos
{
    namespace drivers
    {
        namespace drives
        {
            class AdvancedTechnologyAttachment
            {
                protected:
                osos::kernel::hwcom::Port16Bit dataPort;
                osos::kernel::hwcom::Port8Bit  errorPort;
                osos::kernel::hwcom::Port8Bit  sectorCountPort;
                osos::kernel::hwcom::Port8Bit  lbaLowPort;
                osos::kernel::hwcom::Port8Bit  lbaMidPort;
                osos::kernel::hwcom::Port8Bit  lbaHighPort;
                osos::kernel::hwcom::Port8Bit  devicePort;
                osos::kernel::hwcom::Port8Bit  commandPort;
                osos::kernel::hwcom::Port8Bit  controlPort;

                // This terminology would've traditionally been called "master" and "sl*ve", as part of that
                // terminology, but I said it once (in pic.cpp) and I'll say it again here:
                // That terminology is heavily outdated. I'd rather not get into this rabbit hole.
                // As such, we use "lead" and "follow".
                bool lead;

                uint16_t bytesPerSector;

                public:
                AdvancedTechnologyAttachment(uint16_t portBase, bool lead);
                ~AdvancedTechnologyAttachment();

                void IdentifyDrive();
                void Read28Bit(uint32_t sector, char* data, int count);
                void Write28Bit(uint32_t sector, char* data, int count);
                void FlushDrive(); // Flush>Drive< to differentiate from kernel-NRSh function Flush>Shell<.

                // TODO one day: Add a twin-journal system
            };
        }
    }
}

#endif