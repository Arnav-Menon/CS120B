/*	Author: Arnav Menon
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #2  Exercise #2
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
    DDRC = 0xFF; PORTC = 0x00; 
    /* Insert your solution below */
    unsigned char cntavail = 0x00; // temp var to hold the value of B
    unsigned char PA_0 = 0x00;
    unsigned char PA_1 = 0x00;
    unsigned char PA_2 = 0x00;
    unsigned char PA_3 = 0x00;
    
    while (1) {
	    // 1) read inputi
	    PA_0 = PINA & 0x01;
	    PA_1 = PINA & 0x02;
	    PA_2 = PINA & 0x04;
	    PA_3 = PINA & 0x08;
	    
	    cntavail = 0x00; // reset it every iteration so the value doesn't add up

	    // 2) perform computation
	    if (PA_0 == 0x00) {
		    cntavail++; 
	    }
	    if (PA_1 == 0x00) {
		    cntavail++;
	    }
	    if (PA_2 == 0x00) {
		    cntavail++;
	    }
	    if (PA_3 == 0x00) {
		    cntavail++;
	    }
	    
	    // 3) write output
	    PORTC = cntavail;
    }
    return 0;
}