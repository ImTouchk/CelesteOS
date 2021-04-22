#ifndef INTERRUPT_KEYBOARD_H
#define INTERRUPT_KEYBOARD_H

#include "../types.hpp"

namespace Interrupt::qwertyKB {
    extern const char asciiTable[];
    char translate(byte scancode, bool uppercase);

    void handler(byte scancode);
}

#endif // INTERRUPT_KEYBOARD_H
