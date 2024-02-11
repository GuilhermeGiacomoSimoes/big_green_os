C_SOURCES = $(wildcard kernel/*.c drivers/*.c boot/*.c lib/*.c memory/*.c client/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h boot/*.h lib/*.h memory/*.h client/*.h)
OBJ_FILES = ${C_SOURCES:.c=.o drivers/interrupts.o}

CC ?= x86_64-elf-gcc
LD ?= x86_64-elf-ld

all: run

kernel.bin: boot/kernel-entry.o ${OBJ_FILES}
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

os-image.bin: boot/mbr.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -fda $<

echo: os-image.bin
	xxd $<

%.o: %.c ${HEADERS}
	$(CC) -m32 -ffreestanding -fno-pie -fno-stack-protector -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

%.dis: %.bin
	ndisasm -b 32 $< > $@

clean:
	$(RM) *.bin *.o *.dis *.elf
	$(RM) kernel/*.o
	$(RM) boot/*.o boot/*.bin
	$(RM) drivers/*.o
	$(RM) lib/*.o
	$(RM) client/*.o
	$(RM) memory/*.o
