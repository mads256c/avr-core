#ifndef __SERVO_H__
#define __SERVO_H__

#include <stdint.h>
#include <avr/io.h>

namespace Servo
{
    constexpr uint16_t TOP = 40000; //20ms @ 16MHz
    constexpr uint16_t OCR1A_MIN = 40000 / 20; //20ms / 20 = 1ms
    constexpr uint16_t OCR1A_MAX = 40000 / 10; //20ms / 10 = 2ms

    // Remember to set pin 9 to output.
    static inline void Begin()
    {
        ICR1 = TOP;        //Set TOP value 
	    OCR1A = OCR1A_MAX; //Set Duty cycle

        TCCR1A = (1 << COM1A1) | (1 << WGM11);              //Set COM bits and the first two waveform generator bits
        TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); //Set second two waveform generator bits and prescalar 
    }

    static inline void SetPosition(const uint16_t position)
    {
        OCR1A = position;
    }
}
#endif