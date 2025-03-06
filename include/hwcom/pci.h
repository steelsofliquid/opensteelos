
// PCI is short for "Peripheral Component Interconnect".
// (I just shortened it to PCI here though the video uses the full
// name of this system (PCI is what I'm referring to) as I want my code
// to be rather simple)

// This file focuses on getting NanamiOS to work with PCI devices.

// To learn more about PCI, you can read up on any info about it
// on Wikipedia. Just like you can look up barebones info about Nanami Madobe
// there, too!

#ifndef __OSOS__HWCOM__PCI_H
#define __OSOS__HWCOM__PCI_H

#include <hwcom/port.h>
#include <common/types.h>
#include <hwcom/interrupts.h>
#include <drivers/driver.h>

#include <dmm.h>

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

            osos::common::uint16_t vendor_id; // Up to 65 thousand vendor IDs can be assigned.
            osos::common::uint16_t device_id;

            osos::common::uint8_t class_id;
            osos::common::uint8_t subclass_id;
            osos::common::uint8_t interface_id;

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

            osos::common::uint32_t PCIRead(osos::common::uint16_t bus, osos::common::uint16_t device, osos::common::uint16_t function, osos::common::uint32_t registeroffset);
            void PCIWrite(osos::common::uint16_t bus, osos::common::uint16_t device, osos::common::uint16_t function, osos::common::uint32_t registeroffset, 
                osos::common::uint32_t value);
            bool DeviceHasFunctions(osos::common::uint16_t bus, osos::common::uint16_t device);

            void SelectDrivers(osos::drivers::DriverManager* driverManager, osos::hwcom::InterruptManager* interrupts);
            osos::drivers::Driver* GetDriver(PCIDevDesc dev, osos::hwcom::InterruptManager* interrupts);
            PCIDevDesc GetDeviceDescriptor(osos::common::uint16_t bus, osos::common::uint16_t device, osos::common::uint16_t function);
            BaseAddressRegister GetBasAdrReg(osos::common::uint16_t bus, osos::common::uint16_t device, osos::common::uint16_t function, osos::common::uint16_t bar);

        };
    }
}

#endif
