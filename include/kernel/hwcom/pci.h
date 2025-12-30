
// PCI is short for "Peripheral Component Interconnect".
// (I just shortened it to PCI here though the video uses the full
// name of this system (PCI is what I'm referring to) as I want my code
// to be rather simple)

// This file focuses on getting OpenSteel/OS to work with PCI devices.

// To learn more about PCI, you can read up on any info about it
// on Wikipedia. Unlike information about Neisa Sapphira-Aera.

// 2025-11-06 amendment: Shouldn't have shortened Peripheral
// Component Interconnect to PCI. While simpler, it also would make some
// kinda mad. Why can't we all have very simple terms?? T_T

#ifndef __OSOS__KERNEL__HWCOM__PCI_H
#define __OSOS__KERNEL__HWCOM__PCI_H

#include <common/types.h>
#include <common/lib/libio.h>
#include <kernel/hwcom/driverModel.h>
#include <kernel/hwcom/driverManager.h>
#include <kernel/hwcom/interrupts.h>
#include <kernel/hwcom/port.h>
#include <kernel/mem/dmm.h>
// #include <drivers/amd_am79c973.h>.h>>

namespace osos
{
    namespace kernel
    {
        namespace hwcom
        {
            enum BaseAddressRegisterType
            {
                MemoryMapping = 0,
                InputOutput = 1,

            };

            class BaseAddressRegister
            {
                public:
                bool prefetchable;
                uint8_t* address;
                uint32_t size;
                BaseAddressRegisterType type;

            };

            class PCIDevDesc // Short for "PCI Device Descriptor", or "PeripheralComponentInterconnectDeviceDescriptor".
            {
                public:
                uint32_t portBase;
                uint32_t interrupt; // I don't remember this being in the interrupts header...

                uint16_t bus;
                uint16_t device;
                uint16_t function;

                uint16_t vendorId; // Up to 65 thousand vendor IDs can be assigned.
                uint16_t deviceId;

                uint8_t classId;
                uint8_t subclassId;
                uint8_t interfaceId;

                uint8_t revision;


                PCIDevDesc();
                ~PCIDevDesc();

            };

            class PCIController
            {
                osos::kernel::hwcom::Port32Bit dataPort;
                osos::kernel::hwcom::Port32Bit commandPort;

                public:
                PCIController();
                ~PCIController();

                uint32_t PCIRead(uint16_t bus, uint16_t device, uint16_t function, uint32_t registerOffset);
                void PCIWrite(uint16_t bus, uint16_t device, uint16_t function, uint32_t registerOffset, 
                    uint32_t value);
                bool deviceHasFunctions(uint16_t bus, uint16_t device);

                void SelectDrivers(osos::kernel::hwcom::DriverManager* driverManager, osos::kernel::hwcom::InterruptManager* interrupts);
                osos::kernel::hwcom::DriverModel* GetDriver(PCIDevDesc dev, osos::kernel::hwcom::InterruptManager* interrupts);
                PCIDevDesc GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function);
                BaseAddressRegister GetBasAdrReg(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar);

            };
        }
    }
}

#endif
