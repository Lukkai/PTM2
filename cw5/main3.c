/*
 *  Cwiczenie 5
 *  regulator pi z wyjsciem pwm i regulowanymi nastawami
 * 
 *  Created on: 18 luty 2021
 *      Author: Lukasz Chwistek
 */



#define __AVR_ATmega32__
#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "GLOBAL.h"


const int ograniczenie = 10000;
float Kp = 1.0f;
float Ti = 1.0f;
volatile int nastawa;
volatile int uchyb;


int ADC_start(int pin)
{
	ADMUX = pin;
    ADMUX |= (1<<REFS0);                                    //napiecie referencyjne AVcc 5V
    ADMUX &= ~(1<<ADLAR);									//10bit, przesunięcie w lewo	  
	ADCSRA |= (1<<ADPS0) | (1<< ADPS1) | (1<< ADPS2) | (1<<ADEN);  //prescaler 128,  umożliwienie konwersji
    ADCSRA|= (1<<ADSC);										//start conversion
	while (ADCSRA & (1<<ADSC));								//wykonanie pomiaru
	_delay_ms(100);
    
    int analog = ADCL;
    analog += (ADCH<<8);                          //ADCL must be read first, then ADCH
    return analog;
  //return ADC;
}
/*Once ADCL is read, ADC access to Data Registers is blocked. This means that if ADCL has been read, and
a conversion completes before ADCH is read, neither register is updated and the result from the conversion is lost.
When ADCH is read, ADC access to the ADCH and ADCL Registers is re-enabled. */

void PWM_init()
{
    TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
    TCCR2 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
    DDRD|=(1<<PD7);



    /*TCCR1A |= (1<<WGM00) | (1<<WGM01) | (COM01) | (1<<CS00);
    TCCR1B |= (1<<WGM00) | (1<<WGM01) | (COM01) | (1<<CS00);
	DDRD |= (1<<DDD7);  
    TIMSK |= (1<<TOIE1)		*/		                       //PB3 ustawiony jako wyjscie, OC0 jako wyjsciowy wektor
    /*TCCR1A |= (1<<COM1A1);                     //fast PWM, wyjscie na PB, Clear OC1A on compare match, set OC1A at BOTTOM (non-inverting mode)
    TCCR1B |= (1<<CS00) | (1<<CS01);           //prescaler 64
    TIMSK |= (1<<OCIE0) | (1<<OCIE2);    */                            //aktywne przerwania

                                           //wejscia a0-a7
}

float skalowanie_regulatora(int wartosc)
{
    return ((float)wartosc)/1024;
}

int sumowanie(int wartosc)
{
    static int suma = 0;
    if(abs(suma += wartosc) > ograniczenie)
    {
        suma = ograniczenie;
    }
    return suma;
}

int nastawy_regulatora(int wartosc)
{
    int uchyb = nastawa -  wartosc;
    int wynik1 = Kp*uchyb;
    int wynik2 = Ti*sumowanie(uchyb);
    int wynik = wynik1 + wynik2;
    return wynik < 0xff ? (wynik < 0 ? 0 : wynik) : 0xff;
}

ISR(TIMER0_OVF_vect)
{
    OCR0 = nastawy_regulatora(ADC_start(PIN2));
}

int main(void)
{   
    PWM_init();
    
    sei();

    while(1)
    {
        Kp = skalowanie_regulatora(ADC_start(PIN0));
        Ti = skalowanie_regulatora(ADC_start(PIN1));

        nastawa = 20;
        _delay_ms(250);
        nastawa = 220;
        _delay_ms(250);
    }
}