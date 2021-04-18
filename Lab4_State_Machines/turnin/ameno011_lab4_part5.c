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

enum STATES { SM1_Start, SM1_Init, SM1_Correct, SM1_Wrong, SM1_Unlock, SM1_Lock } state;
unsigned char index = 0x00;
unsigned char array[7] = {0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01};
void Tick() {
	// state transitions
	switch(state) {
		case SM1_Start:
			state = SM1_Init;
			break;
		case SM1_Init:
			if ((PINA & 0x87) == array[index]) {
				state = SM1_Correct;
			}
			else if ((PINA & 0x87) == 0x80) {
				state = SM1_Lock;
			}
			else if (PINA == 0x00) {
				state = SM1_Init;
			}
			else {
				state = SM1_Wrong;
			}
		case SM1_Correct:
			if ((index == 0x07) && (PORTB == 0x01)) {
				state = SM1_Lock;
			}
			else if (index == 0x07) {
				state = SM1_Unlock;
			}
			else if ((PINA & 0x07) == array[index]) {
				index++;
				state = SM1_Correct;
			}
			else {
				state = SM1_Start;
			}
			//else if ((PINA & 0x07) == 0x00) {
			//	index = 0x00;
			//	state = SM1_Init;
			//}
			//else {
			//	state = SM1_Correct;
			//}
			break;
		case SM1_Wrong:
			index = 0x00;
			state = SM1_Init;
			break;
		case SM1_Unlock:
			//if (PINA == 0x01 && PORTB == 0x01) {
			//	state = SM1_Unlock;
			//}
			//else 
			if ((PINA & 0x87) == 0x00) {
				index = 0x00;
				state = SM1_Init;
			}
			else if ((PINA & 0x87) == 0x80) {
				state = SM1_Lock;
			}
			else {
				state = SM1_Unlock;
			}
			break;
		case SM1_Lock:
			if ((PINA & 0x87) == 0x80) {
				state = SM1_Lock;
			}
			else {
				index = 0x00;
				state = SM1_Init;
			}
			break;
		default:
			state = SM1_Start;
			break;
	}

	// state actions
	switch(state) {
		case SM1_Start:
			PORTB = 0x00;
			break;
		case SM1_Init:
			break;
		case SM1_Correct:
			break;
		case SM1_Wrong:
			break;
		case SM1_Unlock:
			PORTB = 0x01;
			break;
		case SM1_Lock:
			PORTB = 0x00;
			break;
		default:
			PORTB = 0x00;
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
