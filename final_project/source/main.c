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
#include <util/delay.h> // ysed for _delay_ms function
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "lcd.h" // LCD code in directory

#define TIME 1000

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRD = 0x3F; DDRC = 0x00; // set D0..D5 as outputs for LCD, use PORTC as inputs

    byte score = 0;
	
    LCD_Init(); 			// initialize LCD controller
    LCD_Message("Let's play The Tune ");	// welcome message
    LCD_Message("Score: ");
    LCD_Integer(score);
    msDelay(2000);			// wait
    //LCD_Message("Let's play The Tune ");	// welcome message
    //LCD_Message("Score: ");
    //LCD_Integer(score);
    while (1) {
	   LCD_Clear(); 			// clear screen otherwise it will print on top
	   score++;
	   LCD_Message("Let's play The Tune ");		// fill screen with ASCII characters
	   LCD_Message("Score: ");
	   LCD_Integer(score);
	   msDelay(TIME);
    }
 
    return 1;
}
