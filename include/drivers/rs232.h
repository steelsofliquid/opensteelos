#ifndef __OSOS__DRIVERS__RS232_H
#define __OSOS__DRIVERS__RS232_H

    #include <common/types.h>
    #include <drivers/driver.h>
    #include <hwcom/port.h>
    #include <globalfuncs.h>

    namespace osos
    {
        namespace drivers
        {
            class RecommendedStandard232Driver
            {
                osos::hwcom::Port8Bit COM1;

            public:
                RecommendedStandard232Driver();
                ~RecommendedStandard232Driver();
                static osos::common::int32_t InitialiseSerial();

                osos::common::int32_t serialReceived();
                osos::common::int8_t ReadSerial();
                osos::common::int32_t isTransitEmpty();
                void WriteToSerial(osos::common::uint8_t material);

                virtual void Activate();
                virtual osos::common::uint32_t HandleInterrupt(osos::common::uint32_t esp);
            };
        }
    }

#endif