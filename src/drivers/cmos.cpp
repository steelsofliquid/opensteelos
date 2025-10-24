// A legitimate purpose-built driver, holy shit
// though honestly i may not need it

#include <drivers/cmos.h>

using namespace osos;
using namespace osos::common;
using namespace osos::hwcom;
using namespace osos::drivers;



ClockBatteryDriver::ClockBatteryDriver() :
    portAddressCMOS(0x70),
    portDataCMOS(0x71)
{
}

ClockBatteryDriver::~ClockBatteryDriver()
{
}


// private functions are a subject of personal debate - should I add these to the thing? would it make things easier or harder?
void ClockBatteryDriver::GetIndex(uint8_t register)
{
}

void ClockBatteryDriver::ReadData()
{
}

void ClockBatteryDriver::WriteData(uint8_t value)
{
}



void ClockBatteryDriver::ReadCMOS(int32_t port)
{
    uint8_t tvalue, index;
    for (index = 0; index < 128; index++)
    {
        // is this really the best way to write it?

        asm("cli");
        portAddressCMOS.Write(port);
        portDataCMOS.Read();
        asm("sti");
    }
}

int ClockBatteryDriver::WriteCMOS(int32_t port, int32_t value)
{
    asm("cli");
    portAddressCMOS.Write(port);
    portDataCMOS.Write(value);
    asm("sti");
}


int ClockBatteryDriver::GetProgressUpdateFlag()
{
    WriteCMOS(0x70, 0x0A);
}

uint8_t ClockBatteryDriver::GetRTCregister(int32_t register)
{
}

void ClockBatteryDriver::ReadRTC()
{
}

void ClockBatteryDriver::GrabSystemTime()
{
}