CC ?= x86_64-elf-gcc
LD ?= x86_64-elf-ld

all: run

kernel.bin: kernel-entry.o interrupts.o kernel.o
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

kernel-entry.o: boot/kernel-entry.asm
	nasm $< -f elf -o $@

interrupts.o: drivers/interrupts.asm
	nasm $< -f elf -o $@

kernel.o:  
	$(CC) -fno-pie -nostdlib -ffreestanding -fno-stack-protector -m32 -c kernel/kernel.c -o kernelkernel.o 
	$(CC) -fno-pie -nostdlib -ffreestanding -fno-stack-protector -m32 -c drivers/vga.c -o vga.o 
	$(CC) -fno-pie -nostdlib -ffreestanding -fno-stack-protector -m32 -c lib/memory.c -o memory.o 
	$(CC) -fno-pie -nostdlib -ffreestanding -fno-stack-protector -m32 -c drivers/keyboard.c -o keyboard.o 
	nasm drivers/interrupts.asm -f elf -o interrupts.o
	$(CC) -fno-pie -nostdlib -ffreestanding -fno-stack-protector -m32 -o $@ interrupts.o kernelkernel.o vga.o memory.o keyboard.o

mbr.bin: boot/mbr.asm
	nasm $< -f bin -o $@

os-image.bin: mbr.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -fda $<

clean:
	$(RM) *.bin *.o *.dis
