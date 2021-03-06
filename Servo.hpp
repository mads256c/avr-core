// Made by Gruppe 7
// Mads Bluhme Jeppesen
// Simon Søndergaard Christensen
// Mads Beyer Mogensen
// This group is lucky to get this amazing source code :)

#ifndef __SERVO_H__
#define __SERVO_H__

#include <stdint.h>
#include <avr/io.h>

// Encapsulates functions used to do servo pwm.
namespace Servo
{
// Calculated using (cpufrequency / prescaler) / newfrequency.
// Prescaler is chosen by the lowest that does not overflow the timer.
// We have 16 bits total so that can max hold the value 65535, until it overflows.
// If TOP is higher than that chose a higher prescaler.
// For other CPU frequencies another prescaler may be chosen.
// The lowest possible prescaler should be used to maximize precision.
constexpr uint16_t TOP = ((F_CPU) / 8) / 50; //20ms @ 16MHz

// These values describes the minimum and maximum duty cycles.
//constexpr uint16_t OCR1A_MIN = 40000 / 20; //20ms / 20 = 1ms
//constexpr uint16_t OCR1A_MAX = 40000 / 10; //20ms / 10 = 2ms

// Datasheet says that we the pulse has to be between 1-2ms.
// We tested the signal and the signal was perfect on the oscilloscope.
// But we only got a ~90 degree range doing that.
// To get the full 180 degree range; we had to guess the duty cycle.
// Yes you read right: We. Had. To. Guess...
// Anyway these are the values we got by increasing and decreasing
// the duty cycle until the servo could not go any further.
// ...Actually this did not give us the full advertized 180 degree range.
// We only got a 170 degree range. Yeah... Not only did the datasheet
// lie, but the servo did not even go the full 180 degrees.
// This was the point where we threw our hands up in the air and just went with it.

constexpr uint16_t OCR1A_MIN = TOP / 40;     //20ms / 40 = 0.5ms
constexpr uint16_t OCR1A_MAX = TOP / 8.8888; //20ms / 8.8888.. = 2.25ms

// Sets pin 9 up as a servo pwm pin, using timer1.
// Remember to set pin 9 to output before calling this.
static inline void Begin()
{
    ICR1 = TOP;        // Set TOP value.
    OCR1A = OCR1A_MAX; // Set the duty cycle just to make sure the servo gets a valid signal.

    // Enable fast pwm, on pin 9 with a prescaler of 8.
    TCCR1A = (1 << COM1A1) | (1 << WGM11);              // Enable PB1 (OC1A) for readwrite and waveform generator mode bit.
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Set second two waveform generator mode bits and prescaler.
}

// Sets the position of the servo.
// The valid values are between OCR1A_MIN and OCR1A_MAX
static inline void SetPosition(const uint16_t position)
{
    OCR1A = position;
}
} // namespace Servo
#endif
