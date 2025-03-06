
GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = object/loader.o \
 object/gdt.o \
 object/drivers/driver.o \
 object/hwcom/port.o \
 object/hwcom/interruptstubs.o \
 object/hwcom/interrupts.o \
 object/hwcom/pci.o \
 object/drivers/keyboard.o \
 object/drivers/mouse.o \
 object/drivers/pit.o \
 object/drivers/speaker.o \
 object/drivers/vga.o \
 object/dmm.o \
 object/multitasking.o \
 object/kernel.o

object/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(GPPPARAMS) -o $@ -c $<
	
object/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $< 
	
opensteelcore.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)


install: opensteelcore.bin
	sudo cp $< /boot/opensteelcore.bin


OpenSteelOS.iso: opensteelcore.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=5' > iso/boot/grub/grub.cfg
	echo 'set default=5' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "OpenSteel/OS 0.22.37 [2025 March 5th]" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/opensteelcore.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso


.PHONY: clean
clean:
	rm -f $(objects) opensteelcore.bin OpenSteelOS.iso
