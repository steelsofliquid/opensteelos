#include <drivers/drv/ata.h>

using namespace osos;
using namespace osos::drivers;
using namespace osos::drivers::drives;
using namespace osos::kernel;
using namespace osos::kernel::hwcom;

//static RecommendedStandard232Driver rs232AtaHelper;





AdvancedTechnologyAttachment::AdvancedTechnologyAttachment(uint16_t portBase, bool lead, uint8_t irqNumber, InterruptManager* manager) :
    dataPort        (portBase),
    errorPort       (portBase + 1),
    sectorCountPort (portBase + 2),
    lbaLowPort      (portBase + 3),
    lbaMidPort      (portBase + 4),
    lbaHighPort     (portBase + 5),
    devicePort      (portBase + 6),
    commandPort     (portBase + 7),
    controlPort     (portBase + 0x206),
    InterruptHandler(manager, irqNumber) // Unlike the RS232 or keyboard drivers, the IRQ number can't be hardcoded due to different HDDs using different IRQs
{
    bytesPerSector = 512;
    this->lead = lead;

    driverAttributes.name      = "ATA Hard Disk Driver";
    driverAttributes.publisher = "SteelsOfLiquid";
    driverAttributes.type      = "drives";

    driverAttributes.isInitialised = false;
    driverAttributes.isActive      = false;

    driverAttributes.hasInterruptRequest  = true;
    driverAttributes.interruptRequestLine = (interruptNumber - 0x20); // de facto. could also be 0x0F
    driverAttributes.vectorOffset         = interruptNumber;
}

AdvancedTechnologyAttachment::~AdvancedTechnologyAttachment()
{
}

bool AdvancedTechnologyAttachment::PollDrive(bool checkDrqBit)
{
    uint8_t status;

    while ((status = commandPort.Read()) & 0x80);

    if (status & 0x01) return false;
    if (status & 0x20) return false;
    if (checkDrqBit && !(status & 0x08)) return false;

    return true;
}

void AdvancedTechnologyAttachment::IdentifyDrive()
{
    devicePort.Write(lead ? 0xA0 : 0xB0);
    controlPort.Write(0);

    devicePort.Write(0xA0);
    for (int i = 0; i < 4; i++) commandPort.Read();
    uint8_t status = commandPort.Read();
    if (status == 0xFF) return;

    devicePort.Write(lead ? 0xA0 : 0xB0);
    sectorCountPort.Write(0);
    lbaLowPort.Write(0);
    lbaMidPort.Write(0);
    lbaHighPort.Write(0);
    commandPort.Write(0xEC);

    status = commandPort.Read();
    if (status == 0xFF) return;

    if (!PollDrive(true)) return;

    if (status & 0x01)
    {
        //rs232AtaHelper.WriteString("ERROR");
        printf("\nError trying to recognise the hard disk.                                   %R[err]%R", 0x0C, 0x0F);
        return;
    }

    for (uint16_t i = 0; i < 256; i++)
    {
        uint16_t data = dataPort.Read();
    }
    //rs232AtaHelper.WriteString("DISK OK");
}

void AdvancedTechnologyAttachment::Read28Bit(uint32_t sector, char* data, int count)
{
    if (sector & 0xF0000000) return;
    if (count > bytesPerSector) return;

    devicePort.Write((lead ? 0xE0 : 0xF0) | ((sector & 0x0F000000) >> 24));
    errorPort.Write(0);
    sectorCountPort.Write(1);

    lbaLowPort.Write ( sector & 0x000000FF );
    lbaMidPort.Write ((sector & 0x0000FF00) >> 8);
    lbaHighPort.Write((sector & 0x00FF0000) >> 16);
    commandPort.Write(0x20);

    uint8_t status = commandPort.Read();
    if (!PollDrive(true)) return;

    if (status & 0x01)
    {
        //rs232AtaHelper.WriteString("\n<!> Error trying to read HDD.");
        return;
    }

    for (uint16_t i = 0; i < count; i += 2)
    {
        uint16_t writeData = dataPort.Read();

        data[i] = writeData & 0x00FF;
        if ((i + 1) < count) data[i + 1] = (writeData >> 8) & 0x00FF;
        //printf("%c%c", data[i], (i + 1 < count) ? data[i + 1] : 0);
    }

    for (uint16_t i = count + (count % 2); i < bytesPerSector; i += 2) dataPort.Read();
}

void AdvancedTechnologyAttachment::Write28Bit(uint32_t sector, char* data, int count)
{
    if (sector & 0xF0000000) return;
    if (count > bytesPerSector) return;

    devicePort.Write((lead ? 0xE0 : 0xF0) | ((sector & 0x0F000000) >> 24));
    errorPort.Write(0);
    sectorCountPort.Write(1);

    lbaLowPort.Write ( sector & 0x000000FF );
    lbaMidPort.Write ((sector & 0x0000FF00) >> 8);
    lbaHighPort.Write((sector & 0x00FF0000) >> 16);
    commandPort.Write(0x30);

    if (!PollDrive(true)) return;

    for (uint16_t i = 0; i < count; i += 2)
    {
        uint16_t writeData = (uint8_t)data[i];
        if ((i + 1) < count) writeData |= ((uint16_t)(uint8_t)data[i + 1]) << 8;

        dataPort.Write(writeData);
    }

    for (uint16_t i = count + (count % 2); i < bytesPerSector; i += 2) dataPort.Write(0x0000);
    if (!PollDrive(false)) return;
    FlushDrive();
}

void AdvancedTechnologyAttachment::FlushDrive()
{
    devicePort.Write(lead ? 0xE0 : 0xF0);
    commandPort.Write(0xE7);

    uint8_t status = commandPort.Read();
    while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) status = commandPort.Read();

    if (status & 0x01)
    {
        //rs232AtaHelper.WriteString("\n<!> Error trying to flush HDD.");
        return;
    }
}