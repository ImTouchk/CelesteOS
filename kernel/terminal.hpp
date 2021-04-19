#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.hpp"
#include "boot.hpp"

class BasicTerminal {
public:
    BasicTerminal(ScreenData& screenData, PSF1_FONT& sysFont);

    template<typename... Args>
    void print(Args&&... args)
    {
        (write(static_cast<Args&&>(args)), ...);
    }

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
    void write(const char* string);
    void write(usize number);
    void write(fsize number);
    void write(isize number);

    void write_pixel(const ScreenPoint& point);

private:
    PSF1_FONT& m_Font;
    ScreenData& m_ScreenData;
    ScreenPoint m_Cursor;
    u32 m_Color;
    u32* m_Buffer;
};

#endif // TERMINAL_H
