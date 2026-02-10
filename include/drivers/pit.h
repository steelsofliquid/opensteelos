// I think we should venture out of the WYOOS tutorial for the first time.

#ifndef __OSOS__DRIVERS__PIT_H // It's October 2024. I'm slowly rebooting this project, as seen with NanamiOS 1.00.29 (considered 0.09.30).
#define __OSOS__DRIVERS__PIT_H

#include <common/types.h>
#include <common/lib/libio.h>
#include <kernel/hwcom/interrupts.h>
#include <kernel/hwcom/driverModel.h>
#include <kernel/hwcom/port.h>
#include <kernel/multitasking.h>

namespace osos
{
	namespace drivers
	{
		class ProgrammableIntervalTimer : public osos::kernel::hwcom::DriverModel, public osos::kernel::hwcom::InterruptHandler
		{
			osos::kernel::hwcom::Port8Bit Channel0;
			osos::kernel::hwcom::Port8Bit Channel1;
			osos::kernel::hwcom::Port8Bit Channel2;
			osos::kernel::hwcom::Port8Bit PITComPort; // PIT Command Port
			osos::kernel::hwcom::Port8Bit ProgIC;
			osos::kernel::TaskManager* taskman;
				
			public:
			ProgrammableIntervalTimer(osos::kernel::hwcom::InterruptManager* manager, osos::kernel::TaskManager* taskman);
			~ProgrammableIntervalTimer();

			osos::kernel::hwcom::InterruptHandler* InterruptHandlerForme();
				
			uint32_t ReadPIT();
			void SetPITCount(uint32_t count);

			virtual void StartDriver();
			virtual uint32_t HandleInterrupt(uint32_t esp);
		}; // initially forgot a semicolon (almost said semilion for some reason) -▿-
	}
}

#endif
