#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.hpp"
#include "boot.hpp"

class BasicTerminal {
public:
    BasicTerminal(ScreenData& screenData, PSF1_FONT& sysFont);
    void write(const char* buffer, ...);
    void clear();
    void set_color(const u32 color);

private:
    struct ScreenPoint {
        u32 x;
        u32 y;
    };

    void new_line();
    void parse_char(const char c);
    void write_char(const char c);
    void write_string(const char* string);
    void write_unsigned(u64 number);
    void write_decimal(f64 number);
    void write_int(i64 number);

    void write_pixel(const ScreenPoint& point);

private:
    PSF1_FONT& m_Font;
    ScreenData& m_ScreenData;
    ScreenPoint m_Cursor;
    u32 m_Color;
    u32* m_Buffer;
};

#endif // TERMINAL_H
