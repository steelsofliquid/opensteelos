# This is an index of all of the inline assembly in OpenSteel/OS C and C++ files.
# Do not link this file to the main project, it is here for those who may find it useful.

# kernel.cpp
cli
hlt

cli

cli
hlt

cli
mov %%cr0, %0 # =r (cr0)
sti

hlt

# gdt.cpp
lgdt %0 # =p


# globalfuncs.h
outb %0, %1 # a, Nd
inb %1, %0 #=a, Nd
cpuid


# cmos.cpp
cli
sti

cli
sti


# pit.cpp
cli
sti

cli
sti


# idt.cpp
lidt %0 #...
sidt %0 # =m


# interrupts.cpp
pushf
\t
pop %0 # =g

sti

cli


# port.cpp
outb %0, %1 # a, Nd
inb %1, %0 # =a, Nd

outb %0, %1 # a, Nd
jmp 1f
1: jmp 1f
1:

outw %0, %1 # a, Nd
inw %1, %0 # =a, Nd

outl %0, %1 # a, Nd
inl %1, %0 # =a, Nd