#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "idt.hpp"

namespace Interrupt {
    struct frame;

    namespace handlers {
        __attribute__((interrupt)) void pageFault(frame* frame);
        __attribute__((interrupt)) void doubleFault(frame* frame);
        __attribute__((interrupt)) void generalProtFault(frame* frame);
        __attribute__((interrupt)) void keyboardInterrupt(frame* frame);
        __attribute__((interrupt)) void mouseInterrupt(frame* frame);
    }

    constexpr int PIC1_COMMAND = 0x20;
    constexpr int PIC1_DATA    = 0x21;
    constexpr int PIC2_COMMAND = 0xA0;
    constexpr int PIC2_DATA    = 0xA1;
    constexpr int ICW1_INIT    = 0x10;
    constexpr int ICW1_ICW4    = 0x01;
    constexpr int ICW4_8086    = 0x01;
    constexpr int PIC_EOI      = 0x20;

    void remapController();

    namespace PIC {
        void endMaster();
        void endSlave();
    }
}

#endif // INTERRUPTS_H
