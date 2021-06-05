/*	Author: Arnav Menon
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #11  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *      I got the LCD code from a PDF from w8bh.net. Type "Add an LCD to your AVR microcontroller" into Google and it should be one of the first links.
 * 
 *      LINK TO VIDEO: https://drive.google.com/file/d/1tRGdpu04_i-yTgBt63Wsq8JS9eouYuXq/view?usp=sharing
 */
#include <avr/io.h>
#include <util/delay.h> // ysed for _delay_ms function
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "lcd.h" // LCD code in directory
#include "timer.h"

#define TIME 500

typedef struct task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct) (int);
} task;

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

unsigned char x;
unsigned int buttonPress = 0; // initialize as false
byte score = 0; // what is user's current score
byte y = 0;

unsigned int tune_one = 0; // initialze as false
unsigned int tune_two = 0; // initialize as false
unsigned int tune_three = 0; // initialize as false

unsigned int play_tune_one = 0; // if user wants this tune, set to 1
unsigned int play_tune_two = 0; // if user wants this tune, set to 1
unsigned int play_tune_three = 0; // if user wants this tune, set to 1

unsigned char one[6] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 }; // correect sequence for tune 1
unsigned char two[8] = { 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x80, 0x40 }; // tune 2
unsigned char three[10] = { 0x80, 0x40, 0x80, 0x10, 0x20, 0x80, 0x40, 0x80, 0x08, 0x10 }; // tune 3

// counters for all 3 tunes
unsigned short one_count = 0;
unsigned short two_count = 0;
unsigned short three_count = 0; 

enum ButtonSMStates { Start, Check, Release };
int ButtonSM (int state) {
	switch(state) {
		case Start:
			state = Check;
			break;
		case Check:
			if ((~PINC & 0x01) == 0x01) { // PC0 pressed, play tune_one
				tune_one = 1;
				state = Release;
			}
			else if ((~PINC & 0x02) == 0x02) { // PC1 pressed, play tune_two
				tune_two = 1; // global flag to play tune_two
				state = Release;
			}
			else if ((~PINC & 0x04) == 0x04) { // PC2 pressed, play tune_three
				tune_three = 1;
				state = Release;
			}
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
		case Start: break;
		case Check: break;
		case Release: break;
		default: break;
	}

	return state;
}

enum PianoInputsState { Piano_Start, Piano_Check };
int PianoSM (int state) {
	switch(state) {
		case Piano_Start:
			if ((~PINA & 0x01) == 0x01)
				state = Piano_Check;
			else if ((~PINA & 0x02) == 0x02)
				state = Piano_Check;
			else if ((~PINA & 0x04) == 0x04)
				state = Piano_Check;
			else if ((~PINA & 0x08) == 0x08)
				state = Piano_Check;
			else if ((~PINA & 0x10) == 0x10)
				state = Piano_Check;
			else if ((~PINA & 0x20) == 0x20)
				state = Piano_Check;
			else if ((~PINA & 0x40) == 0x40)
				state = Piano_Check;
			else if ((~PINA & 0x80) == 0x80)
				state = Piano_Check;
			else
				state = Piano_Start;
			break;
		case Piano_Check:
			if ((~PINA & 0xFF) == 0x00)
				state = Piano_Start;
			else
				state = Piano_Check;
			break;
		default:
			state = Piano_Start;
			break;
	}

	switch(state) {
		case Piano_Start:
			if (!tune_one && !tune_two && !tune_three) // if no tunes are playing, then turn off spekaer
				set_PWM(0);
			break;
		case Piano_Check:
			if ((~PINA & 0x01) == 0x01) {
				set_PWM(523.25); // high C
			}
			else if ((~PINA & 0x02) == 0x02) {
				set_PWM(493.88); // B
			}
			else if ((~PINA & 0x04) == 0x04) {
				set_PWM(440.00); // A
			}
			else if ((~PINA & 0x08) == 0x08) {
				set_PWM(392.00); // G
			}
			else if ((~PINA & 0x10) == 0x10) {
				set_PWM(349.23); // F
			}
			else if ((~PINA & 0x20) == 0x20) {
				set_PWM(329.63); // E
			}
			else if ((~PINA & 0x40) == 0x40) {
				set_PWM(293.66); // D
			}
			else if ((~PINA & 0x80) == 0x80) {
				set_PWM(261.63); // middle C
			}
			else {
				set_PWM(0); // nothing
			}
			break;
		default:
			break;

	}

	return state;
}


