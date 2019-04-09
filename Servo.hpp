#ifndef __SERVO_H__
#define __SERVO_H__

#include <stdint.h>
#include <avr/io.h>

namespace Servo
{
    constexpr uint16_t TOP = 40000; //20ms @ 16MHz

    //constexpr uint16_t OCR1A_MIN = 40000 / 20; //20ms / 20 = 1ms
    //constexpr uint16_t OCR1A_MAX = 40000 / 10; //20ms / 10 = 2ms

    //Datasheet says that we the pulse has to be between 1-2ms.
    //But we only get ~90 degrees doing that.
    //To get the full 180 we need to guess the duty cycle.
    //Yes you read right: We. Have. To. Guess...
    //Anyway these are the values we got by increasing and decreasing 
    //values until the servo could not go any further.

    constexpr uint16_t OCR1A_MIN = 40000 / 40; //20ms / 40 = 0.5ms
    constexpr uint16_t OCR1A_MAX = 40000 / 8.8888; //20ms / 8.8888.. = 2.25ms


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