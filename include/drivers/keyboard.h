
#ifndef __OSOS__DRIVERS__KEYBOARD_H
#define __OSOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <common/sysHelpers.h>
#include <kernel/hwcom/driverModel.h>
#include <kernel/hwcom/interrupts.h>
#include <kernel/hwcom/port.h>

namespace osos
{
    namespace drivers
    {
        enum KeystrokeMode
        {
            PrintOnly      = 0,
            NotifyOnly     = 1,
            PrintAndNotify = 2
        };

        class KeyboardEventHandler
        {
            public:
            KeyboardEventHandler();
            
            char SendKeystroke(char key);
            
            virtual void OnKeyDown(char c);
            virtual void OnKeyUp(char);

        };

        class KeyboardDriver : public osos::kernel::hwcom::InterruptHandler, public osos::kernel::hwcom::DriverModel
        {
            osos::kernel::hwcom::Port8Bit dataPort;
            osos::kernel::hwcom::Port8Bit commandPort;

            KeyboardEventHandler* handler;
            
            public:
            KeyboardDriver(osos::kernel::hwcom::InterruptManager* manager, KeyboardEventHandler *handler);
            ~KeyboardDriver();

            osos::kernel::hwcom::InterruptHandler* InterruptHandlerForme();
            
            virtual osos::common::uint32_t HandleInterrupt(osos::common::uint32_t esp);
            virtual void StartDriver();
            
        };
    }
}
    
#endif
