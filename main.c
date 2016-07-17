/*
 * main.c
 *
 *  Created on: 17 lip 2016
 *      Author: Tomasz Chojak
 *
 * 7 segment LED with common annode as output at PORTC
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
 * *** PORTC as output ***
 * PC0 -> a
 * PC1 -> b
 * PC2 -> c
 * PC3 -> d
 * PC4 -> e
 * PC5 -> f
 * PC6 -> g
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
#include <util/delay.h>

#define LEDPORT PORTC //output - 7 segments LED, bits 0-6
#define LEDDDR DDRC //direction (input or output)

#define SEG_A (1<<0) //segment a as LSB of PORTC
#define SEG_B (1<<1)
#define SEG_C (1<<2)
#define SEG_D (1<<3)
#define SEG_E (1<<4)
#define SEG_F (1<<5)
#define SEG_G (1<<6)

void showOnLEDSegments(uint8_t val);

int main(void)
{
	static uint8_t DIGITS[10] = {
			~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F), 		//0
			~(SEG_B|SEG_C), 								//1
			~(SEG_A|SEG_B|SEG_G|SEG_C|SEG_D), 				//2
			~(SEG_A|SEG_B|SEG_G|SEG_C|SEG_D), 				//3
			~(SEG_F|SEG_G|SEG_B|SEG_C), 					//4
			~(SEG_A|SEG_F|SEG_G|SEG_C|SEG_D), 				//5
			~(SEG_A|SEG_F|SEG_E|SEG_D|SEG_C|SEG_G), 		//6
			~(SEG_A|SEG_B|SEG_C|SEG_F), 					//7
			~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G), 	//8
			~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G), 		//9
	};

	LEDDDR = 0xFF; //all pins of that port as output
	uint8_t toDisplay = 0;

	while (1)
	{
		showOnLEDSegments(toDisplay);
		toDisplay = (toDisplay + 1) % 10;
		_delay_ms(500);
	}


	void showOnLEDSegments(uint8_t val)
	{
		if (val < 10)
		{
			LEDPORT = DIGITS[val];
		} else
		{
			LEDPORT = 0xFF; //blank leds
		}
	}
}
