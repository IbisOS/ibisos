#ifndef _KPRINTF_H_
#define _KPRINTF_H_
#define WIDTH 80
#define HEIGHT 25
#include <c/stddef.h>
#include <c/stdint.h>
#include <c/string.h>

enum VGAColor {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GREY = 7,
    DARK_GREY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15,
};

/*
Function: makeEntryColor
Makes a VGA entry color using foreground and background
@fg: Foreground color
@bg: Background color
*/

uint8_t makeEntryColor(enum VGAColor fg, enum VGAColor bg);

/*
Function: makeEntry
Makes a VGA entry
@uc: Unsigned character to make entry using
@color: VGA entry color
*/

uint16_t makeEntry(unsigned char uc, uint8_t color);

/*
Function: initializeTerminalScreen
Initializes terminal screen for debugging purposes
@fg: Foreground color
@bg: Background color
*/

void initializeTerminalScreen(enum VGAColor fg, enum VGAColor bg);

/*
Function: putEntry
Prints character to screen with color and coordinates
@c: Character to be printed
@color: Color of character
@x: X-coordinate to print at
@y: Y-coorindate to print at
*/

void putEntry(char c, uint8_t color, size_t x, size_t y);

/*
Function: putChar
Prints character and adjusts row and column after
@c: Character to print
*/

void putChar(char c);

/*
Function: write
Writes string to screen given length
@str: String to write to screen
@len: Length of string
*/

void write(const char *str, size_t len);

/*
Function: writeString
Writes string to screen
@str: String to write
*/

void writeString(const char *str);
#endif