#ifndef IO_H
#define IO_H

#include "../types.hpp"

namespace IO::Bus {
    void send(u16 port, byte value);
    byte recieve(u16 port);
    void wait();
}


#endif // IO_H
