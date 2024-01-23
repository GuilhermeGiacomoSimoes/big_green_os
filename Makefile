all: run

kernel.bin: kernel-entry.o kernel.o
	i386-elf-ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

kernel-entry.o: boot/kernel-entry.asm
	nasm $< -f elf -o $@

kernel.o:  
	i386-elf-gcc -fno-pie -nostdlib -ffreestanding -m32 -c kernel/kernel.c -o kernelkernel.o 
	i386-elf-gcc -fno-pie -nostdlib -ffreestanding -m32 -c drivers/vga.c -o vga.o 
	i386-elf-gcc -fno-pie -nostdlib -ffreestanding -m32 -c lib/memory.c -o memory.o 
	i386-elf-gcc -fno-pie -nostdlib -ffreestanding -m32 -o $@ kernelkernel.o vga.o memory.o 

mbr.bin: boot/mbr.asm
	nasm $< -f bin -o $@

os-image.bin: mbr.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -fda $<

clean:
	$(RM) *.bin *.o *.dis
