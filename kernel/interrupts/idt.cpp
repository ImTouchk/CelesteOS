#include "idt.hpp"
#include "interrupts.hpp"

namespace Interrupt {
    void entry::setOffset(usize offset)
    {
        offset0 = (u16)(offset & 0x000000000000ffff);
        offset1 = (u16)((offset & 0x00000000ffff0000) >> 16);
        offset2 = (u32)((offset & 0xffffffff00000000) >> 32);
    }

    u64 entry::getOffset()
    {
        u64 offset = 0;
        offset |= (u64)offset0;
        offset |= (u64)offset1 << 16;
        offset |= (u64)offset2 << 32;
        return offset;
    }

    void initialize(Memory::runtime& runtime, Interrupt::specialRegister& idtr)
    {
        idtr = {
            .limit  = 0x0FFF,
            .offset = (u64)runtime.frameAllocator.request()
        };

        entry* pageFault = (entry*)(idtr.offset + 0xE * sizeof(entry));
        pageFault->setOffset((usize)handler);
        pageFault->typeAttributes = typeAttributes::interruptGate;
        pageFault->selector = 0x08;

        __asm("lidt %0" : : "m" (idtr));
    }
}
