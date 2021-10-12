; MASTER BOOT RECORD
; the first thing to notice is that we are going to switch between 16 bit real mode and 32 bit protected mode so we need to tell the assembler theter it should generate 16bit or 32bit instructions. This can be done by using
; the [bits 16] and [bits32 ] directives, respectively. We are starting off with 16 bit instructions as the BIOS jumps to the boot loader while the CPI is still in 16 bit mode.
[bits 16]
[org 0x7c00]

; where to load the kernel to
KERNEL_OFFSET equ 0x1000

; BIOS sets boot drive in 'dl';store for later use
mov [BOOT_DRIVE], dl

; setup stack
mov bp, 0x9000
mov sp, bp

call load_kernel
call switch_to_32bit

jmp $

%include "disk.asm"
%include "gdt.asm"
%include "switch-to-32bit.asm"

[bits 16]
load_kernel:
	mov bx, KERNEL_OFFSET 	; bx -> destination
	mov dh, 2				; dh -> num sectors
	mov dl, [BOOT_DRIVE]	; dl -> disk
	call disk_load
	ret

[bits 32]
BEGIN_32BIT:
	call KERNEL_OFFSET ; give control to the kernel
	jpm $ ; loop in case kernel returns

; boot drive variable
BOOT_DRIVE db 0

; padding
times 510 - ($-$$) db 0

; magic number
dw 0xaa55
