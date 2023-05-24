/*
LCD_Naar_Locatie(x, y); verandert waar je iets neerzet op het scherm(waar de cursor staat)
Stuur_LCD_String("x"); hiermee kan je teksten neerzetten op de plek van de cursor
LCD_Scherm_Leeg(); hiermee maak je het scherm leeg
itoa(variabele, array, 10); hiermee kan je variabelen omzetten naar strings -- moet je een array voor maken -- 10 is voor decimaal
*/
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "LCDenBluetooth.h"

int main(void)
{
    init();

    LCD_Naar_Locatie(4, 1);
    Stuur_LCD_String("Begonnen");
    LCD_Naar_Locatie(4, 2);
    Stuur_LCD_String("Start");

    char array[100];

    while(1)
    {

        UDR0 = '8';

        for(int i = 0; i < 10; i++)
        {
            itoa(i, array, 10);
            LCD_Naar_Locatie(1, 1);
            Stuur_LCD_String(array);
            while((UCSR0A & (1 << TXC0)) == 0){}
            UDR0 = array;
            _delay_ms(250);
            LCD_Scherm_Leeg();
        }
    }

    return 0;
}
