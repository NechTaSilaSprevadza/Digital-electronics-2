
/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "segment.h"        // Seven-segment display library for AVR-GCC

uint8_t singles = 0;
uint8_t decimals = 0;

/* Function definitions ----------------------------------------------*/
int main(void)
{
    // Configure SSD signals
    SEG_init();

    // Configure Timer/Counters
	TIM0_overflow_4ms();
	TIM0_overflow_interrupt_enable();
	TIM1_overflow_1s();
	TIM1_overflow_interrupt_enable();
	
    // Enables interrupts by setting the global interrupt mask
	sei();
	
    // Infinite loop
    while (1)
    {
    }
    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
ISR(TIMER0_OVF_vect)
{
	static uint8_t pos = 0;
	if (pos == 0)
	{
		SEG_update_shift_regs(singles, pos);
		pos = 1;
	} 
	else
	{
		SEG_update_shift_regs(decimals, pos);
		pos = 0;
	}
}

ISR(TIMER1_OVF_vect)
{
	singles++;
	if(singles > 9)
	{
		singles = 0;
		decimals++;
		if(decimals>5)
		{
			decimals = 0;
		}
	}
}
