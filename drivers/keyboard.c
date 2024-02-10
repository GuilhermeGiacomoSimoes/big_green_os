#pragma once 

#include <stdint.h>

#include "vga.h"
#include "helper.h"
#include "keyboard.h"

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

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

idt_register_t idt_reg;
void load_idt() 
{
	idt_reg.base = (uint32_t) &idt;
	const int idt_entries = IDT_ENTRIES;
	const int size_idt_gate = sizeof(idt_gate_t);
	idt_reg.limit = idt_entries * size_idt_gate - 1;
	asm volatile("lidt (%0)" : : "r" (&idt_reg));
}

void set_idt_gate(int n, uint32_t handler)
{
	idt[n].low_offset = low_16(handler);

 	///kernel address
	idt[n].selector = 0x08;

	///some bits that always need to 
	///be set to 0 for interrupt gates
	idt[n].always0 = 0;


	///This flags is a concatenation os multiples fields of gate.
	/// [1] 1 bit to indicating whether the gate is active. Will be set to 1
	/// [00] 2 bits the descriptor privilege level indicates what privilege
	/// is required to invoke the handler. Will be set to 00
	/// [0] the bit that always 0
	/// [1] 1 bit indicating whether the code segment is 32bit. Will be set 
	/// to 1
	/// [110] 3 bits indicating the gate type. Will e set to 110 as we are 
	/// defining an interrupt gate.
	/// 
	///and 10001110 = 0x8e
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

void isr_install()
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


	///
	/// PIC REMAPPING
	///
	
	/// ICW1
	/// the 0x11 is a initialize command.
	/// We must send this 0x11 for the both PICs.
	/// After command initialize (0x11) the PICs
	/// then wait for the following three inputs
	/// on the data ports (0x21 for the first PIC
	/// and 0xA1 for the secondary PIC)
	port_byte_out(0x20, 0x11);
	port_byte_out(0xA0, 0x11);
	
	/// ICW2
	/// first command will be set to 0x20 (32) for the
	/// primary PIC and 0x28 (40) for the secondary PIC
	port_byte_out(0x21, 0x20);
	port_byte_out(0xA1, 0x28);
	
	/// ICW3
	/// This is a wiring between PICs.
	/// We will tell the primary PIC to accept
	/// IRQs from the secondary PIC on IRQ1 (0x04)
	///
	/// The secondary PIC will be marked as secondary
	/// by setting 0x02
	port_byte_out(0x21, 0x04);
	port_byte_out(0xA1, 0x02);
	
	/// ICW4
	/// we set 0x01 in order to enable the 8086 mode
	port_byte_out(0x21, 0x01);
	port_byte_out(0xA1, 0x01);
	
	/// OCW1
	/// We finally send the first operation command word 0x00
	/// to enable all IRQs. 
	port_byte_out(0x21, 0x0);
	port_byte_out(0xA1, 0x0);


	set_idt_gate(32, (uint32_t) irq0);
	set_idt_gate(33, (uint32_t) irq1);
	set_idt_gate(34, (uint32_t) irq2);
	set_idt_gate(35, (uint32_t) irq3);
	set_idt_gate(36, (uint32_t) irq4);
	set_idt_gate(37, (uint32_t) irq5);
	set_idt_gate(38, (uint32_t) irq6);
	set_idt_gate(39, (uint32_t) irq7);
	set_idt_gate(40, (uint32_t) irq8);
	set_idt_gate(41, (uint32_t) irq9);
	set_idt_gate(42, (uint32_t) irq10);
	set_idt_gate(43, (uint32_t) irq11);
	set_idt_gate(44, (uint32_t) irq12);
	set_idt_gate(45, (uint32_t) irq13);
	set_idt_gate(46, (uint32_t) irq14);
	set_idt_gate(47, (uint32_t) irq15);

	load_idt();
}

typedef void (*isr_t)(registers_t *);
isr_t interrupt_handlers[256];

void irq_handler(registers_t *r)
{
	if(interrupt_handlers[r->int_no] != 0) {
		isr_t handler = interrupt_handlers[r->int_no];
		handler(r);
	}

	///end of interrupts
	///the primary is a 0-7
	///and the secondary 8-15
	///
	///this is required for the PIC to know that 
	///the interrupt is handled and it can send
	///further interrupts.
	port_byte_out(0x20, 0x20); // primary EOI
	if(r->int_no < 40) {
		port_byte_out(0xA0, 0x20); //secondary EOI
	}
}

