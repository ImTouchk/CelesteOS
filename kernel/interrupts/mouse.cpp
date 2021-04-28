#include "io.hpp"
#include "mouse.hpp"

namespace Interrupt::mouse {
    void initialize()
    {
        IO::Bus::send(0x64, 0xA8); 
        /* enable aux device */
        mouse::wait();
        IO::Bus::send(0x64, 0x20);
        /*  ^----------------------------\
            notify the keyboard controller that
            we want to  send data to the mouse
         */
        mouse::waitInput();

        byte status = IO::Bus::recieve(0x60);
        status     |= 0b10;
        mouse::waitInput();

        IO::Bus::send(0x64, 0x60);
        mouse::wait();

        IO::Bus::send(0x60, status);
        /* set compaq status byte */

        mouse::write(0xF6); 
        /* default mouse settings */
        mouse::read();

        mouse::write(0xF4);
        mouse::read();
    }

    byte read()
    {
        mouse::waitInput();
        return IO::Bus::recieve(0x60);
    }

    void write(byte value)
    {
        mouse::wait();
        IO::Bus::send(0x64, 0x60);
        mouse::wait();
        IO::Bus::send(0x60, value);
    }

    void wait()
    {
        usize timeout = 100000;
        while(timeout-- > 0) {
            if((IO::Bus::recieve(0x64) & 0b10) == 0)
                return;
        }
    }

    void waitInput()
    {
        usize timeout = 100000;
        while(timeout-- > 0) {
            if(IO::Bus::recieve(0x64) & 0b1)
                return;
        }
    }
}
