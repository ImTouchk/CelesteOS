#include "print.h"
#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t letter;
    uint8_t color;
} Char;

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static size_t row = 0;
static size_t col = 0;
static uint8_t color = PRINT_COLOR_WHITE | (PRINT_COLOR_BLACK << 4);
static Char* buffer = (Char*)0xB8000;

static void print_clear_row(size_t row)
{
    Char empty = (Char) {
        letter: ' ',
        color: color
    };

    for(size_t i = 0; i < VGA_WIDTH; i++) {
        buffer[i * VGA_WIDTH + row] = empty;
    }
}

static void print_newline()
{
    col = 0;
    if(row < VGA_HEIGHT - 1) {
        row++;
        return;
    }

    for(size_t x = 0; x < VGA_WIDTH; x++) {
        for(size_t y = 1; y < VGA_HEIGHT; y++) {
            Char character = buffer[y * VGA_WIDTH + x];
            buffer[(y - 1) * VGA_WIDTH + x] = character;
        }
    }
    print_clear_row(VGA_WIDTH - 1);
}

static void print_char(char character)
{
    if(character == '\n') {
        print_newline();
        return;
    }
    if(col > VGA_WIDTH) {
        print_newline();
    }
    buffer[row * VGA_WIDTH + col] = (Char) {
        letter: (uint8_t)character,
        color: color,
    };
    col++;
}

void print_clear()
{
    for(size_t i = 0; i < VGA_WIDTH; i++) {
        print_clear_row(i);
    }
}

void print_set_color(PrintColor fg, PrintColor bg)
{
    color = (uint8_t)fg | (bg << 4);
}

void print_string(char* string)
{
    size_t off = 0;
    while(string[off] != '\0') {
        print_char(string[off]);
        off++;
    }
}

void draw_rect(PrintColor outline, size_t w, size_t h)
{
    const size_t width = 5 + w;
    const size_t height = 5 + h;
    for(size_t x = 0; x < width; x++) {
        for(size_t y = 0; y < height; y++) {
            buffer[y * VGA_WIDTH + x] = (Char) {
                letter: (uint8_t)178,
                color: outline,
            };
        }
    }
}