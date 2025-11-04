#ifndef __OSOS__DRIVERS__AMD_AM79C973_H
#define __OSOS__DRIVERS__AMD_AM79C973_H

#include <common/types.h>
#include <drivers/driver.h>
#include <hwcom/pci.h>
#include <hwcom/port.h>
#include <hwcom/interrupts.h>

namespace osos
{
    namespace drivers
    {

        class PCNetFastIII : public Driver, public hwcom::InterruptHandler
        {
                struct InitializeBlock
                {
                    osos::common::uint16_t mode;

                    unsigned reserved1 : 4;
                    unsigned numSendBuffers : 4;
                    unsigned reserved2 : 4;
                    unsigned numRecvBuffers : 4;

                    osos::common::uint64_t physicalAddress : 48;
                    osos::common::uint16_t reserved3;
                    osos::common::uint64_t logicalAddress;

                    osos::common::uint32_t recvBufferDescrAddress;
                    osos::common::uint32_t sendBufferDescrAddress;
                };

                struct BufferDescriptor
                {
                    osos::common::uint32_t address;
                    osos::common::uint32_t flags1;
                    osos::common::uint32_t flags2;
                    osos::common::uint32_t avail;
                } __attribute__((packed));
        
                osos::hwcom::Port16Bit MACAddress0Port;
                osos::hwcom::Port16Bit MACAddress2Port;
                osos::hwcom::Port16Bit MACAddress4Port;
                osos::hwcom::Port16Bit registerDataPort;
                osos::hwcom::Port16Bit registerAddressPort;
                osos::hwcom::Port16Bit resetPort;
                osos::hwcom::Port16Bit busControlregdataPort;

                InitializeBlock initBloc;

                BufferDescriptor* sendBufferDescr;
                osos::common::uint8_t sendBufferDescrMemory[2048+15];
                osos::common::uint8_t sendBuffers[2*1024+15][8];
                osos::common::uint8_t currentSendBuffer;

                BufferDescriptor* recvBufferDescr;
                osos::common::uint8_t recvBufferDescrMemory[2048+15];
                osos::common::uint8_t recvBuffers[2*1024+15][8];
                osos::common::uint8_t currentRecvBuffer;

                public:
                PCNetFastIII(osos::hwcom::PCIDevDesc dev, osos::hwcom::InterruptManager* interrupts);
                ~PCNetFastIII();

                virtual void Activate();
                virtual int Reset();
                virtual osos::common::uint32_t HandleInterrupt(osos::common::uint32_t esp);
        };
    }
}


#endif