#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include <stdint.h>

void isr_install();

void register_interrupt_handler(uint8_t n, 
		void (*isr_t)(void *));

#endif
