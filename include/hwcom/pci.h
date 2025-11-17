
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

#ifndef __OSOS__HWCOM__PCI_H
#define __OSOS__HWCOM__PCI_H

#include <hwcom/port.h>
#include <common/types.h>
#include <hwcom/interrupts.h>
#include <drivers/driver.h>

#include <dmm.h>
// #include <drivers/amd_am79c973.h>.h>>

namespace osos
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
            osos::common::uint8_t* address;
            osos::common::uint32_t size;
            BaseAddressRegisterType type;

        };

        class PCIDevDesc // Short for "PCI Device Descriptor", or "PeripheralComponentInterconnectDeviceDescriptor".
        {
        public:
            osos::common::uint32_t portBase;
            osos::common::uint32_t interrupt; // I don't remember this being in the interrupts header...

            osos::common::uint16_t bus;
            osos::common::uint16_t device;
            osos::common::uint16_t function;

            osos::common::uint16_t vendorId; // Up to 65 thousand vendor IDs can be assigned.
            osos::common::uint16_t deviceId;

            osos::common::uint8_t classId;
            osos::common::uint8_t subclassId;
            osos::common::uint8_t interfaceId;

            osos::common::uint8_t revision;


            PCIDevDesc();
            ~PCIDevDesc();

        };

        class PCIController
        {
            osos::hwcom::Port32Bit dataPort;
            osos::hwcom::Port32Bit commandPort;

        public:
            PCIController();
            ~PCIController();

            osos::common::uint32_t PCIRead(osos::common::uint16_t bus, osos::common::uint16_t device, osos::common::uint16_t function, osos::common::uint32_t registerOffset);
            void PCIWrite(osos::common::uint16_t bus, osos::common::uint16_t device, osos::common::uint16_t function, osos::common::uint32_t registerOffset, 
                osos::common::uint32_t value);
            bool deviceHasFunctions(osos::common::uint16_t bus, osos::common::uint16_t device);

            void SelectDrivers(osos::drivers::DriverManager* driverManager, osos::hwcom::InterruptManager* interrupts);
            osos::drivers::Driver* GetDriver(PCIDevDesc dev, osos::hwcom::InterruptManager* interrupts);
            PCIDevDesc GetDeviceDescriptor(osos::common::uint16_t bus, osos::common::uint16_t device, osos::common::uint16_t function);
            BaseAddressRegister GetBasAdrReg(osos::common::uint16_t bus, osos::common::uint16_t device, osos::common::uint16_t function, osos::common::uint16_t bar);

        };
    }
}

#endif