enum CheckTuneOneState { One_Init, One_Check, One_Release };
int CheckTuneOne(int state) {

	switch(state) {
		case One_Init:
			if (one_count == 0 && two_count == 0 && three_count == 0)
				PORTB = one_count;
			if ((play_tune_one == 1) && (~PINA & 0xFF) == one[one_count]) // if user wanted tune one, check it
				state = One_Check;
			else if (((~PINA & 0xFF) != one[one_count]) && ((~PINA & 0xFF) != 0x00)) {
				state = One_Init;
				one_count = 0;
			}
			else
				state = One_Init;
			break;
		case One_Check:
			if (two_count == 0 && three_count == 0)
				PORTB = one_count;
			if (one_count == 6) {
				play_tune_one = 0;
				one_count = 0; // reset counter
				score++; // got the point for that tune
				state = One_Init;
			}
			else if ((~PINA & 0xFF) == one[one_count] && (one_count < 6)) { // bc 6 elements in array
				state = One_Release;
			}
			else {
				state = One_Init;
			}
			break;
		case One_Release:
			if ((~PINA & 0xFF) == one[one_count]) // wait for button release
				state = One_Release;
			else
				state = One_Check;
		default:
			state = One_Init;
			break;
	}

	switch(state) {
		case One_Init:
			break;
		case One_Check:
			one_count++;
			break;
		case One_Release:
			break;
		default:
			break;
	}

	return state;
}

enum CheckTuneTwoState { Two_Init, Two_Check, Two_Release };
int CheckTuneTwo(int state) {
	switch(state) {
		case Two_Init:
			if (one_count == 0 && two_count == 0 && three_count == 0)
				PORTB = two_count;
			if (play_tune_two && (~PINA & 0xFF) == two[two_count]) // if user wanted tune two, check it
				state = Two_Check;
			else if (((~PINA & 0xFF) != two[two_count]) && ((~PINA & 0xFF) != 0x00)) {
				state = Two_Init;
				two_count = 0;
			}
			else
				state = Two_Init;
			break;
		case Two_Check:
			if (one_count == 0 && three_count == 0)
				PORTB = two_count;
			if (two_count == 8) {
				play_tune_two = 0;
				two_count = 0; // reset count
				score++; // got the point for that tune
				state = Two_Init;
			}
			else if ((~PINA & 0xFF) == two[two_count] && two_count < 8) // bc 8 elements in array
				state = Two_Release;
			else
				state = Two_Init;
			break;
		case Two_Release:
			if ((~PINA & 0xFF) == two[two_count]) // wait for button release
				state = Two_Release;
			else
				state = Two_Check;
		default:
			state = Two_Init;
			break;
	}

	switch(state) {
		case Two_Init:
			break;
		case Two_Check:
			two_count++;
			break;
		case Two_Release:
			break;
		default:
			break;
	}

	return state;
}

