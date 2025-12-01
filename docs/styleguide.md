# OpenSteel/OS Style Guide

*Although previously laid out in the overview file, it wasn't formally developed.*


## Which case to use?

It's important to understand which case is the most optimal, whether that is camelCase, PascalCase, or something else. As such, for variables, camelCase is recommended the most (by me). I should note that variable does not mean everything that isn't a `void`. Functions may be defined using something like `uint32_t` or `uint8_t`, and so determine what is actually a variable before going about, changing everything with an `int` or `uint32_t` type to be in camelCase. In the case of an acronym, it should be treated like a word. Functions, on the other hand, should either be written in PascalCase or in full lowercase. In this case, I would also keep acronyms as uppercase. The main exception to this is when writing pre-existing functions for libraries, such as those in `libstr.cpp`. Other exceptions exist, like in the `IDT_` variables in the interrupts source code, but you should try to stick to camelCase for variables and PascalCase for functions.

## Indentation

Indents should be 4 spaces. As to how braces should be organised and indents handled, it should be relatively obvious. *MOST* circumstances consist of the provided style of braces and indentation, however exceptions exist. Try to maintain it, as depicted below:

    [C++]
    #include <header.h>

    using namespace osos;
    using namespace osos::common;

    void function()
    {
         filler content;
    }
    ...

___

    [C]
    #ifndef __OSOS__SAMPLEHEADER_H
    #define __OSOS__SAMPLEHEADER_H

    #include <common/types.h>

    namespace osos
    {
        void function();
    }

    #endif


Exceptions can consist of these scenarios:
 - When the attributes/parameters of a function are too long to be desirably readable.
 - When the contents of an `if-then` statement or `for` loop are one line and would be better and more readable, like in the keyboard driver.

These exceptions, in action:

    [Exception 1 - Too long to fit in one line - From kernel.cpp]
    printf("memory heap: 0x%x%x%x%x, ",
       ((heap >> 24) & 0xFF),
       ((heap >> 16) & 0xFF),
       ((heap >> 8 ) & 0xFF),
       ((heap      ) & 0xFF)
    );

___

    [Exception 2 - Extremely simple - From keyboard.cpp]
    case 0x29: if(Shift) handler->OnKeyDown('~'); else handler->OnKeyDown('`'); break;
    case 0x02: if(Shift) handler->OnKeyDown('!'); else handler->OnKeyDown('1'); break;
    case 0x03: if(Shift) handler->OnKeyDown('@'); else handler->OnKeyDown('2'); break;

### Indentations in classes

It can get messy with the `public:`, `protected:` and `private:` sections of classes. As such, for those bits, indent them as such:

    ...
    class SampleClass
    {
        private:
        osos::common::uint32_t PrivateFunction();

        public:
        void SampleFunction();
        void OtherSampleFunction();
    };


## Order of Inclusions

The order things should be written at the top of the program should be done so in a specific order. This "order of inclusions" should see any general annotation of the source code file come FIRST before anything, followed by the series of `#include <header.h>` if a major source code file that brings the content provided by those headers all to the table like as if a big holiday dinner (this scenario can be found in the kernel source code), or a singular `#include <header.h>` if the piece is supposed to be a component the other parts can rely on (it is *hard* to describe the point, but think a bit like what you may see in `multitasking.cpp` or `drivers/rs232.cpp`). In the latter of these two scenarios, the includes you'll use for functions like port read/write or `uintXX_t` definitions should be placed in the header file for the source code they need to be used in. For clarification, what I mean is this:

> Let's make up a scenario where someone wants to write a driver for the Hoenn Pokédex from the Pokémon franchise. Yes, I know. A bit of a "childish" example to some. But I needed to think of a fictional electronic device so that I can make it quite clear that this is an example, and so fictional electronic device = good example scenario, because you probably won't be using OpenSteel/OS on a Pokédex or some other fictional device. Hopefully Game Freak and Nintendo don't mind. But I digress. You'll probably, if conforming to the design choices of OpenSteel/OS source code, be writing your voids and uint8_ts in a header, and the code for them in a source code file. Here's how you would handle the `#include` bits in the header, and the source code:

    [In the hypothetical header hoennpokedex.h]
    #include <common/types.h>
    #include <drivers/driver.h>
    #include <hwcom/interrupts.h>
    #include <hwcom/ports.h>
    ... [namespace, class and functions go after this point]

    [In the hypothetical file hoennpokedex.cpp]
    #include <hoennpokedex.h>

There is also the more middle-of-the-road option for some components, such as the Nathan Renaud Shell (`cli.cpp`). These components are provided by OpenSteel/OS, but aim to act as more autonomous components, and so conform to a mix of the two cases. Using the Nathan Renaud Shell as the example scenario here, this middle-of-the-road option is as such:

    [In cli.h]
    #include <common/types.h>
    #include <lib/libstr.h>

    [in cli.cpp]
    #include <cli.h>
    #include <cliCommands.h>

As such, the headers are available when needed. They should also be organised in an orderly fashion, and, like shown above, organised in alphanumeric order. However, there's more. While the headers are now included, the order of inclusions is basically, in the case of OpenSteel/OS, a fancy term for the procedure for how to organise the top/start of the file.

### Namespaces

Namespaces come after the `#include` headers. However, the process is different between headers and main source code files.

In the headers, it is as such:

    ...
    namespace osos
    {
        [contents]
    }

Meanwhile, in the main source code files, it is as such:

    ...

    using namespace osos;
    using namespace osos::common;

### The rest

Namespaces are followed by external variables and voids in the main source. As such, you'll have this for the start of a source code file:

    // Preliminary comment

    #include <common/types.h>
    #include <drivers/cmos.h>
    #include <drivers/pit.h>
    #include <hwcom/ports.h>
    #include <lib/libstr.h>

    using namespace osos;
    using namespace osos::common;
    using namespace osos::drivers;
    using namespace osos::hwcom;
    using namespace osos::libs;

    extern volatile uint32_t tickCount;
    
    void printf(char* str, ...);





    Source code starts proper...

And so, that's how source code files should be started. As for headers:

    // Preliminary comment

    #include <common/types.h>
    #include <drivers/cmos.h>
    #include <drivers/pit.h>
    #include <hwcom/ports.h>
    #include <lib/libstr.h>

    namespace osos
    {
        namespace drivers
        {
            header code goes in here...
        }
    }

Currently in progress. Will update over time!