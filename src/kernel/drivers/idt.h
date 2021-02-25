#ifndef IDT_H
#define IDT_H

#include <stdint.h>

typedef struct __attribute__((__packed__)) {
    uint16_t offset_lowerbits;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_higherbits;
} IDT_Entry;

void irq_init(void);
void irq0_handler(void);  void irq8_handler(void);
void irq1_handler(void);  void irq9_handler(void);
void irq2_handler(void);  void irq10_handler(void);
void irq3_handler(void);  void irq11_handler(void);
void irq4_handler(void);  void irq12_handler(void);
void irq5_handler(void);  void irq13_handler(void);
void irq6_handler(void);  void irq14_handler(void);
void irq7_handler(void);  void irq15_handler(void);

#endif // IDT_H