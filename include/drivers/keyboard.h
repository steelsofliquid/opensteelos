
#ifndef __OSOS__DRIVERS__KEYBOARD_H
#define __OSOS__DRIVERS__KEYBOARD_H

    #include <common/types.h>
    #include <hwcom/interrupts.h>
    #include <drivers/driver.h>
    #include <hwcom/port.h>

    namespace osos
    {
        namespace drivers
        {
            class KeyboardEventHandler
            {
            public:
                KeyboardEventHandler();
                
                virtual void OnKeyDown(char);
                virtual void OnKeyUp(char);

            };

            class KeyboardDriver : public osos::hwcom::InterruptHandler, public Driver
            {
                osos::hwcom::Port8Bit dataport;
                osos::hwcom::Port8Bit commandport;

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
