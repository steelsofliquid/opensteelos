#ifndef __CLI_H
#define __CLI_H

#include <common/types.h>
#include <lib/libstr.h>

namespace osos
{
    struct command_t
    {
        const char* name;
        void (*func)(int argc, char** argv);
        const char* helpDesc;
    };

    class NathanRenaudShell
    {
        NathanRenaudShell();
        ~NathanRenaudShell();

        void ParseCommand(char* input);
    };
}

#endif