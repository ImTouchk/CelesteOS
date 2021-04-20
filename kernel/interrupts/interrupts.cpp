#include "interrupts.hpp"

namespace Interrupt {
    __attribute__((interrupt)) void handler(frame* frame)
    {
        while(true) {
            __asm("cli");
            __asm("hlt");
        }
    }
}