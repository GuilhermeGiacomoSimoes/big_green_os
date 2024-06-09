C_SOURCES = $(wildcard kernel.c)
OBJ_FILES = ${C_SOURCES:.c=.o}
BOOT_DIR = arch/x86/boot/

CC ?= x86_64-elf-gcc
LD ?= x86_64-elf-ld

all: run

run: create_dirs bootloader
	@echo "booloader is ready"
	@echo "you can run the follow command to view in qemu: "
	@echo "qemu-system-i386 -hda bootloader -m 4G"

bootloader: ${BOOT_DIR}/mbr.bin fake_kernel
	cat $^ > $@

fake_kernel: ${BOOT_DIR}/kernel-entry.o kernel.o
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm 
	nasm $< -f bin -o $@

kernel.o: kernel.c
	$(CC) -m32 -ffreestanding -fno-pie -fno-stack-protector -c $< -o $@ 

create_dirs:
	@echo "Creating directories for fake bootloader"
	mkdir -p arch/x86/boot
	cp -rv ../../../arch/x86/boot/* arch/x86/boot/

clean:
	find . -name "*.o" -exec rm -rf {} +
	find . -name "*.bin" -exec rm -rf {} +
	find . -name "*.dis" -exec rm -rf {} +
	find . -name "*.elf" -exec rm -rf {} +
	rm -rf arch
	rm bootloader
	rm fake_kernel
