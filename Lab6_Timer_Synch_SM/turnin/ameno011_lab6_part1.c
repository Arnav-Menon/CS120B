/*	Author: Anrav Menon
 *  Partner(s) Name: 
 *	Lab Section:22
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	LINK TO VIDEO: https://drive.google.com/file/d/1ec3GN5PBI8jLYDQkiUxRBm49Bo3d25fP/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum STATES { Start, LED0, LED1, LED2 } state;

void Tick() {

	switch(state) {
		case Start:
			state = LED0;
			break;
		case LED0:
			state = LED1;
			break;
		case LED1:
			state = LED2;
			break;
		case LED2:
			state = LED0;
			break;
		default:
			state = Start;
			break;
	}

	switch(state) {
		case Start:
			PORTC = 0x00;
			break;
		case LED0:
			PORTC = 0x01;
			break;
		case LED1:
			PORTC = 0x02;
			break;
		case LED2:
			PORTC = 0x04;
			break;
		default:
			PORTC = 0x00;
			break;
	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRC = 0xFF; // set port C to output
    PORTC = 0x00; // init port C to 0's
    TimerSet(1000);
    TimerOn();
    /* Insert your solution below */
    while (1) {
	    Tick(); // call SM function
	    while (!TimerFlag);
	    TimerFlag = 0;
    }
    
    return 1;
}
