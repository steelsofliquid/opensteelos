#include <hwcom/pci.h>
using namespace osos;
using namespace osos::common;
using namespace osos::drivers;
using namespace osos::hwcom;


PCIDevDesc::PCIDevDesc()
{
}

PCIDevDesc::~PCIDevDesc()
{
}

PCIController::PCIController()
: dataPort(0xCFC),
  commandPort(0xCF8)
{
}

PCIController::~PCIController()
{
}

uint32_t PCIController::PCIRead(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset)
{
    uint32_t id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC);
    commandPort.Write(id);
    uint32_t result = dataPort.Read();
    return result >> (8* (registeroffset % 4));

}

/* Build 26 on 2023 November 6th:
I named these two things "PCIRead" and "PCIWrite" because I didn't want to run into problems
with the normal "Read" and "Write" code. The original code stated "PerhipheralComponentInterconnectController", "Read", and "Write". */

void PCIController::PCIWrite(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value)
{
    uint32_t id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC);
    commandPort.Write(id);
    dataPort.Write(value);
    
}

bool PCIController::DeviceHasFunctions(uint16_t bus, uint16_t device) // device has functions?
{
    return PCIRead(bus, device, 0, 0x0E) & (1<<7);
}

void printf(char* str);
void printfHex(uint8_t);

void PCIController::SelectDrivers(DriverManager* driverManager, InterruptManager* interrupts)
{
    for(int bus = 0; bus < 8; bus++)
    {
        for(int device = 0; device < 32; device++)
        {
            int numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;
            for(int function = 0; function < numFunctions; function++)
            {
                PCIDevDesc dev = GetDeviceDescriptor(bus, device, function);

                if(dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                    continue; // What the fuck? Kyaaaaa, these indents... >_<
                
                for(int barNum = 0; barNum < 6; barNum++)
                {
                    BaseAddressRegister bar = GetBasAdrReg(bus, device, function, barNum);
                    if(bar.address && (bar.type == InputOutput))
                        dev.portBase = (uint32_t)bar.address; // Scrap the comment above. We're six layers deep. I'm sorry Linus torvalds... >_<
                    
                    Driver* driver = GetDriver(dev, interrupts);
                    if(driver != 0)
                        driverManager->AddDriver(driver);
                }

                printf("\n[pci] bus ");
                printfHex(bus & 0xFF);

                printf(" dev ");
                printfHex(device & 0xFF);

                printf(" fun ");
                printfHex(function & 0xFF);

                printf(" venid ");
                printfHex((dev.vendor_id & 0xFF00) >> 8);
                printfHex(dev.vendor_id & 0xFF);
                printf(" devid ");
                printfHex((dev.device_id & 0xFF00) >> 8);
                printfHex(dev.device_id & 0xFF);

                // "//" the printf(s) if your verbose-phobic or just insist everything must be understandable to the end user. Verbose boot shit should either
                // awe the end user or give them a reason for you to explain the inner workings.
            }
        }
    }
}

BaseAddressRegister PCIController::GetBasAdrReg(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar)
{
    BaseAddressRegister result;
    // Bars!

    uint32_t headertype = PCIRead(bus, device, function, 0x0E) & 0x7F;
    int maxBARs = 6 - (4*headertype);
    if(bar >= maxBARs)
        return result;

    uint32_t bar_value = PCIRead(bus, device, function, 0x10 + 4*bar);
    result.type = (bar_value & 0x1) ? InputOutput : MemoryMapping;
    uint32_t temp;

    if(result.type == MemoryMapping)
    {

        switch((bar_value >> 1) & 0x3)
        {
            case 0: // [in an angry gamer nerd voice] 32 bits.
            case 1: // 20 bits!? THAT DOESN'T EVEN MAKE SENSE!! WHY IS IT 20?
            case 2: // 64 bits.
                break;
        }

        result.prefetchable = ((bar_value >> 3) & 0x1) == 0x1;
    }
    else
    {
        result.address = (uint8_t*)(bar_value & ~0x3);
        result.prefetchable = false;
    }

    return result;
}

Driver* PCIController::GetDriver(PCIDevDesc dev, InterruptManager* interrupts)
{
    Driver *driver = 0;
    switch(dev.vendor_id)
    {
        case 0x1022: // AMD
            switch(dev.device_id)
            {
                /* case 0x2000: // idk what this is, no device repository exists yet >_<
                    driver = (Driver*)MemoryManager::activeMemoryManager->malloc(sizeof(amd_am79c973));
                    if(driver != 0)
                        new (driver) amd_am79c973(&dev, interrupts);
                    // printf("[pci] Device Found! AMD am79c973\n");
                    break;
                // Supposed to be an AM79C973 driver.
                */
            }
            break;
        
        case 0x8086: // Intel
            break;
    }

    switch(dev.class_id)
    {
        case 0x03: // for generic graphics
            switch(dev.subclass_id)
            {
                case 0x00: // VGA graphics. Like my ancient ahh ViewSonic TX-Series CRT from 1991!
                    break;
            }
            break;
    }

    return driver;
}

PCIDevDesc PCIController::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function)
{
    PCIDevDesc result;

    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = PCIRead(bus, device, function, 0x00);
    result.device_id = PCIRead(bus, device, function, 0x02);

    result.class_id = PCIRead(bus, device, function, 0x0B);
    result.subclass_id = PCIRead(bus, device, function, 0x0A);
    result.interface_id = PCIRead(bus, device, function, 0x09);

    result.revision = PCIRead(bus, device, function, 0x08);
    result.interrupt = PCIRead(bus, device, function, 0x3C);

    return result;
}

