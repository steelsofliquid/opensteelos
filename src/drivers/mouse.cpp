
#include <drivers/mouse.h>

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;
using namespace osos::kernel;
using namespace osos::kernel::hwcom;

void printf(char*, ...);

MouseEventHandler::MouseEventHandler()
{
}

void MouseEventHandler::OnActivate()
{
}

void MouseEventHandler::OnMouseDown(uint8_t button)
{
}

void MouseEventHandler::OnMouseUp(uint8_t button)
{
}

void MouseEventHandler::OnMouseMove(int x, int y)
{
}



MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandler* handler)
: InterruptHandler(manager, 0x2C),
dataPort(0x60),
commandPort(0x64)
{
    driverAttributes.name      = "Generic PS/2 Mouse Driver";
    driverAttributes.publisher = "SteelsOfLiquid";
    driverAttributes.type      = "hidMouse";

    driverAttributes.isInitialised = false;
    driverAttributes.isActive      = false;

    driverAttributes.hasInterruptRequest  = true;
    driverAttributes.interruptRequestLine = 0x0C;
    driverAttributes.vectorOffset         = interruptNumber;

    this->handler = handler;
}

MouseDriver::~MouseDriver()
{
}

InterruptHandler* MouseDriver::InterruptHandlerForme()
{
    return this;
}

void MouseDriver::StartDriver()
{

    offset = 0;
    buttons = 0;

    if(handler != 0)
        handler->OnActivate();

    commandPort.Write(0xA8); // Activate interrupts
    commandPort.Write(0x20); // Get current state
    uint8_t status = dataPort.Read() | 2;
    commandPort.Write(0x60); // Set the state
    dataPort.Write(status);

    commandPort.Write(0xD4);
    dataPort.Write(0xF4);
    dataPort.Read();
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t status = commandPort.Read();
    if(!(status & 0x20))
        return esp;


    buffer[offset] = dataPort.Read();

    if(handler == 0)
        return esp;
    
    offset = (offset + 1) % 3;

    if(offset == 0)
    {
        if(buffer[1] != 0 || buffer[2] != 0)
        {
            handler->OnMouseMove( buffer[1], -buffer[2]);

        }
    
        for(uint8_t i = 0; i < 3; i++)
        {
            if((buffer[0] & (0x1<<i)) != (buttons & (0x1<<i)))
            {
                if(buttons & (0x1<<i))
                    handler->OnMouseUp(i+1);
                else
                    handler->OnMouseDown(i+1);
            }
        }
        buttons = buffer[0];
        
    }

    return esp;
}
