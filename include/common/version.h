// OpenSteel/OS Version Headers/Data API
//
// This stuff is in relation to official version info and build date info,
// and due to the current toolchain limits, has to be entered manually.
// Hence, why it gets a lot of commits and is usually one of the first
// commits in a batch, as doing a lot of them is simpler to me than
// simply making a PR and merging it all into one thing.
//
// As such, it is usually the hobbyist's choice if they want to compile
// OpenSteel/OS for their machine, and if they want to use the non-incremented
// things, or be either formal or whatnot (I wouldn't say formal, but...)
// with this header, and whether or not if the version is 0.22.288 and the
// build date is 01-01-2026 if they want to increment it to, say,
// 0.22.289 and 19-1-2038 (better hope for a 64-bit or 128-bit compiler)

#ifndef __OSOS__COMMON__VERSION_H
#define __OSOS__COMMON__VERSION_H

#include <common/types.h>


// Version Information

extern int32_t verMajor; // main version number
extern int32_t verMinor; // minor build number (more valuable)
extern int32_t verBuild; // Build number


// Build Date

// This is moreso when a build was compiled on the development end, not neccesarily the date someone compiles OpenSteel/OS code on.
// If someone adjusts it as such, that's moreso their choice and if they want to be formal with it.

extern int32_t buildYr;  // Build year
extern int32_t buildMon; // Build month
extern int32_t buildDay; // Build day

#endif