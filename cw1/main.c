/*
 * main.c
 *
 *  Created on: 2 gru 2020
 *      Author: Lukasz Chwistek
 */
#define __AVR_ATmega32__
#define F_CPU 8000000UL  //definiujemy F_CPU na 8MHz

#include <avr/io.h>
#include <util/delay.h>




int main(void){
	DDRA |= (1 << PA2) | (1 << PA3);	//piny PA2 i PA3 ustawione jako wyjscia
											//pozostale sa domyslnie wejsciami
	PORTA = 0xFF;						 	//wszystkie piny portu A ustawione jako stan wysoki


	while(1)
	{
		if(!(PINA & (1 << 0))) 		//jezeli na pinie 0 portu A wystopi 0 to
		{
			_delay_ms(500); 		//opoznienie by zniwelowac drgania stykow
			PORTA &= ~(1 << PA2);	//ustawienie wartosci 0 na pinie 2 portu A
		}

		if(!(PINA & (1 << 1)))
		{
			_delay_ms(500);
			PORTA &= ~(1 << PA3);
		}

		if(!(PINA & (1 << 0)))
		{
			_delay_ms(500);
			PORTA |= (1 << PA2); 	//ustawienie warto�ci 1 na pinie 2 portu A
		}

		if(!(PINA & (1 << 1)))
		{
			_delay_ms(500);
			PORTA |= (1 << PA3);
		}

	}

	return 0;
}
