
#ifndef __OSOS__DRIVERS__MOUSE_H // Using the keyboard driver as a template
#define __OSOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <common/lib/libio.h>
#include <kernel/hwcom/driverModel.h>
#include <kernel/hwcom/interrupts.h>
#include <kernel/hwcom/port.h>

namespace osos
{
    namespace drivers
    {
        class MouseEventHandler
        {
            public:
            MouseEventHandler();
                
            virtual void OnActivate();
            virtual void OnMouseDown(uint8_t button);
            virtual void OnMouseUp(uint8_t button);
            virtual void OnMouseMove(int x, int y);

        };

        class MouseDriver : public osos::kernel::hwcom::InterruptHandler, public osos::kernel::hwcom::DriverModel
        {
            osos::kernel::hwcom::Port8Bit dataPort;
            osos::kernel::hwcom::Port8Bit commandPort; // from keyboard driver

            uint8_t buffer[3];
            uint8_t offset;
            uint8_t buttons;

            MouseEventHandler* handler;

            public:
            MouseDriver(osos::kernel::hwcom::InterruptManager* manager, MouseEventHandler* handler);
            ~MouseDriver();

            osos::kernel::hwcom::InterruptHandler* InterruptHandlerForme();

            virtual uint32_t HandleInterrupt(uint32_t esp);
            virtual void StartDriver();

        };
    }
}

#endif
