#ifndef __CLI_H
#define __CLI_H

#include <common/types.h>
#include <common/sysHelpers.h>
#include <drivers/snd/speaker.h>
#include <userland/lib/libstr.h>

namespace osos
{
    struct command_t
    {
        const char* name;
        osos::common::commandVoid func;
    };

    class NathanRenaudShell
    {
        public:
        NathanRenaudShell();
        ~NathanRenaudShell();

        void Initialise();
        void Trim(char* str);
        const char* GetToken(char* str);
        void ParseCommand(char* input);

        void HandleInput(char c);
    };
}

#endif