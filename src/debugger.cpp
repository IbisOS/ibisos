#include <debugger>
size_t row, column;
uint8_t terminalColor;
uint16_t *buffer;

/*
Function: makeEntryColor
Makes a VGA entry color using foreground and background
@fg: Foreground color
@bg: Background color
*/

uint8_t makeEntryColor(enum VGAColor fg, enum VGAColor bg)
{
    return fg | bg << 4;
}

/*
Function: makeEntry
Makes a VGA entry
@uc: Unsigned character to make entry using
@color: VGA entry color
*/

uint16_t makeEntry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

/*
Function: initializeTerminalScreen
Initializes terminal screen for debugging purposes
@fg: Foreground color
@bg: Background color
*/

void initializeTerminalScreen(enum VGAColor fg, enum VGAColor bg)
{
    row = 0;
    column = 0;
    terminalColor = makeEntryColor(fg, bg);
    buffer = (uint16_t *) 0xB8000;
    for(size_t y = 0; y < HEIGHT; y++)
    {
        for(size_t x = 0; x < WIDTH; x++)
        {
            const size_t index = y * WIDTH + x;
            buffer[index] = makeEntry(' ', terminalColor);
        }
    }
}

/*
Function: putEntry
Prints character to screen with color and coordinates
@c: Character to be printed
@color: Color of character
@x: X-coordinate to print at
@y: Y-coorindate to print at
*/

void putEntry(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * WIDTH + x;
    buffer[index] = makeEntry(c, color);
}

/*
Function: putChar
Prints character and adjusts row and column after
@c: Character to print
*/

void putChar(char c)
{
    if(c == '\n')
        row += 1;
    else
        putEntry(c, terminalColor, column, row);
    if(++column == WIDTH)
    {
        column = 0;
        if(++row == HEIGHT)
        {
            row = 0;
        }
    }
}

/*
Function: write
Writes string to screen given length
@str: String to write to screen
@len: Length of string
*/

void write(const char *str, size_t len)
{
    for(size_t index = 0; index < len; index++)
    {
        putChar(str[index]);
    }    
}

/*
Function: writeString
Writes string to screen
@str: String to write
*/

void writeString(const char *str)
{
    write(str, strlen(str));
}
