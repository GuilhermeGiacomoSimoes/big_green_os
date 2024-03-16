ARCH ?= $(uname -a)

HEADERS = $(wildcard client/*.h lib/*.h memory/*.h include/*.h)

ifeq ($(ARCH),i386)
C_SOURCES = $(wildcard arch/x86/*.c client/*.c lib/*.c memory/*.c)
OBJ_FILES = ${C_SOURCES:.c=.o arch/x86/io/interrupts.o}
BOOT_DIR = arch/x86/boot/
endif

ifeq ($(ARCH),avr)
C_SOURCES = $(wildcard arch/avr/*.c client/*.c lib/*.c memory/*.c)
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
	@echo "$(ARCH)"
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
	find . -name "*.o" -exec rm -rf {} +
	find . -name "*.bin" -exec rm -rf {} +
	find . -name "*.dis" -exec rm -rf {} +
	find . -name "*.elf" -exec rm -rf {} +
