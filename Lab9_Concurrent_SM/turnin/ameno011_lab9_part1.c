/*	Author: Arnav Menon
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab 9  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	LINK TO VIDEO: https://drive.google.com/file/d/1kl8xfxfW4EpwICOj8WvWkSQfvUABIkj_/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include <avr/interrupt.h>
#endif

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1ms
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

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
ISR(TIMER1_COMPA_vect)
{
	// CPU automatically calls when TCNT0 == OCR0 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; 			// Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { 	// results in a more efficient compare
		TimerISR(); 				// Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

enum ThreeLEDSM { Iterate } state1;
unsigned char threeLEDs = 0x01;
void ThreeLED() {
	switch(state1) {
		case Iterate:
			state1 = Iterate;
			break;
		default:
			state1 = Iterate;
			break;
	}

	switch(state1) {
		case Iterate:
			if (threeLEDs == 0x01) 	
				threeLEDs = 0x02;
			else if (threeLEDs == 0x02) 
				threeLEDs = 0x04;
			else if (threeLEDs == 0x04) 
				threeLEDs = 0x01;
			break;
		default:
			threeLEDs = 0x00;
			break;
	}
}

enum BlinkingLEDSM { Blink } state2;
unsigned char blinkingLED = 0;
void BlinkingLED() {
	switch(state2) {
		case Blink:
			state2 = Blink;
			break;
		default:
			state2 = Blink;
			break;
	}

	switch(state2) {
		case Blink:
			blinkingLED = !blinkingLED; // toggle between 1 and 0
			break;
		default:
			blinkingLED = 0;
			break;
	}
}

enum CombineLEDSM { Go } state3;
void CombineLEDS() {
	switch(state3) {
		case Go:
			state3 = Go;
			break;
		default:
			state3 = Go;
			break;
	}

	switch(state3) {
		case Go:
			PORTC = threeLEDs;
			if (blinkingLED)  
				PORTC = PORTC | 0x08;
			break;
		default:
			PORTC = 0x00;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRC = 0xFF; PORTC = 0x00; // set PORTB as output
    /* Insert your solution below */
    TimerSet(1000);
    TimerOn();
    while (1) {
	    ThreeLED();
	    BlinkingLED();
	    CombineLEDS();
	    while (!TimerFlag);
	    TimerFlag = 0;
    }
    return 1;
}
