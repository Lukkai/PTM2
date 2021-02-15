/*
 * LCD_HD44780.cpp
 *
 *  Created on: 12-10-2013
 *      Author: Wojtek
 */

//-------------------------------------------------------------------------------------------------
// Wyswietlacz alfanumeryczny ze sterownikiem HD44780
// Sterowanie w trybie 4-bitowym bez odczytu flagi zajetosci
// z dowolnym przypisaniem sygnalow sterujacych
// Plik : HD44780.c
// Mikrokontroler : Atmel AVR
// Kompilator : avr-gcc
// Autor : Radoslaw Kwiecien
// Zrodlo : http://radzio.dxp.pl/hd44780/
// Data : 24.03.2007
//-------------------------------------------------------------------------------------------------
#include "LCD_HD44780.h"

//-------------------------------------------------------------------------------------------------
//
// Procedura inicjalizacji kontrolera HD44780.
//
//-------------------------------------------------------------------------------------------------
void LCD_HD44780::init(void) {
	unsigned char i;
	LCD_DB4_DIR |= LCD_DB4; // Konfiguracja kierunku pracy wyprowadzen
	LCD_DB5_DIR |= LCD_DB5; //
	LCD_DB6_DIR |= LCD_DB6; //
	LCD_DB7_DIR |= LCD_DB7; //
	LCD_E_DIR |= LCD_E;   //
	LCD_RS_DIR |= LCD_RS;  //
	_delay_ms(15); // oczekiwanie na ustalibizowanie sie napiecia zasilajacego
	LCD_RS_PORT &= ~LCD_RS; // wyzerowanie linii RS
	LCD_E_PORT &= ~LCD_E;  // wyzerowanie linii E

	for (i = 0; i < 3; i++) // trzykrotne powtorzenie bloku instrukcji
			{
		LCD_E_PORT |= LCD_E; //  E = 1
		LCD_HD44780::outNibble(0x03); // tryb 8-bitowy
		LCD_E_PORT &= ~LCD_E; // E = 0
		_delay_ms(5); // czekaj 5ms
	}

	LCD_E_PORT |= LCD_E; // E = 1
	LCD_HD44780::outNibble(0x02); // tryb 4-bitowy
	LCD_E_PORT &= ~LCD_E; // E = 0

	_delay_ms(1); // czekaj 1ms
	LCD_HD44780::writeCommand(
			HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE
					| HD44780_4_BIT); // interfejs 4-bity, 2-linie, znak 5x7
	LCD_HD44780::writeCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF); // wylaczenie wyswietlacza
	LCD_HD44780::writeCommand(HD44780_CLEAR); // czyszczenie zawartosci pamieci DDRAM
	_delay_ms(2);
	LCD_HD44780::writeCommand(
			HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT); // inkrementaja adresu i przesuwanie kursora
	LCD_HD44780::writeCommand(
			HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF
					| HD44780_CURSOR_NOBLINK); // wlacz LCD, bez kursora i mrugania
}

//-------------------------------------------------------------------------------------------------
//
// Funkcja wystawiajaca polbajt na magistrale danych
//
//-------------------------------------------------------------------------------------------------
void LCD_HD44780::outNibble(unsigned char nibbleToWrite) {
	if (nibbleToWrite & 0x01)
		LCD_DB4_PORT |= LCD_DB4;
	else
		LCD_DB4_PORT &= ~LCD_DB4;

	if (nibbleToWrite & 0x02)
		LCD_DB5_PORT |= LCD_DB5;
	else
		LCD_DB5_PORT &= ~LCD_DB5;

	if (nibbleToWrite & 0x04)
		LCD_DB6_PORT |= LCD_DB6;
	else
		LCD_DB6_PORT &= ~LCD_DB6;

	if (nibbleToWrite & 0x08)
		LCD_DB7_PORT |= LCD_DB7;
	else
		LCD_DB7_PORT &= ~LCD_DB7;
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu bajtu do wyswietacza (bez rozronienia instrukcja/dane).
//
//-------------------------------------------------------------------------------------------------
void LCD_HD44780::write(unsigned char dataToWrite) {
	LCD_E_PORT |= LCD_E;
	LCD_HD44780::outNibble(dataToWrite >> 4);
	LCD_E_PORT &= ~LCD_E;
	LCD_E_PORT |= LCD_E;
	LCD_HD44780::outNibble(dataToWrite);
	LCD_E_PORT &= ~LCD_E;
	_delay_us(50);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu rozkazu do wyswietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_HD44780::writeCommand(unsigned char commandToWrite) {
	LCD_RS_PORT &= ~LCD_RS;
	LCD_HD44780::write(commandToWrite);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu danych do pamieci wyswietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_HD44780::writeData(unsigned char dataToWrite) {
	LCD_RS_PORT |= LCD_RS;
	LCD_HD44780::write(dataToWrite);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja wyswietlenia napisu na wyswietlaczu.
//
//-------------------------------------------------------------------------------------------------
void LCD_HD44780::writeText(char * text) {
	while (*text)
		LCD_HD44780::writeData(*text++);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja ustawienia wspolrzednych ekranowych
//
//-------------------------------------------------------------------------------------------------
void LCD_HD44780::goTo(unsigned char x, unsigned char y) {
	LCD_HD44780::writeCommand(HD44780_DDRAM_SET | (x + (0x40 * y)));
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja czyszczenia ekranu wyswietlacza.
//
//-------------------------------------------------------------------------------------------------
void LCD_HD44780::clear(void) {
	LCD_HD44780::writeCommand(HD44780_CLEAR);
	_delay_ms(2);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja przywrocenia poczatkowych wsporzednych wyswietlacza.
//
//-------------------------------------------------------------------------------------------------
void LCD_HD44780::home(void) {
	LCD_HD44780::writeCommand(HD44780_HOME);
	_delay_ms(2);
}
//
//Wyswietla na wyswietlaczu LCD liczbe przekazana w argumencie funkcji
void LCD_HD44780::showNumber(int32_t liczba) {
	LCD_HD44780::clear();
	char text[16];
	sprintf(text, "%d", liczba);
	LCD_HD44780::writeText(text);
}
