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

static inline void TransmitString(const char* str)
{
    while(*str)
    {
        Transmit(*str);
        str++;
    }
}

static inline void TransmitUint8(const uint8_t number, const uint8_t base = 10)
{
    char buffer[9] = {0};

    TransmitString(Util::Uint8ToString(number, buffer, base));
}

static inline void TransmitUint16(const uint16_t number, const uint8_t base = 10)
{
    char buffer[17] = {0};

    TransmitString(Util::Uint16ToString(number, buffer, base));
}

static inline void TransmitUint32(const uint32_t number, const uint8_t base = 10)
{
    char buffer[33] = {0};

    TransmitString(Util::Uint32ToString(number, buffer, base));
}

static inline void TransmitUint64(const uint64_t number, const uint8_t base = 10)
{
    char buffer[65] = {0};

    TransmitString(Util::Uint64ToString(number, buffer, base));
}
} // namespace Serial

#endif