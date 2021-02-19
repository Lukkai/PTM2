/*
 *  Cwiczenie 4
 *  regulator dwustawny z histerezą oraz wyswietlanie stanu na LCD
 * 
 *  Created on: 14 luty 2021
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
	ADMUX = (1<<REFS0);											  //napiecie referencyjne AVcc 5V
	ADCSRA = (1<<ADPS0) | (1<< ADPS1) | (1<< ADPS2) | (1<<ADEN);  //prescaler 128,  enable conversion
}

static uint16_t ADC_10bit()
{
	ADCSRA|= (1<<ADSC);										//start conversion
	while (ADCSRA & (1<<ADSC));								//wykonanie pomiaru
	return ADC;
}

void write (int16_t in, int16_t out, float histeresis)	//wypisywanie na wyswietlaczu
{
	float Vmin=0.0f;
	float Vmax=5.0f;
	char input[16], output[16], hist[16];
	float value=((float)in/1024.0f)*(Vmax-Vmin)+Vmin;
	dtostrf(value,0,3,input);
	dtostrf(out,0,3,output);
	dtostrf(histeresis,0,3,hist);
	LCD_Clear();
	LCD_WriteText(input);
	LCD_WriteText(" V");
	LCD_GoTo(0,1);
	LCD_WriteText(output);
	LCD_WriteText(" H: ");
	LCD_WriteText(hist);
}

int main()
{
	DDRD|=(1<<DDD0);
	PORTC |= (1 << PC6) | (1 << PC7);
	int16_t in;
	int16_t out;
	ADC_init();
	LCD_Initialize();
	const float setpoint = 0.5f;
	volatile float histeresis = 0.1f;
	float on = setpoint + histeresis;
	float off = setpoint - histeresis;
	while(1)
	{
		if(!(PINC & (1<<PIN7)))
		{
			histeresis+=0.05f;
		}
		if (!(PINC & (1<<PIN6)))
		{
			if(histeresis>0)
			{
				histeresis-=0.05f;
			}
		}
		on = setpoint + histeresis;
		off = setpoint - histeresis;

		int16_t outon = on*1024;
		int16_t outoff = off*1024;

		in=ADC_10bit();
		if(in > outon)
		{
			out=1;
			PORTD |= (1 << PD0);
		}
		else if (in < outoff)
		{
			out=0;
			PORTD &= ~(1 << PD0);
		}
		write(in,out,histeresis);
		_delay_ms(1000);
	}
}