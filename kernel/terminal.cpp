#include "terminal.hpp"

BasicTerminal::BasicTerminal(ScreenData& screenData, PSF1_FONT& sysFont)
    : m_ScreenData(screenData), m_Font(sysFont)
{
    m_Cursor = { .x = 0, .y = 0 };
    m_Color  = 0x00FFFFFF;
}

void BasicTerminal::clear()
{
    const u32& bufferSize = m_ScreenData.bufferSize;
    u32* buffer           = (u32*)m_ScreenData.pFrontBuffer;

    for(u32 i = 0; i < bufferSize; i++) {
        buffer[i] = 0x00FF0000;
    }

    m_Cursor = { .x = 0, .y = 0 };
}

void BasicTerminal::setColor(const PixelColor color)
{
    m_Color = static_cast<u32>(color);
}

void BasicTerminal::write(const char *string)
{
    const char* current = string;
    while(*current != '\0') {
        parseChar(*current);
        current++;
    }
}

void BasicTerminal::write(const char c)
{
    char* fontGlyph    = (char*)m_Font.pGlyphBuffer + (c * m_Font.pHeader->charSize);
    ScreenPoint& point = m_Cursor;

    for(u32 y = point.y; y < (point.y + 16); y++) {
        for(u32 x = point.x; x < (point.x + 8); x++) {
            bool isGlyphPixel = (*fontGlyph & (0b100000000 >> (x - point.x))) > 0;
            if(isGlyphPixel) {
                ScreenPoint current = { .x = x, .y = y };
                writePixel(current);
            }
        }
    }
}

void BasicTerminal::writePixel(const ScreenPoint& point)
{
    u32* address = (u32*)m_ScreenData.pFrontBuffer;
    address[point.y * m_ScreenData.pxPerScanline + point.x] = m_Color;
}

void BasicTerminal::newLine()
{
    m_Cursor.x  = 0;
    m_Cursor.y += 16;

    if(m_Cursor.y >= m_ScreenData.height)
        clear();
}

void BasicTerminal::parseChar(const char c)
{
    if(c == '\n') {
        newLine();
        return;
    }

    write(c);
    m_Cursor.x += 8;
    if(m_Cursor.x >= m_ScreenData.pxPerScanline)
        newLine();
}