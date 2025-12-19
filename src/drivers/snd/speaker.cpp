#include <drivers/snd/speaker.h>

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;
using namespace osos::kernel;
using namespace osos::kernel::hwcom;

bool drvFuncLocked = true;

Speaker::Speaker()
 : PITChannel2(0x42),
   PITCom(0x43),
   SpeakerPort(0x61)
{
    driverAttributes.name      = "PC Speaker Driver";
    driverAttributes.publisher = "SteelsOfLiquid";
    driverAttributes.type      = "audioDevice";

    driverAttributes.isInitialised = false;
    driverAttributes.isActive      = false;

    driverAttributes.hasInterruptRequest  = false;
}

Speaker::~Speaker()
{
}



void Speaker::StartDriver()
{
	drvFuncLocked = false;
}



// Hardware Functions
void Speaker::play_sound(uint32_t nFrequence)
{
	if (!drvFuncLocked)
	{
		uint32_t div;
		uint8_t tmp;

		div = 1193180 / nFrequence;

		PITCom.Write(0xB6);
		PITChannel2.Write((uint8_t) (div));
		PITChannel2.Write((uint8_t) (div >> 8));
		
		tmp = SpeakerPort.Read();

		if (tmp != (tmp | 3)) {
			SpeakerPort.Write(tmp | 3);
		}
	}
}

void Speaker::nosound()
{
	if (!drvFuncLocked)
	{
		uint8_t tmp = SpeakerPort.Read();
		
		SpeakerPort.Write(tmp & 0xFC);
	}
}



// Meledies for system sounds
Speaker::meledy startup[] =
{
	{262, 6},
	{523, 6},
	{698, 6},
	{330, 6}
};



// System Sounds
void Speaker::BeepVariable(uint32_t frequency, uint32_t interval)
{
	// this feels bound for a clunky ass disaster in the future, how can i make the above line all-encompassing how can i make the above line all-encompassing how can i make the above line all-encompassing how can i make the above line all-encomp-
	// sure enough, that disaster happened already, i bet.
	// Furthermore, another catastrophic thing happened in pinning this at 1000 Hz and 10 ticks. That beep is rancid and startling.
	// I'd never wish for the Nanami/OS 0.21.31 - OpenSteel/OS 0.22.62 beep() function to be invoked upon anyone:
	//  - A baby,
	//  - A dog,
	//  - The elderly,
	//  - A sleeping one,
	//  - Not even my enemies.
	//
	// If it's that bad as first, it needs to be revamped. In 0.21.31, it wasn't as bad since the PIT driver was fairly broken, but
	// now, with 2025 approaching its end, and the PIT driver being developed and mature, it is a problem. And so, please use this
	// responsibly, and not make a jarring beep noise.

    play_sound(frequency); // try to keep this under 3500
	HardSleep(interval);   // try to keep this under 10
	nosound();
}

void Speaker::BeepBasic()
{
	play_sound(900);
	HardSleep(7);
	nosound();
}

void Speaker::LifeChime()
{
	// built off of MIDI technologies
	// it also could be more optimised -_-

	BeepVariable(261, 30);
	BeepVariable(523, 30);
	BeepVariable(698, 30);
	BeepVariable(330, 30);
}

void Speaker::RestChime()
{
	BeepVariable(330, 42);
	BeepVariable(698, 60);
	BeepVariable(523, 35);
	BeepVariable(131, 17);
}

// a