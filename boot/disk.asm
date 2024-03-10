disk_load:
	; send all general purpose register 
	; (ax, bx, cx, dx) to stack using pusha
	pusha
	push dx

	mov ah, 0x02 ; change to read mode
	mov al, dh   ; read dh number of sectors
	mov cl, 0x02 ; start from sector 2
	             ; (as sector 1 is our boot sector)
	mov ch, 0x00 ; cylinder 0
	mov dh, 0x00 ; head 0

	; dl = drive number is set as input to disk_load
	; es:bx = buffer pointer is set as input too 

	int 0x13      ; BIOS interrupt

	; if it happens disk errors the BIOS sets carry = 1
	jc disk_error ; check carry = 1 bit for error
	 
	pop dx        ; get back original number of sectors to read
	cmp al, dh    ; BIOS sets 'al' to the X of sectors actually read
	              ; compare it to 'dh' and error out if they are != 

	jne sectors_error
	; if(al != dh){ sectors_error() }
	popa
	ret

disk_error:
	jmp disk_loop

sectors_error:
	jmp disk_loop

disk_loop:
	jmp $
