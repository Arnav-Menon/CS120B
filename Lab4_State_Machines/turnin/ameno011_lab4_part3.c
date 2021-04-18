/*	Author: Arnav Menon
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #4  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum STATES { SM1_Start, SM1_Lock, SM1_ActuallyWait, SM1_WaitUnlock, SM1_Unlock1, SM1_Unlock2 } state;
void Tick() {

        unsigned char x = PINA & 0x07;
        unsigned char y = PINA & 0x07;
        unsigned char pound = PINA & 0x07;
        unsigned char lock = PINA & 0x87;

        // state transitions
        switch(state) {
                case SM1_Start:
                        if (pound == 0x04) {
                                state = SM1_Unlock1;
                        }
                        else {
                                state = SM1_Lock;
                        }
                        break;
                case SM1_Lock:
                        if (lock == 0x80) {
                                state = SM1_Lock;
                        }
                        else if (pound == 0x04) {
                                state = SM1_Unlock1;
                        }
                        else {
                                state = SM1_Lock;
                        }
                        break;
                case SM1_Unlock1:
                        if (lock == 0x80) {
                                state = SM1_Lock;
                        }
                        else if (PINA == 0x06) {
                                state = SM1_Lock;
                        }
			else if (pound == 0x04) {
				state = SM1_Unlock1;
			}
			else if (PINA == 0x00) {
				state = SM1_ActuallyWait;
			}
                        //else if (y == 0x02) {
                        //        state = SM1_Unlock2;
                        //}
                        else {
                                state = SM1_Lock;
                        }
                        break;
		case SM1_ActuallyWait:
			if (y == 0x02) {
				state = SM1_Unlock2;
			}
			else if (PINA == 0x00) {
				state = SM1_ActuallyWait;
			}
			else {
				state = SM1_Lock;
			}
			break;
                case SM1_WaitUnlock:
                        if (PINA == 0x06) {
                                state = SM1_Lock;
                        }
			//else if (y == 0x02) {
			//	state = SM1_Unlock2;
			//}
                        else if (pound == 0x04 || PINA == 0x00) {
                                state = SM1_WaitUnlock;
                        }
			else if (y == 0x02) {
				state = SM1_Unlock2;
			}
                        else {
                                state = SM1_Lock;
                        }
                        break;
                case SM1_Unlock2:
                        if (lock == 0x80 || PINA == 0x06) {
                              state = SM1_Lock;
                        }
                        else {
                              state = SM1_WaitUnlock;
                        }
                        break;
                default:
                        state = SM1_Start;
                        break;
        }

        // state actions
        switch(state) {
                case SM1_Start:
                        break;
                case SM1_Lock:
                        PORTB = 0x00;
                        break;
		case SM1_ActuallyWait:
			break;
                case SM1_Unlock1:
                        break;
                case SM1_WaitUnlock:
                        break;
                case SM1_Unlock2:
                        PORTB = 0x01;
                        break;
                default:
                        break;
        }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF; // configure port A's 8 pins as outputs
    DDRB = 0xFF; PORTB = 0x00; // configure port B's 8 pins as outputs

    /* Insert your solution below */
    while (1) {
	    Tick(); // call SM function
    }
    return 1;
}
