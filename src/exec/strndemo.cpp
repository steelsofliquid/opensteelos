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

int8_t* DemoResultA;
int8_t* DemoResultB;
int8_t* DemoResultC;
int8_t* DemoResultD;
int8_t* DemoResultE;
int8_t* DemoResultF;
int8_t* DemoResultG;
int8_t* DemoResultH;
int8_t* DemoResultI;

extern "C" void ExStringDemoProgramMain()
{
    StringLibrary StringLibrary;

    printf("\a\b");

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

    printf("\a These will be the following strings used as demonstration material:         \n");
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

    sleep(30);

    printf("\a Demonstration 1. String Copying");
    sleep(1);
    printf("\nThis demo features strcpy and strncpy, and uses strings C and D.");
    sleep(2);

    DemoResultA = StringLibrary.strcpy(DemoMaterialC, DemoMaterialD);
    printf("\n\nResult of strcpy with strings C and D: \n");
    printf(DemoResultA);

    DemoResultB = StringLibrary.strncpy(DemoMaterialC, DemoMaterialD, 4);
    printf("\nResult of strncpy with first five of string D: \n");
    printf(DemoResultB);

    sleep(15);

    printf("\a Demonstration 2. String Concatenation");
    sleep(1);
    printf("\nThis demo features strcat, and uses string A, B and C.\n");
    sleep(2);

    DemoResultC = StringLibrary.strcat(DemoMaterialB, ", isn\'t she?");
    DemoResultD = StringLibrary.strcat(DemoMaterialA, ", consectetur adipiscing elit...");
    DemoResultE = StringLibrary.strcat(DemoMaterialD, " is the creator and lead developer of the Linux kernel.");

    printf("\n"); printf(DemoResultC);
    printf("\n"); printf(DemoResultD);
    printf("\n"); printf(DemoResultE);

    sleep(15);

    printf("\a Demonstration 3. Seeking Content");
    sleep(1);
    printf("\nThis demo features strchr, strrchr, and strstr. It may contain bugs.");
    sleep(2);

    DemoResultF = StringLibrary.strchr(DemoMaterialC, 'W');
    DemoResultG = StringLibrary.strrchr(DemoMaterialD, 'T');
    DemoResultH = StringLibrary.strstr(DemoMaterialB, "Neisa");

    printf(DemoResultF);
    printf(DemoResultG);
    printf(DemoResultH);

    printf("STOP! StringDemoProgramMain() in strndemo.cpp ends at line 123 (v) and has no   ");
    printf("way to continue. It is of my upmost apology >_<                                 ");

}