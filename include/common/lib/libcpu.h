// OpenSteel/OS CPU Library
// This library contains functions related to the CPU, and uses a whole lot of CPUID.
// Not entirely a "standardised" library, but useful for OpenSteel/OS. And hey, it's
// my project, and often you'll be writing your own libraries for important functions,
// so I am ALLOWED to have stuff like this, even if I don't assume it's standard.

#ifndef __OSOS__COMMON__LIB__LIBCPU_H
#define __OSOS__COMMON__LIB__LIBCPU_H

#include <common/types.h>
#include <common/lib/libasm.h>


void cpudet(); // set off CPUID to do its thing

const char* cpuven(); // CPU vendor ID'ing
const char* cpubrd(); // CPU brand ID'ing (aka get the CPU model, like Pentium 4)

// TODO: functions for ID'ing SSE, MMX etc.

#endif