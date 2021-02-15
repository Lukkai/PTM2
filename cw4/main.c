/*
 *  Cwiczenie 3
 *  regulator dwustawny z histerezą oraz wyswietlanie stanu na LCD
 * 
 *  Created on: 21 stycznia 2011
 *      Author: Lukasz Chwistek
 */

#define __AVR_ATmega32__
#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "GLOBAL.h"
#include "HD44780.h"


static void ADC_init()
{
	ADMUX = 0;
	ADCSRA = 1<<ADPS0| 1<< ADPS1	| 1<< ADPS2	| 1<<ADEN;
}
static int32_t ADC_czytaj()
{
	ADCSRA|= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	return ADC;
}

void wyswietl (int32_t in, int32_t out, double histereza)
{
	double Vmin=0;
	double Vmax=5;
	char wejscie[16], wyjscie[16], hist[16];
	double wynik=(in/1023.0)*(Vmax-Vmin)+Vmin;
	dtostrf(wynik,0,3,wejscie);
	dtostrf(out,0,3,wyjscie);
	dtostrf(histereza,0,3,hist);
	LCD_Clear();	// czyszczenie lcd
	LCD_WriteText(wejscie);
	LCD_WriteText(" V");
	LCD_GoTo(0,1);
	LCD_WriteText(wyjscie);
	LCD_WriteText(" H: ");
	LCD_WriteText(hist);
}

int main()
{
	DDRA|=(1<<PA1);
	sbi(PORTC,PC7);
	sbi(PORTC,PC6);
	int32_t in;
	int32_t out;
	ADC_init();
	LCD_Initialize();
	double histereza=0.1;
	double up = 0.5 + histereza;
	double down = 0.5 - histereza;
	while(1)
	{
		if(bit_is_clear(PINC,7))
		{
			histereza+=0.05;
		}
		if (bit_is_clear(PINC,6))
		{
			if(histereza>0)
				{
					histereza-=0.05;
				}
		}
		up = 0.5 + histereza;
		down = 0.5 - histereza;
		int wlacz=up*1023;
		int zgas=down*1023;

		in=ADC_czytaj();
		if( in > wlacz)
		{
			out=1;
			sbi(PORTA,1);
		}
		else if (in < zgas )
		{
			out=0;
			cbi(PORTA,1);
		}
		wyswietl(in,out,histereza);
		_delay_ms(1000);
	}
}