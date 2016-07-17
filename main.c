/*
 * main.c
 *
 *  Created on: 17 lip 2016
 *      Author: Tomasz Chojak
 *
 * 7 segment LED with common annode as output at PORTC
 * Remember about resistors attached to all segments.
 *
 *	*** segments a-g ***
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

#define SEG_A (1<<0) //segment a as LSB of PORTC
#define SEG_B (1<<1)
#define SEG_C (1<<2)
#define SEG_D (1<<3)
#define SEG_E (1<<4)
#define SEG_F (1<<5)
#define SEG_G (1<<6)

int main(void)
{
	PORTC = 0xFF; //set PORTC as output - leds
}
