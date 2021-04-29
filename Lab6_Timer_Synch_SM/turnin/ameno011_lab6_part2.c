/*	Author: Anrav Menon
 *  Partner(s) Name: 
 *	Lab Section:22
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	LINK TO VIDEO: https://drive.google.com/file/d/1f2Yc5-x8zi7S4j0IkjVryIIjTS3Cfau2/view?usp=sharing 
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum STATES { Init, Start, Wait, Wait1, Continue1, Continue } state;
unsigned char array[4] = {0x01, 0x02, 0x04, 0x02 }; 
unsigned short i = 0;

void Tick() {

	switch(state) {
		case Init:
			state = Start;
			break;
		case Start:
			if ((~PINA & 0x01) == 0x01) { // if button pressed
				state = Wait;
			}
			else {
				state = Continue;
			}
			break;
		case Wait:
			if ((~PINA & 0x01) == 0x01) { // button still pressed, wait for realease
				state = Wait;
			}
			else {
				state = Wait1;
			}
			break;
		case Wait1:
			if ((~PINA & 0x01) == 0x01) { // button pressed, continue sequence
				state = Continue1;
			}
			else {
				state = Wait1;
			}
			break;
		case Continue1:
			state = Continue;
			break;
		case Continue:
			if ((~PINA & 0x01) == 0x01) { // button pressed, wait
				state = Wait;
			}
			else {
				state = Continue;
			}
			break;
		default:
			state = Init;
			break;
	}

	switch(state) {
		case Init:
			PORTC = 0x00;
			break;
		case Start:
			PORTC = array[0];
			i = 1;
			break;
		case Wait:
			break;
		case Wait1:
			break;
		case Continue1:
			break;
		case Continue:
			if (i < 4) {
				PORTC = array[i];
				i++;
			}
			else {
				PORTC = array[0];
				i = 1;
			}
			break;
		default:
			PORTC = 0x00;
			break;
	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRC = 0xFF; PORTC = 0x00; // set PORTC as output
    DDRA = 0x00; PORTA = 0xFF;
    TimerSet(300); // 300 ms
    TimerOn();
    /* Insert your solution below */
    while (1) {
	    Tick(); // call SM function
	    while (!TimerFlag);
	    TimerFlag = 0;
    }
    
    return 1;
}
