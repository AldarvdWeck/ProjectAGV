/*
Test H-brug code
*/

#define PEN1	PC7//digital 30
#define PIN11	PC6//digital 31
#define PIN12	PC5//digital 32
#define PIN21	PC4//digital 33
#define PIN22	PC3//digital 34
#define PEN2	PC2//digital 36
#define PORT    PORTC
#define DDR     DDRC


#include <math.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

int rechtspuls = 0;
volatile int i = 0;
int linkspuls = 0;
int v = 0;

void h_bridge_set_percentage(signed char percentage);
void h_bridge_set_percentage2(signed char percentage);

void init()
{
    DDR |= (1 << PEN1);
    DDR |= (1 << PIN11);
    DDR |= (1 << PIN12);
    DDR |= (1 << PIN21);
    DDR |= (1 << PIN22);
    DDR |= (1 << PEN2);

    PORT &= ~(1 << PEN1);
	PORT &= ~(1 << PIN11);
	PORT &= ~(1 << PIN12);
	PORT &= ~(1 << PIN21);
	PORT &= ~(1 << PIN22);
	PORT &= ~(1 << PEN2);


	TCCR0B = (0 << CS02) | (0 << CS01) | (1 << CS00);
    TIMSK0 = (1 << OCIE0A) | (1 << TOIE0);

    OCR0A = 0;

    sei();

    TCCR1B |= (1<<CS10);//no prescaler
    DDRD |= (1 << PD1);//trig digital pin 20

    EIMSK |= (1 << INT0);//recieve digital pin 21
    EICRA |= (1 << ISC00);

    TCCR3B |= (1<<CS30);//no prescaler
    DDRD |= (1 << PD3);//Trig digital pin 18

    EIMSK |= (1 << INT2);//recieve digital pin 19
    EICRA |= (1 << ISC20);

}

ISR(INT0_vect)
{
    if(i == 1)
    {
        TCCR1B = 0;
        linkspuls = TCNT1;
        TCNT1 = 0;
        i = 0;
    }
    if(i == 0)
    {
        TCCR1B |= (1<<CS10);
        i = 1;
    }
}

//ISR(INT2_vect)
//{
//    if(v == 1)
//    {
//        TCCR3B = 0;
//        rechtspuls = TCNT3;
//        TCNT3 = 0;
//        v = 0;
//    }
//    if(v == 0)
//    {
//        TCCR3B |= (1<<CS30);
//        v = 1;
//    }
//}

ISR(TIMER0_OVF_vect)
{
    PORT ^= (1 << PEN1);
    PORT ^= (1 << PEN2);
}

ISR(TIMER0_COMPA_vect)
{
    PORT |= (1 << PEN1);
    PORT |= (1 << PEN2);
}

int main(void)
{
    init();

    h_bridge_set_percentage(20);

    int lengterechts = 0;
    int lengtelinks = 0;

    while(1)
    {
        PORTD |= (1<<PD1);
        PORTD |= (1<<PD3);
        _delay_us(15);
        PORTD &= ~(1<<PD1);
        PORTD &= ~(1<<PD3);

        lengterechts = rechtspuls * 0.01071875;
        lengtelinks = linkspuls * 0.01071875;

        if(lengtelinks <= 30)
        {
            PORT &= ~(1 << PIN11);
            PORT |= (1 << PIN12);
            PORT &= ~(1 << PIN21);
            PORT |= (1 << PIN22);
        }
//        if(lengterechts <= 30)
//        {
//            PORT |= (1 << PIN11);
//            PORT &= ~(1 << PIN12);
//            PORT |= (1 << PIN21);
//            PORT &= ~(1 << PIN22);
//        }
//        else
//        {
//            PORT &= ~(1 << PIN11);
//            PORT |= (1 << PIN12);
//            PORT |= (1 << PIN21);
//            PORT &= ~(1 << PIN22);
//        }
    }

    return 0;
}

void h_bridge_set_percentage(signed char percentage)
{
	if (percentage >= 0 && percentage <= 100)
	{
			OCR0A = (255*percentage)/100;
	}
}
