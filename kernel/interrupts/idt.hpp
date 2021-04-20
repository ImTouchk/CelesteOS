#ifndef IDT_H
#define IDT_H

#include "../types.hpp"
#include "../memory/memory.hpp"

namespace Interrupt {
    struct entry {
        u16 offset0;
        u16 selector;
        byte zero;
        byte typeAttributes;
        u16 offset1;
        u32 offset2;
        u32 ignore;

        void setOffset(usize offset);
        u64 getOffset();
    };

    struct specialRegister {
        u16 limit;
        usize offset;
    } __attribute__((packed));

    namespace typeAttributes {
        constexpr int interruptGate  = 0b10001110;
        constexpr int callGate       = 0b10001100;
        constexpr int trapGate       = 0b10001111;
    }

    void initialize(
        Memory::runtime& runtime,
        Interrupt::specialRegister& idtr
    );
}

#endif // IDT_H
