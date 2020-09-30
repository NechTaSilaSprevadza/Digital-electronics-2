// Lab_02 - leds

/* Defines -----------------------------------------------------------*/
#define LED_GREEN   PB5     // AVR pin where green LED is connected
#define LED_RED     PC0
#define BTN			PD0
#define BLINK_DELAY 250
#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions

/* Functions ---------------------------------------------------------*/
int main(void)
{
    /* GREEN LED */
    // Set pin as output in Data Direction Register...
    DDRB = DDRB | (1<<LED_GREEN);
    // ...and turn LED off in Data Register
    PORTB = PORTB & ~(1<<LED_GREEN);

    /* second LED */
    // WRITE YOUR CODE HERE
    DDRC = DDRC | (1<<LED_RED);
	PORTC = PORTC & ~(1<<LED_RED);
	
	/* button pullup */
    DDRD = DDRD & ~ (1<<BTN);
    PORTD = PORTD | (1<<BTN);
	

    // Infinite loop
    while (1)
    {
        // Pause several milliseconds
        _delay_ms(BLINK_DELAY);

		if (bit_is_clear(PIND, BTN))
		{
			// WRITE YOUR CODE HERE
			// Invert LED in Data Register
			// PORTB = PORTB xor 0010 0000
			PORTB = PORTB ^ (1<<LED_GREEN);
			PORTC = PORTC ^ (1<<LED_RED);
		}
    }

    // Will never reach this
    return 0;
}