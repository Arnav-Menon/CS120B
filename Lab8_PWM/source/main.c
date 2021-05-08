/*	Author: Arnav Menon
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #8  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	LINK TO VIDEO: https://drive.google.com/file/d/1iYkZi4-CMaG1sAPHsBY6vkJDt8IBd8iL/view?usp=sharing 
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "avr/interrupt.h"
#endif

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1ms
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B 	= 0x0B;	// bit3 = 1: CTC mode (clear timer on compare)
					// bit2bit1bit0=011: prescaler /64
					// 00001011: 0x0B
					// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
					// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A 	= 125;	// Timer interrupt will be generated when TCNT1==OCR1A
					// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
					// So when TCNT1 register equals 125,
					// 1 ms has passed. Thus, we compare to 125.
					// AVR timer interrupt mask register

	TIMSK1 	= 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1 = 0;

	// TimerISR will be called every _avr_timer_cntcurr milliseconds
	_avr_timer_cntcurr = _avr_timer_M;

	//Enable global interrupts
	SREG |= 0x80;	// 0x80: 1000000
}

void TimerOff() {
	TCCR1B 	= 0x00; // bit3bit2bit1bit0=0000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT0 == OCR0 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; 			// Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { 	// results in a more efficient compare
		TimerISR(); 				// Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// set TimerISR() to tick every M seconds
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

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

enum STATES { Init, Start, Play, Release } state;
unsigned short i = 0;
void Tick() {
	unsigned char a = ~PINA & 0x07;

	switch(state) {
		case Init:
			PWM_on();
			state = Start;
			break;
		case Start:
			if (a == 0x01)
				state = Play;
			else
				state = Start;
			break;
		case Play:
			if (i >= 50 && a == 0x01) // button still pressed after sequence over, don't restart
				state = Release;
			else if (i >= 50) 
				state = Init;
			else 
				state = Play;
			break;
		case Release:
                        if ((~PINA & 0x07) == 0x00) // wait for button unpress
                                state = Init;
                        else
                                state = Release;
                        break;
		default:
			state = Init;
			break;

	}
	switch(state) {
		case Init:
			break;
		case Start:
			i = 0;
			break;
		case Play:
			if (i < 6)
				set_PWM(261.63);
			else if (i < 11)
				set_PWM(261.63);
			else if (i < 15)
				set_PWM(293.66);
			else if (i < 20)
				set_PWM(261.63);
			else if (i < 26)
				set_PWM(349.23);
			else if (i < 30)
				set_PWM(329.63);
			else if (i < 38)
				set_PWM(261.63);
			else if (i < 40)
				set_PWM(293.66);
			else if (i < 45)
				set_PWM(261.63);
			else if (i < 48)
				set_PWM(392.00);
			else if (i < 50)
				set_PWM(349.23);
			i++;
			break;
		case Release:
			PWM_off();
			i = 0;
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
    TimerSet(100);
    TimerOn();
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;	
    }
    return 1;
}
