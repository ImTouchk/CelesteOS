#include "interrupts.hpp"

extern void Panic(const char* message);

namespace Interrupt::handlers {
    __attribute__((interrupt)) void pageFault(frame* frame)
    {
        Panic("Page fault occurred.");
        while(true) {}
    }

    __attribute__((interrupt)) void doubleFault(frame* frame)
    {
        Panic("Double fault occurred.");
        while(true) {}
    }

    __attribute__((interrupt)) void generalProtFault(frame* frame)
    {
        while(true) {}
    }
}