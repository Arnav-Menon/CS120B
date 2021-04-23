/*	Author: Arnav Menon
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #5  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	LINK TO VIDEO: https://drive.google.com/file/d/1dA3OwwQcBGXKw-cYqvNTdmCpqxJhRsu9/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum STATES { SM1_Start, SM1_Init, SM1_PlusOn, SM1_Wait } state;
unsigned char array[15] = {0x08, 0x18, 0x1C, 0x3C, 0x3E, 0x7E, 0x7F, 0xFF, 0x7F, 0x7E, 0x3E, 0x3C, 0x1C, 0x18, 0x08}; // array of light patterns
unsigned short i = 0; // counter for the array
void Tick() {
	// state transitions
	switch(state) {
		case SM1_Start:
			state = SM1_Init;
			break;
		case SM1_Init:
			if ((~PINA & 0x01) == 0x01) { // if A0 button pressed
				state = SM1_PlusOn;
			}
			else { // no button pressed, stay here and wait
				state = SM1_Init;
			}
			break;
		case SM1_PlusOn:
			state = SM1_Wait;
			break;
		case SM1_Wait:
			if ((~PINA & 0x01) == 0x01) { // if button still pressed, stay here until depressed
				state = SM1_Wait;
			}
			else {
				state = SM1_Init; // button depressed, go back to Init state and wait for button press
			}
			break;
		default:
			state = SM1_Start;
	}

	// state actions
	switch(state) {
		case SM1_Start:
			PORTC = 0x00;
			break;
		case SM1_Init:
			break;
		case SM1_PlusOn:
			if (i < 15) { // if all lights not on
				PORTC = array[i]; // assing pattern at index i to PORTC
				i++; // incremnt i
			}
			else {
				PORTC = 0x00; // reset lights to al off
				i = 0; // reset i so we can start over
			}
			break;
		case SM1_Wait:
			break;
		default:
			PORTC = 0x00;
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


