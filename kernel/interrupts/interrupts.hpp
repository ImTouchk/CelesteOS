#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "idt.hpp"

namespace Interrupt {
    struct frame;

    namespace handlers {
        __attribute__((interrupt)) void pageFault(frame* frame);
        __attribute__((interrupt)) void doubleFault(frame* frame);
        __attribute__((interrupt)) void generalProtFault(frame* frame);
    }
}

#endif // INTERRUPTS_H
