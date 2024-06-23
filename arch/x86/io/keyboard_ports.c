#include "../../../drivers/keyboard/keyboard_ports.h"

int send_data_port() 
{
	return 0x64;
}

int read_data_port() 
{
	return 0x60;
}

int irq_keyboard() 
{
	return 33;
}

int keyboard_reset()
{
	return 0xff;
}