enum CheckTuneThreeState { Three_Init, Three_Check, Three_Release };
int CheckTuneThree(int state) {
	switch(state) {
		case Three_Init:
			if (one_count == 0 && two_count == 0 && three_count == 0)
				PORTB = three_count;
			if (play_tune_three && (~PINA & 0xFF) == three[three_count]) // if user wanted tune three, check it
				state = Three_Check;
			else if (((~PINA & 0xFF) != three[three_count]) && ((~PINA & 0xFF) != 0x00)) {
				state = Three_Init;
				three_count = 0;
			}
			else
				state = Three_Init;
			break;
		case Three_Check:
			if (one_count == 0 && two_count == 0)
				PORTB = three_count;
			if (three_count == 10) {
				play_tune_three = 0;
				three_count = 0; // reset counter
				score++; // got the point for that melody
				state = Three_Init;
			}
			else if ((~PINA & 0xFF) == three[three_count] && three_count < 6) // bc 6 elements in array
				state = Three_Release;
			else
				state = Three_Init;
			break;
		case Three_Release:
			if ((~PINA & 0xFF) == three[three_count]) // wait for button release
				state = Three_Release;
			else
				state = Three_Check;
		default:
			state = Three_Init;
			break;
	}

	switch(state) {
		case Three_Init:
			break;
		case Three_Check:
			three_count++;
			break;
		case Three_Release:
			break;
		default:
			break;
	}

	return state;
}

enum PlaySpeakerState { Speaker_Init, Speaker_Check, Speaker_Start, Speaker_Play_Tune_One, Speaker_Play_Tune_Two, Speaker_Play_Tune_Three, Speaker_Release };
unsigned short i = 0;
int PlaySpeaker(int state) {
	switch(state) {
		case Speaker_Init:
			PWM_on();
			state = Speaker_Check;
			break;
		case Speaker_Check:
			if (tune_one == 1 || tune_two == 1 || tune_three == 1)
				state = Speaker_Start;
			else
				state = Speaker_Check;
			break;
		case Speaker_Start:
			if (tune_one == 1) {
				play_tune_one = 1; // set global flag as 1
				state = Speaker_Play_Tune_One;
			}
			else if (tune_two == 1) {
				play_tune_two = 1; // set gloal flag to 1
				state = Speaker_Play_Tune_Two;
			}
			else if (tune_three == 1) {
				play_tune_three = 1; // set global flag to 1
				state = Speaker_Play_Tune_Three;
			}
			else
				state = Speaker_Start;
			break;
		// play all F notes
		case Speaker_Play_Tune_One:
			if (i >= 11 && ((~PINC & 0x01) == 0x01)) { // button still pressed after sequence is over, dont restart
				state = Speaker_Release;
				set_PWM(0);
			}
			else if (i >= 11) { // melody over
				state = Speaker_Check;
				set_PWM(0);
			}
			else
				state = Speaker_Play_Tune_One;
			break;
		// play scale starting at E
		case Speaker_Play_Tune_Two:
			if (i >= 8 && ((~PINC & 0x02) == 0x02)) { // button still pressed after sequence is over, dont restart
				state = Speaker_Release;
				set_PWM(0);
			}
			else if (i >= 8) { // melody over
				state = Speaker_Check;
				set_PWM(0);
			}
			else
				state = Speaker_Play_Tune_Two;
			break;
		// play happy birthday
		case Speaker_Play_Tune_Three:
	//		PORTB = 0x00;
			if (i >= 11 && ((~PINC & 0x02) == 0x02)) { // button still pressed after sequence is over, dont restart
				state = Speaker_Release;
				set_PWM(0);
			}
			else if (i >= 11) { // melody over
				state = Speaker_Check;
				set_PWM(0);
//				PORTB = 0x02;
			}
			else
				state = Speaker_Play_Tune_Three;
			break;
		case Speaker_Release:
			PWM_off();
			if ((~PINC & 0x01) == 0x00) {
				state = Speaker_Init;
			}
			else {
				state = Speaker_Release; // wait for button unpress
			}
			break;
		default:
			state = Speaker_Init;
			break;
	}

	switch(state) {
		case Speaker_Init: break;
		case Speaker_Check: 
			tune_one = 0;
			tune_two = 0;
			tune_three = 0;
			break;
		case Speaker_Start: i = 0; break;
		case Speaker_Play_Tune_One:
			if (i < 1)
				set_PWM(349.23);
			else if (i < 2)
				set_PWM(0);
			else if (i < 3)
				set_PWM(349.23);
			else if (i < 4)
				set_PWM(0);
			else if (i < 5)
				set_PWM(349.23);
			else if (i < 6)
				set_PWM(0);
			else if (i < 7)
				set_PWM(349.23);
			else if (i < 8)
				set_PWM(0);
			else if (i < 9)
				set_PWM(349.23);
			else if (i < 10)
				set_PWM(0);
			else if (i < 11)
				set_PWM(349.23);
			i++;
			break;
		case Speaker_Play_Tune_Two:
			if (i < 1)
				set_PWM(329.63);
			else if (i < 2)
				set_PWM(349.23);
			else if (i < 3)
				set_PWM(392.00);
			else if (i < 4)
				set_PWM(440.00);
			else if (i < 5)
				set_PWM(493.88);
			else if (i < 6)
				set_PWM(523.25);
			else if (i < 7)
				set_PWM(261.63);
			else if (i < 8)
				set_PWM(293.66);
			i++;
			break;
		case Speaker_Play_Tune_Three:
			if (i < 1) 
				set_PWM(261.63);
			else if (i < 2)
				set_PWM(293.66);
			else if (i < 3)
				set_PWM(261.63);
			else if (i < 4)
				set_PWM(349.23);
			else if (i < 6)
				set_PWM(329.63);
			else if (i < 7)
				set_PWM(261.63);
			else if (i < 8)
				set_PWM(293.66);
			else if (i < 9)
				set_PWM(261.63);
			else if (i < 10)
				set_PWM(392.00);
			else if (i < 11)
				set_PWM(349.23);
			i++;
			break;
		case Speaker_Release:
			i = 0;
			tune_one = 0;
			tune_two = 0; // reset global flag
			tune_three = 0;
			break;
		default: break;
	}

	return state;
}

