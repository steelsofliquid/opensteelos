#include <drivers/cmos.h>

#define BUILDYEAR       2025

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;
using namespace osos::kernel;
using namespace osos::kernel::hwcom;

int centuryRegister = 0x00; // Set by ACPI table parsing



ClockBatteryDriver::ClockBatteryDriver() :
    portAddressCMOS(0x70),
    portDataCMOS(0x71)
{
}

ClockBatteryDriver::~ClockBatteryDriver()
{
}



uint8_t ClockBatteryDriver::ReadCMOS(int32_t port)
{
    uint8_t value;

    //asm volatile("cli");
    portAddressCMOS.Write(port & 0x7F);
    value = portDataCMOS.Read();
    //asm volatile("sti");

    return value;
}

void ClockBatteryDriver::WriteCMOS(int32_t port, int32_t value)
{
    //asm volatile("cli");
    portAddressCMOS.Write(port & 0x7F);
    portDataCMOS.Write(value);
    //asm volatile("sti");
}


void ClockBatteryDriver::PadRTCInteger(char* output, uint8_t val)
{
    output[0] = '0' + (val / 10 );
    output[1] = '0' + (val % 10);
    output[2] = '\0';
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

RealTimeClockRegisters ClockBatteryDriver::GetClockRegisters(RealTimeClockRegisters registerSet)
{
    while(GetProgressUpdateFlag());

    registerSet.second = GetRTCregister(0x00);
    registerSet.minute = GetRTCregister(0x02);
    registerSet.hour   = GetRTCregister(0x04);
    registerSet.day    = GetRTCregister(0x07);
    registerSet.month  = GetRTCregister(0x08);
    registerSet.year   = GetRTCregister(0x09);

    return registerSet;
}

RealTimeClockRegisters ClockBatteryDriver::ReadRTC()
{
    uint8_t century;
    uint8_t lastCentury;

    uint8_t regB;

    RealTimeClockRegisters clockRegisters, lastTimeRegisters;
    clockRegisters = GetClockRegisters(clockRegisters);

    if (centuryRegister != 0) century = GetRTCregister(centuryRegister);

    // Verify all of the RTC registers
    do
    {
        lastTimeRegisters = clockRegisters;
        lastCentury = century;

        clockRegisters = GetClockRegisters(clockRegisters);
        if (centuryRegister != 0) century = GetRTCregister(centuryRegister);

    } while ( (lastTimeRegisters.second != clockRegisters.second) || (lastTimeRegisters.minute != clockRegisters.minute) ||
              (lastTimeRegisters.hour   != clockRegisters.hour)   || (lastTimeRegisters.day    != clockRegisters.day)    ||
              (lastTimeRegisters.month  != clockRegisters.month)  || (lastTimeRegisters.year   != clockRegisters.year)   ||
              (lastCentury != century)
      );
    
    // get register b and convert bcd to binary if needed
    regB = GetRTCregister(0x0B);
    if (!(regB & 0x04))
    {
        auto bcdToBinary = [](uint8_t val)
        {
            return (val & 0x0F) + ((val / 16) * 10);
        };

        clockRegisters.second = bcdToBinary(clockRegisters.second);
        clockRegisters.minute = bcdToBinary(clockRegisters.minute);
        clockRegisters.hour  = (clockRegisters.hour & 0x80) | bcdToBinary(clockRegisters.hour);
        clockRegisters.day    = bcdToBinary(clockRegisters.day);
        clockRegisters.month  = bcdToBinary(clockRegisters.month);
        clockRegisters.year   = bcdToBinary(clockRegisters.year);

        if (centuryRegister != 0)
        {
            century = GetRTCregister(centuryRegister);
            century = bcdToBinary(century);
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
        clockRegisters.year += century * 100;
    }
    else
    {
        clockRegisters.year += (BUILDYEAR / 100) * 100;
        if (clockRegisters.year < BUILDYEAR) clockRegisters.year += 100;
    }

    return clockRegisters;
}
