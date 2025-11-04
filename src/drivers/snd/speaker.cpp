#include <globalfuncs.h>
#include <drivers/snd/speaker.h>
#include <drivers/pit.h>

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;
using namespace osos::hwcom;

Speaker::Speaker()
 : PITChannel2(0x42),
   PITCom(0x43),
   SpeakerPort(0x61)
{
}

Speaker::~Speaker()
{
}

void Speaker::play_sound(uint32_t nFrequence)
{
    uint32_t Div;
	uint8_t tmp;

	Div = 1193180 / nFrequence;

	PITCom.Write(0xB6);
	PITChannel2.Write((uint8_t) (Div));
	PITChannel2.Write((uint8_t) (Div >> 8));
	
	tmp = SpeakerPort.Read();

	if (tmp != (tmp | 3)) {
		SpeakerPort.Write(tmp | 3);
	}
	
}

void Speaker::nosound()
{
    uint8_t tmp = SpeakerPort.Read();
	
	SpeakerPort.Write(tmp & 0xFC);
}

void Speaker::beep()
{
	// this feels bound for a clunky ass disaster in the future, how can i make the above line all-encompassing how can i make the above line all-encompassing how can i make the above line all-encompassing how can i make the above line all-encomp-

    play_sound(1000);
	HardSleep(10);
	nosound();
}

// a