/*	Author: Arnav Menon
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #2  Exercise #4
 *	Exercise Description: Ouput in binary the number of available spaces in a 4-spot parking lot [optional - include for your own benefit]
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
    DDRB = 0x00; PORTB = 0xFF; // input
    DDRC = 0x00; PORTC = 0xFF; // input
    DDRD = 0xFF; PORTD = 0x00; // output

    /* Insert your solution below */
    unsigned char weightA = 0x00;
    unsigned char weightB = 0x00;
    unsigned char weightC = 0x00;
    unsigned char total = 0x00;
    unsigned char diff = 0x00;

    while (1) {
	    // 1) read inputi
	    weightA = PINA;
	    weightB = PINB;
	    weightC = PINC;

	    total = weightA + weightB + weightC;

	    if (weightA > weightC) {
		    diff = weightA - weightC;
	    }
	    else {
		    diff = weightC - weightA;
	    }

	    // 2) perform computation
	    if (total > 0x8C) {
	            PORTD = PORTD | 0x01; // weight limit exceeded, set PD0 = 1
	    }
	    if (diff > 0x50) {
		    PORTD = PORTD | 0x02; // sides are unbalanced, set PD1 = 1
	    }
	    
	    // 3) write output
	    if (PORTD > 0x01) {
		    total = total & 0xFC; // reset bits 0 1 so we can capture into PORTD
	    }

	    PORTD = PORTD | total;
    }

    return 0;
}
