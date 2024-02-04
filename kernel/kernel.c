#include "../drivers/vga.h"
#include "../drivers/keyboard.h"

void k_main (void) 
{
	clear_screen();
	isr_install();
	asm volatile("sti");
	init_keyboard();
}
