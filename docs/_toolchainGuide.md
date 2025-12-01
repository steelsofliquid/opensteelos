# Useful bits when developing OpenSteel/OS

This stuff may be helpful, or it may not. Likely the former.

___

## Basic Overview

As previously stated in the base `readme.md`, you should be using Linux to compile OpenSteel/OS. As such, you should have access to some decent tools already. The main thing you'll be working with will be the makefile, of which most components are dictated by the `make` command. So, what is actually incorporated in the makefile?

## Makefile bits

OpenSteel/OS' makefile is composed of multiple parts: the `PARAMS` section, `TIMESTAMP`, and `objects` can be grouped into a general prerequisites bit, followed by the parts for making specific files (and also the kernel binary, `opensteelcore.bin`), and followed by additional tasks for installing OpenSteel/OS manually, making bootable OpenSteel/OS media, and cleaning out the `object/` directory that everythig compiles to and links up to. There are four sets of toolchain parameters: `GPPPARAMS` (C++), `CCPARAMS` (C), `ASPARAMS` (assembly), and `LDPARAMS` (linker script). `GPPPARAMS`, `CCPARAMS`, and `ASPARAMS` all link to compiling an object of their specific type, while `LDPARAMS` are the parameters for the linker script that is used to create the kernel binary.

If you want to skip building the OS object-by-object, you can simply run `make opensteelcore.bin`. Be aware that you may need a more powerful system to do this in the future, as the OpenSteel/OS repository expands.

The ability to create bootable OpenSteel/OS media is also available with the command `make OpenSteel/OS.iso`. This requires Linux and GRUB, and the necessary packages for booting off of a legacy BIOS environment, for now.

## Other useful terminal commands

Outside of the makefile, if you want to see all of the appearances of a specific term, you can use this command:

    grep -R "replace this text with the desired term" -n

This should, ideally, find all of the instances of a term, for example (not recommended), `grep -R "uint32_t" -n`. The other main command you should be familiar with is `qemu-system-x86_64 -cdrom OpenSteelOS.iso` (replace `OpenSteelOS.iso` with where the built disc image is located). There are some important flags you should ideally play around with. You can set up audio working with `-audiodev pa,id=snd0 -machine pcspk-audiodev=snd0`. You can also play around with the CPU of choice by using the `-cpu` flag. Most options include a `-v1` at their ends. The available options aren't too vast, and don't include Transmeta, Cyrix or IDT processors, but they do include legacy processors (like the 486, Pentium and Pentium II).
