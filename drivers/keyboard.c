#include <stdint.h>
#include "vga.h"

#define low_16(address) (uint16_t) \
	((address) & 0xFFFF)

#define high_16(address) (uint16_t) \
	(((address) >> 16) & 0xFFFF)

typedef struct {
	uint16_t low_offset;
	uint16_t selector;
	uint8_t always0;
	uint8_t flags;
	uint16_t high_offset;
} __attribute__((packed)) idt_gate_t;

idt_gate_t idt[256];

void set_idt_gate(int n, uint32_t handler)
{
	idt[n].low_offset = low_16(handler);
	idt[n].selector = 0x08;
	idt[n].always0 = 0;
	idt[n].flags = 0x8e;
	idt[n].high_offset = high_16(handler);
}

char *exception_messages[] = {
    "Division by zero",
    "Debug",
	"NMI",
	"Breakpoint (which benefits from the shorter 0xCC encoding of INT 3)",
	"Overflow",
	"Bound Range Exceeded",
	"Invalid Opcode",
	"Coprocessor not available",
	"Double Fault",
	"Coprocessor Segment Overrun (386 or earlier only)",
	"Invalid Task State Segment",
	"Segment not present",
	"Stack Segment Fault",
	"General Protection Fault",
	"Page Fault",
	"Reserved",
	"Floating Point Exception",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-Point Exception",
	"Virtualization Exception",
	"Control Protection Exception (only available with CET)",
};

typedef struct {
	///data segment selector
	uint32_t ds;

	///general purpose registers pushed by pusha
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;

	///pushed by isr procedure
	uint32_t int_no, err_code;

	///pushed by CPU automatically
	uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

void isr_handler(registers_t *r)
{
	print_string(exception_messages[r->int_no]);
}
