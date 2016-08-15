/*
 * main.c
 *
 *  Created on: 17 lip 2016
 *      Author: Tomasz Chojak
 *
 * 7 segment LED with common annode as output at PORTB
 * Remember about resistors attached to each segment.
 *
 *	*** segments scheme a-g ***
 *		 a
 * 		 --
 * 	   f|  |b
 * 	     --g
 * 	   e|  |c
 * 	     --
 * 	     d
 *
 * *** PORTB as output ***
 * PB0 -> a
 * PB1 -> b
 * PB2 -> c
 * PB3 -> d
 * PB4 -> e
 * PB5 -> f
 * PB6 -> g
 *
 * *** which segment should be on to display a digth? ***
 * 0 -> abcdef
 * 1 -> bc
 * 2 -> abged
 * 3 -> abgcd
 * 4 -> fgbc
 * 5 -> afgcd
 * 6 -> afedcg
 * 7 -> abcf
 * 8 -> abcdefg
 * 9 -> abcdfg
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LEDPORT PORTB //output - 7 segments LED, bits 0-6
#define LEDDDR DDRB //direction (input or output)
#define ANODEPORT PORTD
#define ANODEDIR DDRD
#define CA1 (1<<PD0) //bit no. 0 port D
#define CA2 (1<<PD1) //bit no. 1 port D
#define CA3 (1<<PD2) //bit no. 2 port D
#define CA4 (1<<PD3) //bit no. 3 port D

#define SEG_A (1<<0) //segment a as LSB of PORTC
#define SEG_B (1<<1)
#define SEG_C (1<<2)
#define SEG_D (1<<3)
#define SEG_E (1<<4)
#define SEG_F (1<<5)
#define SEG_G (1<<6)


volatile uint8_t toDisplay_1 = 0; // 4th digth (ones)
volatile uint8_t toDisplay_10 = 0; // 3th digth (tens)
volatile uint8_t toDisplay_100 = 0; // 2nd digth (hundreds)
volatile uint8_t toDisplay_1000 = 0; // 1st digth (thousends)

void showOnLEDSegments(uint8_t val);

static uint8_t DIGITS[10] = {
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F), 		//0
		~(SEG_B|SEG_C), 								//1
		~(SEG_A|SEG_B|SEG_G|SEG_E|SEG_D), 				//2
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_G), 				//3
		~(SEG_F|SEG_G|SEG_B|SEG_C), 					//4
		~(SEG_A|SEG_F|SEG_G|SEG_C|SEG_D), 				//5
		~(SEG_A|SEG_F|SEG_E|SEG_D|SEG_C|SEG_G), 		//6
		~(SEG_A|SEG_B|SEG_C|SEG_F), 					//7
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G), 	//8
		~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G), 		//9
};


int main(void)
{
	LEDDDR = 0xFF; //all pins of that port as output
	ANODEDIR |= CA1 | CA2 | CA3 | CA4; //bit 0,1,2 and 3 port D as output (anodes)
	ANODEPORT &= ~(CA1 | CA2 | CA3 | CA4); //turn off anodes
	uint8_t toDisplay = 0;


	//set timer0
	TCCR0A |= (1<<WGM01); //CTC timer mode
	TCCR0B |= (1<<CS00) | (1<<CS02); //set preskaler to 1024
	//we want 200 Mhz multiplexing, so math is
	//OCR0A = 8000000(clock) / 1024(preskaler) / 200(desire frequency of multipleksing) - 1 = 38
	OCR0A = 38;
	TIMSK0 |= (1<<OCIE0A); //enable interrupt compare match

	sei(); //global innteruptions on
	while (1)
	{
		toDisplay = (toDisplay + 1) % 10000;

		//here we are dissasembling whole number to single digiths
		toDisplay_1000 = toDisplay / 1000;
		toDisplay_100 = (toDisplay % 1000)/100;
		toDisplay_10 = (toDisplay % 100)/10;
		toDisplay_1 = (toDisplay % 10);
		_delay_ms(800);
	}
}

void showOnLEDSegments(uint8_t val)
{
		LEDPORT = DIGITS[val];
}

ISR(TIMER0_COMPA_vect)
{
	static uint8_t tmp = 1;
	ANODEPORT = ~tmp;

	if(tmp == 1)
	{
		showOnLEDSegments(toDisplay_1000);
	}
	else if (tmp == 2)
	{
		showOnLEDSegments(toDisplay_100);
	}
	else if (tmp == 4)
	{
		showOnLEDSegments(toDisplay_10);
	}
	else if (tmp == 8)
	{
		showOnLEDSegments(toDisplay_1);
	}

	tmp <<= 1;
	if (tmp > 8)
	{
		tmp = 1;
	}
}
