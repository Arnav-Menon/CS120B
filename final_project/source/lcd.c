// got this code from w8bh.net

#include "lcd.h"

// ------------------------------------------------------------------------------
// MISC ROUTINES

/*void SetupPortD() {
	DDRD = 0x3F;
}*/

void msDelay(int delay) {
	for (int i = 0; i < delay; i++)
		_delay_ms(1);
}

void FlashLED() {
	SetBit(PORTD, LED); // PD5
	msDelay(250);
	ClearBit(PORTD, LED);
	msDelay(250);
}

void PulseEnableLine() {
	SetBit(PORTD, LCD_E); 				// take LCD enable line high
	_delay_us(40);					// wait 40 microseconds
	ClearBit(PORTD, LCD_E);				// take LCD enable line low
}

void SendNibble(byte data) {
	PORTD &= 0xC3;					// 1100.0011 = clear 4 data lines into controller
	if (data & _BV(4)) SetBit(PORTD, DAT4);
	if (data & _BV(5)) SetBit(PORTD, DAT5);
	if (data & _BV(6)) SetBit(PORTD, DAT6);
	if (data & _BV(7)) SetBit(PORTD, DAT7);
	PulseEnableLine();				// clock 4 bits into controller
}

void SendByte(byte data) {
	SendNibble(data);				// send upper 4 bits
	SendNibble(data << 4);				// send lower 4 bits
	ClearBit(PORTD, 5);
}

void LCD_Cmd(byte cmd) {
	ClearBit(PORTD, LCD_RS);			// R/S line 0 = character data
	SendByte(cmd);					// send it
}

void LCD_Char(byte ch) {
	SetBit(PORTD, LCD_RS);				// R/S line 1 = character data
	SendByte(ch);					// send it
}

void LCD_Init() {
	LCD_Cmd(0x33); 					// initialize controller
	LCD_Cmd(0x32); 					// set to 4-bit input mode
	LCD_Cmd(0x28); 					// 2 line, 5x7 matrix
	LCD_Cmd(0x0C); 					// turn cursor off (0x0E to enable)
	LCD_Cmd(0x06); 					// cursor direction = right
	LCD_Cmd(0x01); 					// start with clear display
	msDelay(3); 					// wait for LCD to initialize
}

void LCD_Clear() {					// clear the LCD display 
	LCD_Cmd(CLEARDISPLAY);
	msDelay(3); 					// wait for LCD to process command
}

void LCD_Home() { 					// home LCD cursor (without clearing)
	LCD_Cmd(SETCURSOR);
}

void LCD_Goto(byte x, byte y) { 			// put LCD cursor on specified line
	byte addr = 0; 					// line 0 begins at addr 0x00
	switch (y) {
		case 1: addr = 0x40; break; 		// line 1 begins at addr 0x40
		case 2: addr = 0x14; break;
		case 3: addr = 0x54; break;
	}
	LCD_Cmd(SETCURSOR + addr + x); 			// update cursor with x,y position
}

void LCD_Line(byte row) { 				// put cursor on specified line
	 LCD_Goto(0,row);
}

void LCD_Message(const char *text) { 			// display string on LCD
	while (*text) 					// do until /0 character
		LCD_Char(*text++);			// send char & update char pointer
}

void LCD_Hex(int data) {				// displays the hex value of DATA at current LCD cursor position
	char st[8] = "";				// dave enough space for result
	itoa(data, st, 16);				// convert to ascii hex
	//LCD_Message("0x");				// add prefix if desired
	LCD_Message(st);
}

void LCD_Integer(int data) {				// displays the integer value of DATA at current LCD cursor position
	char st[8] = "";				// save enough space for result
	itoa(data, st, 10);				// convert to ascii
	LCD_Message(st);				// display in on LCD
}


// ---------------------------------------------------------------------------------------------------
// DEMO FUNCTIONS


void UpdateCursor(byte count) {				// helper function for FillScreen
	switch(count) {
		case 0: LCD_Line(0); break;
		case 16: LCD_Line(1); break;
		case 32: LCD_Line(2); break;
		case 48: LCD_Line(3); break;
	}
}

char GetNextChar(char ch) {				// helper function for FIllScreen
	if ((ch < 0x20) | (ch >= 0xFF))
		return 0x20;
	if ((ch >= 0x7F) & (ch < 0xA0))
		return 0xA0;
	return ++ch;
}

void FillScreen() {
	// fills LCD screen with ascii chars
	// be sure to set NUMCHARS to 32 or 64 chars, depending on the size of display
	// 32 looks good on 16x2 dispalys, 64 looks good on 20x4
	// four line displays also show an incrementing 1-99 page counter
	char ch = 'A';
	LCD_Clear();
	for (byte count = 1; count < 100; count++) {
		LCD_Goto(18, 0);
		LCD_Integer(count);			// show counter (vis on 4-liners only)
		for (byte i = 0; i < NUMCHARS; i++) {	// do screenful of characters
			UpdateCursor(i);		// go to next line. if necessary
			LCD_Char(ch);			// show current ascii character
			ch = GetNextChar(ch);		// update to next character
			//msDelay(1);			// set animation speed
		}
	}
}

