/*	Author: Arnav Menon 
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab #7  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	LINK TO VIDEO: https://drive.google.com/file/d/1g4yDrnlt0BNXe0VaIWnxTUG6G0weZAaG/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned short adc = 0x00;
unsigned short MAX = 0xFF;
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: settging this bit enables analog to digital conversion
	// ADSC: setting this bit starts the first conversion
	// ADATE: setting this but enables auto-triggering. Since we are in Free RUnning Mode, a new conversion will trigger whenever the previous conversion completes
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF; // set PORTA as input 
    DDRC = 0xFF; PORTC = 0x00; // set PORTC as output
    DDRD = 0xFF; PORTD = 0x00; // set PORTD as output
    
    ADC_init();	
    /* Insert your solution below */
    while (1) {
	    adc = ADC; // get ADC value
	    unsigned char pc = (char) adc;
	    if (pc >= MAX/2) {
		    PORTC = 0x01;
	    }
	    else {
		    PORTC = 0x00;
	    }
    }
    return 1;
}
