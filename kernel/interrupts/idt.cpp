#include "idt.hpp"
#include "interrupts.hpp"
#include "mouse.hpp"
#include "io.hpp"

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
        pageFault->setOffset((usize)handlers::pageFault);
        pageFault->typeAttributes = typeAttributes::interruptGate;
        pageFault->selector = 0x08;

        entry* doubleFault = (entry*)(idtr.offset + 0x8 * sizeof(entry));
        doubleFault->setOffset((usize)handlers::doubleFault);
        doubleFault->typeAttributes = typeAttributes::interruptGate;
        doubleFault->selector       = 0x08;

        entry* generalProtFault = (entry*)(idtr.offset + 0xD * sizeof(entry));
        generalProtFault->setOffset((usize)handlers::generalProtFault);
        generalProtFault->typeAttributes = typeAttributes::interruptGate;
        generalProtFault->selector       = 0x08;

        entry* keyboardInter = (entry*)(idtr.offset + 0x21 * sizeof(entry));
        keyboardInter->setOffset((usize)handlers::keyboardInterrupt);
        keyboardInter->typeAttributes = typeAttributes::interruptGate;
        keyboardInter->selector       = 0x08;
        
        entry* mouseInter = (entry*)(idtr.offset + 0x2C * sizeof(entry));
        mouseInter->setOffset((usize)handlers::mouseInterrupt);
        mouseInter->typeAttributes = typeAttributes::interruptGate;
        mouseInter->selector       = 0x08;

        __asm("lidt %0" : : "m" (idtr));
        Interrupt::remapController();
        __asm("sti");

        mouse::initialize();
        IO::Bus::send(PIC1_DATA, 0b11111001);
        IO::Bus::send(PIC2_DATA, 0b11101111);
    }
}
