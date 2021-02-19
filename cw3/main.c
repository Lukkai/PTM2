/*
 *  Cwiczenie 3
 * 
 *  Created on: 21 stycznia 2011
 *      Author: Lukasz Chwistek
 */

#define __AVR_ATmega32__
#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

/* timer_period = F_CPU/(freq_out*2*N) -1 */

void PWM_init()
{
	/*ustawienie PWM z nieodwróconym wyjściem, prescaler 8 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);   //rejestr sterujący licznikiem, fastPWM, prescaler 8
	OCR0 = 0;						                           //wypelnienie sygnalu PWM
	DDRB |= (1<<DDB3);  				                       //PB3 ustawiony jako wyjscie, OC0 jako wyjsciowy wektor
    TIMSK = (1<<TOIE0);                                        //włączenie przerwania na timerze0
}

uint8_t sin_wave[0xFF];
uint8_t T = 0;

/*Przerwanie z przepełnienia timera0*/
ISR(TIMER0_OVF_vect)
{
    OCR0 = sin_wave[T];
    T++;
}

int main(void)
{
    PWM_init();
    
    //parametry sinusa i jego wartosci
    const float freq_out = 1.0f / 256.0f;                      //czestotliosc wyjsciowa sinusa aby dla jednego okresu miec 256 przerwan PWM
    const float omega = freq_out * 2 * M_PI;                   //obliczenie pulsacji
    uint8_t t = 0;
    while (t < 255)
    {
        sin_wave[t] = (sin(omega * t) * 128 + 127);              //generowany sinus przeniesiony o 127 i przeskalowany do wartosci -128 - 128
        t++;
    }

    sei();                                                     // uruchomienie przerwan

    while(1);                                                  //pętla programu
}


/* dla wygenerowania sygnału sinusoidalnego o fsin = 15Hz wykorzystujemy filtr RC, gdzie R = 1500kOhm i C=640uF
   dla wygenerowania sygnału prostokatnego przy przykladowym R = 270kOhm to C = 23uF */