// The OpenSteel/OS RS232 (COM and Serial) driver.

#include <drivers/rs232.h>

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;
using namespace osos::hwcom;

RecommendedStandard232Driver::RecommendedStandard232Driver() :
    COM1(0x3F8)
{
}

RecommendedStandard232Driver::~RecommendedStandard232Driver()
{
}



int32_t RecommendedStandard232Driver::InitialiseSerial()
{
    asm volatile ("cli");
    outb(0x3F8 + 3, 0x80);
    outb(0x3F8 + 0, 0x03);
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x03);
    outb(0x3F8 + 2, 0xC7);
    outb(0x3F8 + 4, 0x0B);
    outb(0x3F8 + 4, 0x1E);
    outb(0x3F8 + 0, 0xAE);

    if (inb(0x3F8 + 0) != 0xAE) return 1;

    outb(0x3F8 + 4, 0x0F);
    asm volatile ("sti");
    return 0;
}



bool RecommendedStandard232Driver::thrBitReady()
{
}

bool RecommendedStandard232Driver::drBitReady()
{
}



uint8_t RecommendedStandard232Driver::GetLineStatus()
{
}

uint8_t RecommendedStandard232Driver::GetModemStatus()
{
}

bool RecommendedStandard232Driver::hasError()
{
}

RecommendedStandard232Driver::LineErrorType RecommendedStandard232Driver::GetError()
{
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

void RecommendedStandard232Driver::WriteToSerial(uint8_t material)
{
    while (isTransitEmpty() == 0);
    COM1.Write(material);
}



void RecommendedStandard232Driver::Activate()
{
}

uint32_t RecommendedStandard232Driver::HandleInterrupt(uint32_t esp)
{
}