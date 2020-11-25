
/* Defines -----------------------------------------------------------*/
#define KEYPAD0     PC0     // AVR pin where keypad is connected
#define KEYPAD1     PC1     // AVR pin where keypad is connected
#define KEYPAD2     PC2     // AVR pin where keypad is connected
#define KEYPAD3     PC3     // AVR pin where keypad is connected
#define KEYPAD4     PC4     // AVR pin where keypad is connected
#define KEYPAD5     PC5     // AVR pin where keypad is connected
#define KEYPAD6     PC6     // AVR pin where keypad is connected
#define DAC0        PB0     // AVR pin where DAC is connected
#define DAC1        PB1     // AVR pin where DAC is connected
#define DAC2        PB2     // AVR pin where DAC is connected
#define DAC3        PB3     // AVR pin where DAC is connected
#define DAC4        PB4     // AVR pin where DAC is connected
#define DAC5        PB5     // AVR pin where DAC is connected
#define DAC6        PB6     // AVR pin where DAC is connected
#define DAC7        PB7     // AVR pin where DAC is connected
#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for UART speed configuration
#endif

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include "uart.h"           // Peter Fleury's UART library

// Global variables
static uint8_t counter = 0; // overflow counter value set to 0
static uint8_t keypad = 0;  // keypad value set to 0

// Look-up table for sine wave generation
uint8_t  sinewave[256] = {
    0x80, 0x83, 0x86, 0x89, 0x8C, 0x90, 0x93, 0x96,
    0x99, 0x9C, 0x9F, 0xA2, 0xA5, 0xA8, 0xAB, 0xAE,
    0xB1, 0xB3, 0xB6, 0xB9, 0xBC, 0xBF, 0xC1, 0xC4,
    0xC7, 0xC9, 0xCC, 0xCE, 0xD1, 0xD3, 0xD5, 0xD8,
    0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8,
    0xEA, 0xEB, 0xED, 0xEF, 0xF0, 0xF1, 0xF3, 0xF4,
    0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFA, 0xFB, 0xFC,
    0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFD,
    0xFD, 0xFC, 0xFB, 0xFA, 0xFA, 0xF9, 0xF8, 0xF6,
    0xF5, 0xF4, 0xF3, 0xF1, 0xF0, 0xEF, 0xED, 0xEB,
    0xEA, 0xE8, 0xE6, 0xE4, 0xE2, 0xE0, 0xDE, 0xDC,
    0xDA, 0xD8, 0xD5, 0xD3, 0xD1, 0xCE, 0xCC, 0xC9,
    0xC7, 0xC4, 0xC1, 0xBF, 0xBC, 0xB9, 0xB6, 0xB3,
    0xB1, 0xAE, 0xAB, 0xA8, 0xA5, 0xA2, 0x9F, 0x9C,
    0x99, 0x96, 0x93, 0x90, 0x8C, 0x89, 0x86, 0x83,
    0x80, 0x7D, 0x7A, 0x77, 0x74, 0x70, 0x6D, 0x6A,
    0x67, 0x64, 0x61, 0x5E, 0x5B, 0x58, 0x55, 0x52,
    0x4F, 0x4D, 0x4A, 0x47, 0x44, 0x41, 0x3F, 0x3C,
    0x39, 0x37, 0x34, 0x32, 0x2F, 0x2D, 0x2B, 0x28,
    0x26, 0x24, 0x22, 0x20, 0x1E, 0x1C, 0x1A, 0x18,
    0x16, 0x15, 0x13, 0x11, 0x10, 0x0F, 0x0D, 0x0C,
    0x0B, 0x0A, 0x08, 0x07, 0x06, 0x06, 0x05, 0x04,
    0x03, 0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03,
    0x03, 0x04, 0x05, 0x06, 0x06, 0x07, 0x08, 0x0A,
    0x0B, 0x0C, 0x0D, 0x0F, 0x10, 0x11, 0x13, 0x15,
    0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24,
    0x26, 0x28, 0x2B, 0x2D, 0x2F, 0x32, 0x34, 0x37,
    0x39, 0x3C, 0x3F, 0x41, 0x44, 0x47, 0x4A, 0x4D,
    0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E, 0x61, 0x64,
    0x67, 0x6A, 0x6D, 0x70, 0x74, 0x77, 0x7A, 0x7D
};

