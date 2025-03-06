// I think we should venture out of the WYOOS tutorial for the first time.

#ifndef __OSOS__DRIVERS__PIT_H // It's October 2024. I'm slowly rebooting this project, as seen with NanamiOS 1.00.29 (considered 0.09.30).
#define __OSOS__DRIVERS__PIT_H

	#include <common/types.h>
	#include <drivers/driver.h>
	#include <hwcom/port.h>
	
	namespace osos
	{
		namespace drivers
		{
			class ProgrammableIntervalTimer : public Driver
			{
				osos::hwcom::Port8Bit Channel0;
				osos::hwcom::Port8Bit Channel1;
				osos::hwcom::Port8Bit Channel2;
				osos::hwcom::Port8Bit PITComPort;
				
			public:
				ProgrammableIntervalTimer();
				~ProgrammableIntervalTimer();
				
				osos::common::uint32_t ReadPIT();
				void SetPITCount(osos::common::uint32_t count);
			}; // initially forgot a semicolon (almost said semilion for some reason) -▿-
		}
	}

#endif
