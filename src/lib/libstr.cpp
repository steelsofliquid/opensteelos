#include <lib/libstr.h>

using namespace osos;
using namespace osos::common;
using namespace osos::libs;
// me on my way to dump the most inspiring and influential quotes in my comments

/*
This is the new 0.22.41 libstr. In 0.22.40, when libstr was first written, it used
uint8_ts all over and that caused problems when first trying to write a demo program.
As it turns out, a large majority of strings written in C and C++ are not based off
of unsigned chars. For context, in C, a string is an array of chars, practically.

In order to utilise the library, you need to write the following code early in the
file's execution:
StringLibrary StringLibrary;

Also, you should likely write it as, for example, if you want to use strlen:
StringLibrary.strlen(LoremIpsum);
*/


namespace osos{

    namespace libs{

        size_t strlen(const char* string)
        {
            size_t len = 0;
            while (string[len])
                len++; // very simple
            
            return len;
        }

        int32_t strcmp(const char* string1, const char* string2)
        {
            // Built my interpretation off of the glibc manual. So does this make this GNU/OpenSteel/OS, GNU + OpenSteel/OS or is still just OpenSteel/OS?

            const int8_t* a = (const int8_t*) string1;
            const int8_t* b = (const int8_t*) string2;

            size_t i = 0;

            while (a[i] != '\0' && b[i] != '\0')
            {
                if (a[i] < b[i])  // i may have copied some of this from libmem.cpp's memcmp, but the base structure should work anyways...
                    return -1;
                else if (b[i] < a[i])
                    return 1;

                i++;
            }

            return 0; // According to the glibc manual, if the strings are equal, strcmp returns 0.
        }

        int32_t strncmp(const char* string1, const char* string2, size_t n)
        {
            const int8_t* a = (const int8_t*) string1;
            const int8_t* b = (const int8_t*) string2;

            for (size_t i = 0; i < n; i++)
            {
                if (a[i] < b[i])
                    return -1;
                else if (b[i] < a[i])
                    return 1;
            }

            return 0;
        }


        int8_t* strcpy(char* destination, const char* source)
        {
            // don't you love a derivative of memcpy? well go look at libmem.cpp and compare strcpy with memcpy!
            
            int8_t* dest = (int8_t*) destination;
            const int8_t* sourc = (const int8_t*) source;

            size_t i = 0;

            while(dest != sourc) // now THIS mcguivered shite from 4 in the morning is SOMETHING! *passes out and wakes up at noon*
                dest[i] = sourc[i];
                i++;

            return destination;
        }

        int8_t* strncpy(char* destination, const char* source, size_t n)
        {
            int8_t* dest = (int8_t*) destination;
            const int8_t* sourc = (const int8_t*) source;

            for (size_t i = 0; i < n; i++)
                dest[i] = sourc[i];

            return destination;
        }

        int8_t* strcat(char* destination, const char* source)
        {
            // this should be a tinge like strcpy, but presumably just appending the string.
            // actually, by doing field research, it's not appending a string.

            int8_t* dest = (int8_t*) destination;
            const int8_t* sourc = (const int8_t*) source;

            size_t i = osos::libs::strlen(dest); // hopefully this doesn't make g++ spontaneouly combust, probably won't since dmm.cpp uses its own functions and it's fine
            size_t j = 0;

            while (sourc[j] != '\0')
            {
                dest[i] = sourc[j];
                i++;
                j++; // i really love using a second variable. no seriously!
            }

            return destination;
        }


        int8_t* strchr(const char* string, int32_t character)
        {
            const int8_t* str = (const int8_t*) string;
            int32_t charac = (int32_t) character;
            int8_t* result; // We give this at the end as it seems simple

            size_t i = 0; // for str[i]
            size_t j = 0; // for result[j]

            while (str[i] != '\0')
            {
                if (str[i] != charac)
                    i++;
                else if (str[i] == charac)
                    break;
            }

            while (str[i] != '\0')
            {
                result[j] = str[i];
                i++;
                j++;
            }

            return result;
        }

        int8_t* strrchr(const char* string, int32_t character)
        {
            // strchr, but in reverse! dear neisa-sama...

            /*
            Allow me to explain this. Trust the process.

            1. We run a mock strlen as a prerequisite. This is important as unless I can use a function defined earlier in this file (i probably can,
            in which case this is subject to revision in the future), as we need to find the endpoint of the string.

            2. Afte the mock strlen is run, we pass len on to i.

            3. We run the code adjusted from strchr in a reverse fashion.

            4. Return with result.
            */

            const int8_t* str = (const int8_t*) string;
            int32_t charac = (int32_t) character;
            int8_t* result;

            size_t i = 0;
            size_t j = 0;
            size_t len = 0;

            while (string[len]) // Prerequisite copy of strlen.
                len++;
            i = len; // pass result onto i

            while (i != 0)
            {
                if (str[i] != charac)
                {
                    result[j] = str[i];
                    i--;
                    j++;
                }
                else if (str[i] == charac)
                {
                    result[j] = str[i];
                    break;
                }
            }

            return result;
        }

        int8_t* strstr(const char* haystack, const char* needle)
        {
            // This is honestly a bit broken right now, apologies. Will be changed as needed once an executable can be made to demonstrate these functions

            const int8_t* hays = (const int8_t*) haystack; // [insert f1 movie joke here]
            const int8_t* need = (const int8_t*) needle;
            int8_t* result;

            size_t i = 0;
            size_t j = 0;
            size_t k = 0;

            while (hays[i] != '\0')
            {
                if (hays[i] != need[j])
                {
                    i++;
                    j++;
                }
                else if (hays[i] == need[j])
                {
                    if (need[j] == '\0')
                    {
                        j = 0;
                        break;
                    }
                    else if (need[j] != '\0')
                    {
                        result[k] = hays[i];
                        i++;
                        j++;
                        k++;
                    }
                }
            }

            while (hays[i] != '\0')
            {
                result[k] = hays[i];
                i++;
                k++;
            }

            return result;
        }
        
    }

}