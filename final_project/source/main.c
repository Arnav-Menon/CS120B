/*	Author: Arnav Menon
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #11  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "lcd.h" // LCD source code in source directory

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRD = 0x3F; DDRC = 0x00; // set D0..D5 as outputs for LCD, use PORTC as inputs

    LCD_Init(); 			// initialize LCD controller
    LCD_Message("Hello World!");	// welcome message
    msDelay(2000);			// wait
    while (1) {
	   FillScreen();		// fill screen with ASCII characters
    }
 
    return 1;
}
