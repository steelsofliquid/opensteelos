// Let this sink in: I wrote a program hardcoded into the system before an executable format even existed.
// Just to clarify, this demo is not complete and can't be for now. I need an integer to string convertor.

#include <common/types.h>
#include <lib/libstr.h>
#include <drivers/pit.h>
#include <multitasking.h>

using namespace osos;
using namespace osos::common;
using namespace osos::drivers;
using namespace osos::libs;

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

int32_t PID = 0002; // Program Identifier (4 characters max)
uint8_t NAME = 'STRNDEMO'; // Filename/Program Name (8 characters max)

uint8_t DESCRIPTION = 'OpenSteel/OS String Demo'; // Formal name (24 characters max)
uint8_t PUBLISHER = 'steelsofliquid'; // Program developer (also 24 characters max)
int32_t VER_MA = 1;
int32_t VER_MI = 01;
int32_t VER_BU = 1;

int8_t* DemoMaterialA = "Lorem ipsum dolor sit amet";
int8_t* DemoMaterialB = "Neisa is the most beautiful woman of the land";
int8_t* DemoMaterialC = "Hello World!";
int8_t* DemoMaterialD = "Linus Torvalds";

void ExStringDemoProgramMain()
{
    StringLibrary StringLibrary;

    // Startup screen
    printf("   OpenSteel/OS API and Libraries Demonstration Program: String Manipulation   *");
    sleep(1);

    printf("                                                                                ");
    printf("  ___                                                                           ");
    printf(" /  /   This is a demonstration executable that this copy of OpenSteel/OS was   ");
    printf(" \\__\\   programmed to launch into upon finishing the boot process. Do not be    ");
    printf("  \\  \\  alarmed, as this is intentional.                                        ");
    printf("  /__/                                                                          ");
    printf("                                                                                ");
    sleep(1);

    printf(" OpenSteel/OS 0.22.41 \"Hakurei\" LibSTR Demo (strndemo.o) ______________________ ");
    printf("      This program demo is designed as a test of the internal library libstr.o. ");
    printf(" It should be able to display string copying, comparison, length, concatenation ");
    printf(" and searching for a given character (normal and reverse) and substring. You    ");
    printf(" can view each function at the URL below.                                       ");
    printf(" https://github.com/steelsofliquid/opensteelos/tree/main/src/lib                ");
    printf("                                                                                ");
    sleep(1);

    printf(" The demo should start momentarily...                                           ");
    sleep(10);

    printf("\aThese will be the following strings used as demonstration material:           ");
    sleep(1);
    printf("Demo String A: ");
    printf(DemoMaterialA); // doubles as testing the use of printf with non-quotation things
    printf("\nDemo String B: ");
    printf(DemoMaterialB);
    printf("\nDemo String C: ");
    printf(DemoMaterialC);
    printf("\nDemo String D: ");
    printf(DemoMaterialD);
    printf("\n\n*Some addiional quick sample materials will also be used.");

    sleep(4);

    StringLibrary.strcmp(DemoMaterialA, "balls");
    StringLibrary.strlen(DemoMaterialA);

    printf("STOP! StringDemoProgramMain() in strndemo.cpp ends at line 86 (v) and has no    ");
    printf("way to continue. It is of my upmost apology >_<                                 ");

}