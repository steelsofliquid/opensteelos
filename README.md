# OpenSteel/OS
OpenSteel/OS is an operating system based off of the Write Your Own Operating System tutorial with a few extra things added in. This is currently a re-organized Nanami/OS, but that is subject to change in the future.

What OpenSteel/OS changes:
 - Organization of code
 - A more lenient roadmap
 - Some of the structure of the OS' appearance

System Requirements:
 - Processor: Any x86 or x86-64 CPU
 - Memory: 64 Megabytes
 - Video Memory: 9 Megabytes
 - Hard Disk: None
 - Optical Drive Required
 - PS/2 or USB Keyboard (the latter is advised)

Compiling on your own:
It is advised you use the makefile provided. You will need Linux, which you're likely already using. Great! ^_^ Well, you'll need tools for working with the i386 (iirc) platform, and a copy of GRUB with a specific folder, at least from my experience as Debian wouldn't compile the ISO correctly while Ubuntu could. I'm unsure. Meh. A pre-built copy is provided, so if this seems too intimidating or you just want to try the OS, you don't have to compile and whatnot. To compile, please for the love of Neisa-sama, SPECIFY "object/subdir(s)/*filename*.o" and not the source code file. I made this mistake after months away from working on the project and I want to clarify that.
If the makefile mentions "PoS" (Piece of Shit), read the short story right above this line. I'd like to automate sending compilation results to a text file, but that's for another time.

Major Issues:
 - Interrupts may freak out on USB mice.
 - PS/2 mice and trackpads do not work on physical hardware, except the IBM ThinkPad R51. (See hardware compatibility list!)
 - Backspace will not work when trying to backspace onto the previous line.
 - PCI will detect all devices after one is found as the first found and known device.
 - When video memory scrolls up a line, it also takes the mouse cursor with it, creating some unpleasant spaghetti effect.

OS Sources:
- Write Your Own Operating System
- OSDev.org Wiki
- The PIT and speaker drivers are relatively similar to those found in osakaOS.
- Some small bits of code written entirely myself, such as the backspace function
- Some code based off of the results of artificial insmelligence, but fuck vibe coding so don't expect any blatant "ohh this was vibe coded" signs. You should try to experiment with it (not in pronounced ways), even if you dislike it (like me).

OpenSteel/OS Copyright 2023-2025 SteelsOfLiquid.
