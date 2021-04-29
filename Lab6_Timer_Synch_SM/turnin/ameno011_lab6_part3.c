/*	Author: Arnav Menon
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #6  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	LINK TO VIDEO: https://drive.google.com/file/d/1f5AV9Jzr3fEQOm7p7QaAPHBTCuT9DpLd/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum STATES { SM1_Start, SM1_Begin, SM1_Init, SM1_Reset, SM1_Plus, SM1_Minus, SM1_PlusOn, SM1_MinusOn } state;
unsigned char counter = 0x09; // counter to count 1 second when holding button
void Tick() {
	// state transitions
	switch(state) {
		case SM1_Start:
			state = SM1_Begin;
			break;
		case SM1_Begin:
			if ((~PINA & 0x01) == 0x01) {
				state = SM1_PlusOn;
			}
			else {
				state = SM1_Init;
			}
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
			else {
				state = SM1_Init;
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
			counter = 0x00;
			break;
		case SM1_MinusOn:
			if (PORTC > 0x00) {
				PORTC = PORTC - 1;
			}
			counter = 0x00;
			break;
		case SM1_Reset:
			PORTC = 0x00;
			break;
		case SM1_Plus:
			if (counter == 0x09 && PORTC < 0x09) {
				PORTC = PORTC + 1;
				counter = 0x00;
			}
			counter++;
			break;
		case SM1_Minus:
			if (counter == 0x09 && PORTC > 0x00) {
				PORTC = PORTC - 1;
				counter = 0x00;
			}
			counter++;
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
    TimerSet(100);
    TimerOn();
    /* Insert your solution below */
    while (1) {
	    Tick(); // call SM function
	    while(!TimerFlag);
	    TimerFlag = 0;
    }
    return 1;
}
