# OpenSteel/OS
OpenSteel/OS is an x86 operating system building heavily on top of the Write Your Own Operating System tutorial using my own additions and revisions. The project started as an independent "fork" of Nanami/OS, which was discontinued on the spot for its horribly organised/managed repository (*by providing source code for every build at once, linker script makes up 1.2% of the repo's code!*), its repository's improper licensing, and rather silly theme of being dedicated to an anime girl that already represents another OS. Currently, OpenSteel/OS is still in very early development, with a baby userland and much incomplete kernel, and is still using another bootloader.   
![OpenSteel/OS 0.22 after booting up in VirtualBox.](https://github.com/steelsofliquid/opensteelos/blob/main/screenshot.png "OpenSteel/OS 0.22 after booting up in VirtualBox.")

To see my thoughts and whatnot on the current situation of the project, check out my log repo, https://github.com/steelsofliquid/steelsofliqud_log. *This repo also contains my thoughts and whatnot on my other projects.

This repository was designed to be more clear and concise compared to major open source projects, such as Linux and particularly LibreOffice. Anyone interested should be able to work on open source, and so for those who either (a) want to start their own OS project wanting insight into how they should organise their project, (b) those who are trying to find the source code, (c) those who want to help out or develop a fork of OpenSteel/OS, and/or (d) not too familiar with coding and wish to see simpler layouts, you're welcome. Anywhere I think it's not as clear, I'll provide a readme with context so that it's more understandable.

# Project currently on hold
I am working on OpenSteel/OS slowly but as of 2026 March 21 my focus is on maintaining my channel's stability, finishing high school (I got the option to graduate early) and getting things set up and whatnot on my laptop once more, as I switched from Kubuntu to Ubuntu MATE (Though I think KDE is cool my install was acting up and I really like MATE already; I also swithced from Brave to LibreWolf for most of my browsing as it does not need a keyring/wallet like Brave does on (GNU)Linux). I've put in a bug in issues I am working on, as it could hinder usability of OpenSteel/OS with USB keyboards.

Anyways, I'll be on spring break and 5 and a half months' spare time shortly so expect the floodgates to open with progress ^^

## *What's new in OpenSteel/OS (as of 2025-11-16)*
 - PIT driver heavily reworked
 - Better organisation
 - Modified experience (startup sound, bootscreen, etc.)
 - Changes to video memory functions (addition of screen clearing and integer printing)
 - Inclusion of function libraries
 - Starting to add documentation

## *Current Roadmap*
Right now, it's quite basic. If I do get into college (sorry to any admissions director, employer, professor or boss who may have been exposed to the more profane parts of my vocabularity and my semi-unhinged commentary; *I do try to be concious on what I say and I like to have fun in my code comments ^_^*), I would bet good money that I'll take courses related to computer science and engineering, which, of course, includes OSes. Honestly, I'm hot among colleges and I've got my eyes deadset on one. And even still, I have a college textbook on OSes, so I could learn many things from that.    
  
But, I do have some stuff to learn from the tutorial still, so, for the provisional future, here's some stuff:  
 - Paging (0.22 "Denver")
 - Basic APIC support (maybe a bit overkill; 0.22 "Denver")
 - HDD support (0.22 "Denver" for ATA)
 - System calls (0.22 "Denver")
 - Filesystem Support (FAT in 0.22 "Denver")
 - Including userland and a shell with Unix-style command names (i.e. shell = command line, commands like echo, cat, grep and mkdir, development expected for 0.22 "Denver")
 - Network card drivers (0.29 "TBD" or later)
 - Networking (such as TCP and UDP) (0.33 "TBD" or later)

## *Code Conventions*
The code used, while predominantly C++, uses a lot of C-style syntax instead of C++ syntax. When working on this project, whether a direct contribution or on your own, if you want to add things I'd recommend retaining the C-style syntax as it's quite simple and easy to understand. (the virgin std::cout versus the chad printf)

## *System Requirements*
 > *Importantly, OpenSteel/OS is designed to work on a large range of legacy computers, dating back to the late 80's and 90's; this will stick for as long as I can keep it up even if I do not endorse or recommend using OpenSteel/OS on an Intel Pentium or Transmeta Crusoe in later versions.*
 - Processor: x86 or x86-64, preferably something Super Socket 7 or later (This project has been tested and can boot on QEMU-emulated i486 CPUs, this support extent is intentional but not recommended)
 - Memory: At least 32 megabytes
 - Video Memory: 9 Megabytes
 - Hard Disk: Optional
 - Optical Drive: Required
 - PS/2 or USB Keyboard

If you don't the hassle of running this project on physical hardware, OpenSteel/OS has been tested in QEMU, VirtualBox and 86Box with multiple configurations in each. You will likely have the best experience with QEMU with audio enabled or with Socket 370 configurations in 86Box. VMWare Workstation and Player have not been tested since the Nanami/OS days, and VirtualBox mostly works if you don't mind a lack of audio.

## *Compiling on your own*
It is advised you use the makefile provided. You will need Linux, which you're likely already using. Great! ^_^  
Well, you'll need tools for working with the i386 (iirc) platform (namely binutils and libc6-dev-i386), the G++ compiler (that's what the turorial said, it is a part of gcc and so you very likely already have it), and a copy of GRUB with a specific folder, at least from my experience as Debian wouldn't compile the ISO correctly while Ubuntu could. I'm unsure. Meh. If you want to compile OpenSteel/OS on a low-power system, please figure out a resources limiter or compile each file by hand. The latter may be tedious, but I'd rather you not inflict thermal damage on your system.  
A pre-built copy is provided, so if this seems too intimidating or you just want to try the OS, you don't have to compile and whatnot. To compile, please for the love of Neisa-sama, SPECIFY "object/subdir(s)/*filename*.o" and not the source code file. I made this mistake after months away from working on the project and I want to clarify that.  
 > If you are having difficulty creating a bootable ISO of OpenSteel/OS, you may need to install the `grub-pc-bin` package. This issue may affect some Linux distros, such as vanilla Debian or Ubuntu MATE, but not on other distros such as vanilla Ubuntu or Kubuntu. Fedora, Mint, Cachy, Zorin, Arch Gentoo etc. have not been tested.

  
## *Major Issues*
 - PCI will detect all devices after one is found as the first found and known device.

## *Questions and Answers*
 - *Why did you rename the project from Nanami/OS to OpenSteel/OS?* I am no longer chronically obsessed with Nanami Madobe, and felt the project's name and theme was too exotic. And that's why it's not Neisa/OS: I may be obsessed with Neisa (Sapphira-Aera) but not to the point of turning her into a de facto OS-tan (of which her age would make her an oddball, as most OS-tans are children or adolescents while Neisa is an adult). Mind you, this is **ALREADY** the second time I've renamed the project! It was first called KitsuneOS, then NanamiOS, later stylised as Nanami/OS, and now it's OpenSteel/OS. There's a slash because that naming convention makes it sound cool and professional.
 - *Why did you even make an OS dedicated to Nanami Madobe?* Because when I began the project I was, well, very very obsessed with Nanami Madobe. Be glad who I obsess over and/or idolize consists of fictional characters and currently living people I probably will never meet.
 - *Is the original Nanami/OS code still available, maybe I could continue it?* I've still got the old repostiory up, just go look for nanamios among my repos. If you want to continue the project, go ahead, just as long as you credit me for being the initial developer, and as long as it's a fork. Make it a true Nanami Madobe OS! It's practically abandoned.
 - *What led you to an interest in OS dev?* I was interested as early as before I even made my YouTube channel (both SteelsOfLiquid and naterenaud303), but didn't know where to start. After being recommended a video on osakaOS, which mentioned the WYOOS tutorial, it got me interested.
 - *Why not make a Linux distro?* While it would be theoretically easier, my end goal is my own project, with my own desktop environment and under-the-hood components (even though OpenSteel/OS is based off of a tutorial). I do respect Linux and the work of Linus Torvalds, just that I want to create a project with its own kernel.
 - *What hardware are you using to develop this?* a Lenovo ThinkPad T480 running Kubuntu. I've always found Ubuntu and its derivatives the most trustworthy when making these live CDs. ;)
 - *I've been looking and it seems you haven't updated this in [x] amount of time. Why?* First, I prefer to have the main branch of the repo to be in a working, less buggy state, and so that means not updating *it* until after my latest build contains the desired functionality. Second, I do have a life outside of software development, and mind you, a life outside of software development that can get really *busy*. Kyaa, the way I'm wording this makes me sound like a mediocre incompetent person.

## *OS Sources*
- Write Your Own Operating System
- OSDev.org Wiki
- GNU C Library documentation
- Some code based off of the results of artificial insmelligence, but fuck vibe coding so don't expect any blatant "ohh this was vibe coded" signs. You should try to experiment with it (not in pronounced ways), even if you dislike it (like me). And still, I won't vibe code as ChatGPT's adaptation of the existing printf function felt like it gave me an aneurysm, though I may as well be a bit incompetent right now.  
  
OpenSteel/OS Copyright 2023-2025 SteelsOfLiquid.
