#include "../../include/vga.h"
#include "../../include/interrupts.h" 
#include "../../include/keyboard.h"

void main (void) 
{
	clear_screen();

	print_string("Installing interrupt service routines \n");
	isr_install();

	print_string("Initializing keyboard \n");
	init_keyboard();

	clear_screen();
	print_string(">_ ");
}
