#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdint.h>
#include <string.h>

namespace Util
{
template <typename T>
static inline T Abs(T value)
{
    if (value < 0)
        return -value;

    return value;
}

template <typename T>
static inline char *UintToString(T value, char *buffer, const uint8_t base)
{
    uint8_t i = 0;

    while (value)
    {
        const uint8_t remainder = value % base;

        if (remainder >= 10)
        {
            buffer[i++] = 65 + (remainder - 10);
        }
        else
        {
            //Convert number to ascii number
            buffer[i++] = 48 + remainder;
        }

        value /= base;
    }

    if (i == 0)
    {
        buffer[i++] = '0';
    }

    buffer[i] = '\0';

    return strrev(buffer);
}

template <typename T, typename U>
static inline char *IntToString(const T value, char *buffer, const uint8_t base)
{
    // consider absolute value of number

    U n = value;
    if (value < 0)
        n = -value;
    else
        n = value;

    uint8_t i = 0;
    while (n)
    {
        const uint8_t remainder = n % base;

        if (remainder >= 10)
            buffer[i++] = 65 + (remainder - 10);
        else
            buffer[i++] = 48 + remainder;

        n = n / base;
    }

    // if number is 0
    if (i == 0)
        buffer[i++] = '0';

    // If base is 10 and value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10)
        buffer[i++] = '-';

    buffer[i] = '\0'; // null terminate string

    // reverse the string and return it
    return strrev(buffer);
}

} // namespace Util

#endif