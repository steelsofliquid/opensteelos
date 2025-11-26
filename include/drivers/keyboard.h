
#ifndef __OSOS__DRIVERS__KEYBOARD_H
#define __OSOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hwcom/interrupts.h>
#include <drivers/driver.h>
#include <hwcom/port.h>
#include <cli.h>

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

        class KeyboardDriver : public osos::hwcom::InterruptHandler, public Driver
        {
            osos::hwcom::Port8Bit dataPort;
            osos::hwcom::Port8Bit commandPort;

            KeyboardEventHandler* handler;
        public:
            KeyboardDriver(osos::hwcom::InterruptManager* manager, KeyboardEventHandler *handler);
            ~KeyboardDriver();
            virtual osos::common::uint32_t HandleInterrupt(osos::common::uint32_t esp);
            virtual void Activate();

        };
    }
}
    
#endif
