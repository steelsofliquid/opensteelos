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
    //
    // I've also opted to split up commands by category. There's four categories here,
    // labelled as such.

    // Technical commands; related to the kernel, userland or hardware or some other shit I forgor to bring up
    void cmdCpudata(uint32_t argc, const char** argv);
    void cmdUptime(uint32_t argc, const char** argv);
    void cmdShutdown(uint32_t agrc, const char** argv); // feature needs service
    //void cmdDevices(uint32_t argc, const char** argv);  // command needs service
    //void cmdServices(uint32_t argc, const char** argv); // command needs service
    //void cmdIrqcount(uint32_t argc, const char** argv);

    // Hardcoded commands for future programs/implementations
    void cmdPacinae(uint32_t argc, const char** argv);
    void cmdSetup(uint32_t argc, const char** argv); // haha, checkmate torvalds, user-friendly installers prevent fragme- oh wait they only help prevent that.

    // Everyday use commands, the shit you'd expect to find on an average terminal ("OMG SHE'S HACKING O_o")
    void cmdEcho(uint32_t argc, const char** argv);    // feature needs syscall(?)
    void cmdCls(uint32_t argc, const char** argv);
    //void cmdClock(uint32_t agrc, const char** argv); // command needs service/syscall
    //void cmdBeep(uint32_t agrc, const char** argv);  // command needs service
    
    // Informational commands. Basically either walls of text or the humble ver command.
    void cmdVer(uint32_t argc, const char** argv);
    void cmdHelp(uint32_t argc, const char** argv);
    void cmdLicensing(uint32_t argc, const char** argv);
    void cmdCredits(uint32_t argc, const char** argv);
    void cmdAbout(uint32_t argc, const char** argv);

    // Temporary commands
}

#endif