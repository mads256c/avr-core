#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdint.h>
#include <string.h>

namespace Util
{
    static inline char* Uint8ToString(uint8_t value, char* buffer, const uint8_t base)
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

    static inline char* Uint16ToString(uint16_t value, char* buffer, const uint8_t base)
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

} // Util




#endif