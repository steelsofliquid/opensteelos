// A legitimate purpose-built driver, holy shit
// though honestly i may not need it

#include <drivers/cmos.h>

#define BUILDYEAR       2025

using namespace osos;
using namespace osos::common;
using namespace osos::hwcom;
using namespace osos::drivers;

int centuryRegister = 0x00; // Set by ACPI table parsing



ClockBatteryDriver::ClockBatteryDriver() :
    portAddressCMOS(0x70),
    portDataCMOS(0x71)
{
}

ClockBatteryDriver::~ClockBatteryDriver()
{
}


// private functions are a subject of personal debate - should I add these to the thing? would it make things easier or harder?
void ClockBatteryDriver::GetIndex(uint8_t reg)
{
}

void ClockBatteryDriver::ReadData()
{
}

void ClockBatteryDriver::WriteData(uint8_t value)
{
}



uint8_t ClockBatteryDriver::ReadCMOS(int32_t port)
{
    uint8_t value;

    asm volatile("cli");
    portAddressCMOS.Write(port & 0x7F);
    value = portDataCMOS.Read();
    asm volatile("sti");

    return value;
}

void ClockBatteryDriver::WriteCMOS(int32_t port, int32_t value)
{
    asm volatile("cli");
    portAddressCMOS.Write(port & 0x7F);
    portDataCMOS.Write(value);
    asm volatile("sti");
}


int ClockBatteryDriver::GetProgressUpdateFlag()
{
    WriteCMOS(0x70, 0x0A);
    return (ReadCMOS(0x0A) & 0x80);
}

uint8_t ClockBatteryDriver::GetRTCregister(int32_t reg)
{
    WriteCMOS(0x70, reg);
    return ReadCMOS(reg);
}

RealTimeClockRegisters ClockBatteryDriver::ReadRTC()
{
    uint8_t century;
    uint8_t lastSecond;
    uint8_t lastMinute;
    uint8_t lastHour;
    uint8_t lastDay;
    uint8_t lastMonth;
    uint8_t lastYear;
    uint8_t lastCentury;

    uint8_t regB;

    RealTimeClockRegisters clockRegisters;
    while(GetProgressUpdateFlag());        // are we updating?

    clockRegisters.second = GetRTCregister(0x00);
    clockRegisters.minute = GetRTCregister(0x02);
    clockRegisters.hour   = GetRTCregister(0x04);
    clockRegisters.day    = GetRTCregister(0x07);
    clockRegisters.month  = GetRTCregister(0x08);
    clockRegisters.year   = GetRTCregister(0x09);
    if (centuryRegister != 0) century = GetRTCregister(centuryRegister);

    do
    {
        lastSecond  = clockRegisters.second;
        lastMinute  = clockRegisters.minute;
        lastHour    = clockRegisters.hour;
        lastDay     = clockRegisters.day;
        lastMonth   = clockRegisters.month;
        lastYear    = clockRegisters.year;
        lastCentury = century;

        while(GetProgressUpdateFlag());        // are we updating?

        // and then this block, again...
        clockRegisters.second = GetRTCregister(0x00);
        clockRegisters.minute = GetRTCregister(0x02);
        clockRegisters.hour   = GetRTCregister(0x04);
        clockRegisters.day    = GetRTCregister(0x07);
        clockRegisters.month  = GetRTCregister(0x08);
        clockRegisters.year   = GetRTCregister(0x09);
        if (centuryRegister != 0) century = GetRTCregister(centuryRegister);

    } while ( (lastSecond != clockRegisters.second) || (lastMinute != clockRegisters.minute) ||
              (lastHour   != clockRegisters.hour)   || (lastDay    != clockRegisters.day)    ||
              (lastMonth  != clockRegisters.month)  || (lastYear   != clockRegisters.year)   ||
              (lastCentury != century)
      );
    
    // get register b and convert bcd to binary if needed
    regB = GetRTCregister(0x0B);
    if (!(regB & 0x04))
    {
        clockRegisters.second = (clockRegisters.second & 0x0F) + ((clockRegisters.second / 16) * 10);
        clockRegisters.minute = (clockRegisters.minute & 0x0F) + ((clockRegisters.minute / 16) * 10);
        clockRegisters.hour  = ((clockRegisters.hour   & 0x0F) + (((clockRegisters.hour & 0x70) / 16) * 10)) | (clockRegisters.hour & 0x80);
        clockRegisters.day    = (clockRegisters.day    & 0x0F) + ((clockRegisters.day    / 16) * 10);
        clockRegisters.month  = (clockRegisters.month  & 0x0F) + ((clockRegisters.month  / 16) * 10);
        clockRegisters.year   = (clockRegisters.year   & 0x0F) + ((clockRegisters.year   / 16) * 10);

        if (centuryRegister != 0)
        {
            century = GetRTCregister(centuryRegister);
            century = (century & 0x0F) + ((century / 16) * 10);
        }
    }

    // convert 12h to 24h
    if (!(regB & 0x02) && (clockRegisters.hour & 0x80))
    {
        clockRegisters.hour = ((clockRegisters.hour & 0x7F) + 12) % 24;
    }

    // fullyear calc.
    if (centuryRegister != 0)
    {
        // century = GetRTCregister(centuryRegister);
        clockRegisters.year += century * 100;
    }
    else
    {
        clockRegisters.year += (BUILDYEAR / 100) * 100;
        if (clockRegisters.year < BUILDYEAR) clockRegisters.year += 100;
    }

    return clockRegisters;
}

/*void ClockBatteryDriver::GrabSystemTime()
{
}
*/