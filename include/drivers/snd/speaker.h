#ifndef __OSOS__DRIVERS__SND__SPEAKER_H
#define __OSOS__DRIVERS__SND__SPEAKER_H

#include <common/types.h>
#include <drivers/pit.h>
#include <kernel/hwcom/driverModel.h>
#include <kernel/hwcom/port.h>
#include <globalfuncs.h>

namespace osos
{
    namespace drivers
    {
        class Speaker : public osos::kernel::hwcom::DriverModel
        {
            public:
            osos::kernel::hwcom::Port8Bit PITChannel2;
            osos::kernel::hwcom::Port8Bit PITCom;
            osos::kernel::hwcom::Port8Bit SpeakerPort;

            public:
            Speaker();
            ~Speaker();

            virtual void StartDriver();

            struct meledy
            {
                osos::common::uint32_t frequency;
                osos::common::uint32_t length;
            };
                
            // hardware functions
            void play_sound(osos::common::uint32_t nFrequence);
            void nosound();

            // system sounds (might be transferred to their own object in the future)
            void BeepVariable(osos::common::uint32_t frequency, osos::common::uint32_t interval);
            void BeepBasic();
            void LifeChime();
            void RestChime();
        };
    }
}


#endif
