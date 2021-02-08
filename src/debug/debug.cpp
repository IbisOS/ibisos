#include <debug/debug.h>


int x = 1;
int y = 1;

void debug(const char *text)
{
    //Write the text
    write_string(text, x, y, WHITE, DEFAULT_BACKGROUND_COLOR);
    //Increase y by 18 bits
    y += 18;
}