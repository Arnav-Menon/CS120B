/*	Author Arnav Menon:
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #10  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	LINK TO VIDEO: 
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "keypad.h"
#include "timer.h"
#endif

typedef struct task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct) (int);
} task;

unsigned char x;
unsigned char keypad_input = 0x00;

enum KeypadSMStates { Start, Check  };
int KeypadSM (int state) {
	x = GetKeypadKey();
	switch(state) {
		case Start:
			state = Check;
			break;
		case Check:
			state = Check;
			break;
		default:
			state = Start;
			break;
	}

	switch(state) {
		case Start: break;
		case Check:
			switch(x) {
				case '\0': PORTB = 0x1F; break;
				case '1': PORTB = 0x01; break;
				case '2': PORTB = 0x02; break;
				case '3': PORTB = 0x03; break;
				case '4': PORTB = 0x04; break;
				case '5': PORTB = 0x05; break;
				case '6': PORTB = 0x06; break;
				case '7': PORTB = 0x07; break;
				case '8': PORTB = 0x08; break;
				case '9': PORTB = 0x09; break;
				case 'A': PORTB = 0x0A; break;
				case 'B': PORTB = 0x0B; break;
				case 'C': PORTB = 0x0C; break;
				case 'D': PORTB = 0x0D; break;
				case '*': PORTB = 0x0E; break;
				case '0': PORTB = 0x00; break;
				case '#': PORTB = 0x0F; break;
				default: PORTB = 0x1B; break;
			}
			break;
		default: break;
	}

	return state;
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRC = 0xF0; PORTC = 0x0F; // inputs
    DDRB = 0xFF; PORTB = 0x00; // outputs

    static task task1;
    task *tasks[] = { &task1 };
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    const char start = -1;
    // Task 1 (KeypadSM)
    task1.state = start; // Task initial state
    task1.period = 50;
    task1.elapsedTime = task1.period;
    task1.TickFct = &KeypadSM;

    TimerSet(50);
    TimerOn();

    unsigned short i = 0;
    /* Insert your solution below */
    while (1) {
	    for (i = 0; i < numTasks; i++) {
		    if (tasks[i]->elapsedTime == tasks[i]->period) {
			    tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			    tasks[i]->elapsedTime = 0;
		    }
		    tasks[i]->elapsedTime += 50;
	    }
	    while (!TimerFlag);
	    TimerFlag = 0;
    }
    return 0; // Error: Program should not exit
}
