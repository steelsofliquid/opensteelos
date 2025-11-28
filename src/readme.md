# OpenSteel/OS `src/` directory

The fun stuff.  
___

## General Overview

This directory contains the main source code for practically everything in the project - Built-in libraries, drivers, and of course, the kernel itself. Right now, there isn't much of some components, like a detailed UI or package manager, but those *will* be implemented before it is too late, because if those aren't added, it will cause fragmentation which could deal a fatal blow to the project in the long-long-term.

Each directory here features the code for a key aspect, and the number of directories is *intentionally* as few as possible to ensure it is easy to comprehend. If you don't understand each directory's name, here they are for reference:

| Directory | Purpose |
| :-------- | :------ |
| `common/ ` | Contains additional content for the `common/` headers from `include/`. |
| `drivers/` | Contains all OpenSteel/OS drivers developed in-house, and the driver model. |
| `hwcom/` | Hardware communication. Contains interrupt management, device communication, and port assistance, among other things. |
| `lib/` | Contains the code for bundled libraries. |


Here is a cheat-sheet for this base-level directory's abbreviated contents:

| File | Purpose |
| :--- | :------ |
| `cli.cpp` | Command Line Interface |
| `dmm.cpp` | Dynamic Memory Manager |
| `gdt.cpp` | Global Descriptor Table |

## Additional Files

There are some additional files here that may be helpful, due to either containing APIs or being indexes of some languages. These are as such:
 - `assemblyindex.s` is an index of all assembly code in the project's non-assembly files.
 - `globalfuncs.cpp` contains common functions that may be of use to a specific file.