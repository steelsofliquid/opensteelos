// The oldest OpenSteel/OS driver.
// As of 0.22.396, this driver (and mouse driver) are being refactored to resolve a bug in
// USB keyboards (supported via PS/2 backwards compatibility). As burning a new ISO every time
// I compile a new build is unfeasible, I have to commit this code to main/ and use the Makefile
// to install the build on appropriate testing machines. Sadly, there aren't many 32-bit distros
// left and even still browsers are 64-bit only for the most part these days. So, some of the
// systems of concern (like the ThinkPad R51) will have to go for a manual live-CD burn-and-test.

#include <drivers/keyboard.h>

using namespace osos;
using namespace osos::drivers;
using namespace osos::kernel::hwcom;

extern volatile InterfaceModes currentInterface;

void printfHex(uint8_t);



volatile KeystrokeMode keymode = PrintAndNotify;
volatile keyEvent lastChar;



KeyboardEventHandler::KeyboardEventHandler()
{
}

void KeyboardEventHandler::SendKeystroke(keyCode key, char ch = 0)
{
    //if ((unsigned char)key < 32 && key != '\n' && key != '\b') return 0;

    lastChar.code      = key;
    lastChar.character = ch;
}



void KeyboardEventHandler::OnKeyDown(char c)
{
    //if ((unsigned char)c < 32 && c != '\n' && c != '\b') return;

    if (c == '\n')
    {
        if ((keymode == PrintOnly ) || (keymode == PrintAndNotify)) printf("\n");
        if ((keymode == NotifyOnly) || (keymode == PrintAndNotify)) SendKeystroke(KEY_CHAR, '\n');
    }
    else if (c == '\b')
    {
        if (((keymode == PrintOnly ) || ((keymode == PrintAndNotify)) && (currentInterface == TextModeEditor))) printf("\b");
        if ((keymode == NotifyOnly) || (keymode == PrintAndNotify)) SendKeystroke(KEY_CHAR, '\b');
    }
    else
    {
        if ((keymode == PrintOnly ) || (keymode == PrintAndNotify)) printf("%c", c);
        if ((keymode == NotifyOnly) || (keymode == PrintAndNotify)) SendKeystroke(KEY_CHAR, c);
    }
}

void KeyboardEventHandler::OnKeyUp(char)
{
}



KeyboardDriver::KeyboardDriver(InterruptManager* manager, KeyboardEventHandler *handler)
: InterruptHandler(manager, 0x21),
dataPort(0x60),
commandPort(0x64)
{
    driverAttributes.name      = "Generic PS/2 Keyboard Driver";
    driverAttributes.publisher = "SteelsOfLiquid";
    driverAttributes.type      = "hidKeyboard";

    driverAttributes.isInitialised = false;
    driverAttributes.isActive      = false;

    driverAttributes.hasInterruptRequest  = true;
    driverAttributes.interruptRequestLine = 0x01;
    driverAttributes.vectorOffset         = interruptNumber;

    this->handler = handler;
}

KeyboardDriver::~KeyboardDriver()
{
}

InterruptHandler* KeyboardDriver::InterruptHandlerForme()
{
    return this;
}

