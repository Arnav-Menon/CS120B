/*	Author: Arnav Menon
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #5  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum STATES { SM1_Start, SM1_Begin, SM1_Init, SM1_Reset, SM1_Plus, SM1_Minus, SM1_PlusOn, SM1_MinusOn } state;
void Tick() {
	// state transitions
	switch(state) {
		case SM1_Start:
			state = SM1_Begin;
			break;
		case SM1_Begin:
			state = SM1_Init;
			break;
		case SM1_Init:
			if ((~PINA & 0x03) == 0x03) {
				state = SM1_Reset;
			}
			else if ((~PINA & 0x01) == 0x01) {
				state = SM1_PlusOn;
			}
			else if ((~PINA & 0x02) == 0x02) {
				state = SM1_MinusOn;
			}
			break;
		case SM1_PlusOn:
			state = SM1_Plus;
			break;
		case SM1_Plus:
			if ((~PINA & 0x03) == 0x03) {
				state = SM1_Reset;
			}
			else if ((~PINA & 0x01) == 0x01) {
				state = SM1_Plus;
			}
			else if ((~PINA & 0x02) == 0x02) {
				state = SM1_MinusOn;
			}
			else {
				state = SM1_Init;
			}
			break;
		case SM1_MinusOn:
			state = SM1_Minus;
			break;
		case SM1_Minus:
			if ((~PINA & 0x03) == 0x03) {
				state = SM1_Reset;
			}
			else if ((~PINA & 0x01) == 0x01) {
				state = SM1_PlusOn;
			}
			else if ((~PINA & 0x02) == 0x02) {
				state = SM1_Minus;
			}
			else {
				state = SM1_Init;
			}
			break;
		case SM1_Reset:
			if ((~PINA & 0x03) == 0x03) {
				state = SM1_Reset;
			}
			else {
				state = SM1_Init;
			}
			break;
		default:
			state = SM1_Start;
			break;
	}

	// state actions
	switch(state) {
		case SM1_Begin:
			PORTC = 0x07;
			break;
		case SM1_Start:
			PORTC = 0x07;
			break;
		case SM1_Init:
			break;
		case SM1_PlusOn:
			if (PORTC < 0x09) {
				PORTC = PORTC + 1;
			}
			break;
		case SM1_MinusOn:
			if (PORTC > 0x00) {
				PORTC = PORTC - 1;
			}
			break;
		case SM1_Reset:
			PORTC = 0x00;
			break;
		case SM1_Plus:
			break;
		case SM1_Minus:
			break;
		default:
			PORTC = 0x07;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF; // configure port A's 8 pins as outputs
    DDRC = 0xFF; PORTC = 0x00; // configure port C's 8 pins as outputs

    /* Insert your solution below */
    while (1) {
	    Tick(); // call SM function
    }
    return 1;
}


