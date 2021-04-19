#include "terminal.hpp"

#include <stdarg.h>

BasicTerminal::BasicTerminal(ScreenData& screenData, PSF1_FONT& sysFont)
    : m_ScreenData(screenData), m_Font(sysFont)
{
    m_Cursor = { .x = 0, .y = 0 };
    m_Color  = 0x00FFFFFF;
    m_Buffer = static_cast<u32*>(m_ScreenData.pFrontBuffer);
}

void BasicTerminal::clear()
{
    for(u32 i = 0; i < m_ScreenData.bufferSize; i++) {
        m_Buffer[i] = m_Color;
    }
    m_Cursor = { .x = 0, .y = 0 };
}

void BasicTerminal::set_color(const u32 color)
{
    m_Color = color;
}

void BasicTerminal::write(const char* buffer, ...)
{
    va_list args;
    va_start(args, buffer);

    while(*buffer != '\0') {
        if(*buffer != '%') {
            parse_char(*buffer);
            ++buffer;
            continue;
        }

        switch(*(++buffer)) {
            case 'd': {
                i64 number = va_arg(args, i32);
                write_int(number);
                break;
            }
            case 'u': {
                u64 number = va_arg(args, u32);
                write_unsigned(number);
                break;
            }
            case 'f': {
                f64 number = va_arg(args, f64);
                write_decimal(number);
                break;
            }
            case 's': {
                char* string = va_arg(args, char*);
                write_string(string);
                break;
            }
            default: parse_char(*buffer); break;
        }

        ++buffer;
    }
}

void BasicTerminal::write_decimal(f64 number)
{
    const i64 integerPart = static_cast<i64>(number);
    write_int(integerPart);
    parse_char('.');

    if(number < 0) {
        number *= -1;
        /* write_int() already takes care of the negative sign */
    }

    constexpr int DECIMAL_COUNT = 4;
    char string[DECIMAL_COUNT + 1];
    byte count = 0;

    f64 decimals = number - integerPart;
    for(byte i = 0; i < DECIMAL_COUNT; i++) {
        decimals *= 10;
        string[count++] = '0' + (i64)decimals;
        decimals -= (i64)decimals;
    }

    string[count] = '\0';
    write_string(string);
}

void BasicTerminal::write_int(i64 number)
{
    if(number == 0) {
        parse_char('0');
        return;
    }

    if(number < 0) {
        parse_char('-');
        number *= -1;
    }

    write_unsigned(static_cast<u64>(number));
}

void BasicTerminal::write_unsigned(u64 number)
{
    if(number == 0) {
        parse_char('0');
        return;
    }

    char string[256];
    byte count    = 0;
    u64  reversed = 0;

    while(number != 0) {
        reversed  = (reversed * 10) + (number % 10);
        number   /= 10;
        count++;
    }

    for(byte i = 0; i < count; i++) {
        string[i] = '0' + (reversed % 10);
        reversed /= 10;
    }

    string[count] = '\0';
    write_string(string);
}

void BasicTerminal::write_string(const char* string)
{
    const char* current = string;
    while(*current != '\0') {
        parse_char(*current);
        current++;
    }
}

void BasicTerminal::new_line()
{
    m_Cursor.x = 0;
    m_Cursor.y += 16;
    if(m_Cursor.y >= m_ScreenData.height) {
        m_Color = 0x00FFFFFF;
        clear();
    }
}

void BasicTerminal::parse_char(const char c)
{
    if(c == '\n') {
        new_line();
        return;
    }

    write_char(c);
    m_Cursor.x += 8;
    if(m_Cursor.x >= m_ScreenData.pxPerScanline)
        new_line();
}

void BasicTerminal::write_char(const char c)
{
    char* fontGlyph    = (char*)m_Font.pGlyphBuffer + (c * m_Font.pHeader->charSize);
    ScreenPoint& point = m_Cursor;

    for(u32 y = point.y; y < (point.y + 16); y++) {
        for(u32 x = point.x; x < (point.x + 8); x++) {
            bool isGlyphPixel = (*fontGlyph & (0b100000000 >> (x - point.x))) > 0;
            if(isGlyphPixel) {
                ScreenPoint current = { x, y };
                write_pixel(current);
            }
        }
        fontGlyph++;
    }
}

void BasicTerminal::write_pixel(const ScreenPoint& point)
{
    const u32 index = point.y * m_ScreenData.pxPerScanline + point.x;
    m_Buffer[index] = m_Color;
}
