#include <cstring>

/*
Function: strlen
Gets length of string
@str: String to get length of
*/

size_t strlen(const char *str)
{
    size_t size = 0;
    while(str[size] != '\0')
    {
        size++;
    }
    return size;
}