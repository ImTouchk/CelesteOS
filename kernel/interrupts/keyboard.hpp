#ifndef INTERRUPT_KEYBOARD_H
#define INTERRUPT_KEYBOARD_H

#include "../types.hpp"

namespace Interrupt::qwertyKB {
    extern const char asciiTable[];
    char translate(byte scancode, bool uppercase);

    void handler(byte scancode);

    namespace specialKeys {
        constexpr int LeftShift  = 0x2A;
        constexpr int RightShift = 0x36;
        constexpr int Enter      = 0x1C;
        constexpr int Backspace  = 0x0E;
        constexpr int Spacebar   = 0xB9;
    };
}

#endif // INTERRUPT_KEYBOARD_H
