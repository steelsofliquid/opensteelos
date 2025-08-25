# OpenSteel/OS
OpenSteel/OS is an operating system based off of the Write Your Own Operating System tutorial with a few extra things added in. It's basically Nanami/OS on steroids, but that's because it's built on the Nanami/OS codebase, and that is subject to change in the future.

## *What's new in OpenSteel/OS (as of 2025-08-24)*
 - Organization of code (directories, yo)
 - A more lenient roadmap (like below!)
 - Some of the structure of the OS' appearance
 - Changes to video memory functions

## *Current Roadmap*
Right now, it's quite basic. If I do get into college (sorry to any admissions director or employer who may have been exposed to the more profane parts of my voacbularity), I would bet good money that I'll take courses related to computer science and engineering, which, of course, includes OSes. Honestly, I'm hot among colleges and I've got my eyes deadset on one. And even still, I have a college textbook on OSes, so I could learn many things from that.    
  
But, I do have some stuff to learn from the tutorial still, so, for the provisional future, here's some stuff:  
 - Disabling the mediocre mouse driver by default (OpenSteel/OS will be a text-based OS for now, but it'll get a beautiful GUI in the future if I ever get that far)
 - Network card drivers
 - HDD support
 - System calls
 - Networking (such as TCP and UDP)
 - Filesystem Support
 - Some GUI elements (won't be loaded by default)

## *System Requirements*
 - Processor: x86 or x86-64, preferably something Super Socket 7 or later
 - Memory: At least 16 megabytes
 - Video Memory: 9 Megabytes
 - Hard Disk: Optional
 - Optical Drive: Required
 - PS/2 or USB Keyboard (the latter is advised)

## *Compiling on your own*
It is advised you use the makefile provided. You will need Linux, which you're likely already using. Great! ^_^  
Well, you'll need tools for working with the i386 (iirc) platform (namely binutils and libc6-dev-i386), the G++ compiler (that's what the turorial said), and a copy of GRUB with a specific folder, at least from my experience as Debian wouldn't compile the ISO correctly while Ubuntu could. I'm unsure. Meh.  
A pre-built copy is provided, so if this seems too intimidating or you just want to try the OS, you don't have to compile and whatnot. To compile, please for the love of Neisa-sama, SPECIFY "object/subdir(s)/*filename*.o" and not the source code file. I made this mistake after months away from working on the project and I want to clarify that.  

  
**If the makefile mentions "PoS" (Piece of Shit), read the short story right above this line. I'd like to automate sending compilation results to a text file, but that's for another time.*

## *Major Issues*
 - Interrupts may freak out on USB mice.
 - PS/2 mice and trackpads do not work on physical hardware, except the IBM ThinkPad R51. (See hardware compatibility list!)
 - Backspace will not work when trying to backspace onto the previous line.
 - PCI will detect all devices after one is found as the first found and known device.
 - When video memory scrolls up a line, it also takes the mouse cursor with it, creating some unpleasant spaghetti effect.
 - *The mouse cursor itself is just very problematic, so if you compile it yourself then maybe turn it off with some good ol' comment making.

## *Questions and Answers*
 - *Why did you rename the project from Nanami/OS to OpenSteel/OS?* I am no longer chronically obsessed with Nanami Madobe, and felt the project's name and theme was too exotic. And that's why it's not Neisa/OS: I may be obsessed with Neisa (Sapphira-Aera) but not to the point of turning her into a de facto OS-tan (of which her age would make her an oddball, as most OS-tans are children or adolescents while Neisa is an adult). Mind you, this is **ALREADY** the second time I've renamed the project! It was first called KitsuneOS, then NanamiOS, later stylised as Nanami/OS, and now it's OpenSteel/OS. There's a slash because that naming convention makes it sound cool and professional.
 - *Why did you even make an OS dedicated to Nanami Madobe?* Because when I began the project I was, well, very very obsessed with Nanami Madobe. Be glad who I obsess over and/or idolize consists of fictional characters and currently living people I probably will never meet.
 - *Is the original Nanami/OS code still available, maybe I could continue it?* I've still got the old repostiory up, just go look for nanamios among my repos. If you want to continue the project, go ahead, just as long as you credit me for being the initial developer, and as long as it's a fork. Make it a true Nanami Madobe OS! It's practically abandoned.
 - *What led you to an interest in OS dev?* I was interested as early as before I even made my YouTube channel (both SteelsOfLiquid and naterenaud303), but didn't know where to start. After being recommended a video on osakaOS, which mentioned the WYOOS tutorial, it got me interested.
 - *Why not make a Linux distro?* While it would be theoretically easier, my end goal is my own project, with my own desktop environment and under-the-hood components (even though OpenSteel/OS is based off of a tutorial). I do respect Linux and the work of Linus Torvalds, just that I want to create a project with its own kernel.
 - *What hardware are you using to develop this?* a Lenovo ThinkPad T480 running Kubuntu. I've always found Ubuntu and its derivatives the most trustworthy when making these live CDs. ;)

## *OS Sources*
- Write Your Own Operating System
- OSDev.org Wiki
- The PIT and speaker drivers are relatively similar to those found in osakaOS, though built on top off code from the OSDev.org wiki.
- Some small bits of code written entirely myself, such as the backspace function
- Some code based off of the results of artificial insmelligence, but fuck vibe coding so don't expect any blatant "ohh this was vibe coded" signs. You should try to experiment with it (not in pronounced ways), even if you dislike it (like me). And still, I won't vibe code as ChatGPT's adaptation of the existing printf function felt like it gave me an aneurysm, though I may as well be a bit incompetent right now.  
  
OpenSteel/OS Copyright 2023-2025 SteelsOfLiquid.
