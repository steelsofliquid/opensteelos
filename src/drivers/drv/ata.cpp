#include <drivers/drv/ata.h>

using namespace osos;
using namespace osos::drivers;
using namespace osos::drivers::drives;
using namespace osos::kernel;
using namespace osos::kernel::hwcom;

//static RecommendedStandard232Driver rs232AtaHelper;





AdvancedTechnologyAttachment::AdvancedTechnologyAttachment(uint16_t portBase, bool lead) :
    dataPort        (portBase),
    errorPort       (portBase + 1),
    sectorCountPort (portBase + 2),
    lbaLowPort      (portBase + 3),
    lbaMidPort      (portBase + 4),
    lbaHighPort     (portBase + 5),
    devicePort      (portBase + 6),
    commandPort     (portBase + 7),
    controlPort     (portBase + 0x206)
{
    bytesPerSector = 512;
    this->lead = lead;
}

AdvancedTechnologyAttachment::~AdvancedTechnologyAttachment()
{
}

void AdvancedTechnologyAttachment::IdentifyDrive()
{
    devicePort.Write(lead ? 0xA0 : 0xB0);
    controlPort.Write(0);

    devicePort.Write(0xA0);
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

    while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) status = commandPort.Read();

    if (status & 0x01)
    {
        //rs232AtaHelper.WriteString("ERROR");
        printf("ERROR");
        return;
    }

    for (uint16_t i = 0; i < 256; i++)
    {
        uint16_t data = dataPort.Read();
        printf("%x", data);
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
    while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) status = commandPort.Read();

    if (status & 0x01)
    {
        //rs232AtaHelper.WriteString("\n<!> Error trying to read HDD.");
        return;
    }

    for (uint16_t i = 0; i < count; i += 2)
    {
        uint16_t writeData = dataPort.Read();
        dataPort.Write(writeData);

        data[i] = writeData & 0x00FF;
        printf("%c", data[i]);
        if ((i + 1) < count) data[i + 1] = (writeData >> 8) & 0x00FF;
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

    for (uint16_t i = 0; i < count; i += 2)
    {
        uint16_t writeData = data[i];
        if ((i + 1) < count) writeData |= ((uint16_t)data[i + 1]) << 8;

        dataPort.Write(writeData);
    }

    for (uint16_t i = count + (count % 2); i < bytesPerSector; i += 2) dataPort.Write(0x0000);
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