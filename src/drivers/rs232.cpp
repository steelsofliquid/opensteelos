// The OpenSteel/OS RS232 (COM and Serial) driver.

#include <drivers/rs232.h>

using namespace osos;
using namespace osos::drivers;
using namespace osos::kernel;
using namespace osos::kernel::hwcom;

RecommendedStandard232Driver::RecommendedStandard232Driver(InterruptManager* manager) :
    InterruptHandler(manager, 0x24),
    COM1(0x3F8)
{
    driverAttributes.name      = "RS232 Serial Port Driver";
    driverAttributes.publisher = "SteelsOfLiquid";
    driverAttributes.type      = "inorian"; // TODO: replace the generic typename with what would be most viable

    driverAttributes.isInitialised = false;
    driverAttributes.isActive      = false;

    driverAttributes.hasInterruptRequest  = true;
    driverAttributes.interruptRequestLine = 0x04;
    driverAttributes.vectorOffset         = interruptNumber;
}

RecommendedStandard232Driver::~RecommendedStandard232Driver()
{
}



int32_t RecommendedStandard232Driver::InitialiseSerial()
{
    outb(0x3F8 + 3, 0x80);
    outb(0x3F8 + 0, 0x03);
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x03);
    outb(0x3F8 + 1, 0x00);
    //outb(0x3F8 + 4, 0x0B); // this doesn't seem very useful.
    outb(0x3F8 + 4, 0x1E);
    while ((inb(0x3F8 + 5) & 0x20) == 0); outb(0x3F8 + 0, 0xAE);

    while ((inb(0x3F8 + 5) & 0x01) == 0);
    if (inb(0x3F8 + 0) != 0xAE)
    {
        printf("\nSerial loopback test failed during initialisation.                         %R[err]%R", 0x0C, 0x0F);
        return 1;
    }

    outb(0x3F8 + 4, 0x0F);
    outb(0x3F8 + 2, 0xC7);
    return 0;
}



uint8_t RecommendedStandard232Driver::GetLineStatus()
{
    return inb(0x3F8 + 5);
}

uint8_t RecommendedStandard232Driver::GetModemStatus()
{
    return inb(0x3F8 + 6);
}

bool RecommendedStandard232Driver::hasError()
{
    uint8_t lsrStat = inb(0x3F8 + 5);
    return (lsrStat & 0x1E) != 0;
}

RecommendedStandard232Driver::LineErrorType RecommendedStandard232Driver::GetError()
{
    uint8_t lsrStat = inb(0x3F8 + 5);
    return static_cast<LineErrorType>(lsrStat & 0x1E);
}



int32_t RecommendedStandard232Driver::serialReceived()
{
    return (inb(0x3F8 + 5) & 1);
}

int8_t RecommendedStandard232Driver::ReadSerial()
{
    while (serialReceived() == 0);
    return COM1.Read();
}

int32_t RecommendedStandard232Driver::isTransitEmpty()
{
    return inb(0x3F8 + 5) & 0x20;
}

void RecommendedStandard232Driver::WriteToSerial(char material)
{
    while (isTransitEmpty() == 0);
    COM1.Write(material);
}

void RecommendedStandard232Driver::WriteString(const char* material)
{
    while (*material) WriteToSerial(*material++);
}



void RecommendedStandard232Driver::StartDriver()
{
    InitialiseSerial();
}

uint32_t RecommendedStandard232Driver::HandleInterrupt(uint32_t esp)
{
    uint8_t iir = inb(0x3F8 + 2);
    return esp;
}