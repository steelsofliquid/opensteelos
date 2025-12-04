#ifndef __CLICOMMANDS_H
#define __CLICOMMANDS_H

#include <common/types.h>
#include <common/version.h>
#include <drivers/snd/speaker.h>
#include <drivers/pit.h>
#include <drivers/rs232.h>
#include <lib/libcpu.h>
#include <globalfuncs.h>

namespace osos
{
    // The OpenSteel/OS commands list.
    // The things disabled by commentpins are to be implemented over the next week or
    // two, and weren't originally in 0.22.153 (the build that I'm hosting the code 
    // for on the repository). The reason these are here is because I got bored in
    // class and OS dev is very irresistable.

    void cmdCpudata(osos::common::uint32_t argc, const char** argv);
    void cmdUptime(osos::common::uint32_t argc, const char** argv);
    void cmdShutdown(osos::common::uint32_t agrc, const char** argv);
    void cmdPacinae(osos::common::uint32_t argc, const char** argv);

    void cmdEcho(osos::common::uint32_t argc, const char** argv);
    void cmdCls(osos::common::uint32_t argc, const char** argv);
    //void cmdClock(osos::common::uint32_t agrc, const char** argv);
    //void cmdBeep(osos::common::uint32_t agrc, const char** argv);
    
    void cmdVer(osos::common::uint32_t argc, const char** argv);
    void cmdHelp(osos::common::uint32_t argc, const char** argv);
    void cmdLicensing(osos::common::uint32_t argc, const char** argv);
    void cmdCredits(osos::common::uint32_t argc, const char** argv);
    //void cmdAbout(const char* args);
}

#endif