// Lab_02 - Knight_Rider

/* Defines -----------------------------------------------------------*/
#define LED_RED1    PC1     // AVR pin where red LED is connected
#define LED_RED2    PC2
#define LED_RED3    PC3
#define LED_RED4    PC4
#define LED_RED5    PC5
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
	DDRC = DDRC | (1<<LED_RED1);		// Set pin as output in Data Direction Register...
	PORTC = PORTC & ~(1<<LED_RED1);		// ...and turn LED off in Data Register
	DDRC = DDRC | (1<<LED_RED2);
	PORTC = PORTC & ~(1<<LED_RED2);
	DDRC = DDRC | (1<<LED_RED3);
	PORTC = PORTC & ~(1<<LED_RED3);
	DDRC = DDRC | (1<<LED_RED4);
	PORTC = PORTC & ~(1<<LED_RED4);
	DDRC = DDRC | (1<<LED_RED5);
	PORTC = PORTC & ~(1<<LED_RED5);

	// Program uses 2 leds shining at once instead of 1 for more KnigtRideder style
		PORTC = PORTC ^ (1<<LED_RED1);	// zapne 1
		_delay_ms(BLINK_DELAY);
	while (1)
	{
		PORTC = PORTC ^ (1<<LED_RED2);	// zapne 2
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED1);	// vypne 1
		PORTC = PORTC ^ (1<<LED_RED3);	// zapne 3
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED2);	// vypne 2
		PORTC = PORTC ^ (1<<LED_RED4);	// zapne 4
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED3);	// vypne 3
		PORTC = PORTC ^ (1<<LED_RED5);	// zapne 5
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED4);	// vypne 4
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED4);	// zapne 4
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED5);	// vypne 5
		PORTC = PORTC ^ (1<<LED_RED3);	// zapne 3
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED4);	// vypne 4
		PORTC = PORTC ^ (1<<LED_RED2);	// zapne 2
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED3);	// vypne 3
		PORTC = PORTC ^ (1<<LED_RED1);	// zapne 1
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED2);	// vypne 2
		_delay_ms(BLINK_DELAY);
	}
	return 0;
}