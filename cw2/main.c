/*
 * Cwiczenie 2
 *
 *  Created on: 21 gru 2020
 *      Author: Lukasz Chwistek
 */
#define __AVR_ATmega32__
#define F_CPU 8000000UL  			//definiujemy F_CPU na 8MHz

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>

/* T = F_CPU/(freq*2*N) -1 */

void PWM_init()
{
	/*ustawienie PWM z nieodwróconym wyjściem, bez prescalera*/
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
	OCR0 = 127;						//licznik wstepnie ustawiony na polowe wypelnienia sygnalu PWM
	DDRB|=(1<<DDB3);  				//PB3 ustawiony jako wyjscie, OC0 jako wyjscie
}


void PWM_init1()
{
	/*ustawienie PWM z nieodwróconym wyjściem, bez prescalera*/
	TCCR2 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
	OCR2 = 127;
	DDRD|=(1<<DDD7);  				//PD7 ustawiony jako wyjscie, OC2 jako wyjście
}

int main(void){
									//pozostale bity sa domyslnie wejsciami
	PORTA |= (1<<PA0) | (1<<PA1);	//porty PA0 i PA1 ustawione jako stan wysoki
	PORTD |= (1<<PD5) | (1<<PD6);	//porty PD5 i PD6 ustawione jako stan wysoki
	
	PWM_init();
	PWM_init1();
	
	
	while (1)
	{
		if (!(PIND & (1<<PIN5)))	//jezeli na pinie 5 portu D wystapi 0 to
		{
			if (OCR0<250)
			{
				OCR0+=5;			//zwiekszenie intensywnosci swiatla, zwiekszenie wypełnienia pierwszego PWM
			}	
			_delay_ms(25);		
		}
		if (!(PIND & (1<<PIN6)))
		{			
			if(OCR0>5)
			{
				OCR0-=5;  			//zmniejszenie intensywnosci swiatla
			}
			_delay_ms(25);
		}

		if (!(PINA & (1<<PIN0))) 	//jezeli na pinie 0 portu A wystapi 0 to
		{
			if (OCR2<250)
			{
				OCR2+=5;			//zwiekszenie intensywnosci swiatla, zwiekszenie wypełnienia drugiego PWM
			}
			_delay_ms(25);
		}
		if (!(PINA & (1<<PIN1)))
		{
			if(OCR2>5)
			{
				OCR2-=5; 			//zmniejszenie intensywnosci swiatla
			}
			_delay_ms(25);
		}
	}
	return 0;
}
