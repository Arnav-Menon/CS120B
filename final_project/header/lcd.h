// got this code from w8bh.net


#ifndef __LCD_H__
#define __LCD_H__


// -------------------------------------------------------------------------------
// GLOBAL DEFINES

#define F_CPU 16000000L			// run CPU t 16 MHz
#define LED	5 			// LED on PD5
#define ClearBit(x, y) x &= ~_BV(y)
#define SetBitLCD(x, y) x |= _BV(y)
#define NUMCHARS 64					// number of chars per screen

// ------------------------------------------------------------------------------
// INCLUDES

#include <avr/io.h>
#include <util/delay.h>			// used for _delay_ms function
#include <string.h> 			// string manipulation routines
#include <stdlib.h>

// -------------------------------------------------------------------------------
// TYPEDEFS

typedef uint8_t byte;
typedef int8_t sbyte;

// ------------------------------------------------------------------------------
// MISC ROUTINES

void SetupPortD();

void msDelay(int);

void FlashLed();

// ---------------------------------------------------------------------------
// HD44780-LCD DRIVER ROUTINES
//
// Routines:
// LCD_Init initializes the LCD controller
// LCD_Cmd sends LCD controller command
// LCD_Char sends single ascii character to display
// LCD_Clear clears the LCD display & homes cursor
// LCD_Home homes the LCD cursor
// LCD_Goto puts cursor at position (x,y)
// LCD_Line puts cursor at start of line (x)
// LCD_Hex displays a hexadecimal value
// LCD_Integer displays an integer value
// LCD_Message displays a string
//
// The LCD module requires 6 I/O pins: 2 control lines & 4 data lines.
// PORTD is used for data communications with the HD44780-controlled LCD.
// The following defines specify which port pins connect to the controller:

#define LCD_RS 0 // pin for LCD R/S (eg PB0)
#define LCD_E 1 // pin for LCD enable
#define DAT4 2 // pin for d4
#define DAT5 3 // pin for d5
#define DAT6 4 // pin for d6
#define DAT7 5 // pin for d7

// The following defines are HD44780 controller commands
#define CLEARDISPLAY 0x01
#define SETCURSOR 0x80

void PulseEnableLine();

void SendNibble(byte data);

void SendByte(byte data);

void LCD_Cmd(byte cmd);

void LCD_Char(byte ch);

void LCD_Init();

void LCD_Clear(); 					// clear the LCD display 

void LCD_Home(); 					// home LCD cursor (without clearing)

void LCD_Goto(byte x, byte y); 			// put LCD cursor on specified line

void LCD_Line(byte row); 				// put cursor on specified line

void LCD_Message(const char *text); 			// display string on LCD

void LCD_Hex(int data);				// displays the hex value of DATA at current LCD cursor position

void LCD_Integer(int data);				// displays the integer value of DATA at current LCD cursor position


// ---------------------------------------------------------------------------------------------------
// DEMO FUNCTIONS

void UpdateCursor(byte count);				// helper function for FillScreen

char GetNextChar(char ch);				// helper function for FIllScreen

void FillScreen();
	// fills LCD screen with ascii chars
	// be sure to set NUMCHARS to 32 or 64 chars, depending on the size of display
	// 32 looks good on 16x2 dispalys, 64 looks good on 20x4
	// four line displays also show an incrementing 1-99 page counter

#endif // __LCD_H__
