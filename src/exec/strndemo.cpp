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

int32_t PID = 0002; // Program Identifier (4 characters max)
uint8_t NAME = 'STRNDEMO'; // Filename/Program Name (8 characters max)
uint8_t DESCRIPTION = 'OpenSteel/OS String Demo'; // Formal name (24 characters max)
uint8_t PUBLISHER = 'steelsofliquid'; // Program developer (also 24 characters max)

void StringDemoProgramMain()
{
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

    printf(" OpenSteel/OS 0.22.40 \"Hakurei\" LibSTR Demo (strndemo.o) ______________________ ");
    printf("      This program demo is designed as a test of the internal library libstr.o. ");
    printf(" It should be able to display string copying, comparison, length, concatenation ");
    printf(" and searching for a given character (normal and reverse) and substring. You    ");
    printf(" can view each function at the URL below.                                       ");
    printf(" https://github.com/steelsofliquid/opensteelos/tree/main/src/lib                ");
    printf("                                                                                ");
    sleep(1);

    printf(" The demo should start momentarily...                                           ");
    sleep(10);

    printf("STOP! StringDemoProgramMain() in strndemo.cpp ends at line 50 (v) and has no    ");
    printf("way to continue. It is of my upmost apology >_<                                 ");

}