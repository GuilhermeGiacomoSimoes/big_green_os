; Set the non protected mode ... 16 bits
[bits 16]

; The bootloader must be at address 0x7c00
; Here, then, we set the assembler location counter in 0x7c00
[org 0x7c00]

; OFFSET to load kernel ...  
; when we go initialize kernel, we will load he in address 0x10000
KERNEL_OFFSET equ 0x1000

; Before the bootloader invocation, BIOS storage the selected drive of initialization
; in register dl
mov [BOOT_DRIVE], dl

; setup stack
mov bp, 0x9000
mov sp, bp

call load_kernel
call switch_to_32bit

%include "arch/x86/boot/disk.asm"
%include "arch/x86/boot/gdt.asm"
%include "arch/x86/boot/switch-to-32bit.asm"

[bits 16]
load_kernel:
	mov bx, KERNEL_OFFSET 	; bx -> destination
	mov dh, 2				; dh -> num sectors
	mov dl, [BOOT_DRIVE]	; dl -> disk
	call disk_load
	ret

[bits 32]
BEGIN_32BIT:
	jmp KERNEL_OFFSET ; give control to the kernel

BOOT_DRIVE db 0

;;; padding
times 510 - ($-$$) db 0
dw 0xaa55
