/*	Author: Arnav Menon
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #4  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum STATES { SM1_Start, SM1_Lock, SM1_Unlock1, SM1_Unlock2 } state;
void Tick() {
	
	unsigned char x = PINA & 0x01;
	unsigned char y = PINA & 0x02;
	unsigned char pound = PINA & 0x04;
	unsigned char lock = PINA & 0x80;

	// state transitions
	switch(state) {
		case SM1_Start:
			state = SM1_Lock;
			break;
		case SM1_Lock:
			if (lock) {
				state = SM1_Lock;
			}
			else if (pound && !y && !x) {
				state = SM1_Unlock1;
			}
			else {
				state = SM1_Lock;
			}
			break;
		case SM1_Unlock1:
			if (lock) {
				state = SM1_Lock;
			}
			else if (!pound && y && !x) {
				state = SM1_Unlock2;
			}
			else {
				state = SM1_Lock;
			}
			break;
		case SM1_Unlock2:
			if (lock) {
				state = SM1_Lock;
			}
			else {
				state = SM1_Unlock2;
			}
			break;
		default:
			state = SM1_Start;
			break;
	}

	// state actions
	switch(state) {
		case SM1_Start:
			break;
		case SM1_Lock:
			PORTB = 0x00;
			break;
		case SM1_Unlock1:
			break;
		case SM1_Unlock2:
			PORTB = 0x01;
			break;
		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF; // configure port A's 8 pins as outputs
    DDRB = 0xFF; PORTB = 0x00; // configure port B's 8 pins as outputs

    /* Insert your solution below */
    while (1) {
	    Tick(); // call SM function
    }
    return 1;
}
