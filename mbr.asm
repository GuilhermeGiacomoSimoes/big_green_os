; MASTER BOOT RECORD
; the first thing to notice is that we are going to switch between 16 bit real mode and 32 bit protected mode so we need to tell the assembler theter it should generate 16bit or 32bit instructions. This can be done by using
; the [bits 16] and [bits32 ] directives, respectively. We are starting off with 16 bit instructions as the BIOS jumps to the boot loader while the CPI is still in 16 bit mode.
; 
; In NASM, the [org 0x7c00] directive sets the assembler location counter. We specify the memory address where the BIOS is placing the boot loader. This is important when using labels as they will have to be translated 
; to memory addresses when we generate machine code and those addresses need to have the correct offset.
; 
; Preceding the boot loader invocation, the BIOS stores the selected boot drive in the dl register. We are storing this information in memory inside the BOOT_DRIVE variable so we can use the dl register for something 
; else without the risk of overwriting this information
;
; Before we can call the kernel loading procedure, we need to setup the stack by setting the stack pointer registers sp (top of stack, grows downwards) and bp (bottom of stack). We will place the bottom of the stack 
; in 0x9000 to make sure we are far away enough from our other boot loader related memory to avoid collisions. The stack will be used, e.g., by the call and ret statements to keep track of memory addresses when 
; executing assembly procedures.


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
