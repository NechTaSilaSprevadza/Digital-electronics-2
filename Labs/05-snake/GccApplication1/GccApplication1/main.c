
/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "segment.h"        // Seven-segment display library for AVR-GCC

uint8_t value = 0;
static uint8_t pos = 0;

/* Function definitions ----------------------------------------------*/
int main(void)
{
	// Configure SSD signals
	SEG_init();

	// Configure Timer/Counters
	TIM1_overflow_262ms();
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

ISR(TIMER1_OVF_vect)
{
	SEG_update_shift_regs(value, pos);
	value++;
	if(value > 5)
	{
		value = 0;
	}
}
