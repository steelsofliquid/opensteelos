#ifndef __OSOS__DRIVERS__RS232_H
#define __OSOS__DRIVERS__RS232_H

#include <common/types.h>
#include <common/lib/libasm.h>
#include <common/lib/libio.h>
#include <kernel/hwcom/driverModel.h>
#include <kernel/hwcom/interrupts.h>
#include <kernel/hwcom/port.h>

namespace osos
{
    namespace drivers
    {
        class RecommendedStandard232Driver : public osos::kernel::hwcom::DriverModel, public osos::kernel::hwcom::InterruptHandler
        {
            osos::kernel::hwcom::Port8Bit COM1;

            public:
            RecommendedStandard232Driver(osos::kernel::hwcom::InterruptManager* manager);
            ~RecommendedStandard232Driver();

            enum LineErrorType
            {
                NoError        = 0,
                OverrunError   = 1,
                ParityError    = 2,
                FramingError   = 3,
                BreakInterrupt = 4
            };

            static int32_t InitialiseSerial();

            uint8_t GetLineStatus();
            uint8_t GetModemStatus();
            bool hasError();
            LineErrorType GetError();

            int32_t serialReceived();
            int8_t ReadSerial();
            int32_t isTransitEmpty();
            void WriteToSerial(char material);
            void WriteString(const char* material);

            virtual void StartDriver();
            virtual uint32_t HandleInterrupt(uint32_t esp);
        };
    }
}

#endif