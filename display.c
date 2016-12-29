
#include "display.h"
#include "config.h"
#include "gamma.h"

#include <avr/io.h>
#include <avr/interrupt.h>
















/*------------------------------------------

				Display

--------------------------------------------*/

unsigned char Display::brightness = BRIGHTNESS_DEFAULT;


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

	
	updateBrightness();


	DDRB |= (1 << PB2);
	// PD6 is now an output

	

	TCCR1A |= (1 << COM1B1) | (1 << COM1B0);
	// set none-inverting mode

	TCCR1A |= (1 << WGM11) | (1 << WGM10);
	// set fast PWM Mode

	TCCR1B |= (1 << CS11);
	// set prescaler to 8 and starts PWM
}

void Display::updateBrightness()
{
	OCR1B = Gamma10b10s(brightness);
}

void Display::IncreaseBrightness()
{
	if (brightness < BRIGHTNESS_DEFAULT)
	{
		brightness++;
		updateBrightness();
	}
}
void Display::DecreaseBrightness()
{
	if (brightness > 0)
	{
		brightness--;
		updateBrightness();
	}
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


















