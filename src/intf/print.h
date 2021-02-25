#ifndef PRINT_H
#define PRINT_H

#include <stddef.h>

typedef enum {
    PRINT_COLOR_BLACK = 0,
	PRINT_COLOR_BLUE = 1,
	PRINT_COLOR_GREEN = 2,
	PRINT_COLOR_CYAN = 3,
	PRINT_COLOR_RED = 4,
	PRINT_COLOR_MAGENTA = 5,
	PRINT_COLOR_BROWN = 6,
	PRINT_COLOR_LIGHT_GREY = 7,
	PRINT_COLOR_DARK_GREY = 8,
	PRINT_COLOR_LIGHT_BLUE = 9,
	PRINT_COLOR_LIGHT_GREEN = 10,
	PRINT_COLOR_LIGHT_CYAN = 11,
	PRINT_COLOR_LIGHT_RED = 12,
	PRINT_COLOR_LIGHT_MAGENTA = 13,
	PRINT_COLOR_LIGHT_BROWN = 14,
	PRINT_COLOR_WHITE = 15,
} PrintColor;

void print_clear();
void print_set_color(PrintColor fg, PrintColor bg);
void print_string(char* string);

void draw_rect(PrintColor outline, size_t w, size_t h);

#endif