#include "drivers/vga.c"

void k_main (void) {
	clear_screen();

	print_string("Hello World\n");
}
