/*	Author: Arnav Menon
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum STATES { Start, onPress, onRelease, offPress, offRelease } state;
void Tick() {
	// state transitions
	switch(state) {
		case Start:
			state = onPress;
			break;
		case onPress:
			state = ((PINA & 0x01) == 0x01) ? onPress: onRelease;
			break;
		case onRelease:
			state = ((PINA & 0x01) == 0x01) ? offPress : onRelease;
			break;
		case offPress:
			state = ((PINA & 0x01) == 0x01) ? offPress : offRelease;
			break;
		case offRelease:
			state = ((PINA & 0x01) == 0x01) ? onPress : offRelease;
			break;
		default:
			state = Start;
			break;
	}

	// state actions
	switch(state) {
		case Start:
			PORTB = 0x01;
			break;
		case onPress:
			PORTB = 0x01;
		case onRelease:
			break;
		case offPress:
			PORTB = 0x02;
			break;
		case offRelease:
			break;
		default:
			PORTB = 0x01;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF; // configure port A's 8 pins as outputs
    DDRB = 0xFF; PORTB = 0x00; // configure port B's 8 pins as outputs

    unsigned char inputA = 0x00;
    /* Insert your solution below */
    while (1) {
	Tick(); // call SM function
    }
    return 1;
}
