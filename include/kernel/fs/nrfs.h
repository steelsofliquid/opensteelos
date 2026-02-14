#ifndef __OSOS__KERNEL__FS__NRFS_H
#define __OSOS__KERNEL__FS__NRFS_H

#include <common/types.h>
#include <drivers/drv/ata.h>

namespace osos
{
    namespace kernel
    {
        namespace filesystem
        {
            class NathanRenaudFileSystem
            {
                public:
                NathanRenaudFileSystem();
                ~NathanRenaudFileSystem();
            };
        }
    }
}

#endif