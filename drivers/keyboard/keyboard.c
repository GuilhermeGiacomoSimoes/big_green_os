#include "keyboard.h"
#include "keyboard_ports.h"
#include "../../include/interrupts.h"
#include "../../include/helper.h"
#include "ibmpc-ps2.h"

void init_keyboard()
{
	register_interrupt_handler(irq_keyboard(), kb_callback_ibmpcps2);
}

void reset()
{
	port_byte_out(send_data_port(), keyboard_reset());
}
