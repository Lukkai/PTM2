/*
 * main.c
 *
 *  Created on: 2 gru 2020
 *      Author: Lukasz Chwistek
 */
#define __AVR_ATmega32__
#define F_CPU 8000000UL  //definiujemy F_CPU na 8MHz

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

/* 
 * PINx		- rejestr zwracajacy stan pinow
 * PORTx	- rejestr do ustawiania stanów na pinach
 * DDRx		- rejestr ustalajacy kierunek pracy 0 - wejscie, 1 - wyjscie
 */

int main(void){
	DDRA |= (1 << PA2) | (1 << PA3);	//piny PA2 i PA3 ustawione jako wyjscia
											//pozostale sa domyslnie wejsciami
	PORTA = 0xFF;						 	//wszystkie piny portu A ustawione jako stan wysoki

	uint8_t led1 = 0, led2 = 0;
	
	while(1)
	{
		

		if(!(PINA & (1 << PINA0))) 		//jezeli na pinie 0 portu A wystopi 0 to
		{
			_delay_ms(200); 		//opoznienie by zniwelowac drgania stykow
			if(0 == led1)
			{
				PORTA &= ~(1 << PA2);	//ustawienie wartosci 0 na pinie 2 portu A, wlaczenie diody
				led1=1;	
			}
			else
			{
				PORTA |= (1 << PA2); //ustawienie wartosci 1 na pinie 2 portu A, czyli wylaczenie diody
				led1 = 0;
			}
			while(!(PINA & (1 << PINA0))){}
		}
		

		if(!(PINA & (1 << PINA1)))
		{
			_delay_ms(200);
			if(0 == led2)
			{
				PORTA &= ~(1 << PA3);
				led2 = 1;
			}
			else
			{
				PORTA |= (1 << PA3);
				led2 = 0;
			}
			while(!(PINA & (1 << PINA1))){}
		}

	}

	return 0;
}
