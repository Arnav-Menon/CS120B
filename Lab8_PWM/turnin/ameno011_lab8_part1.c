/*	Author: Arnav Menon
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #8  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	LINK TO VIDEO: https://drive.google.com/file/d/1hExEF8dHKm-7fb9CZK2MLP5TRDeyKjBS/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
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

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF; // configure PORTA as inputs
    DDRB = 0x4F; PORTB = 0x00; // configure PORTB as outputs
    /* Insert your solution below */
    PWM_on();
    while (1) {
	    unsigned char a = ~PINA & 0x01;
	    unsigned char b = ~PINA & 0x02;
	    unsigned char c = ~PINA & 0x04;
	   // PWM_on();
	    if (!a && !b && !c)
		    set_PWM(0);
	    else if (a && !b && !c)
		    set_PWM(261.63);
	    else if (!a && b && !c)
		    set_PWM(293.66);
	    else if (!a && !b && c)
		    set_PWM(329.63);
	    else 
		    set_PWM(0);
    }
//    PWM_off();
    return 1;
}
