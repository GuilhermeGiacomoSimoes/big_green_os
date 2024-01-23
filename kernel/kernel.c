#include "../drivers/vga.h"

void k_main (void) 
{
	clear_screen();
	print_string("hello world\n");
}
