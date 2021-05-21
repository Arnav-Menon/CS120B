/*	Author Arnav Menon:
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #10  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	LINK TO VIDEO: https://drive.google.com/file/d/1nTVfysAZULE9Lf0zCzuouA8e-LM_FO6W/view?usp=sharing
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
unsigned int myIndex = 0;
unsigned int isLocked = 1; // initialize as true

enum KeypadSMStates { Start, Check, Wait, Wait_1 };
unsigned char array[6] = {'#', '1', '2', '3', '4', '5'};
int KeypadSM (int state) {
	x = GetKeypadKey();
	switch(state) {
		case Start:
			state = Check;
			break;
		case Check:
			if (x == array[myIndex])
				state = Wait;
			else if (x == '\0') // still waiting on input
				state = Wait;
			else { // input was incorrect, start over
				state = Start;
				myIndex = 0;
			}
			break;
		case Wait:
			if (x == '\0') { // button was "released"
				state = Check;
			//	myIndex++;
			}
			else if (x == array[myIndex]) {
				state = Wait_1;
			//	myIndex++;
			}
			else {
				state = Start;
				myIndex = 0;
			}
			break;
		case Wait_1:
			if (x == array[myIndex])
				state = Wait_1;
			else if (x == '\0') {
				myIndex++;
				state = Check;
			}
			break;
		default:
			state = Start;
			break;
	}

	switch(state) {
		case Start: PORTB = 0x00; break;
		case Check:

			    PORTA = myIndex;

		    if (myIndex == 6) { // successfully unlocoked door
			    PORTB = 0x01;
			    myIndex = 0x00;
			    isLocked = 0; // unlock door
		    }
		    break;
		case Wait: PORTA = myIndex; break;
		case Wait_1: PORTA = myIndex; break;
		default: PORTB = 0x00; break;
	}

	return state;
}

enum LockUnlockSM { Init, Action };
int LockUnlockDoorSM (int state) {
	switch(state) {
		case Init:
			if ((~PINB & 0xE0) == 0x80) // PB7 pressed
				state = Action;
			else
				state = Init;
			break;
		case Action:
			if ((~PINB & 0xE0) == 0x80) // button still pressed
				state = Action;
			else
				state = Init;
			break;
		default:
			state = Init;
			break;
	}

	switch(state) {
		case Init:
			break;
		case Action:
			if (!isLocked) { // lock door
				PORTB = 0x00;
				isLocked = 1;
			}
			break;
		default: break;

	}
	
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRC = 0xF0; PORTC = 0x0F; // inputs
    DDRB = 0x1F; PORTB = 0xE0; // outputs
    
    DDRA = 0xFF; PORTA = 0x00;

    static task task1, task2;
    task *tasks[] = { &task1, &task2 };
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    const char start = -1;
    // Task 1 (KeypadSM)
    task1.state = start; // Task initial state
    task1.period = 50;
    task1.elapsedTime = task1.period;
    task1.TickFct = &KeypadSM;

    task2.state = start;
    task2.period = 50;
    task2.elapsedTime = task2.period;
    task2.TickFct = &LockUnlockDoorSM;

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
