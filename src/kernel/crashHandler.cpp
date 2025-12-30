#include <kernel/crashHandler.h>

using namespace osos;
using namespace osos::kernel;



void osos::kernel::panic(uint32_t errorId)
{
    static volatile bool inStateOfPanic = false;

    // go right here if already in panic
    if (inStateOfPanic) asm volatile ("cli; hlt");

    inStateOfPanic = true;
    asm volatile ("cli");
    const char* errorTextID[26] =
    {
        "DIVIDE_BY_ZERO", "DEBUG_TRAP", "NON_MASKABLE_INTERRUPT", "BREAKPOINT",
        "OVERFLOW_INCIDENT", "BOUND_RANGE_EXCEEDED", "INVALID_OPCODE", "DEVICE_UNAVAILABLE",
        "DOUBLE_FAULT", "COPROCCESSOR_OVERRUN", "INVALID_TASK_STATE_SEGMENT", "SEGMENT_NOT_PRESENT",
        "STACK_SEGMENT_FAULT", "GENERAL_PROTECTION_FAULT", "PAGE_FAULT", "0x0F",
        "X87_FLOATING_POINT_EXCEPTION", "MISALIGNED_MEMORY", "HARDWARE_ERROR", "SIMD_FLOATING_POINT_EXCEPTION",
        "VIRTUALISATION_EXCEPTION", "CONTROL_PROTECTION_EXCEPTION", "DRIVER_VIOLATION", "KILLSWITCH_INVOKED",
        "NO_ACCESSIBLE_FPU_MODULES", "OUT_OF_MEMORY"
    };

    const char* errorName = "UNKNOWN_EXCEPTION";
    if (errorId < 24)
        errorName = errorTextID[errorId];

    // red screen of death
    printf("%R\a", 0x4F);
    printf(" <!> STOP                                                                   >_< ");
    printf("                                                                                ");
    printf(" OpenSteel/OS %d.%d.%d\n", verMajor, verMinor, verBuild);
    printf(" Exception ID 0x%x - %s\n", errorId, errorName); // keep it simple, only tell the end user what program/process broke it and why
    printf("                                                                                ");
    printf(" A problem has occurred and OpenSteel/OS has shut down.                         ");

    switch (errorId)
    {
        case 0x00:
            printf(" Attempt to divide by zero in the kernel or a kernel-level task caused a        ");
            printf(" system crash. Please contact the vendor of the faulting task to resolve this   ");
            printf(" problem.                                                                       ");
            break;

        case 0x09:
            printf(" Your system invoked a legacy error that has not been used since the Intel i486 ");
            printf(" series of processors. If you are running OpenSteel/OS on a computer with an    ");
            printf(" Intel Pentium (1993) or newer, you should not be seeing this error. If you are ");
            printf(" running OpenSteel/OS on a computer with an Intel i486 series processor or      ");
            printf(" older, please turn off the computer and restart it.                            ");
            break;

        case 0x12:
            printf(" This error, also called a machine check, indicates that there was a serious    ");
            printf(" hardware problem and that the CPU aborted itself. Please contact your hardware ");
            printf(" vendor(s) or distributor(s) for technical support.                             ");
            break;

        case 0x16:
            printf(" A task attempted to access a driver function without an appropriate system     ");
            printf(" call or API or construct its own driver outside of the driver manager.         ");
            break;

        case 0x17:
            printf(" A killswitch has been flipped to intentionally crash OpenSteel/OS. This could  ");
            printf(" be for a variety of reasons, ranging from you just want to have fun to you     ");
            printf(" could not access a shutdown interface safely and have a killswitch keybind.    ");
            printf(" Regardless, please try to avoid this error and shut the PC down normally, so   ");
            printf(" that you are not as numb to a serious error.                                   ");
            break;
        
        case 0x18:
            printf(" OpenSteel/OS cannot safely start as it could not find a floating point unit    ");
            printf(" module to load. OpenSteel/OS needs access to either an x87 FPU, or MMX, SSE,   ");
            printf(" SSEx, AVX, 3DNow! and/or AltiVec (PowerPC) intruction sets. This is to ensure  ");
            printf(" that floating point numbers (for example, 1.2) can be used. This should not be ");
            printf(" too much of a challenge to find a system with an FPU.                          ");
            break;

        case 0x19:
            printf(" OpenSteel/OS ran out of usable memory. The most likely cause is a memory leak, ");
            printf(" or your computer is experiencing RAM issues or requires a RAM upgrade.         ");
            break;

        default:
            printf(" A description for this error is unavailable.                                   ");
            break;
    }

    printf("                                                                                ");
    printf("     system halted. please press the power button, it is now safe to do so.     ");
    while(1)
    {
        asm volatile ("cli; hlt");
    };
}