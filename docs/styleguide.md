# OpenSteel/OS Style Guide

*Although previously laid out in the overview file, it wasn't formally developed.*


## Which case to use?

It's important to understand which case is the most optimal, whether that is camelCase, PascalCase, or something else. As such, for variables, camelCase is recommended the most (by me). I should note that variable does not mean everything that isn't a `void`. Functions may be defined using something like `uint32_t` or `uint8_t`, and so determine what is actually a variable before going about, changing everything with an `int` or `uint32_t` type to be in camelCase. In the case of an acronym, it should be treated like a word. Functions, on the other hand, should either be written in PascalCase or in full lowercase. In this case, I would also keep acronyms as uppercase. The main exception to this is when writing pre-existing functions for libraries, such as those in `libstr.cpp`. Other exceptions exist, like in the `IDT_` variables in the interrupts source code, but you should try to stick to camelCase for variables and PascalCase for functions.

## Indentation

This should be relatively obvious. *MOST* circumstances consist of the provided style of braces and indentation, however exceptions exist. Try to maintain it, as depicted below:

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


Currently in progress. Will update over time!