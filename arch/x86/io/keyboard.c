#include <stdint.h>
#include <stdbool.h>

#include "interrupts.h"
#include "../../../include/vga.h"
#include "../../../include/helper.h"
#include "../../../include/keyboard.h"
#include "../../../lib/string.h"
#include "../../../client/shell.h"

#define SEND_DATA_PORT 0x64
#define READ_DATA_PORT 0x60

bool backspace(char buff[])
{
	const int len = strlen(buff); 
	if(len > 0) {
		buff[len - 1] = '\0';
		return true;
	} else 
		return false;
}

#define SC_MAX 57
#define BACKSPACE 0x0e
#define ENTER 0x1c

static char key_buff[256];

///This map of code -> letter is a specific keyboard..
///IBM PC PS/2
///
///Each keyboard has your map
///
const char scancode2char[] = {
  '?', '?', '1', '2', '3', '4', '5',
  '6', '7', '8', '9', '0', '-', '=',
  '?', '?', 'Q', 'W', 'E', 'R', 'T',
  'Y', 'U', 'I', 'O', 'P', '[', ']',
  '?', '?', 'A', 'S', 'D', 'F', 'G',
  'H', 'J', 'K', 'L', ';', '\\', '`',
  '?', '\\', 'Z', 'X', 'C', 'V', 'B',
  'N', 'M', ',', '.', '/', '?', '?',
  '?', ' '
};
static void keyboard_callback(struct registers_t *regs) 
{
	const uint8_t scancode = port_byte_in(READ_DATA_PORT);
	if(scancode > SC_MAX) return;

	if(scancode == BACKSPACE) {
		if(backspace(key_buff)) 
			print_backspace();
	} else if (scancode == ENTER) {
		///print_nl();
		execute_command(key_buff);
		key_buff[0] = '\0';
	} else {
		char letter = scancode2char[(int) scancode];
		append(key_buff, letter);
		char str[2] = {letter, '\0'};
		print_string(str);
	}
}

void init_keyboard()
{
	const uint8_t irq = 33;
	register_interrupt_handler(irq, keyboard_callback);
}

void __reset()
{
	const uint8_t reset = 0xff;
	port_byte_out(SEND_DATA_PORT, reset);
}

