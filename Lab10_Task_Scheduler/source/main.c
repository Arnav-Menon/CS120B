/*	Author Arnav Menon:
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #10  Exercise #4
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
#include "avr/interrupt.h"
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
unsigned int newIndex = 0;
unsigned int isLocked = 1; // initialize as true
unsigned int hasChanged = 0; // initialize as false

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

enum KeypadSMStates { Start, Check, Wait, Wait_1 };
unsigned char array[6] = {'#', '1', '2', '3', '4', '5'};
unsigned char newArray[4] = { ' ', ' ', ' ', ' '};
int KeypadSM (int state) {
	x = GetKeypadKey();
	switch(state) {
		case Start:
			state = Check;
			break;
		case Check:
			if (!hasChanged) {
				if (x == array[myIndex])
					state = Wait;
				else if (x == '\0') // still waiting on input
					state = Wait;
				else { // input was incorrect, start over
					state = Start;
					myIndex = 0;
				}
			}
			else {
				if (x == newArray[newIndex])
					state = Wait;
				else if (x == '\0')
					state = Wait;
				else {
					state = Start;
					newIndex = 0;
				}
			}
			break;
		case Wait:
			if (!hasChanged) {
				if (x == '\0') { // button was "released"
					state = Check;
				}
				else if (x == array[myIndex]) {
					state = Wait_1;
				}
				else {
					state = Start;
					myIndex = 0;
				}
			}
			else {
				if (x == '\0') 
					state = Check;
				else if (x == newArray[newIndex])
					state = Wait_1;
				else {
					state = Start;
					newIndex = 0;
				}
			}	
			break;
		case Wait_1:
			if (!hasChanged) {
				if (x == array[myIndex])
					state = Wait_1;
				else if (x == '\0') {
					myIndex++;
					state = Check;
				}
			}
			else {
				if (x == newArray[newIndex])
					state = Wait_1;
				else if (x == '\0') {
					newIndex++;
					state = Check;
				}
			}
			break;
		default:
			state = Start;
			break;
	}

	switch(state) {
		case Start: PORTB = 0x00; break;
		case Check:
		    if (!hasChanged) {
		   	 if (myIndex == 6) { // successfully unlocoked door
				    PORTB = 0x01;
				    myIndex = 0x00;
				    isLocked = 0; // unlock door
			    }
		    }
		    else {
			    if (newIndex == 4) {
				    PORTB = 0x01;
				    newIndex = 0x00;
				    isLocked = 0;
			    }
		    }
		    break;
		case Wait: break;
		case Wait_1: break;
		default: PORTB = 0x00; break;
	}

	return state;
}

enum LockUnlockSM { Init, Action };
int LockUnlockDoorSM (int state) {
	switch(state) {
		case Init:
			if ((~PINB & 0x80) == 0x80) // PB7 pressed
				state = Action;
			else
				state = Init;
			break;
		case Action:
			if ((~PINB & 0x80) == 0x80) // button still pressed
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

enum SpeakerStates { Speaker_Init, Speaker_Check, Speaker_Start, Speaker_Play, Speaker_Release };
unsigned short i = 0;
int PlaySpeaker(int state) {
	//unsigned char button = ~PINA & 0x01; // check if PA0 pressed

	switch(state) {
		case Speaker_Init:
			PWM_on();
			state = Speaker_Check;
			break;
		case Speaker_Check:
			if ((~PINA & 0x01) == 0x01)
				state = Speaker_Start;
			else
				state = Speaker_Check;
			break;
		case Speaker_Start:
			if ((~PINA & 0x01) == 0x01) {
//				PORTB = 0x0F;
				state = Speaker_Play;
			}
			else	
				state = Speaker_Start;
			break;
		case Speaker_Play:
			PORTB = 0x00;
			if (i >= 30 && ((~PINA & 0x01) == 0x01)) { // button still pressed after sequence is over, dont restart
				state = Speaker_Release;
//				PORTB = 0x01;
			}
			else if (i >= 30) { // melody over
				state = Speaker_Init;
//				PORTB = 0x02;
			}
			else
				state = Speaker_Play;
			break;
		case Speaker_Release:
			if ((~PINA & 0x01) == 0x00)
				state = Speaker_Init;
			else
				state = Speaker_Release; // wait for button unpress
			break;
		default:
			state = Speaker_Init;
			break;
	}

	switch(state) {
		case Speaker_Init: break;
		case Speaker_Check: break;
		case Speaker_Start: i = 0; break;
		case Speaker_Play:
		//	PORTA = 0x03;
			if (i < 2)
				set_PWM(261.63);
			else if (i < 6)
				set_PWM(261.63);
			else if (i < 8)
				set_PWM(293.66);
			else if (i < 10)
				set_PWM(261.63);
			else if (i < 14)
				set_PWM(349.23);
			else if (i < 18)
				set_PWM(329.63);
			else if (i < 20)
				set_PWM(261.63);
			else if (i < 23)
				set_PWM(293.66);
			else if (i < 26)
				set_PWM(261.63);
			else if (i < 28)
				set_PWM(392.00);
			else if (i < 30)
				set_PWM(349.23);
			i++;
			break;
		case Speaker_Release:
			//PORTA = 0x04;
			PWM_off();
			i = 0;
			break;
		default: break;
	}

	return state;
}

enum NewPasswordStates { New_Init, New_Signal, New_Password, New_Done, New_Release, New_Wait };
unsigned char y;
int NewPassword(int state) {
	y = GetKeypadKey();

	switch(state) {
		case New_Init:
			state = New_Signal;
			break;
		case New_Signal:
			PORTA = 0xFF;
			if (y == '*' && (~PINB & 0x80) == 0x80) { // if '*' and PB7 are pressed, we want to start recording new password
				state = New_Release;
	//			PORTA = 0x01;
			}
			else
				state = New_Signal; // wait for the two inputs
			break;
		case New_Release:
			if (y == '*' && (~PINB & 0x80) == 0x80)  // still pressed form last time, wait for realase
				state = New_Release;
			else if (y != '\0' && (~PINB & 0x80) != 0x80) // wait for valid input
				state = New_Password;
			else
				state = New_Release;
			break;
		case New_Password:
			PORTA = 0x02;
			if ((~PIND & 0x40) == 0x40) // PD7 pressed, done processign new password
				state = New_Done;
			else  
				state = New_Wait;
			break;
		case New_Wait:
			if (y != '\0')
				state = New_Wait;
			else
				state = New_Release;
			break;
		case New_Done:
	//		PORTA = 0x0A;
			if ((~PIND & 0x40) == 0x40) // button still pressed, wait for release
				state = New_Done;
			else
				state = New_Init;
			break;
		default:
			state = New_Init;	
			break;
	}

	switch(state) {
		case New_Init: break;
		case New_Signal: break;
		case New_Release: break;
		case New_Wait: break;
		case New_Password: 
			if (y != '\0' && newIndex < 4) {
				newArray[newIndex] = y;
				newIndex++;
			}
	//		PORTB = newIndex;
			break;
		case New_Done: hasChanged = 1; newIndex = 0; break;
		default: break;
	}

	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRC = 0xF0; PORTC = 0x0F; // inputs
    DDRB = 0x5F; PORTB = 0xE0; // outputs
    DDRD = 0x3F; PORTD = 0x00; // PD7,6 as input
    DDRA = 0x0E; PORTA = 0x00; // high 4 input, low 4 output

    static task task1, task2, task3, task4;
    task *tasks[] = { &task1, &task2, &task3, &task4 };
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    const char start = -1;
    // Task 1 (KeypadSM)
    task1.state = start; // Task initial state
    task1.period = 50;
    task1.elapsedTime = task1.period;
    task1.TickFct = &KeypadSM;

    // Task 2 (Lock/Unlock Door)
    task2.state = start;
    task2.period = 50;
    task2.elapsedTime = task2.period;
    task2.TickFct = &LockUnlockDoorSM;

    // Task 3 (PlaySpeaker)
    task3.state = start;
    task3.period = 100;
    task3.elapsedTime = task3.period;
    task3.TickFct = &PlaySpeaker;

    // Task 4 (NewPassword)
    task4.state = start;
    task4.period = 50;
    task4.elapsedTime = task4.period;
    task4.TickFct = &NewPassword;

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
