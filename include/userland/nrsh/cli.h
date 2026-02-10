#ifndef __CLI_H
#define __CLI_H

#include <common/types.h>
#include <common/lib/libstr.h>
#include <common/lib/libio.h>
#include <common/sysHelpers.h>
#include <drivers/snd/speaker.h>
#include <drivers/keyboard.h>
#include <kernel/crashHandler.h>

namespace osos
{
    struct command_t
    {
        const char* name;
        commandVoid func;
    };

    class NathanRenaudShell
    {
        private:
        char* commandHistoryEntries[32];
        size_t cmdHstCount;

        public:
        NathanRenaudShell();
        ~NathanRenaudShell();

        void Initialise();

        void RedrawInputLine();
        void SaveToHistory(char* command);
        char* GrabFromHistory(size_t index);
        void LoadHistoryEntry(size_t index);

        uint32_t Tokenise(char* input, char* argv[], uint32_t max);
        void Trim(char* str);
        const char* GetToken(char* str);
        void ParseCommand(char* input);

        void HandleInput(osos::drivers::keyEvent key);
        void HandleChar(char c);
    };
}

#endif