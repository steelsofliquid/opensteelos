# Overview of OpenSteel/OS

---


This project is currently in a bit of a development freeze until the state of my life can improve. That does not mean discontinuation; at the latest this project may resume development in May or June of 2026 when I am done attending my current school, and the school I desire attending afterwards has a school of engineering and computer science, and so, assuming my academic life is more stable there, it may continue to see development even after my first days/weeks there. But I digress.

OpenSteel/OS is a free and open source operating system that aims to be different than its Unix-y colleagues (i.e. Linux and BSD) and proprietary competitors and their open-source equivalents. The second and more "formal" iteration of my operating system (based off of the *Write Your Own Operating System* tutorial - [GitHub](https://github.com/AlgorithMan-de/wyoos) [YouTube](https://www.youtube.com/@writeyourownoperatingsystem) ), it is the direct successor to the earlier Nanami/OS (well, with all of the anime themed OS wannabe stripped out of it, but a lot of that was stripped from later Nanami/OS builds), and builds off of its foundation. It is currently written in C++ with C headers and C-style syntax, as well as an object-oriented approach, but I want to transition the OS to being written in pure C. It began development proper in March 2025, and a lot of its development has been the result of short bursts of major progress. In June of 2025, the long-long-term plans had a GUI in them as OpenSteelEnvy's aim was absorbed into this project, but GUI development will not start for a fair bit of time. The current version in development is 0.22 "Hakurei", in development block "Denver". This system will be explained shortly.

This document aims to provide a general overview of OpenSteel/OS technically.

## Versioning system

---


OpenSteel/OS follows a somewhat similar versioning system to traditional Microsoft software (up to a point): For clarification, a three-part build string consisting of 0.22.44, for example, with major, minor, and build number sections, respectively. This versioning system was also used with what is now my unrelated Scratch project series, Steel OS Classic. A major version is usually given a codename, with the theme being the surnames of characters from the *Touhou Project* series. Development is split into smaller blocks, called development blocks (which will be abbreviated as DB throughout these documents). These cover a small section of development, and are given their own codenames, named after different cities. A lot of these development blocks are named after cities in North American countries (specifically America and Canada), mainly due to my existing knowledge of cities in these countries as a Michigander with French and Canadian heritage. In cases where a version is not viewed as significant enough, depending on the case it will either retain the codename of the previous release or will be developed under one single development block with a DB codename.


You can use the following table as a guide for codenames:

 | Version Number | Changes | Codename | Reason |
 | :------------- | :------- | :-------- | :----- |
 | 0.42 | Adds new tools and driver model overhauls. | Konpaku | Major release. |
 | 0.43 | Fixes bugs from 0.42. | Konpaku | Insignificant patch for 0.42. |
 | 0.44 | Adds new device drivers and bug fixes. | Grand Rapids | Not as significant. |
 | 0.45 | Fixes bugs in drivers and model. | Grand Rapids | Very much insignificant. |
 | 1.00 | Shell overhaul or anything. | Komeji | All x.00 releases are major by default. |


### The significance of the release is based on a case-by-case basis. It may not be consistent.
What I mean by this is this:
> Let's say you have, in theory, a set of versions released. It's 2026, and OpenSteel/OS 0.39 "Kirisame" has been released. It brings some big features, like some diagnostics tools, improved and expanded task manager functions (allowing for more tasks to be run without sacrificing stability), optimisation of the kernel (making running it much smoother) and significantly, a font manager with a set of fonts. Sounds like a big deal. A couple months later, OpenSteel/OS 0.40 "Margatroid" is released (because of course we have to appeal to the Touhou shippers). It's another major release, since it adds new drivers among other things. 
>> So why are two major releases pushed out consecutively, and, mind you, within a few months of each other? One of the things added in 0.40 "Margatroid" (I probably am mispelling that) is 64-bit support.

> Meanwhile, a few weeks later, OpenSteel/OS 0.41 "Spokane" is released, adding some additional drivers for displays and network cards. It's not as significant, since not much was added, although 0.40 did have a number of drivers as one of its key additions. This could be due to driver significance, as for all you know, these drivers could be OpenSteel/OS' first trackpad drivers, or they could allow for something like AC97 or generic HDMI support. (*OpenSteel/OS, being in text mode, is currently as of 0.22.42denv capable of running on HDMI monitors presumably, and so this is not of note. Someone please run it on modern physical hardware and use an HDMI connection so I can correct this.*) Fast forward to January 2027, and OpenSteel/OS 0.42 "Konpaku" is relased. It adds new tools and brings an overhaul to the driver model.
>> The reason OpenSteel/OS 0.42 was released as a major release was because OpenSteel/OS 0.41 likely didn't have any major bugs, and any bugs that weren't detrimental were likely patched in 0.42.

> 0.43 is released a few days later. It's an emergency patch of sorts, but otherwise insigificant and retains 0.42's codename as a new development block wasn't started. 0.44 adds more things and fixes bugs, and a new block was started, thus OpenSteel/OS 0.44 "Grand Rapids". And 0.45, 0.46, 0.47, 0.48 and 0.49 are insigificant bug fixes, and so retain the "Grand Rapids" codename of 0.44.

TL;DR - There'a bunch of reasons why you could have 0.22 "Hakurei", 0.23 - 0.24 "Kazami", 0.25 - 0.28 "Detroit", 0.29 - 0.36 "Kochiya", 0.37 "Saginaw", 0.38 "Izayoi", 0.39 "Kirisame", 0.40 "Margatroid", 0.41 "Spokane", 0.42 - 0.43 "Konpaku", and 0.44 - 0.49 "Grand Rapids". It honestly just depends.



## Directories and File Organisation

---


> As of the 20th of October, 2025, this directory structure has yet to be implemented, due to issues with the current codebase in relation to the PIT driver and possibly interrupt manager.

The way OpenSteel/OS is organised is fairly simple, in theory. There are a set of base directories: `include/`, `docs/`, ~~`object/`~~, `src/` and ~~`_archive`~~. A sixth directory, `linkers/`, may be added in the future if the complexity of the language increases, but there's five for now.
 - `include/` contains all of the header files. It has practically exclusive reign of the `common/` subdir and namespace.
 - `src/` contains the source code for everything, including the kernel, bootloader, drivers, and other components.
 - `docs/` contains all of the technical documentation.

   
 ~~- `object/` contains compiled content for those who don't want to compile OpenSteel/OS themselves.~~ Not needed here.


 ~~- `_archive/` contains materials no longer used in OpenSteel/OS by default, including experimental content.~~ Not needed here.

The `include/` and `src/` directories contain relatively similar subdirectories:
 - `drivers/` contains driver source code, including network, graphics, HIDs, and chip drivers.
 - `hwcom/` contains source code for key device communication components, such as port communications and interrupt management.
 - `lib/` contains source code for universal library code. Some of its files are based off of libc.

### File Structure

---


The file structure of an OpenSteel/OS source code file is designed to follow specific cases. Variables should be in camel case (likeThis), and other things will likely use this format going forward. The formatting of C++ should ideally, when possible, use C-style syntax. I don't care whether you use object-oriented or normal programming conventions, as of October 2025 a lot of the code uses an object-oriented approach but that liberty won't be available after transitoning to C. When it comes to the naming of core system components, it shouldn't be either "OpenSteel/OS ThingName" or "OpenSteelThingName", but either a simple "thingname"/"Thing Name" or be given an affectionate name. A debugger, for example:


 - OpenSteel/OS Debugger:   ✗
 - OpenSteelDebugger:       ✗
 - debugger/Debugger:       ✓
 - autopsy:                 ✓


 If you want to use the former two naming conventions, it's not the *end* of the world, but I (steelsofliquid) personally, as this project's benevolent dictator for life, may be a bit fussy about that. I just like consistency in my project, so try to ideally stick to my naming. If it seems weird and you can get me to agree to a better way to compose given parts of the source code, feel free to convince me if possible. I can adjust. Other than this, the only thing I'd advise is to use the terms "Lead" and "Follow" instead of "Master" and "Slave". I know the master/slave terminology has stood for a long time, but it was coined in the early 20th century if I remember correctly and I think that such terminology is morally wrong. Not to mention, I already replaced all of the usage of master and slave with lead and follow. (As I mentioned, this was made for *moral/ethical* reasons, not *political* ones. So if you don't like it, well, *adjust*. I'm not saying you can't use these in your own projects.)

### Actual Kernel Structure

 ---


 As for the actual structure of the kernel, many components are fairly centralised and modular. The multitasker, scheduler, interrupt manager, memory manager, driver manager and drivers are held in their own files, while being designed to be invoked mainly in the kernel. Exceptions exist, like between aspects like the scheduler and the interrupts manager, but most components are oriented towards the kernel. Inside my documentation, I'll go in more detail about each core function and a selection of drivers in dedicated docs.

 As such, the structure of the kernel will be described better in the dedicated kernel documentation. To put it simply, the actual kernel code doesn't do much other than booting. This process, to put it simply, goes about by displaying headers (an ASCII one and a more technical one), loads GDT, performs preliminary memory management, registers tasks, initialises drivers, selects PCI drivers, and then activates interrupts. Drivers will usually have two functions that connect to the driver and interrupt managers respectively: `Activate()` and `handleInterrupt(uint32_t esp)`. The driver manager calls a driver file's `Activate()` function when the driver in question needs to be activated, and the same process practically applies to the `doHandleInterrupt(uint8_t interruptNumber, uint32_t esp)` function in the interrupt manager. It gets quite intertwined, and so components will be documented with their association with other components.

 My long-term plan is to first get some basic drivers (PIT and CMOS) done, and then to ensure interrupts are stable. Task management and memory management will be improved in the kernel to allow for components such as paging and better multitasking.


## Summary

 ---


 This document goes over a basic overview of OpenSteel/OS. It is subject to being updated in the future. Overall, it goes over the roadmap, versioning system, directory organiation, desired file text structure and an introduction into how the kernel works.
