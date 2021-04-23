  
/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab#5 Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF; // configure port A's 8 pins as inputs
    DDRC = 0xFF; PORTC = 0x00; // configure port C's 8 pins and outputs
    
    unsigned char tempA = 0x00;
    unsigned char tempC = 0x00;

    /* Insert your solution below */
    while (1) {
	    
	    tempA = PINA & 0x0F; // get lower four bits

	    if (tempA > 0x0C) { // level is 13, 14, or 15
		    tempC = 0x3F; // light PC5..PC0
	    }
	    else if (tempA > 0x09) { // level is 10, 11, or 12
		    tempC = 0x3E; // light PC5..PC1
	    }
	    else if (tempA > 0x06) { // level is 7, 8, or 9
		    tempC = 0x3C; // light PC5..PC2
	    }
	    else if (tempA > 0x04) { // level is 5 or 6
		    tempC = 0x38; // light PC5..PC3
	    }
	    else if (tempA > 0x02) { // level is 3 or 4
		    tempC = 0x70; // light PC6..PC4
	    }
	    else if (tempA > 0x00)  { // level is 1 or 2 
		    tempC = 0x60; // light PC6..PC5
	    }
	    else { // level is 0
		    tempC = 0x40; // light PC6
	    }
	    
	    PORTC = tempC;
	
    }
    
    return 0;
}












