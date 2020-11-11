/***********************************************************************
 * 
 * Analog-to-digital conversion with displaying result on LCD and 
 * transmitting via UART.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function
#include "uart.h"           // Peter Fleury's UART library
#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay func
#endif

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Use Timer/Counter1
 * and start ADC conversion four times per second. Send value to LCD
 * and UART.
 */
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(0, 0);
    lcd_puts("value:");
    lcd_gotoxy(0, 1);
    lcd_puts("key:       par:");

    // Configure ADC to convert PC0[A0] analog value
    // Set ADC reference to AVcc
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1);
    // Set input channet to ADC0
	ADMUX &= ~(1 << MUX0);
	ADMUX &= ~(1 << MUX1);
	ADMUX &= ~(1 << MUX2);
	ADMUX &= ~(1 << MUX3);
    // Enable ADC module
	ADCSRA |= (1 << ADEN);
    // Enable conversion complete interrupt
	ADCSRA |= (1 << ADIE);
    // Set clock prescaler to 128
	ADCSRA |= (1 << ADPS0);
	ADCSRA |= (1 << ADPS1);
	ADCSRA |= (1 << ADPS2);
    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Enable interrupt and set the overflow prescaler to 262 ms
	TIM1_overflow_1s();
	TIM1_overflow_interrupt_enable();
    // Initialize UART to asynchronous, 8N1, 9600
	uart_init(UART_BAUD_SELECT(9600,F_CPU));
    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**
 * ISR starts when Timer/Counter1 overflows. Use single conversion mode
 * and start conversion four times per second.
 */
ISR(TIMER1_OVF_vect)
{
    // Start ADC conversion
	ADCSRA |= (1 << ADSC);
}

/* -------------------------------------------------------------------*/
/* ADC complete interrupt routine.
 * Update LCD and UART transmiter. */
/**
 * ISR starts when ADC complets the convertion. Display value on LCD
 * and send it to UART.
 */
ISR(ADC_vect)
{
    // WRITE YOUR CODE HERE
    uint16_t value = ADC;
	uint8_t parity = 0;
    char lcd_string[10] = "          ";

    value = ADC;					// Copy ADC result to 16-bit variable
	lcd_gotoxy(6, 0);
	lcd_puts(lcd_string);

	// Decimal
	itoa(value, lcd_string, 10);
    lcd_gotoxy(6, 0);
	lcd_puts(lcd_string);

	// UART decimal
	uart_puts("ADC value in decimal: ");
	uart_puts(lcd_string);
	uart_puts("\r\n");

	// Hexa
	itoa(value, lcd_string, 16);
	lcd_gotoxy(13, 0);
	lcd_puts(lcd_string);

	// Buttons
	lcd_gotoxy(4,1);
	lcd_puts("      ");		
	lcd_gotoxy(4,1);
	if (value>1016)
	{
		lcd_puts("None");
	}
	if (value>85 && value<115)
	{
		lcd_puts("Up");
	}
	if (value>210 && value<270)
	{
		lcd_puts("Down");
	}
	if (value>360 && value<420)
	{
		lcd_puts("Left");
	}
	if (value<10)
	{
		lcd_puts("Right");
	}
	if (value>600 && value<680)
	{
		lcd_puts("Select");
	}
	
	// Odd parity bit
	for (uint16_t i=512; i>=1; i=i/2)
	{
		if (value>=i)
		{
			value=value-i;
			parity++;
		}
	}
	parity++;
	parity=parity%2;
	itoa(parity, lcd_string, 10);
	lcd_gotoxy(15,1);
	lcd_puts(lcd_string);
} 
