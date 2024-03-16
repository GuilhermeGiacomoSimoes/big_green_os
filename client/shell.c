#include "../lib/string.h"
#include "../arch/x86/io/vga.h"
#include "shell.h"

void execute_command(char *input)
{
	if(strcmp(input, "EXIT") == 0) {
		print_string("Stopping the cpu. Bye\n");
		asm volatile("hlt");
	}

	print_string("unknown command: ");
	print_string(input);
	print_string("\n>_ ");
}
