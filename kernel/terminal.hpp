#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.hpp"
#include "boot.hpp"

enum class PixelColor : u32 {
    White = 0x00FFFFFF,
    Black = 0x00000000,
    Red   = 0x000000FF,
    Blue  = 0x00FF0000,
    Green = 0x0000FF00
};

class BasicTerminal {
public:
    BasicTerminal(ScreenData& screenData, PSF1_FONT& sysFont);
    void setColor(const PixelColor color);
    //void write(const char* buffer, ...);
    void write(const char* string);
    //void write(const i64 number);
    //void write(const u64 number);
    //void write(const f64 number);
    void write(const char c);
    void clear();

private:
    struct ScreenPoint {
        u32 x;
        u32 y;
    };

    void newLine();
    void parseChar(const char c);
    void writePixel(const ScreenPoint& point);

private:
    PSF1_FONT& m_Font;
    ScreenData& m_ScreenData;
    ScreenPoint m_Cursor;
    u32 m_Color;
};

#endif // TERMINAL_H
