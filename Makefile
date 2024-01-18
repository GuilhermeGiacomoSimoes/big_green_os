all: run

vga.o: drivers/vga.c
	clang -fno-pie -m32 -ffreestanding -c $< -o $@

kernel.bin: kernel-entry.o kernel.o
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

kernel-entry.o: boot/kernel-entry.asm
	nasm $< -f elf -o $@

kernel.o: vga.o kernel.c
	clang -fno-pie -m32 -ffreestanding -c $< -o $@

mbr.bin: boot/mbr.asm
	nasm $< -f bin -o $@

os-image.bin: mbr.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -fda $<

clean:
	$(RM) *.bin *.o *.dis
