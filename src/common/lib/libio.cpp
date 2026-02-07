// OpenSteel/OS headers
#include <common/lib/libio.h>

// GCC headers
#include <stdarg.h>

extern volatile uint16_t curX;
extern volatile uint16_t curY;

void printf(char* str, ...) // the main screen output function.
{
    va_list params;
    va_start (params, str);

    static uint16_t* videoMemory = (uint16_t*)0xb8000;
    static uint8_t x = 0 , y = 0;
    static uint8_t currentColour = 0x0F;

    for(int i = 0; str[i] != '\0'; ++i)
    {

        switch(str[i])
        {

            /*
                Just a simple rundown on what each thing here does:

                 - \n means new line. That is the case for most languages.

                 - \b means backspace. I wrote this bit myself as I had to figure it out one morning in government class (what does an operating system have to do with politics T_T)

                 - \a means "clear the screen". There's something around that is messing with it and including a bullet point, though.

                 - Anything else on the keyboard placed in printf() will just input an ordinary IBM Code Page 437 character.
                 - Use \' or \" if you need to insert an apostrophe or quotation mark as you'll break it otherwise!
                 - Don't input anything not in the keyboard unless you want to break the graphics, I tried it and that's what happened!
            */

            case '\n':
            {
                y++; curY++;
                x = 0; curX = 0;
                break;
            }

            case '\b':
            {
                if(x == 0)
                {
                    if(y != 0)
                    {
                        y--; curY--;
                        int backX = 79;
                        while (backX >= 0 && (videoMemory[80*y+backX] & 0x00FF) == ' ') backX--;
                        if (backX < 0)
                        {
                            x = 0;
                            curX = 0;
                        }
                        else
                        {
                            x = backX;
                            curX = backX;
                        }

                        videoMemory[80*y+x] = (currentColour << 8) | ' ';
                    }
                }
                else
                {
                    x--; curX--;
                    videoMemory[80*y+x] = (currentColour << 8) | ' ';
                }
                break;
            }
      
            case '\a':
            {
                for (y = 0; y < 25; y++)
                    for (x = 0; x < 80; x++)
                        videoMemory[80*y+x] = (currentColour << 8) | ' ';
                x = 0; curX = 0;
                y = 0; curY = 0;
                break;
            }

            case '\r':
            {
                x = 0; curX = 0;

                for (x = 0; x < 80; x++) videoMemory[80*y+x] = (currentColour << 8) | ' ';
                x = 0; curX = 0;
            }

            case '%':
                i++;
                switch (str[i])
                {
                    case 'i': case 'd':
                    {
                        int intval = va_arg(params, int);
                        char buffer[12];

                        itoa(intval, buffer, 10);

                        for (int j = 0; buffer[j] != '\0'; j++)
                        {
                            videoMemory[80*y+x] = (currentColour << 8) | buffer[j];
                            x++; curX++;
                        }

                        break;
                    }

                    case 'c':
                    {
                        char charVal = va_arg(params, int);
                        videoMemory[80*y+x] = (currentColour << 8) | charVal;
                        x++; curX++;

                        break;
                    }

                    case 'x':
                    {
                        uint8_t key = va_arg(params, int);

                        char* foo = "00";
                        char* hex = "0123456789ABCDEF";
                        foo[0] = hex[(key >> 4) & 0xF];
                        foo[1] = hex[key & 0xF];
            
                        videoMemory[80*y+x] = (currentColour << 8) | foo[0]; x++; curX++;
                        videoMemory[80*y+x] = (currentColour << 8) | foo[1]; x++; curX++;

                        break;
                    }

                    case 's':
                    {
                        const char* s = va_arg(params, const char*);
                        if (!s) s = "0x14_invalid";

                        while (*s != '\0')
                        {
                            videoMemory[80*y+x] = (currentColour << 8) | *s;
                            x++; curX++; s++;
                        }
                        break;
                    }

                    case 'R':
                    {
                        currentColour = (uint8_t)va_arg(params, int);
                        break;
                    }

                    default:
                    {
                        i--;
                        videoMemory[80*y+x] = (currentColour << 8) | str[i];
                        x++; curX++;
                        break;
                    }
                }

            continue;

            default:
            {
                videoMemory[80*y+x] = (currentColour << 8) | str[i];
                x++; curX++;
                break;
            }
        }

        if(x >= 80)
        {
            x = 0; curX = 0;
            y++; curY++;
        }

        if(y >= 25)
        {
            for(int r = 1; r < 25; r++)
                for(int c = 0; c < 80; c++)
                {
                    if(r == 0)
                    {
                    }
                    else
                    {
                        videoMemory[80 * (r - 1) + c] = videoMemory[80 * r + c];

                        // it ain't perfect (or close to, *for now*) but it works. will need to make it not print cursor spaghetti in a later build
                    }
                }
      
            for(x = 0; x < 80; x++)
            videoMemory[80 * 24 + x] = (currentColour << 8);

            x = 0; curX = 0;
            y = 24; curY = 24;
        }
    }

    va_end(params);
}