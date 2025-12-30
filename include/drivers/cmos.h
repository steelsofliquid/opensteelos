#ifndef __OSOS__DRIVERS__CMOS_H
#define __OSOS__DRIVERS__CMOS_H

#include <common/types.h>
#include <common/lib/libasm.h>
#include <kernel/hwcom/driverModel.h>
#include <kernel/hwcom/port.h>

namespace osos
{
    namespace drivers
    {

        struct RealTimeClockRegisters
        {
            uint8_t second;
            uint8_t minute;
            uint8_t hour;
            uint8_t day;
            uint8_t month;
            uint32_t year;
        };

        class ClockBatteryDriver : public osos::kernel::hwcom::DriverModel
        {
            public:
            osos::kernel::hwcom::Port8Bit portAddressCMOS;
            osos::kernel::hwcom::Port8Bit portDataCMOS;

            public:
            ClockBatteryDriver();
            ~ClockBatteryDriver();

            uint8_t ReadCMOS(int32_t port);
            void WriteCMOS(int32_t port, int32_t value);

            // this is to assist the realtime clock
            void PadRTCInteger(char* output, uint8_t val);

            // these functions are for the realtime clock
            int GetProgressUpdateFlag();
            uint8_t GetRTCregister(int32_t reg);
            RealTimeClockRegisters GetClockRegisters(RealTimeClockRegisters registerSet);
            RealTimeClockRegisters ReadRTC();

            // We do not need a HandleInterrupt(uint32_t esp) for this driver.
        };
    }
}

#endif