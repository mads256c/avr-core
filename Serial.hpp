#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdint.h>
#include <avr/io.h>

#include "Util.hpp"

#define F_CPU 16000000UL

namespace Serial
{
static inline void Begin(const uint32_t baudrate)
{
    //const uint16_t ubrr = F_CPU / 16 / baudrate - 1;
    const uint16_t ubrr = (F_CPU / 8 / baudrate - 1) / 2;

    /*Set baud rate */
    /* UBRR0H contains the 4 most significant bits of the
	baud rate. UBRR0L contains the 8 least significant
	bits.*/
    UBRR0H = (uint8_t)((ubrr >> 8) & 0xFF);
    UBRR0L = (uint8_t)(ubrr & 0xFF);

    /*Enable transmitter and receiver*/
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    /* Set frame format: 8data */
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

static inline uint8_t Receive()
{
    /* Wait for data to be received */
    while (!(UCSR0A & (1 << RXC0)))
    {
    }
    /* Get and return received data from buffer */
    return UDR0;
}

static inline void Transmit(const uint8_t data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)))
    {
    }
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

static inline void TransmitString(const char *str)
{
    while (*str)
    {
        Transmit(*str);
        str++;
    }
}

template<typename T>
static inline void TransmitInt(const T number, const uint8_t base = 10)
{
    if constexpr (Util::IsSame<T, int8_t>::value)
    {
        char buffer[(sizeof(T) * 8) + 1] = {0};

        TransmitString(Util::IntToString<T, uint8_t>(number, buffer, base));
    }
    else
    {
        static_assert(false, "T is not a int type");
    }
    
}

static inline void TransmitInt8(const int8_t number, const uint8_t base = 10)
{
    char buffer[9] = {0};

    TransmitString(Util::IntToString<int8_t, uint8_t>(number, buffer, base));
}

static inline void TransmitInt16(const int16_t number, const uint8_t base = 10)
{
    char buffer[17] = {0};

    TransmitString(Util::IntToString<int16_t, uint16_t>(number, buffer, base));
}

static inline void TransmitInt32(const int32_t number, const uint8_t base = 10)
{
    char buffer[33] = {0};

    TransmitString(Util::IntToString<int32_t, uint32_t>(number, buffer, base));
}

static inline void TransmitInt64(const int64_t number, const uint8_t base = 10)
{
    char buffer[65] = {0};

    TransmitString(Util::IntToString<int64_t, uint64_t>(number, buffer, base));
}

//T is the type of uint to print
//size is the size of the buffer array. It should be number of digits + 1.
//If size is 0 it uses a safe but huge buffer array.
template <typename T, uint8_t size = 0>
static inline void TransmitUint(const T number, const uint8_t base = 10)
{
    if constexpr (size == 0)
    {
        //Convert byte length to bits and add 1 for null terminator.
        char buffer[(sizeof(T) * 8) + 1] = {0};

        TransmitString(Util::UintToString<T>(number, buffer, base));
    }
    else
    {
        char buffer[size] = {0};

        TransmitString(Util::UintToString<T>(number, buffer, base));
    }
}
} // namespace Serial

#endif