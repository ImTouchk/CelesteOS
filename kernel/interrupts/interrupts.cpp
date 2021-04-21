#include "io.hpp"
#include "interrupts.hpp"

extern void Panic(const char* message);
extern BasicTerminal* pTerminal;

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

    __attribute__((interrupt)) void keyboardInterrupt(frame* frame)
    {
        pTerminal->print("a");

        byte scanCode = IO::Bus::recieve(0x60);
        PIC::endMaster();
    }
}

namespace Interrupt::PIC {
    void endMaster()
    {
        IO::Bus::send(PIC1_COMMAND, PIC_EOI);
    }

    void endSlave()
    {
        IO::Bus::send(PIC2_COMMAND, PIC_EOI);
        IO::Bus::send(PIC1_COMMAND, PIC_EOI);
    }
}

namespace Interrupt {
    void remapController()
    {
        byte a1, a2;

        a1 = IO::Bus::recieve(PIC1_DATA);
        IO::Bus::wait();

        a2 = IO::Bus::recieve(PIC2_DATA);
        IO::Bus::wait();

        IO::Bus::send(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
        IO::Bus::wait();

        IO::Bus::send(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
        IO::Bus::wait();

        IO::Bus::send(PIC1_DATA, 0x20);
        IO::Bus::wait();
        IO::Bus::send(PIC2_DATA, 0x28);
        IO::Bus::wait();

        IO::Bus::send(PIC1_DATA, 4);
        IO::Bus::wait();
        IO::Bus::send(PIC2_DATA, 2);
        IO::Bus::wait();

        IO::Bus::send(PIC1_DATA, ICW4_8086);
        IO::Bus::wait();
        IO::Bus::send(PIC2_DATA, ICW4_8086);
        IO::Bus::wait();

        IO::Bus::send(PIC1_DATA, a1);
        IO::Bus::wait();
        IO::Bus::send(PIC2_DATA, a2);
        IO::Bus::wait();
    }
}