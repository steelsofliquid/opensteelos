#ifndef __OSOS__DRIVERS__SPEAKER_H
#define __OSOS__DRIVERS__SPEAKER_H

#include <common/types.h>
#include <drivers/driver.h>
#include <hwcom/port.h>

namespace osos
{
    namespace drivers
    {
        class Speaker
        {
            public:
                osos::hwcom::Port8Bit PITChannel2;
                osos::hwcom::Port8Bit PITCom;
                osos::hwcom::Port8Bit SpeakerPort;

            public:
                Speaker();
                ~Speaker();

                void play_sound(osos::common::uint32_t nFrequence);
                void nosound();
                void beep();

        };
    }
}


#endif
