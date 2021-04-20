#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "idt.hpp"

namespace Interrupt {
    struct frame;
    __attribute__((interrupt)) void handler(frame* frame);
}

#endif // INTERRUPTS_H
