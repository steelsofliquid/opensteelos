
#ifndef __OSOS__DRIVERS__MOUSE_H // Using the keyboard driver as a template
#define __OSOS__DRIVERS__MOUSE_H

    #include <common/types.h>
    #include <hwcom/interrupts.h>
    #include <drivers/driver.h>
    #include <hwcom/port.h>

    namespace osos
    {
        namespace drivers
        {
            class MouseEventHandler
            {
            public:
                MouseEventHandler();
                
                virtual void OnActivate();
                virtual void OnMouseDown(osos::common::uint8_t button);
                virtual void OnMouseUp(osos::common::uint8_t button);
                virtual void OnMouseMove(int x, int y);

            };

            class MouseDriver : public osos::hwcom::InterruptHandler, public Driver
            {
                osos::hwcom::Port8Bit dataport;
                osos::hwcom::Port8Bit commandport; // from keyboard driver

                osos::common::uint8_t buffer[3];
                osos::common::uint8_t offset;
                osos::common::uint8_t buttons;

                MouseEventHandler* handler;
            public:
                MouseDriver(osos::hwcom::InterruptManager* manager, MouseEventHandler* handler);
                ~MouseDriver();
                virtual osos::common::uint32_t HandleInterrupt(osos::common::uint32_t esp);
                virtual void Activate();

            };
        }
    }

#endif
