#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdint.h>
#include <avr/io.h>

#include "Util.hpp"

namespace Serial
{
// Initializes serial communication.
// Specify the baudrate at which communication will take place.
static inline void Begin(const uint32_t baudrate)
{
    // This is how the atmega328p datasheet says we should calculate this value.
    // This does not work with big baudrates tho.
    //const uint16_t ubrr = F_CPU / 16 / baudrate - 1;

    // So we stole the other formula from arduino libs.
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

// Receives a byte from serial.
// This function blocks so it will only return once it gets serial data.
static inline uint8_t Receive()
{
    /* Wait for data to be received */
    while (!(UCSR0A & (1 << RXC0)))
    {
    }
    /* Get and return received data from buffer */
    return UDR0;
}

// Transmits a byte.
// This function blocks so it will only return once the serial data is sent.
static inline void Transmit(const uint8_t data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)))
    {
    }
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

// Transmits a string
// Make sure the string is null-terminated or the function would print garbage.
static inline void TransmitString(const char *str)
{
    while (*str)
    {
        Transmit(*str);
        str++;
    }
}

// Transmits a signed int.
// It converts a signed int to a string and then transmits it.
// Use the base argument to change the base at which the number is printed.
template <typename T>
static inline void TransmitInt(const T number, const uint8_t base = 10)
{
    char buffer[(sizeof(T) * 8) + 1] = {0};

    if constexpr (Util::IsSame<T, int8_t>::value)
    {
        TransmitString(Util::IntToString<T, uint8_t>(number, buffer, base));
    }
    else if constexpr (Util::IsSame<T, int16_t>::value)
    {
        TransmitString(Util::IntToString<T, uint16_t>(number, buffer, base));
    }
    else if constexpr (Util::IsSame<T, int32_t>::value)
    {
        TransmitString(Util::IntToString<T, uint32_t>(number, buffer, base));
    }
    else
    {
        static_assert(Util::IsSame<T, int64_t>::value, "invalid int type");
        TransmitString(Util::IntToString<T, uint64_t>(number, buffer, base));
    }
}

// Transmits an unsigned int.
// It converts a unsigned int to a string and then transmits it.
// Use the base argument to change the base at which the number is printed.
template <typename T, uint8_t size = 0>
static inline void TransmitUint(const T number, const uint8_t base = 10)
{
    static_assert(Util::IsSame<T, uint8_t>::value || 
    Util::IsSame<T, uint16_t>::value || 
    Util::IsSame<T, uint32_t>::value ||
    Util::IsSame<T, uint64_t>::value,
    "Type T is not a valid uint");

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