#include "idt.h"
#include "pic.h"
#include "print.h"

#include <stddef.h>

extern int load_idt();
extern int irq0(); extern int irq1();
extern int irq2(); extern int irq3();
extern int irq4(); extern int irq5();
extern int irq6(); extern int irq7();
extern int irq8(); extern int irq9();
extern int irq10(); extern int irq11();
extern int irq12(); extern int irq13();
extern int irq14(); extern int irq15();

static size_t TABLE_INDEX[16] = {
    32, 33, 34, 35, 36, 37,
    38, 39, 40, 41, 42, 43,
    44, 45, 46, 47
};

static uint64_t ADDRESS_INDEX[16] = {
    irq0,  irq1,  irq2,  irq3,
    irq4,  irq5,  irq6,  irq7,
    irq8,  irq9,  irq10, irq11,
    irq12, irq13, irq14, irq15,
};

static uint64_t IDT_Pointer[2];
static uint64_t IDT_Address;
static IDT_Entry IDT[256];

void irq_init(void)
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 40);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    for(size_t i = 0; i < 15; i++) {
        size_t index = TABLE_INDEX[i];
        size_t address = ADDRESS_INDEX[i];
        
        IDT[index].offset_lowerbits = address & 0xffff;
        IDT[index].selector = 0x08;
        IDT[index].zero = 0;
        IDT[index].type_attr = 0x8e;
        IDT[index].offset_higherbits = (address & 0xffff0000) >> 16;
    }

    IDT_Address = (uint64_t)IDT;
    IDT_Pointer[0] = (sizeof(IDT_Entry) * 256) + ((IDT_Address & 0xffff) << 16);
    load_idt(IDT_Pointer);

    print_string("Kernel - Intrerrupts initialized.\n");
}

void irq0_handler(void)
{
    pic_send_eoi(0);
}
 
void irq1_handler(void)
{
    pic_send_eoi(1);
}
 
void irq2_handler(void)
{
    pic_send_eoi(2);
}
 
void irq3_handler(void)
{
    pic_send_eoi(3);
}
 
void irq4_handler(void)
{
    pic_send_eoi(4);
}
 
void irq5_handler(void)
{
    pic_send_eoi(5);
}
 
void irq6_handler(void)
{
    pic_send_eoi(6);
}
 
void irq7_handler(void)
{
    pic_send_eoi(7);
}
 
void irq8_handler(void)
{
    pic_send_eoi(8);        
}
 
void irq9_handler(void)
{
    pic_send_eoi(9);
}
 
void irq10_handler(void)
{
    pic_send_eoi(10);
}
 
void irq11_handler(void)
{
    pic_send_eoi(11);
}
 
void irq12_handler(void)
{
    pic_send_eoi(12);
}
 
void irq13_handler(void)
{
    pic_send_eoi(13);
}
 
void irq14_handler(void)
{
    pic_send_eoi(14);
}
 
void irq15_handler(void)
{
    pic_send_eoi(15);
}