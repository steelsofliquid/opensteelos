#ifndef __CLICOMMANDS_H
#define __CLICOMMANDS_H

#include <common/types.h>
#include <common/version.h>
#include <drivers/pit.h>
#include <lib/libcpu.h>
#include <globalfuncs.h>

namespace osos
{
    // The OpenSteel/OS commands list.
    // The things disabled by commentpins are to be implemented over the next week or
    // two, and weren't originally in 0.22.153 (the build that I'm hosting the code 
    // for on the repository). The reason these are here is because I got bored in
    // class and OS dev is very irresistable.

    void cmdCpudata(const char* args);
    void cmdUptime(const char* args);
    void cmdShutdown(const char* args);

    void cmdEcho(const char* args);
    void cmdCls(const char* args);
    //void cmdClock(const char* args);
    //void cmdBeep(const char* args);
    
    void cmdVer(const char* args);
    void cmdHelp(const char* args);
    void cmdLicensing(const char* args);
    void cmdCredits(const char* args);
    //void cmdAbout(const char* args);
}

#endif