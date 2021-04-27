#include "keyboard.hpp"
#include "../terminal.hpp"

extern BasicTerminal* pTerminal;

static bool isLeftShiftPressed  = false;
static bool isRightShiftPressed = false;


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
        if(uppercase) return asciiTable[scancode] - 32;
        return asciiTable[scancode];
    }

    void handler(byte scancode)
    {
        switch(scancode) {
            case specialKeys::Spacebar: {
                pTerminal->space();
                return;
            }

            case specialKeys::LeftShift: isLeftShiftPressed = true; return;
            case specialKeys::LeftShift + 0x80: {
                isLeftShiftPressed = false;
                return;
            }

            case specialKeys::RightShift: isRightShiftPressed = true; return;
            case specialKeys::RightShift + 0x80: {
                isRightShiftPressed = false;
                return;
            }

            case specialKeys::Enter: {
                pTerminal->new_line();
                return;
            }

            case specialKeys::Backspace: {
                pTerminal->clear_last();
                return;
            }
            default: break;
        }

        char ascii = translate(scancode, isLeftShiftPressed | isRightShiftPressed);
        char string[2] = { ascii, '\0' };
        if(ascii != 0) {
            pTerminal->print(string);
        }
    }
}