int main(void)
{	
    // 4x3 keyboard input configuration
    GPIO_config_input_nopull(&DDRC, KEYPAD0);
    GPIO_config_input_nopull(&DDRC, KEYPAD1);
    GPIO_config_input_nopull(&DDRC, KEYPAD2);
    // 4x3 keyboard output configuration
    GPIO_config_output(&DDRC, KEYPAD3);
    GPIO_config_output(&DDRC, KEYPAD4);
    GPIO_config_output(&DDRC, KEYPAD5);
    GPIO_config_output(&DDRC, KEYPAD6);

    // DAC output configuration
    GPIO_config_output(&DDRB, DAC0);
    GPIO_config_output(&DDRB, DAC1);
    GPIO_config_output(&DDRB, DAC2);
    GPIO_config_output(&DDRB, DAC3);
    GPIO_config_output(&DDRB, DAC4);
    GPIO_config_output(&DDRB, DAC5);
    GPIO_config_output(&DDRB, DAC6);
    GPIO_config_output(&DDRB, DAC7);

    // Initialize LCD display
    lcd_init(LCD_DISP_ON);

    // Write to LCD display
    lcd_gotoxy(0, 0);
    lcd_puts("Output Signal");

    // Configuration of 8-bit Timer/Counter0 for signal generator
    // Overflow interrupt every 128 us
    TIM0_overflow_128us();
    TIM0_overflow_interrupt_enable();
	
    // Configuration of 16-bit Timer/Counter0 for signal generator
    // Overflow interrupt every 262 us
    TIM1_overflow_262ms();
    TIM1_overflow_interrupt_enable();
	
    // Initialization of UART to asynchronous, 8N1, 9600
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

// Interrupts from Timer/Counter0 can generate 6 various signals
ISR(TIMER0_OVF_vect)
{
    // sine wave
    if (keypad == 1)
    {
        if (counter == 0)
        {
            GPIO_write_low(&PORTB, DAC0);
            GPIO_write_low(&PORTB, DAC1);
            GPIO_write_low(&PORTB, DAC2);
            GPIO_write_low(&PORTB, DAC3);
            GPIO_write_low(&PORTB, DAC4);
            GPIO_write_low(&PORTB, DAC5);
            GPIO_write_low(&PORTB, DAC6);
            GPIO_write_low(&PORTB, DAC7);
        }
        uint8_t sinevalue = sinewave[counter];     //helping variable used to set output pin values
        if (sinevalue-127>0)                {GPIO_write_high(&PORTB, DAC7); sinevalue=sinevalue-128;}
        else GPIO_write_low(&PORTB, DAC7);
        if (sinevalue-63>0)                 {GPIO_write_high(&PORTB, DAC6); sinevalue=sinevalue-64;}
        else GPIO_write_low(&PORTB, DAC6);
        if (sinevalue-31>0)                 {GPIO_write_high(&PORTB, DAC5); sinevalue=sinevalue-32;}
        else GPIO_write_low(&PORTB, DAC5);
        if (sinevalue-15>0)                 {GPIO_write_high(&PORTB, DAC4); sinevalue=sinevalue-16;}
        else GPIO_write_low(&PORTB, DAC4);
        if (sinevalue-7>0)                  {GPIO_write_high(&PORTB, DAC3); sinevalue=sinevalue-8;}
        else GPIO_write_low(&PORTB, DAC3);
        if (sinevalue-3>0)                  {GPIO_write_high(&PORTB, DAC2); sinevalue=sinevalue-4;}
        else GPIO_write_low(&PORTB, DAC2);
        if (sinevalue-->0)                  {GPIO_write_high(&PORTB, DAC1); sinevalue=sinevalue-2;}
        else GPIO_write_low(&PORTB, DAC1);
        if (sinevalue>0)                    {GPIO_write_high(&PORTB, DAC0);}
        else GPIO_write_low(&PORTB, DAC0);
        counter++;
        if (counter == 255) counter = 0;
	}

    // square wave
    if (keypad == 2)
    {
        if (counter == 0)
        {
            GPIO_write_high(&PORTB, DAC0);
            GPIO_write_high(&PORTB, DAC1);
            GPIO_write_high(&PORTB, DAC2);
            GPIO_write_high(&PORTB, DAC3);
            GPIO_write_high(&PORTB, DAC4);
            GPIO_write_high(&PORTB, DAC5);
            GPIO_write_high(&PORTB, DAC6);
            GPIO_write_high(&PORTB, DAC7);
        }
        counter++;
        if (counter == 128)
        {
            GPIO_write_low(&PORTB, DAC0);
            GPIO_write_low(&PORTB, DAC1);
            GPIO_write_low(&PORTB, DAC2);
            GPIO_write_low(&PORTB, DAC3);
            GPIO_write_low(&PORTB, DAC4);
            GPIO_write_low(&PORTB, DAC5);
            GPIO_write_low(&PORTB, DAC6);
            GPIO_write_low(&PORTB, DAC7);
        }
        if (counter == 255) counter = 0;
    }

    // rising sawtooth wave
    if (keypad == 3)
    {
        if (counter == 255) counter = 0;
        if (counter == 0)
        {
            GPIO_write_low(&PORTB, DAC0);
            GPIO_write_low(&PORTB, DAC1);
            GPIO_write_low(&PORTB, DAC2);
            GPIO_write_low(&PORTB, DAC3);
            GPIO_write_low(&PORTB, DAC4);
            GPIO_write_low(&PORTB, DAC5);
            GPIO_write_low(&PORTB, DAC6);
            GPIO_write_low(&PORTB, DAC7);
        }
        counter++;
        if (counter %1 == 0)		GPIO_toggle(&PORTB, DAC0);
        if (counter %2 == 0)		GPIO_toggle(&PORTB, DAC1);
        if (counter %4 == 0)		GPIO_toggle(&PORTB, DAC2);
        if (counter %8 == 0)		GPIO_toggle(&PORTB, DAC3);
        if (counter %16 == 0)		GPIO_toggle(&PORTB, DAC4);
        if (counter %32 == 0)		GPIO_toggle(&PORTB, DAC5);
        if (counter %64 == 0)		GPIO_toggle(&PORTB, DAC6);
        if (counter %128 == 0)		GPIO_toggle(&PORTB, DAC7);
    }

    // falling sawtooth wave
    if (keypad == 4)
    {
        if (counter == 255) counter = 0;
        if (counter == 0)
        {
            GPIO_write_high(&PORTB, DAC0);
            GPIO_write_high(&PORTB, DAC1);
            GPIO_write_high(&PORTB, DAC2);
            GPIO_write_high(&PORTB, DAC3);
            GPIO_write_high(&PORTB, DAC4);
            GPIO_write_high(&PORTB, DAC5);
            GPIO_write_high(&PORTB, DAC6);
            GPIO_write_high(&PORTB, DAC7);
        }
        counter++;
        if (counter %1 == 0)		GPIO_toggle(&PORTB, DAC0);
        if (counter %2 == 0)		GPIO_toggle(&PORTB, DAC1);
        if (counter %4 == 0)		GPIO_toggle(&PORTB, DAC2);
        if (counter %8 == 0)		GPIO_toggle(&PORTB, DAC3);
        if (counter %16 == 0)		GPIO_toggle(&PORTB, DAC4);
        if (counter %32 == 0)		GPIO_toggle(&PORTB, DAC5);
        if (counter %64 == 0)		GPIO_toggle(&PORTB, DAC6);
        if (counter %128 == 0)		GPIO_toggle(&PORTB, DAC7);
    }

    // triangle wave
    if (keypad == 5)
    {
        if (counter == 0)
        {
            GPIO_write_low(&PORTB, DAC0);
            GPIO_write_low(&PORTB, DAC1);
            GPIO_write_low(&PORTB, DAC2);
            GPIO_write_low(&PORTB, DAC3);
            GPIO_write_low(&PORTB, DAC4);
            GPIO_write_low(&PORTB, DAC5);
            GPIO_write_low(&PORTB, DAC6);
            GPIO_write_low(&PORTB, DAC7);
        }
        counter++;
        if (counter <= 127)
        {
            if (counter %1 == 0)		GPIO_toggle(&PORTB, DAC1);
            if (counter %2 == 0)		GPIO_toggle(&PORTB, DAC2);
            if (counter %4 == 0)		GPIO_toggle(&PORTB, DAC3);
            if (counter %8 == 0)		GPIO_toggle(&PORTB, DAC4);
            if (counter %16 == 0)		GPIO_toggle(&PORTB, DAC5);
            if (counter %32 == 0)		GPIO_toggle(&PORTB, DAC6);
            if (counter %64 == 0)		GPIO_toggle(&PORTB, DAC7);
        }
        if (counter == 127) counter=1;
    }

    // dirac pulses
    if (keypad == 6)
    {
        if (counter == 255) counter = 0;
        if (counter == 0)
        {
            GPIO_write_high(&PORTB, DAC0);
            GPIO_write_high(&PORTB, DAC1);
            GPIO_write_high(&PORTB, DAC2);
            GPIO_write_high(&PORTB, DAC3);
            GPIO_write_high(&PORTB, DAC4);
            GPIO_write_high(&PORTB, DAC5);
            GPIO_write_high(&PORTB, DAC6);
            GPIO_write_high(&PORTB, DAC7);
        }
        if (counter == 5)
        {
            GPIO_write_low(&PORTB, DAC0);
            GPIO_write_low(&PORTB, DAC1);
            GPIO_write_low(&PORTB, DAC2);
            GPIO_write_low(&PORTB, DAC3);
            GPIO_write_low(&PORTB, DAC4);
            GPIO_write_low(&PORTB, DAC5);
            GPIO_write_low(&PORTB, DAC6);
            GPIO_write_low(&PORTB, DAC7);
        }
        counter++;
    }

    // output = 0V
    if ((keypad == 7)||(keypad == 8)||(keypad == 9)||(keypad == 10)||(keypad == 11)||(keypad == 12))
    {
        if (counter == 255) counter = 0;
        if (counter == 0)
        {
            GPIO_write_low(&PORTB, DAC0);
            GPIO_write_low(&PORTB, DAC1);
            GPIO_write_low(&PORTB, DAC2);
            GPIO_write_low(&PORTB, DAC3);
            GPIO_write_low(&PORTB, DAC4);
            GPIO_write_low(&PORTB, DAC5);
            GPIO_write_low(&PORTB, DAC6);
            GPIO_write_low(&PORTB, DAC7);
        }
        counter++;
    }
}

// Interrupts from Timer/Counter1 control Keypad, Display and UART
ISR(TIMER1_OVF_vect)
{
    // 4x3 keypad service
    GPIO_write_high(&PORTC, KEYPAD3);
    GPIO_write_low(&PORTC, KEYPAD4);
    GPIO_write_low(&PORTC, KEYPAD5);
    GPIO_write_low(&PORTC, KEYPAD6);
    if (GPIO_read(&PINC, KEYPAD0) == 1)     {keypad = 1; counter=0;}
    if (GPIO_read(&PINC, KEYPAD1) == 1)     {keypad = 2; counter=0;}
    if (GPIO_read(&PINC, KEYPAD2) == 1)     {keypad = 3; counter=0;}
    GPIO_write_low(&PORTC, KEYPAD3);
    GPIO_write_high(&PORTC, KEYPAD4);
    GPIO_write_low(&PORTC, KEYPAD5);
    GPIO_write_low(&PORTC, KEYPAD6);
    if (GPIO_read(&PINC, KEYPAD0) == 1)     {keypad = 4; counter=0;}
    if (GPIO_read(&PINC, KEYPAD1) == 1)     {keypad = 5; counter=0;}
    if (GPIO_read(&PINC, KEYPAD2) == 1)     {keypad = 6; counter=0;}
    GPIO_write_low(&PORTC, KEYPAD3);
    GPIO_write_low(&PORTC, KEYPAD4);
    GPIO_write_high(&PORTC, KEYPAD5);
    GPIO_write_low(&PORTC, KEYPAD6);
    if (GPIO_read(&PINC, KEYPAD0) == 1)     {keypad = 7; counter=0;}
    if (GPIO_read(&PINC, KEYPAD1) == 1)     {keypad = 8; counter=0;}
    if (GPIO_read(&PINC, KEYPAD2) == 1)     {keypad = 9; counter=0;}
    GPIO_write_low(&PORTC, KEYPAD3);
    GPIO_write_low(&PORTC, KEYPAD4);
    GPIO_write_low(&PORTC, KEYPAD5);
    GPIO_write_high(&PORTC, KEYPAD6);
    if (GPIO_read(&PINC, KEYPAD0) == 1)     {keypad = 10; counter=0;}
    if (GPIO_read(&PINC, KEYPAD1) == 1)     {keypad = 11; counter=0;}
    if (GPIO_read(&PINC, KEYPAD2) == 1)     {keypad = 12; counter=0;}

    // Displaying chosen signal on an LCD and UART transmitting
    if (keypad == 0) uart_puts("Waiting for input \r\n");                                                   // keypad value = 0, waiting for input
    if (keypad == 1)                                                                                        // keypad value = 1, sine wave generation
    {
        lcd_gotoxy(0, 1);
        lcd_puts("                ");
        lcd_gotoxy(0, 1);
        lcd_puts("Sine");
        uart_puts("Output signal: Sine wave \r\n");
    }
    if (keypad == 2)                                                                                        // keypad value = 2, square wave generation
    {
        lcd_gotoxy(0, 1);
        lcd_puts("                ");
        lcd_gotoxy(0, 1);
        lcd_puts("Square");
        uart_puts("Output signal: Square wave \r\n");
    }
    if (keypad == 3)                                                                                        // keypad value = 3, rising sawtooth wave generation
    {
        lcd_gotoxy(0, 1);
        lcd_puts("                ");
        lcd_gotoxy(0, 1);
        lcd_puts("Rising Sawtooth");
        uart_puts("Output signal: Rising sawtooth wave \r\n");
    }
    if (keypad == 4)                                                                                        // keypad value = 4, falling sawtooth wave generation
    {
        lcd_gotoxy(0, 1);
        lcd_puts("                ");
        lcd_gotoxy(0, 1);
        lcd_puts("Falling Sawtooth");
        uart_puts("Output signal: Falling sawtooth wave \r\n");
    }
    if (keypad == 5)                                                                                        // keypad value = 5, triangle wave generation
    {
        lcd_gotoxy(0, 1);
        lcd_puts("                ");
        lcd_gotoxy(0, 1);
        lcd_puts("Triangle");
        uart_puts("Output signal: Triangle wave \r\n");
    }
    if (keypad == 6)                                                                                        // keypad value = 6, dirac pulses generation
    {
        lcd_gotoxy(0, 1);
        lcd_puts("                ");
        lcd_gotoxy(0, 1);
        lcd_puts("Dirac pulses");
        uart_puts("Output signal: Dirac pulses \r\n");
    }
    if ((keypad == 7)||(keypad == 8)||(keypad == 9)||(keypad == 10)||(keypad == 11)||(keypad == 12))        // keypad value = (7|8|9|10|11|12), 0V generation
    {
        lcd_gotoxy(0, 1);
        lcd_puts("                ");
        lcd_gotoxy(0, 1);
        lcd_puts(" ---- None ---- ");
        uart_puts("Output signal: ---- None ---- \r\n");
    }
}
