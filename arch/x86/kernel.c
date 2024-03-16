#include "io/vga.h"
#include "io/keyboard.h"

void k_main (void) 
{

	clear_screen();
	print_string("Installing interrupt service routines \n");
	isr_install();

	print_string("Enabling external interrupts. \n");
	asm volatile("sti");

	print_string("Initializing keyboard \n");
	init_keyboard();

	clear_screen();
	print_string(">_ ");
}
