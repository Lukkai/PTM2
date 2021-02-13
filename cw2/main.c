/*
 * Cwiczenie 2
 *
 *  Created on: 21 gru 2020
 *      Author: Lukasz Chwistek
 */
#define __AVR_ATmega32__
#define F_CPU 8000000UL  //definiujemy F_CPU na 8MHz

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>

void PWM_init()
{
	/*ustawienie PWM z nieodwróconym wyjściem, bez prescalera*/
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
	DDRB|=(1<<PB3);  /*PB3 ustawiony jako wyjscie, OC0 jako wyj�cie*/
}


void PWM_init1()
{
	/*ustawienie PWM z nieodwróconym wyjściem, bez prescalera*/
	TCCR2 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
			DDRD|=(1<<PD7);  /*PD7 ustawiony jako wyjscie, OC2 jako wyjście*/
}

int main(void){
											//pozostale bity sa domyslnie wejsciami
	PORTA |= (1<<PA0) | (1<<PA1);			//porty PA0 i PA1 ustawione jako stan wysoki
	PORTD |= (1<<PD5) | (1<<PD6);			//porty PD5 i PD6 ustawione jako stan wysoki

	unsigned char duty = 0, duty1 = 0;

	PWM_init();
	PWM_init1();

	while (1)
	{
		if (!(PIND & (1<<5))) //jezeli na pinie 5 portu D wystapi 0 to
		{
			if (duty<254)
			{
				duty++;		/*zwiekszenie intensywnosci swiatla*/
			}
			_delay_ms(50);
		}
		if (!(PIND & (1<<6)))
		{
			if(duty>1)
			{
				duty--;  /*zmniejszenie intensywnosci swiatla*/
			}
			_delay_ms(50);
		}
		OCR0=duty;  /*sterowanie przeładowaniem licznika*/

		if (!(PINA & (1<<0))) //jezeli na pinie 0 portu A wystapi 0 to
		{
			if (duty1<254)
			{
				duty1++;		/*zwiekszenie intensywnosci swiatla*/
			}
			_delay_ms(50);
		}
		if (!(PINA & (1<<1)))
		{
			if(duty1>1)
			{
				duty1--;  /*zmniejszenie intensywnosci swiatla*/
			}
			_delay_ms(50);
		}
		OCR2=duty1;  /*sterowanie przeładowaniem licznika*/


	}

	return 0;
}