enum PrintSMStates { Print_Print };
int PrintSM (int state) {
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
			break;
		default: break;
	}

	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRD = 0x3F; // set D0..D5 as outputs for LCD
    DDRC = 0x00; PORTC = 0xFF;  // use PORTC as inputs for what meoldy to play 
    DDRB = 0xFF; // PB6 is 1 for the speaker output 
    DDRA - 0x00; PORTA = 0xFF; // use PORTA as inputs for "piano"

    static task task1, task2, task3, task4, task5, task6, task7;
    task *tasks[] = { &task1, &task2, &task3, &task4, &task5, &task6, &task7 };
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    const char start = -1;

    // Task 1 (KeypadSM)
    task1.state = start; // Task initial state
    task1.period = 10;
    task1.elapsedTime = task1.period;
    task1.TickFct = &ButtonSM;

    // Task 2 (PrintSM)
    task2.state = start;
    task2.period = 10;
    task2.elapsedTime = task2.period;
    task2.TickFct = &PrintSM;

    // Task 3 (PlaySpeakerSM)
    task3.state = start;
    task3.period = 10;
    task3.elapsedTime = task3.period;
    task3.TickFct = &PlaySpeaker;

    // Task 4 (PianoInputSM)
    task4.state = start;
    task4.period = 10;
    task4.elapsedTime = task4.period;
    task4.TickFct = &PianoSM;

    // Task 5 (CheckTuneOne)
    task5.state = start;
    task5.period = 10;
    task5.elapsedTime = task5.period;
    task5.TickFct = &CheckTuneOne;

    // Task 6 (CheckTuneTwo)
    task6.state = start;
    task6.period = 10;
    task6.elapsedTime = task6.period;
    task6.TickFct = &CheckTuneTwo;

    // Task 7 (CheckTuneThree)
    task7.state = start;
    task7.period = 10;
    task7.elapsedTime = task7.period;
    task7.TickFct = &CheckTuneThree;

    LCD_Init(); 			// initialize LCD controller
    TimerSet(TIME);
    TimerOn();
    while (1) {
	   for (int i = 0; i < numTasks; i++) {
		   if (tasks[i]->elapsedTime == tasks[i]->period) {
			   tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			   tasks[i]->elapsedTime = 0;
		   }
		   tasks[i]->elapsedTime += tasks[i]->period;
	   }
	   while (!TimerFlag);
	   TimerFlag = 0;
    }
 
    return 1;
}
