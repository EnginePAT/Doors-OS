#include "../include/isr.h"
#include "../include/ports.h"
#include "../include/keyboard.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define IRQ1 1

isr_t interrupt_handlers[256];

void irq_remap() {
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    outb(PIC1_DATA, 0x0);
    outb(PIC2_DATA, 0x0);
}

extern void irq1_handler(); // Assembly ISR stub

void keyboard_handler(registers_t regs) {
    // your code or empty stub
}

void isr_install() {
    irq_remap();
    register_interrupt_handler(IRQ1 + 32, keyboard_handler); // IRQ1 is interrupt 33
    // Setup IDT entries for interrupts including irq1_handler...
    // (You will need to write the IDT setup and load it with lidt)
}

// Simple registration function
void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

// This is called from irq1_handler asm stub
void keyboard_callback(registers_t regs);

void irq_handler(registers_t regs) {
    if (interrupt_handlers[regs.int_no]) {
        interrupt_handlers[regs.int_no](regs);
    }
    // Send End of Interrupt (EOI) to PIC
    if (regs.int_no >= 40) outb(PIC2_COMMAND, 0x20);
    outb(PIC1_COMMAND, 0x20);
}
