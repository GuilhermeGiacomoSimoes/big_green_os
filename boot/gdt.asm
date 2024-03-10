;;; db, dw, and dd are pseuco-instructions..
;;; - false insctructions
;;; - instructions that are ignored by micro processor
;;; - assembler directiver
;;; - instructions that are treated like comments

;;; dw = define word. generally 2 bytes on a typical x86
;;; db = define byte. 8 bits
;;; dd = define double word. generally 4 bytes on a typical x86

;;; db, dw and dd are used to declare initialized data in the output file

;; gdt_start and gdt_end labels are used to compute size
; null segment descriptor
gdt_start:
	dq 0x0


; code segment descriptor
gdt_code:
	dw 0xffff     ; segment length, bits 0-15
	dw 0x0        ; segment base, bits 0-15
	db 0x0        ; segment base, bits 16-23
	db 10011010b  ; flags (8 bits)
	db 11001111b  ; flags (4 bits) + segment length, bits 16-19
	db 0x0        ; segment base, bits 24-31


; data segment descriptor
gdt_data:
	dw 0xffff     ; define word segment length, bits 0-15
	dw 0x0        ; segment base, bits 0-15
	db 0x0        ; segment base, bits 16-23 
	db 10010010b  ; flags (8 bits)
	db 11001111b  ; flags (4 bits) + segment length, bits 16-19
	db 0x0        ; segment base, bits 24-31

gdt_end:

; GDT descriptor
gdt_descriptor:
	dw gdt_end - gdt_start - 1 ; size ( 16 bit )
	dd gdt_start ; address (32 bits)


CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
