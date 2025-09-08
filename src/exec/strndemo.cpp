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
int8_t* DemoResultJ;
int8_t* DemoResultK;
int8_t* DemoResultL;

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

    sleep(15);

    printf("\a Demonstration 4. String Length and Comparison");
    sleep(1);
    printf("\nThis demo features string comparison based off of if-then loops. There is no    ");
    printf("integer to string convertor yet, and so the output is only a rough estimate. This ");
    printf("demo uses strings A\n");
    sleep(2);

    if (StringLibrary.strlen(DemoMaterialA) == 26)
    {
        printf("\nString A has a length of 26.");
        DemoResultI = "EQUALS 26";
    }
    else
    {
        if (StringLibrary.strlen(DemoMaterialA) > 26)
        {
            printf("\nString A has a length greater than 26.");
            DemoResultI = "GREATER THAN 26";
        }
        else
        {
            printf("\nString A has a length less than 26.");
            DemoResultI = "LESS THAN 26";
        }
    }

    if (StringLibrary.strlen(DemoMaterialD) == 26)
    {
        printf("\nString D has a length of 26.");
        DemoResultJ = "EQUALS 26";
    }
    else
    {
        if (StringLibrary.strlen(DemoMaterialD) > 26)
        {
            printf("\nString D has a length greater than 26.");
            DemoResultJ = "GREATER THAN 26";
        }
        else
        {
            printf("\nString D has a length less than 26.");
            DemoResultJ = "LESS THAN 26";
        }
    }

    sleep(1);

    if (StringLibrary.strcmp(DemoMaterialC, "Hello World!") == 0)
    {
        printf("\nString C is equal to the string \"Hello World!\".");
        DemoResultK = "EQUAL";
    }
    else
    {
        printf("String C is not equal to the string \"Hello World!\".");
        DemoResultK = "NOT EQUAL";
    }

    sleep(1);

    if (StringLibrary.strncmp(DemoMaterialB, "Neisa", 4) == 0)
    {
        printf("\nThe first 5 characters of String B are equal to \"Neisa\".");
        DemoResultL = "EQUAL TO \"Neisa\"";
    }
    else
    {
        printf("The first 5 characters of String B are not equal to the string \"Neisa\".");
        DemoResultL = "NOT EQUAL TO \"Neisa\"";
    }

    sleep(15);

    printf("\a Summary\n");
    sleep(1);
    printf("\nThis demo focused on on string functions. Please notify steelsofliquid of any   ");
    printf("anomalies on GitHub at steelsofliquid/opensteelos.\n");
    sleep(1);
    printf("Test Results:");

    sleep(2);

    printf("\nA | "); printf(DemoResultA);
    printf("\nB | "); printf(DemoResultB);
    printf("\nC | "); printf(DemoResultC);
    printf("\nD | "); printf(DemoResultD);
    printf("\nE | "); printf(DemoResultE);
    printf("\nF | "); printf(DemoResultF);
    printf("\nG | "); printf(DemoResultG);
    printf("\nH | "); printf(DemoResultH);
    printf("\nI | "); printf(DemoResultI);
    printf("\nJ | "); printf(DemoResultJ);
    printf("\nK | "); printf(DemoResultK);
    printf("\nL | "); printf(DemoResultL);

    sleep(30);

    printf("\a\bThe demo is now over. You can continue to mess around with the OS, but you can  ");
    printf("now manually power down your system.\n\n");

}