#include <cstddef>
#include <cstdint>

enum color
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    GREY = 7,
    DARK_GREY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15,
};

using namespace std;

std::uint8_t entryColor(enum color fg, enum color bg)
{
    return fg | bg << 4;
}

std::uint16_t entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char *str)
{
    size_t len = 0;
    while(str[len])
        len++;
    return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t row, column;
uint8_t terminalColor;
uint16_t *buffer;

void initializeTerminal()
{
    row = 0;
    column = 0;
    terminalColor = entryColor(RED, WHITE);
    buffer = (uint16_t *) 0xB8000;
    for(size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for(size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            buffer[index] = entry(' ', terminalColor);
        }
    }
}

void setTerminalColor(uint8_t color)
{
    terminalColor = color;
}

void putEntry(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    buffer[index] = entry(c, color);
}

void putChar(char c)
{
    putEntry(c, terminalColor, column, row);
    if(++column == VGA_WIDTH)
    {
        column = 0;
        if(++row == VGA_HEIGHT)
        {
            row = 0;
        }
        if(c == '\n')
            row++;
    }
}

void write(const char *data, size_t size)
{
    for(size_t i = 0; i < size; i++)
        putChar(data[i]);
}

void writeString(const char *data)
{
    write(data, strlen(data));
}

extern "C" void kernelMain()
{
    initializeTerminal();
    writeString("Hello world!");
}