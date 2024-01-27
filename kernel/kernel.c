#include "../drivers/vga.h"

void k_main (void) 
{
	clear_screen();

	char str[] = {'h', 'e', 'l', 'l', 'o'};
	print_string(str);
}
