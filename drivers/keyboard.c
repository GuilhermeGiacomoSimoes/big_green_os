#include <stdint.h>
#include "vga.h"
#include "helper.h"
#include "keyboard.h"

#define IDT_ENTRIES 256;

#define low_16(address) (uint16_t) \
	((address) & 0xFFFF)

#define high_16(address) (uint16_t) \
	(((address) >> 16) & 0xFFFF)

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idt_register_t;


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

void insr_install()
{
	set_idt_gate(0, (uint32_t) isr0);
	set_idt_gate(1, (uint32_t) isr1);
	set_idt_gate(2, (uint32_t) isr2);
	set_idt_gate(3, (uint32_t) isr3);
	set_idt_gate(4, (uint32_t) isr4);
	set_idt_gate(5, (uint32_t) isr5);
	set_idt_gate(6, (uint32_t) isr6);
	set_idt_gate(7, (uint32_t) isr7);
	set_idt_gate(8, (uint32_t) isr8);
	set_idt_gate(9, (uint32_t) isr9);
	set_idt_gate(10, (uint32_t) isr10);
	set_idt_gate(11, (uint32_t) isr11);
	set_idt_gate(12, (uint32_t) isr12);
	set_idt_gate(13, (uint32_t) isr13);
	set_idt_gate(14, (uint32_t) isr14);
	set_idt_gate(15, (uint32_t) isr15);
	set_idt_gate(16, (uint32_t) isr16);
	set_idt_gate(17, (uint32_t) isr17);
	set_idt_gate(18, (uint32_t) isr18);
	set_idt_gate(19, (uint32_t) isr19);
	set_idt_gate(20, (uint32_t) isr20);
	set_idt_gate(21, (uint32_t) isr21);
	set_idt_gate(22, (uint32_t) isr22);
	set_idt_gate(23, (uint32_t) isr23);
	set_idt_gate(24, (uint32_t) isr24);
	set_idt_gate(25, (uint32_t) isr25);
	set_idt_gate(26, (uint32_t) isr26);
	set_idt_gate(27, (uint32_t) isr27);
	set_idt_gate(28, (uint32_t) isr28);
	set_idt_gate(29, (uint32_t) isr29);
	set_idt_gate(30, (uint32_t) isr30);
	set_idt_gate(31, (uint32_t) isr31);
}
