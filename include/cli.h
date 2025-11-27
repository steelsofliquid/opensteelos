#ifndef __CLI_H
#define __CLI_H

#include <common/types.h>
#include <lib/libstr.h>

namespace osos
{
    struct command_t
    {
        const char* name;
        void (*func)(const char* arg);
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