void print_letter(uint8_t scancode)
{
	switch(scancode) {
        case 0x0:
            print_string("ERROR");
            break;
        case 0x1:
            print_string("ESC");
            break;
        case 0x2:
            print_string("1");
            break;
        case 0x3:
            print_string("2");
            break;
        case 0x4:
            print_string("3");
            break;
        case 0x5:
            print_string("4");
            break;
        case 0x6:
            print_string("5");
            break;
        case 0x7:
            print_string("6");
            break;
        case 0x8:
            print_string("7");
            break;
        case 0x9:
            print_string("8");
            break;
        case 0x0A:
            print_string("9");
            break;
        case 0x0B:
            print_string("0");
            break;
        case 0x0C:
            print_string("-");
            break;
        case 0x0D:
            print_string("+");
            break;
        case 0x0E:
            print_string("Backspace");
            break;
        case 0x0F:
            print_string("Tab");
            break;
        case 0x10:
            print_string("Q");
            break;
        case 0x11:
            print_string("W");
            break;
        case 0x12:
            print_string("E");
            break;
        case 0x13:
            print_string("R");
            break;
        case 0x14:
            print_string("T");
            break;
        case 0x15:
            print_string("Y");
            break;
        case 0x16:
            print_string("U");
            break;
        case 0x17:
            print_string("I");
            break;
        case 0x18:
            print_string("O");
            break;
        case 0x19:
            print_string("P");
            break;
        case 0x1A:
            print_string("[");
            break;
        case 0x1B:
            print_string("]");
            break;
        case 0x1C:
            print_string("ENTER");
            break;
        case 0x1D:
            print_string("LCtrl");
            break;
        case 0x1E:
            print_string("A");
            break;
        case 0x1F:
            print_string("S");
            break;
        case 0x20:
            print_string("D");
            break;
        case 0x21:
            print_string("F");
            break;
        case 0x22:
            print_string("G");
            break;
        case 0x23:
            print_string("H");
            break;
        case 0x24:
            print_string("J");
            break;
        case 0x25:
            print_string("K");
            break;
        case 0x26:
            print_string("L");
            break;
        case 0x27:
            print_string(";");
            break;
        case 0x28:
            print_string("'");
            break;
        case 0x29:
            print_string("`");
            break;
        case 0x2A:
            print_string("LShift");
            break;
        case 0x2B:
            print_string("\\");
            break;
        case 0x2C:
            print_string("Z");
            break;
        case 0x2D:
            print_string("X");
            break;
        case 0x2E:
            print_string("C");
            break;
        case 0x2F:
            print_string("V");
            break;
        case 0x30:
            print_string("B");
            break;
        case 0x31:
            print_string("N");
            break;
        case 0x32:
            print_string("M");
            break;
        case 0x33:
            print_string(",");
            break;
        case 0x34:
            print_string(".");
            break;
        case 0x35:
            print_string("/");
            break;
        case 0x36:
            print_string("Rshift");
            break;
        case 0x37:
            print_string("Keypad *");
            break;
        case 0x38:
            print_string("LAlt");
            break;
        case 0x39:
            print_string("Space");
            break;
        default:
            ///'keuyp' event corresponds to the 'keydown' + 0x80
            ///it may still be a scancode we haven't implemented yet, or
            ///maybe a control/escape sequence 
            if (scancode <= 0x7f) {
                print_string("Unknown key down");
            } else if (scancode <= 0x39 + 0x80) {
                print_string("key up ");
                print_letter(scancode - 0x80);
            } else print_string("Unknown key up");
            break;
    }
}

static void keyboard_callback(registers_t *regs) 
{
	uint8_t scancode = port_byte_in(0x60);
	print_letter(scancode);
}

#define IRQ1 33
void register_interrupt_handler(uint8_t n, isr_t handler)
{
	interrupt_handlers[n] = handler;
}

void init_keyboard()
{
	register_interrupt_handler(IRQ1, keyboard_callback);
}
