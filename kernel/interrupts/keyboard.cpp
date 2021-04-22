#include "keyboard.hpp"
#include "../terminal.hpp"

extern BasicTerminal* pTerminal;

namespace Interrupt::qwertyKB {
    const char asciiTable[] = {
        0, 0, '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', '0', '-',
        '=', 0, 0, 'q', 'w',
        'e', 'r', 't', 'y',
        'u', 'i', 'o', 'p',
        '[', ']', 0, 0, 'a',
        's', 'd', 'f', 'g',
        'h', 'j', 'k', 'l',
        ';', '\'', '`', 0,
        '\\', 'z', 'x', 'c',
        'v', 'b', 'n', 'm',
        ',', '.', '/', 0,
        '*', 0, ' '
    };

    char translate(byte scancode, bool uppercase)
    {
        if(scancode > 58) return 0;
        return asciiTable[scancode];
    }

    void handler(byte scancode)
    {
        char ascii = translate(scancode, false);
        char string[2] = { ascii, '\0' };
        if(ascii != 0) {
            pTerminal->print(string);
        }
    }
}
