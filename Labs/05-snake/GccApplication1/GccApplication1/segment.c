
#define F_CPU 16000000

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>
#include "gpio.h"
#include "segment.h"

/* Variables ---------------------------------------------------------*/
// Active-low LED postions a to f
uint8_t segment_value[] = {
	// abcdefgDP
	0b01111111,      // LED a
	0b10111111,      // LED b
	0b11011111,      // LED c
	0b11101111,      // LED d
	0b11110111,      // LED e
	0b11111011,      // LED f
	};

// Active-high position 0 to 3
uint8_t segment_position[] = {
	// p3p2p1p0....
	0b00010000,   // Position 0
	0b00100000,   // Position 1
	0b01000000,   // Position 2
	0b10000000,   // Position 3
	};

/* Function definitions ----------------------------------------------*/
void SEG_init(void)
{
    /* Configuration of SSD signals */
    GPIO_config_output(&DDRD, SEGMENT_LATCH);
    GPIO_config_output(&DDRD, SEGMENT_CLK);
    GPIO_config_output(&DDRB, SEGMENT_DATA);
}

/*--------------------------------------------------------------------*/
void SEG_update_shift_regs(uint8_t segments, uint8_t position)
{
    uint8_t bit_number;
	segments = segment_value[segments];     // 0, 1, ..., 9
	position = segment_position[position];  // 0, 1, 2, 3
	
    // Pull LATCH, CLK, and DATA low
	GPIO_write_low(&PORTD, SEGMENT_LATCH);
	GPIO_write_low(&PORTD, SEGMENT_CLK);
	GPIO_write_low(&PORTB, SEGMENT_DATA);
	
    // Wait 1 us
	_delay_us(1);
	
    // Loop through the 1st byte (segments)
    // a b c d e f g DP (active low values)
    for (bit_number = 0; bit_number < 8; bit_number++)
    {
        // Output DATA value (bit 0 of "segments")
		if ((segments & 1) == 0)
		{
			GPIO_write_low(&PORTB, SEGMENT_DATA);
		}
		else
		{
			GPIO_write_high(&PORTB, SEGMENT_DATA);
		}
		
        // Wait 1 us
		_delay_us(1);
		
        // Pull CLK high
		GPIO_write_high(&PORTD, SEGMENT_CLK);
		
        // Wait 1 us
		_delay_us(1);
		
        // Pull CLK low
		GPIO_write_low(&PORTD, SEGMENT_CLK);
		
        // Shift "segments"
        segments = segments >> 1;
    }

    // Loop through the 2nd byte (position)
    // p3 p2 p1 p0 . . . . (active high values)
    for (bit_number = 0; bit_number < 8; bit_number++)
    {
        // Output DATA value (bit 0 of "position")
		if ((position & 1) == 0)
		{
			GPIO_write_low(&PORTB, SEGMENT_DATA);
		}
		else
		{
			GPIO_write_high(&PORTB, SEGMENT_DATA);
		}
		
        // Wait 1 us
        _delay_us(1);
		
        // Pull CLK high
        GPIO_write_high(&PORTD, SEGMENT_CLK);
		
        // Wait 1 us
        _delay_us(1);
		
        // Pull CLK low
        GPIO_write_low(&PORTD, SEGMENT_CLK);
		
        // Shift "position"
        position = position >> 1;
    }

    // Pull LATCH high
    GPIO_write_high(&PORTD, SEGMENT_LATCH);
	
    // Wait 1 us
    _delay_us(1);
	
}

/*--------------------------------------------------------------------*/
/* SEG_clear */

/*--------------------------------------------------------------------*/
/* SEG_clk_2us */