void KeyboardDriver::StartDriver()
{
    while (commandPort.Read() & 0x02); commandPort.Write(0xAD);
    while (commandPort.Read() & 0x01) dataPort.Read();
    while (commandPort.Read() & 0x02); commandPort.Write(0x20);

    while (!(commandPort.Read() & 0x01));
    uint8_t status = dataPort.Read();
    status |= 0x01;
    status &= ~0x10;

    while (commandPort.Read() & 0x02); commandPort.Write(0x60);
    while (commandPort.Read() & 0x02); dataPort.Write(status);

    while (commandPort.Read() & 0x02); commandPort.Write(0xAE);
    while (commandPort.Read() & 0x01) dataPort.Read();
    while (commandPort.Read() & 0x02); dataPort.Write(0xF4);
    while (!(commandPort.Read() & 0x01)); dataPort.Read();

    while (commandPort.Read() & 0x02); dataPort.Write(0xF3);
    while (!(commandPort.Read() & 0x01)); dataPort.Read();
    while (commandPort.Read() & 0x02); dataPort.Write(0x7F);
    while (!(commandPort.Read() & 0x01)); dataPort.Read();
}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    if (!(commandPort.Read() & 0x01)) return esp;
    
    uint8_t key;

    while(commandPort.Read() & 0x01)
    {
        key = dataPort.Read();
        if (key == 0xFA || key == 0xFE) continue;
    }

    static bool Shift    = false;
    static bool Control  = false;
    static bool Alt      = false;
    static bool capsLock = false;
    static bool extended = false; // It is important that the keys here are coupled with their release scancodes.

    bool released = key & 0x80;

    if(handler)
    {
        if (key == 0xE0)
        {
            extended = true;
            return esp;
        }

        if (extended)
        {
            extended = false;

            if (released)
            {
                switch(key)
                {
                    case 0xC8: handler->SendKeystroke(KEY_UP_REL); break;
                    case 0xD0: handler->SendKeystroke(KEY_DOWN_REL); break;

                    default: break;
                }

                return esp;
            }

            switch(key)
            {
                case 0x48: handler->SendKeystroke(KEY_UP); break;
                case 0x50: handler->SendKeystroke(KEY_DOWN); break;

                default: break;
            }
        }

        else
        {
            /*
                This function is currently designed for Canadian English QWERTY keyboards, also
                used in the United States of America. Not the CMS (Canadian Mutlilingual Standard)
                or Canadien French layouts, or QWERTZ, British QWERTY, AZERTY, Dvorak or other
                layouts.

                As such, this function will be revised in the future to do the following:
                (1) Automatically send a couple of keyboard interrupts to determine which keys make
                    up a given selection (i.e. where keys QWERTY, ASDF, and ZXCV are) of scancodes.
                (2) Calibrate the driver to the keyboard layout.
                (3) Assign OnKeyDown('[key]'); based off of a map from the calibrated result.

                This will likely result in these functions:
                - ScanKeyboardLayout();
                - CalibrateKeyboardLayout();
                - keymap[]; (array)

                But this isn't even close to happening. I'd need to likely buy a Dvorak keyboard and
                import QWERTZ and AZERTY keyboards from Germany and France. I do need to buy a lot of
                additional hardware to use to further driver development - Not just keyboards, but also
                printers, networking cards, sound cards, graphics cards, mice, trackpads and more laptops.
            */

            if (released)
            {
                switch (key)
                {
                    case 0xAA: case 0xB6: Shift = false; break;
                    case 0x9D: Control = false; break;
                    case 0xB8: Alt = false; break;
                }

                return esp;
            }

            switch(key)
            {
                case 0xFA: break;

                case 0x29: if(Shift) handler->OnKeyDown('~'); else handler->OnKeyDown('`'); break;
                case 0x02: if(Shift) handler->OnKeyDown('!'); else handler->OnKeyDown('1'); break;
                case 0x03: if(Shift) handler->OnKeyDown('@'); else handler->OnKeyDown('2'); break;
                case 0x04: if(Shift) handler->OnKeyDown('#'); else handler->OnKeyDown('3'); break;
                case 0x05: if(Shift) handler->OnKeyDown('$'); else handler->OnKeyDown('4'); break;
                case 0x06: if(Shift) handler->OnKeyDown('%'); else handler->OnKeyDown('5'); break;
                case 0x07: if(Shift) handler->OnKeyDown('^'); else handler->OnKeyDown('6'); break;
                case 0x08: if(Shift) handler->OnKeyDown('&'); else handler->OnKeyDown('7'); break;
                case 0x09: if(Shift) handler->OnKeyDown('*'); else handler->OnKeyDown('8'); break;
                case 0x0A: if(Shift) handler->OnKeyDown('('); else handler->OnKeyDown('9'); break;
                case 0x0B: if(Shift) handler->OnKeyDown(')'); else handler->OnKeyDown('0'); break;
                case 0x0C: if(Shift) handler->OnKeyDown('_'); else handler->OnKeyDown('-'); break;
                case 0x0D: if(Shift) handler->OnKeyDown('+'); else handler->OnKeyDown('='); break;

                case 0x0E: handler->OnKeyDown('\b'); break; // backspace.

                case 0x0F: printf("     "); break; // tab
                case 0x10: if (Shift ^ capsLock) handler->OnKeyDown('Q'); else handler->OnKeyDown('q'); break;
                case 0x11: if (Shift ^ capsLock) handler->OnKeyDown('W'); else handler->OnKeyDown('w'); break;
                case 0x12: if (Shift ^ capsLock) handler->OnKeyDown('E'); else handler->OnKeyDown('e'); break;
                case 0x13: if (Shift ^ capsLock) handler->OnKeyDown('R'); else handler->OnKeyDown('r'); break;
                case 0x14: if (Shift ^ capsLock) handler->OnKeyDown('T'); else handler->OnKeyDown('t'); break;
                case 0x15: if (Shift ^ capsLock) handler->OnKeyDown('Y'); else handler->OnKeyDown('y'); break;
                case 0x16: if (Shift ^ capsLock) handler->OnKeyDown('U'); else handler->OnKeyDown('u'); break;
                case 0x17: if (Shift ^ capsLock) handler->OnKeyDown('I'); else handler->OnKeyDown('i'); break;
                case 0x18: if (Shift ^ capsLock) handler->OnKeyDown('O'); else handler->OnKeyDown('o'); break;
                case 0x19: if (Shift ^ capsLock) handler->OnKeyDown('P'); else handler->OnKeyDown('p'); break;
                case 0x1A: if(Shift) handler->OnKeyDown('{'); else handler->OnKeyDown('['); break;
                case 0x1B: if(Shift) handler->OnKeyDown('}'); else handler->OnKeyDown(']'); break;
                case 0x2B: if(Shift) handler->OnKeyDown('|'); else handler->OnKeyDown('\\'); break;

                case 0x1E: if (Shift ^ capsLock) handler->OnKeyDown('A'); else handler->OnKeyDown('a'); break;
                case 0x1F: if (Shift ^ capsLock) handler->OnKeyDown('S'); else handler->OnKeyDown('s'); break;
                case 0x20: if (Shift ^ capsLock) handler->OnKeyDown('D'); else handler->OnKeyDown('d'); break;
                case 0x21: if (Shift ^ capsLock) handler->OnKeyDown('F'); else handler->OnKeyDown('f'); break;
                case 0x22: if (Shift ^ capsLock) handler->OnKeyDown('G'); else handler->OnKeyDown('g'); break;
                case 0x23: if (Shift ^ capsLock) handler->OnKeyDown('H'); else handler->OnKeyDown('h'); break;
                case 0x24: if (Shift ^ capsLock) handler->OnKeyDown('J'); else handler->OnKeyDown('j'); break;
                case 0x25: if (Shift ^ capsLock) handler->OnKeyDown('K'); else handler->OnKeyDown('k'); break;
                case 0x26: if (Shift ^ capsLock) handler->OnKeyDown('L'); else handler->OnKeyDown('l'); break;
                case 0x27: if(Shift) handler->OnKeyDown(':'); else handler->OnKeyDown(';'); break;
                case 0x28: if(Shift) handler->OnKeyDown('\"'); else handler->OnKeyDown('\''); break;

                case 0x2C: if (Shift ^ capsLock) handler->OnKeyDown('Z'); else handler->OnKeyDown('z'); break;
                case 0x2D: if (Shift ^ capsLock) handler->OnKeyDown('X'); else handler->OnKeyDown('x'); break;
                case 0x2E: if (Shift ^ capsLock) handler->OnKeyDown('C'); else handler->OnKeyDown('c'); break;
                case 0x2F: if (Shift ^ capsLock) handler->OnKeyDown('V'); else handler->OnKeyDown('v'); break;
                case 0x30: if (Shift ^ capsLock) handler->OnKeyDown('B'); else handler->OnKeyDown('b'); break;
                case 0x31: if (Shift ^ capsLock) handler->OnKeyDown('N'); else handler->OnKeyDown('n'); break;
                case 0x32: if (Shift ^ capsLock) handler->OnKeyDown('M'); else handler->OnKeyDown('m'); break;
                case 0x33: if(Shift) handler->OnKeyDown('<'); else handler->OnKeyDown(','); break;
                case 0x34: if(Shift) handler->OnKeyDown('>'); else handler->OnKeyDown('.'); break;
                case 0x35: if(Shift) handler->OnKeyDown('?'); else handler->OnKeyDown('/'); break;

                case 0x1C: handler->OnKeyDown('\n'); break; // Enter key
                case 0x39: handler->OnKeyDown(' '); break; // Space key

                case 0x2A: case 0x36: Shift = true; break;
                case 0x1D: Control = true; break;
                case 0x38: Alt = true; break;

                case 0x3A:
                {
                    if (capsLock == false)
                    {
                        capsLock = true;
                        // TODO: add keyboard status light toggles
                        break;
                    }
                    else
                    {
                        capsLock = false;
                        break;
                    }
                }

                case 0x3B:
                {
                    break;
                }
                // Formula 1
                case 0x3C:
                {
                    break;
                }
                // Formula 2
                case 0x3D: break;
                // Formula 3.

                case 0x3E:
                {
                    if (Alt == true)
                        printf("Heh. I see what you did there. If only there was userspace..."); // I suppose you found the ALT+F4 easter egg. Nice.
                    break;
                } // Formula 4

                case 0x3F: // Formula 5
                {
                    break;
                }


                case 0x40:
                {
                    break;
                }


                case 0x41:  // Formula 7
                {
                    break;

                }

                // case 0x42: printf("¬"); break; // Formula 8
                // case 0x43: printf("♫"); break; // Formula 9
                // case 0x44: printf("♥"); break; // Formula 10

                case 0x58: // Formula 12
                {
                    break;
                }

                case 0x45: case 0xC5: break; // NumLock key. Some PCs don't have number pads. My ThinkPad doesn't have one.
                default: break;
            }
        }
    }

    return esp;
}
