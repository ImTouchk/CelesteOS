#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.hpp"
#include "boot.hpp"

class BasicTerminal {
public:
    BasicTerminal(Boot::screenData& screenData, Boot::systemFont& sysFont);

    template<typename... Args>
    void print(Args&&... args)
    {
        (write(static_cast<Args&&>(args)), ...);
    }

    void clear(const u32 color);
    void set_color(const u32 color);
    void new_line();
    void clear_last();
    void space();

private:
    struct ScreenPoint {
        u32 x;
        u32 y;
    };

    void parse_char(const char c);
    void write(const char c);
    void write(const char* string);
    void write(usize number);
    void write(fsize number);
    void write(isize number);

    void write_pixel(const ScreenPoint& point);

private:
    Boot::systemFont& m_Font;
    Boot::screenData& m_ScreenData;
    ScreenPoint m_Cursor;
    u32  m_Color;
    u32* m_Buffer;
    u32  m_BufferSize;
};

#endif // TERMINAL_H
