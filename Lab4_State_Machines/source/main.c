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

enum STATES { SM1_Start, SM1_ReleaseX, SM1_Unlock, SM1_Lock, SM1_Fail, SM1_Wait } state;
void Tick() {
	// state transitions
	switch(state) {
		case SM1_Start:
			if ((PINA & 0x07) == 0x04) { // if PA2 == '#'
				state = SM1_ReleaseX;
			}
			else if ((PINA & 0x07) == 0x06) {
				state = SM1_Fail;
			}
			else {
				state = SM1_Wait;
			}
			break;
		case SM1_ReleaseX:
			if ((PINA & 0x07) == 0x04) { // if button still pressed
				state = SM1_ReleaseX;
			}
			else if ((PINA & 0x07) == 0x06) { 
				state = SM1_Fail;
			}
			else if ((PINA & 0x07) == 0x02) {
				state = SM1_Unlock;
			}
			else {
				state = SM1_Wait;
			}
			break;
		case SM1_Unlock:
			if ((PINA & 0x87) == 0x80) { // if PA7 == 1 inside house
				state = SM1_Lock;
			}
			else if ((PINA & 0x07) == 0x02) {
				state = SM1_Unlock;
			}
			else {
				state = SM1_Wait;
			}
			break;
		case SM1_Lock:
			state = SM1_Wait;
			break;
		case SM1_Fail:
			if ((PINA & 0x07) == 0x04) { // if PA2 == '#'
				state = SM1_ReleaseX;
			}
			else {
				state = SM1_Fail;
			}
			break;
		case SM1_Wait:
			if ((PINA & 0x07) == 0x06) {
				state = SM1_Fail;
			}
			else if ((PINA & 0x07) == 0x04) {
				state = SM1_ReleaseX;
			}
			else if ((PINA & 0x07) == 0x02) { // if PA1 = 'Y'
				state = SM1_Unlock;
			}
			else if ((PINA & 0x87) == 0x80) {
				state = SM1_Lock;
			}
			else {
				state = SM1_Wait;
			}
			break;
		default:
			state = SM1_Start;
	}

	// state actions
	switch(state) {
		case SM1_Start:
			PORTB = 0x00;
			break;
		case SM1_ReleaseX:
			break;
		case SM1_Unlock:
			PORTB = 0x01;
			break;
		case SM1_Lock:
			PORTB = 0x00;
			break;
		case SM1_Fail:
			PORTB = 0x00;
			break;
		case SM1_Wait:
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

