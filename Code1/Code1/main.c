/*
Test H-brug code
*/

#define PEN1	PC7
#define PIN11	PC6
#define PIN12	PC5
#define PIN21	PC4
#define PIN22	PC3
#define PEN2	PC2

#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

void h_bridge_set_percentage(signed char percentage);
void h_bridge_set_percentage2(signed char percentage);

void init()
{
    DDRB |= (1 << PEN1);
    DDRB |= (1 << PIN11);
    DDRB |= (1 << PIN12);
    DDRB |= (1 << PIN21);
    DDRB |= (1 << PIN22);
    DDRB |= (1 << PEN2);

    PORTB &= ~(1 << PEN1);
	PORTB &= ~(1 << PIN11);
	PORTB &= ~(1 << PIN12);
	PORTB &= ~(1 << PIN21);
	PORTB &= ~(1 << PIN22);
	PORTB &= ~(1 << PEN2);

    TCCR1B = (1 << CS12) | (0 << CS11) | (0 << CS10);
    TIMSK1 = (1 << OCIE1A) | (1 << TOIE1);

    OCR1A = 0;

    TCCR3B = (1 << CS32) | (0 << CS31) | (0 << CS30);
    TIMSK3 = (1 << OCIE3A) | (1 << TOIE3);

    OCR3A = 0;

    sei();
}

ISR(TIMER1_OVF_vect)
{
    PORTB ^= (1 << PEN1);
}

ISR(TIMER1_COMPA_vect)
{
    PORTB |= (1 << PEN1);
}

ISR(TIMER3_OVF_vect)
{
    PORTB ^= (1 << PEN2);
}

ISR(TIMER3_COMPA_vect)
{
    PORTB |= (1 << PEN2);
}

int main(void)
{
    init();

    h_bridge_set_percentage(20);
    h_bridge_set_percentage2(20);

    while(1)
    {
        DDRB &= ~(1 << PIN11);
        DDRB |= (1 << PIN12);
        DDRB &= ~(1 << PIN21);
        DDRB |= (1 << PIN22);
    }

    return 0;
}

void h_bridge_set_percentage(signed char percentage)
{
	if (percentage >= 0 && percentage <= 100)
	{
			OCR0A = (65535*percentage)/100;
	}
}

void h_bridge_set_percentage2(signed char percentage)
{
	if (percentage >= 0 && percentage <= 100)
	{
			OCR3A = (65535*percentage)/100;
	}
}
