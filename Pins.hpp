#ifndef __PINS_H__
#define __PINS_H__

#include <stdint.h>
#include <avr/io.h>
#include <avr/builtins.h>

// Encapsulates functions specific to pin manipulation.
namespace Pins
{
// Set the pin mode on a specific pin.
// pin: which pin
// mode: true = output, false = input.
static inline void PinMode(const uint8_t pin, const bool mode)
{
	if (pin <= 7)
	{
		if (mode)
		{
			DDRD |= 1 << pin;
		}
		else
		{
			DDRD &= ~(1 << pin);
		}
	}

	else if (pin <= 13)
	{
		if (mode)
		{
			DDRB |= 1 << (pin - 8);
		}
		else
		{
			DDRB &= ~(1 << (pin - 8));
		}
	}

	else
	{
		if (mode)
		{
			DDRC |= 1 << (pin - 14);
		}
		else
		{
			DDRC &= ~(1 << (pin - 14));
		}
	}
}

// Read a digital pin.
// pin: the pin to read.
static inline bool DigitalRead(const uint8_t pin)
{
	if (pin <= 7)
	{
		return PIND & (1 << pin);
	}

	else if (pin <= 13)
	{
		return PINB & (1 << (pin - 8));
	}

	else
	{
		return PINC & (1 << (pin - 14));
	}
}

// Write to a digital pin.
// pin: the pin to write to
// value: the state the pin should be
static inline void DigitalWrite(const uint8_t pin, const bool value)
{
	if (pin <= 7)
	{
		if (value)
		{
			PORTD |= 1 << pin;
		}
		else
		{
			PORTD &= ~(1 << pin);
		}
	}

	else if (pin <= 13)
	{
		if (value)
		{
			PORTB |= 1 << (pin - 8);
		}
		else
		{
			PORTB &= ~(1 << (pin - 8));
		}
	}

	else
	{
		if (value)
		{
			PORTC |= 1 << (pin - 14);
		}
		else
		{
			PORTC &= ~(1 << (pin - 14));
		}
	}
}

} // namespace Pins

#endif //__PINS_H__
