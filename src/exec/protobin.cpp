// This is sample specification code. Do not compile and run as part of the OS.

#include <common/types.h>
#include <drivers/pit.h>
#include <multitasking.h>

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;
using namespace osos::hwcom;



void printf(char* str);
void sleep(uint32_t interval);

/*
    The provided int32_t and uint8_t values are program information. It goes in this order:
     - PID and NAME are codes for task management to identify the program there.
    PID is 4 digits, at least for now. NAME is 8 digits, no more, no less. This system is
    based off of its equivalent from OpenSteelEnvy, with the main difference being the
    uniform NAME value and PID being four digits.
     - DESCRIPTION, PUBLISHER, VER_MA, VER_MI, and VER_BU are identifiers for the rest of
    the system for the program.
*/

int32_t PID = 0003; // Program Identifier (4 characters max)
uint8_t NAME = 'PROTOBIN'; // Filename/Program Name (8 characters max)

uint8_t DESCRIPTION = 'Example Executable Spec'; // Formal name (24 characters max)
uint8_t PUBLISHER = 'steelsofliquid'; // Program developer (also 24 characters max)
int32_t VER_MA = 0;
int32_t VER_MI = 22;
int32_t VER_BU = 40;

void ExPROGDESCMain() // PROGDESC is the provisional code identifier for now.
{
    printf("Neisa <3");
    sleep(1);
}