#ifndef INTERRUPT_MOUSE_H
#define INTERRUPT_MOUSE_H

namespace Interrupt::mouse {
    void initialize();
    void wait();
    void waitInput();
    void write(byte value);
    byte read();
}

#endif // INTERRUPT_MOUSE_H
