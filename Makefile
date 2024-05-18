ARCH ?= $(uname -a)

HEADERS = $(wildcard arch/x86/io/interrupts.h client/*.h lib/*.h memory/*.h include/*.h)

ifeq ($(ARCH),i386)
C_SOURCES = $(wildcard arch/x86/*.c arch/x86/io/*.c client/*.c lib/*.c memory/*.c)
OBJ_FILES = ${C_SOURCES:.c=.o arch/x86/io/interrupts.o}
BOOT_DIR = arch/x86/boot/
endif

ifeq ($(ARCH),avr)
C_SOURCES = $(wildcard arch/avr/*.c arch/avr/io/*.c client/*.c lib/*.c memory/*.c)
OBJ_FILES = ${C_SOURCES:.c=.o arch/avr/io/interrupts.o}
BOOT_DIR = arch/avr/boot/
endif

CC ?= x86_64-elf-gcc
LD ?= x86_64-elf-ld

all: run

kernel.bin: ${BOOT_DIR}/kernel-entry.o ${OBJ_FILES}
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

os-image.bin: ${BOOT_DIR}/mbr.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	@echo "os-image.bin is redy"
	qemu-system-i386 -m 4G -smp 16 -fda $<

echo: os-image.bin
	xxd $<

kernel.elf: ${BOOT_DIR}/kernel-entry.o ${OBJ_FILES}
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^

debug: os-image.bin kernel.elf
	qemu-system-i386 -s -S -fda os-image.bin -m 4G

%.o: %.c ${HEADERS}
	$(CC) -g -m32 -ffreestanding -fno-pie -fno-stack-protector -c $< -o $@ 

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

%.dis: %.bin
	ndisasm -b 32 $< > $@

help:
	@echo " === welcome to openOS === "
	@echo ""
	@echo " == COMPILATION == "
	@echo "for compile to i386:"
	@echo "make ARCH=i386"
	@echo ""
	@echo "for compile to avr:"
	@echo "make ARCH=avr"
	@echo ""
	@echo " == FOR CLEAR DIRECTORIES == "
	@echo "make clean"

clean:
	find . -name "*.o" -exec rm -rf {} +
	find . -name "*.bin" -exec rm -rf {} +
	find . -name "*.dis" -exec rm -rf {} +
	find . -name "*.elf" -exec rm -rf {} +
