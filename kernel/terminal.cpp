#include "terminal.hpp"

BasicTerminal::BasicTerminal(Boot::screenData& screenData, Boot::systemFont& sysFont)
    : m_ScreenData(screenData), m_Font(sysFont), m_Cursor{0, 0 }
{
    m_Color      = 0x00FFFFFF;
    m_Buffer     = static_cast<u32*>(m_ScreenData.pFrontBuffer);
    m_BufferSize = m_ScreenData.pxPerScanline * m_ScreenData.height;
}

void BasicTerminal::clear(const u32 color)
{
    if(m_ScreenData.pxPerScanline == m_ScreenData.width) {
        /* execute fast method (which works on most computers) */
        for(u32 i = 0; i < m_BufferSize; i++) {
            m_Buffer[i] = color;
        }
    } else {
        /* execute slower method */
        for(u32 x = 0; x < m_ScreenData.pxPerScanline; x++) {
            for(u32 y = 0; y < m_ScreenData.height; y++) {
                m_Buffer[y * m_ScreenData.pxPerScanline + x] = color;
            }
        }
    }

    m_Cursor = { .x = 0, .y = 0 };
}

void BasicTerminal::set_color(const u32 color)
{
    m_Color = color;
}

void BasicTerminal::write(fsize number)
{
    write(static_cast<isize>(number));
    parse_char('.');

    if(number < 0) {
        number *= -1;
        /* write_int() already takes care of the negative sign */
    }

    constexpr int DECIMAL_COUNT = 4;
    char string[DECIMAL_COUNT + 1];
    byte count = 0;

    f64 decimals = number - static_cast<isize>(number);
    for(byte i = 0; i < DECIMAL_COUNT; i++) {
        decimals *= 10;
        string[count++] = '0' + static_cast<isize>(decimals);
        decimals -= static_cast<isize>(decimals);
    }

    string[count] = '\0';
    write(string);
}

void BasicTerminal::write(isize number)
{
    if(number == 0) {
        parse_char('0');
        return;
    }

    if(number < 0) {
        parse_char('-');
        number *= -1;
    }

    write(static_cast<usize>(number));
}

void BasicTerminal::write(usize number)
{
    if(number == 0) {
        parse_char('0');
        return;
    }

    char string[256];
    byte  count    = 0;
    usize reversed = 0;

    while(number != 0) {
        reversed  = (reversed * 10) + (number % 10);
        number   /= 10;
        count++;
    }

    for(byte i = 0; i < count; i++) {
        string[i] = '0' + (byte)(reversed % 10);
        reversed /= 10;
    }

    string[count] = '\0';
    write(string);
}

void BasicTerminal::write(const char* string)
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
        clear(0x00000000);
    }
}

void BasicTerminal::parse_char(const char c)
{
    if(m_Cursor.y >= m_ScreenData.height) {
        clear(0x00000000);
    }

    if(c == '\n') {
        new_line();
        return;
    }

    write(c);
    m_Cursor.x += 8;
    if(m_Cursor.x >= m_ScreenData.pxPerScanline)
        new_line();
}

void BasicTerminal::write(const char c)
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
