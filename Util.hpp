#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdint.h>
#include <string.h>

namespace Util
{
    static inline char* Uint8ToString(const uint8_t value, char* buffer, const uint8_t base)
    {
        uint8_t n = value;

        uint8_t i = 0;

        while (n)
        {
            const uint8_t remainder = n % base;

            if (remainder >= 10)
            {
                buffer[i++] = 65 + (remainder - 10);
            }
            else
            {
                //Convert number to ascii number
                buffer[i++] = 48 + remainder;
            }
            
            n /= base;
        }

        if (i == 0)
        {
            buffer[i++] = '0';
        }

        buffer[i] = '\0';

        return strrev(buffer);
    }

} // Util




#endif