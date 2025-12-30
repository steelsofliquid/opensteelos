#ifndef __CLICOMMANDS_H
#define __CLICOMMANDS_H

#include <common/types.h>
#include <common/lib/libasm.h>
#include <common/lib/libcpu.h>
#include <common/lib/libio.h>
#include <common/version.h>
#include <drivers/snd/speaker.h>
#include <drivers/pit.h>
#include <drivers/rs232.h>
#include <globalfuncs.h>

namespace osos
{
    // The OpenSteel/OS commands list.
    // The things disabled by commentpins are to be implemented over the next week or
    // two, and weren't originally in 0.22.153 (the build that I'm hosting the code 
    // for on the repository). The reason these are here is because I got bored in
    // class and OS dev is very irresistable.

    void cmdCpudata(uint32_t argc, const char** argv);
    void cmdUptime(uint32_t argc, const char** argv);
    void cmdShutdown(uint32_t agrc, const char** argv);
    void cmdPacinae(uint32_t argc, const char** argv);

    void cmdEcho(uint32_t argc, const char** argv);
    void cmdCls(uint32_t argc, const char** argv);
    //void cmdClock(uint32_t agrc, const char** argv);
    //void cmdBeep(uint32_t agrc, const char** argv);
    
    void cmdVer(uint32_t argc, const char** argv);
    void cmdHelp(uint32_t argc, const char** argv);
    void cmdLicensing(uint32_t argc, const char** argv);
    void cmdCredits(uint32_t argc, const char** argv);
    //void cmdAbout(const char* args);
}

#endif