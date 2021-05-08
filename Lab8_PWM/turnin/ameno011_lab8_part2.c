/*	Author: Arnav Menon
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #8  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	LINK TO VIDEO: https://drive.google.com/file/d/1hRxx4WVhmnmUJYgwSPu6QLJdOjcau3eN/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include <stdbool.h>
#endif

// 0.954 hz is lowest frequency possible with this function
// based on settings in PWM_on()
// Passing in 0 as the frequencny will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows music to play uninterrupted
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } // stops timer/counter 
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter

		// prevents OCR3A from overflowing, using prescalar 64
		// 0.954 is smallest frequency that will not ressult in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }

		// prevents OCR3A from overflowing, using prescalra 64
		// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR3A = 0x0000; }

		// set OCR3A based on desired frequency
		else { OCR3A = (short) (8000000 / (128 * frequency)) - 1; }

		TCNT3 = 0; // resets coutner
		current_frequency = frequency; // updates the current freqeuncy
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
		// COM3A0: Toggle PB3 on compare match between coutner and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
		// WGM32: when counter (TCNT3) matches OCR3A, reset coutner
		// CS31 & CS30: Set a prescalar of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

bool isOn = false;
enum STATES { Init, Start, TurnOn, TurnOff, Up, Down, Release } state;
double array[8] = { 261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25 };
unsigned short i = 0;
void Tick() {
	unsigned char a = ~PINA & 0x07;
	unsigned char b = ~PINA & 0x07;
	unsigned char c = ~PINA & 0x07;

	switch(state) {
		case Init:
			state = Start;
			break;
		case Start:
			if (a == 0x01 && !isOn) // turn on machine
				state = TurnOn;
			else if (a == 0x01 && isOn) // turn off machine
				state = TurnOff;
			else if (b == 0x02 && isOn) // go up to next note
				state = Up;
			else if (c == 0x04 && isOn) // go down to next note
				state = Down;
			else
				state = Start;
			break;
		case TurnOn:
			state = Release;
			break;
		case TurnOff:
			state = Release;
			break;
		case Release:
			if ((~PINA & 0x07) == 0x00) // wait for button unpress
				state = Start;
			else
				state = Release;
			break;
		case Up:
			state = Release;
			break;
		case Down:
			state = Release;
			break;
		default:
			break;

	}
	switch(state) {
		case Init:
			break;
		case Start:
			break;
		case TurnOn:
			PWM_on();
			set_PWM(array[0]);
			isOn = true;
			break;
		case TurnOff:
			PWM_off();
			i = 0;
			isOn = false;
			break;
		case Release:
			break;
		case Up:
			if (i < 7) {
				i++;
				set_PWM(array[i]);
			}
			break;
		case Down:
			if (i > 0) {
				i--;
				set_PWM(array[i]);
			}
			break;
		default:
			break;
	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF; // configure PORTA as inputs
    DDRB = 0xFF; PORTB = 0x00; // configure PORTB as outputs
    /* Insert your solution below */
    while (1) {
	Tick();	    
    }
    return 1;
}
