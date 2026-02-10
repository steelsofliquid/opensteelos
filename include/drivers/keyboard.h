
#ifndef __OSOS__DRIVERS__KEYBOARD_H
#define __OSOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <common/sysHelpers.h>
#include <common/lib/libio.h>
#include <kernel/hwcom/driverModel.h>
#include <kernel/hwcom/interrupts.h>
#include <kernel/hwcom/port.h>

namespace osos
{
    namespace drivers
    {
        enum keyCode 
        {
            KEY_NONE,
            KEY_CHAR,
            KEY_UP,
            KEY_DOWN,
            KEY_LEFT,
            KEY_RIGHT,
            KEY_CONTROL,
            KEY_SHIFT,
            KEY_ALT,
            KEY_WIN,
            KEY_FUNCTION,
            KEY_CHAR_REL,
            KEY_UP_REL,
            KEY_DOWN_REL,
            KEY_LEFT_REL,
            KEY_RIGHT_REL,
            KEY_CONTROL_REL,
            KEY_SHIFT_REL,
            KEY_ALT_REL,
            KEY_WIN_REL,
            kEY_FUNCTION_REL
        };

        struct keyEvent
        {
            keyCode code;
            char character;
        };

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
            
            void SendKeystroke(keyCode key, char ch);
            
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
            
            virtual uint32_t HandleInterrupt(uint32_t esp);
            virtual void StartDriver();
            
        };
    }
}
    
#endif
