#include <cli.h>
#include <cliCommands.h>

using namespace osos;
using namespace osos::common;
using namespace osos::libs;

void printf(char* str, ...);

volatile char inputBuffer[256];
volatile uint32_t inputLength = 0;

NathanRenaudShell::NathanRenaudShell()
{
}

NathanRenaudShell::~NathanRenaudShell()
{
}



void NathanRenaudShell::ParseCommand(char* input)
{
}