#include "io.hpp"

namespace IO::Bus {
    void send(u16 port, byte value)
    {
        __asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
    }

    byte recieve(u16 port)
    {
        byte value;
        __asm volatile ("inb %1, %0"
            : "=a"(value)
            : "Nd"(port)
        );
        return value;
    }

    void wait()
    {
        __asm volatile ("outb %0, $0x80" : : "a"((byte)0) );
    }
}
