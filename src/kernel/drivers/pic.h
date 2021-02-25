#ifndef PIC_H
#define PIC_H

#include <stdint.h>

/* 
    Programable Intrerrupt Controller (PIC)
*/

#define PIC1    0x20 /* IO base address for master PIC */
#define PIC2    0xA0 /* IO base address for slave PIC */

#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1 + 1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2 + 2)

#define PIC_EOI     0x20 /* End-of-intrerrupt command code */

void io_wait(void);
void outb(uint16_t port, uint8_t val);
uint8_t intb(uint16_t port);

void pic_send_eoi(uint8_t irq);

#endif // PIC_H