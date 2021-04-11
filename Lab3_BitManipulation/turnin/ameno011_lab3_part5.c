/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab#3 Exercise #5
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
    DDRB = 0xFF; PORTB = 0x00; // configure port B's 8 pins and outputs
    DDRD = 0x00; PORTD = 0xFF; // configure port C's 8 pins and inputs
    
    unsigned char tempB = 0x00;
    unsigned char tempD = 0x00;
    unsigned char finalB = 0x00;
    unsigned short totalWeight = 0x0000; // 32 bit val

    /* Insert your solution below */
    while (1) {

	    tempB = PINB;
	    tempD = PIND;
	    
	    totalWeight = (tempD << 1) | (tempB & 0x01);
	    if (totalWeight >= 0x0046) { // if >= 70, set PB1 = 1
		    finalB = 0x02;
	    }
	    else if (totalWeight > 0x005) { // if > 5, set PB2 = 1
		    finalB = 0x04;
	    }
	    else {
		    finalB = 0x00;
	    }

	    PORTB = finalB;
    }
    
    return 0;
}
