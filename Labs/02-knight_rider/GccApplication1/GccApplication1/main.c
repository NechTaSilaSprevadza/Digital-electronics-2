// Adam Budac - Lab_02_Knight_Rider

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
		PORTC = PORTC ^ (1<<LED_RED1);	// turn 1 on
		_delay_ms(BLINK_DELAY);
	while (1)
	{
		PORTC = PORTC ^ (1<<LED_RED2);	// turn 2 on
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED1);	// turn 1 off
		PORTC = PORTC ^ (1<<LED_RED3);	// turn 3 on
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED2);	// turn 2 off
		PORTC = PORTC ^ (1<<LED_RED4);	// turn 4 on
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED3);	// turn 3 off
		PORTC = PORTC ^ (1<<LED_RED5);	// turn 5 on
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED4);	// turn 4 off
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED4);	// turn 4 on 
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED5);	// turn 5 off
		PORTC = PORTC ^ (1<<LED_RED3);	// turn 3 on
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED4);	// turn 4 off
		PORTC = PORTC ^ (1<<LED_RED2);	// turn 2 on
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED3);	// turn 3 off
		PORTC = PORTC ^ (1<<LED_RED1);	// turn 1 on
		_delay_ms(BLINK_DELAY);
		PORTC = PORTC ^ (1<<LED_RED2);	// turn 2 off
		_delay_ms(BLINK_DELAY);
	}
	return 0;
}