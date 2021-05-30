/*	Author: Arnav Menon
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #11  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <util/delay.h> // ysed for _delay_ms function
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "lcd.h" // LCD code in directory
// keypad doesn't work
//#include "keypad.h"
#include "timer.h"

#define TIME 500

typedef struct task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct) (int);
} task;

unsigned char x;
unsigned int buttonPress = 0; // initialize as false
byte score = 0; // what is user's current score
byte y = 0;

enum ButtonSMStates { Start, Check, Release };
int ButtonSM (int state) {
	switch(state) {
		case Start:
			state = Check;
			break;
		case Check:
			if ((~PINC & 0xFF) == 0x00) // no button is pressed
				state = Release;
			else
				state = Check;
			break;
		case Release:
			if ((~PINC & 0xFF) != 0x00) { // wait for button unpress
				state = Release;
			}
			else
				state = Check;
			break;
		default:
			state = Start;
			break;
	}

	switch(state) {
		case Start: PORTB = 0x06; break;
		case Check:
			   PORTB = 0x07;
			   if ((~PINC & 0xFF) == 0x01) { // if PC0 pressed
				   PORTB = 0x01;
				   score++;
			   }
			   else if ((~PINC & 0x02) == 0x02) { // if PC1 pressed
				   PORTB = 0x02;
				   score++;
			   }
			   else if ((~PINC & 0x04) == 0x04) { // if PC2 pressed
				   PORTB = 0x03;
				   score++;
			   }
			   else if ((~PINC & 0x08) == 0x08) { // if PC3 pressed
				   PORTB = 0x04;
				   score++;
			   }
			   else if ((~PINC & 0x10) == 0x10) { // if PC4 pressed
				   PORTB = 0x05;
				   score++;
			   }
			   break;
		case Release: PORTB = 0x10; /*score--;*/ break;
		default: PORTB = 0x09; break;
	}

	return state;
}

enum PrintSMStates { Print_Print };
int PrintSM (int state) {
//	score++;
	switch(state) {
		case Print_Print: state = Print_Print; break;
		default: state = Print_Print; break;
	}

	switch(state) {
		case Print_Print:
			LCD_Clear();
			LCD_Message("Let's play The Tune ");
			LCD_Message("Score: ");
			LCD_Integer(score);
			//LCD_Integer(y);
			//y++;
			break;
		default: break;
	}

	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRD = 0x3F; // set D0..D5 as outputs for LCD
    DDRC = 0x00; PORTC = 0xFF;  // use PORTC as inputs for keypad
    DDRB = 0xFF; 

//    byte score = 0;

    static task task1, task2;
    task *tasks[] = { &task1, &task2 };
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    const char start = -1;

    // Task 1 (KeypadSM)
    task1.state = start; // Task initial state
    task1.period = 50;
    task1.elapsedTime = task1.period;
    task1.TickFct = &ButtonSM;

    // Task 2 (PrintSM)
    task2.state = start;
    task2.period = 50;
    task2.elapsedTime = task2.period;
    task2.TickFct = &PrintSM;

    LCD_Init(); 			// initialize LCD controller
  /*  LCD_Message("Let's play The Tune ");	// welcome message
    LCD_Message("Score: ");
    LCD_Integer(score); */
    TimerSet(TIME);
    TimerOn();
    //msDelay(TIME);			// wait
    while (1) {
/*	   LCD_Clear(); 			// clear screen otherwise it will print on top
	   LCD_Message("Let's play The Tune ");		// fill screen with ASCII characters
	   LCD_Message("Score: ");
	   LCD_Integer(score);*/
	   for (int i = 0; i < numTasks; i++) {
		   if (tasks[i]->elapsedTime == tasks[i]->period) {
			   tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			   tasks[i]->elapsedTime = 0;
		   }
		   tasks[i]->elapsedTime += 50;
	   }
	   //msDelay(TIME);
	   while (!TimerFlag);
	   TimerFlag = 0;
    }
 
    return 1;
}
