// This is a work-in-progress at the moment, and isn't necessarily complete.

#ifndef __OSOS__DRIVERS__CMOS_H
#define __OSOS__DRIVERS__CMOS_H

#include <common/types.h>
#include <drivers/driver.h>
#include <hwcom/port.h>
#include <hwcom/interrupts.h>
#include <globalfuncs.h>

namespace osos
{
    namespace drivers
    {

        struct RealTimeClockRegisters
        {
            osos::common::uint8_t second;
            osos::common::uint8_t minute;
            osos::common::uint8_t hour;
            osos::common::uint8_t day;
            osos::common::uint8_t month;
            osos::common::uint32_t year;
        };

        class ClockBatteryDriver
        {
            


            private:
            void GetIndex(osos::common::uint8_t reg);
            void ReadData();
            void WriteData(osos::common::uint8_t value);

            public:
            osos::hwcom::Port8Bit portAddressCMOS;
            osos::hwcom::Port8Bit portDataCMOS;

            public:
            ClockBatteryDriver();
            ~ClockBatteryDriver();

            osos::common::uint8_t ReadCMOS(osos::common::int32_t port);
            void WriteCMOS(osos::common::int32_t port, osos::common::int32_t value);

            // these functions are for the realtime clock
            int GetProgressUpdateFlag();
            osos::common::uint8_t GetRTCregister(osos::common::int32_t reg);
            RealTimeClockRegisters ReadRTC();
            // void GrabSystemTime();

            // We do not need a HandleInterrupt(uint32_t esp) for this driver.
        };
    }
}

#endif