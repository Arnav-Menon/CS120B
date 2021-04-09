/*	Author: Arnav Menon
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #2  Exercise #1
 *	Exercise Description: Intro to AVR ToolChain [optional - include for your own benefit]
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
    DDRB = 0xFF; PORTB = 0x00; // configure port B's 8 pins as outputs
    /* Insert your solution below */
    unsigned char tmpB = 0x00; // temp var to hold the value of B
    unsigned char tmpA = 0x00; // temp var to hold the value of A
    while (1) {
	    // 1) read input
	    tmpA = PINA & 0x02; // to read PA0
	    // 2) perform computation
	    if (tmpA == 0x02) { // true if PA1 is 1
		    tmpB = 0x00; // sets tmpB tp bbbbbb00, according to Truth Table given in lab specs
	    }
	    else if (tmpA == 0x00) {
		    tmpB = PINA & 0x01; // (clear rightmost 2 bits,
						 // then set to 10)
	    }
	    // 3) write output
	    PORTB = tmpB;
    }
    return 0;
}
