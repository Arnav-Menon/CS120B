/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:21
 *	Assignment: Lab#3 Exercise #2
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
    DDRB = 0xFF; PORTB = 0x00; // configure port B's 8 pins and outputs
    DDRC = 0xFF; PORTC = 0x00; // configure port C's 8 pins and outputs
    
    unsigned char lowA = 0x00;
    unsigned char highA = 0x00;

    /* Insert your solution below */
    while (1) {
	    
	    lowA = PINA & 0x0F; // get lower 4 bits
	    highA = PINA & 0xF0;

	    PORTB = (PORTB | highA) >> 4;
	    PORTC = (PORTC | lowA) << 4;
	
    }
    
    return 0;
}
