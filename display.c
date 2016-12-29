
#include "display.h"
#include "config.h"
#include <avr/io.h>




/*------------------------------------------

				Display

--------------------------------------------*/

Display::Display()
{
	data = 0UL;
}
void Display::Clear()
{
	data = 0UL;
}

void Display::SetRaw(unsigned char value)
{
	data |= (1UL << value);
}


void Display::Setup()
{
	DISPLAY_DDR |= (1 << DISPLAY_DATA) | (1 << DISPLAY_CLOCK) | (1 << DISPLAY_LATCH);

	DISPLAY_PORT &= ~((1 << DISPLAY_CLOCK) | (1 << DISPLAY_LATCH));

	unsigned long t = 0UL;

	shiftout(&t);


	DDRB |= (1 << PB2);
	PORTB &= ~(1 << PB2);




}

void Display::Show()
{
	shiftout(&data);
}

void Display::shiftout(unsigned long *val)
{
	unsigned long mask = 1;

	while (1)
	{
		if (mask& *val)
			DISPLAY_PORT |= (1 << DISPLAY_DATA);
		else
			DISPLAY_PORT &= ~(1 << DISPLAY_DATA);


		DISPLAY_PORT |= (1 << DISPLAY_CLOCK);
		DISPLAY_PORT &= ~(1 << DISPLAY_CLOCK);


		if (mask & 0x80000000)
			break;

		mask = mask << 1;


	}

	DISPLAY_PORT |= (1 << DISPLAY_LATCH);
	DISPLAY_PORT &= ~(1 << DISPLAY_LATCH);

}


















