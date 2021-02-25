#include "print.h"
#include "drivers/idt.h"

#include <stdint.h>
#include <stddef.h>

void stoi(int number, char* string)
{
    size_t off = 0;
    while(number != 0) {
        string[off] = '0' + (number % 10);
        number /= 10;
        off++;
    }
}

void kernel_main()
{
    print_clear();
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);

    irq_init();
    
    while(1) {
        asm volatile ("hlt");
    }
}