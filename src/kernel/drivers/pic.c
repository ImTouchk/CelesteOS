#include "pic.h"

void io_wait(void)
{
    asm volatile ( "outb %%al, $0x80" : : "a"(0) );
}

uint8_t intb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0" : "=a" (ret) : "Nd" (port));
    return ret;
}

void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

void pic_send_eoi(uint8_t irq)
{
    if(irq >= 8) outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}