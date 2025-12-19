
GPPPARAMS = -m32 -Iinclude -ffreestanding -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
CCPARAMS = -m32 -Iinclude -ffreestanding -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

TIMESTAMP = $(shell date +"%Y-%m-%d %H:%M:%S")

objects = object/loader.o \
 object/common/version.o \
 object/drivers/drv/ata.o \
 object/drivers/snd/speaker.o \
 object/drivers/cmos.o \
 object/drivers/keyboard.o \
 object/drivers/mouse.o \
 object/drivers/pit.o \
 object/drivers/rs232.o \
 object/drivers/vga.o \
 object/kernel/hwcom/driverManager.o \
 object/kernel/hwcom/driverModel.o \
 object/kernel/hwcom/idt.o \
 object/kernel/hwcom/interrupts.o \
 object/kernel/hwcom/interruptstubs.o \
 object/kernel/hwcom/pci.o \
 object/kernel/hwcom/pic.o \
 object/kernel/hwcom/port.o \
 object/kernel/mem/dmm.o \
 object/kernel/gdt.o \
 object/kernel/multitasking.o \
 object/userland/lib/libcpu.o \
 object/userland/lib/libmem.o \
 object/userland/lib/libstr.o \
 object/userland/nrsh/cli.o \
 object/userland/nrsh/cliCommands.o \
 object/globalfuncs.o \
 object/kernel.o

object/%.o: src/%.cpp
	@echo "Initiating object compilation from C++ file: $@"
	mkdir -p $(@D)
	g++ $(GPPPARAMS) -o $@ -c $<
	echo 'Compile Attempt: $@ object file from $< - Time of Compile: $(TIMESTAMP) - Parameters: $(GPPPARAMS)' >> buildlog.txt
	
object/%.o: src/%.c
	@echo "Initiating object compilation from C: file: $@"
	mkdir -p $(@D)
	cc $(CCPARAMS) -o $@ -c $<
	echo 'Compile Attempt: $@ object file from $< - Time of Compile: $(TIMESTAMP) - Parameters: $(CCPARAMS)' >> buildlog.txt

object/%.o: src/%.s
	@echo "Initiating object compilation from assembly file: $@"
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $< 
	echo 'Compile Attempt: $@ object file from $< - Time of Compile: $(TIMESTAMP) - Parameters: $(ASPARAMS)' >> buildlog.txt
	
opensteelcore.bin: linker.ld $(objects)
	@echo "Initiating binary compilation using linker file: $@"
	ld $(LDPARAMS) -T $< -o $@ $(objects)
	echo 'Compile Attempt: $@ binary file linked with $< - Time of Compile: $(TIMESTAMP) - Parameters: $(LDPARAMS)' >> buildlog.txt


install: opensteelcore.bin
	@echo "Using the makefile to install OpenSteel/OS through: $<"
	sudo cp $< /boot/opensteelcore.bin
	echo 'Install Attempt: $@ binary file - Time of Install: $(TIMESTAMP)' >> buildlog.txt


OpenSteelOS.iso: opensteelcore.bin
	@echo "Creating bootable OpenSteel/OS media..."
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "OpenSteel/OS 0.22 Beta 2 Circuit 5" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/opensteelcore.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso
	echo 'Compile Attempt: $@ disc image from $< - Time of Compile: $(TIMESTAMP)' >> buildlog.txt


.PHONY: clean
clean:
	rm -f $(objects) opensteelcore.bin
