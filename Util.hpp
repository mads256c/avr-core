#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdint.h>
#include <string.h>

namespace Util
{
// Return the absolute value.
template <typename T, typename RetT = T>
constexpr inline RetT Abs(T value)
{
    if (value < 0)
        return -((RetT)value);

    return (RetT)value;
}

// Convert a uint to a string
template <typename T>
constexpr inline char *UintToString(T value, char *buffer, const uint8_t base)
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

// Convert int to string
template <typename T, typename U>
constexpr inline char *IntToString(const T value, char *buffer, const uint8_t base)
{
    // consider absolute value of number

    U n = Abs<T, U>(value);

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

char* ToUpper(char* str)
{
    char* p = str;
    while (p != 0)
    {
        if (p >= 'a' && p <= 'z')
        *p -= 32;
        p++;
    }
}

// These to structs are used to do type equallity at compile time.
template <typename T, typename U>
struct IsSame
{
    static const bool value = false;
};

template <typename T>
struct IsSame<T, T>
{
    static const bool value = true;
};

} // namespace Util

#endif