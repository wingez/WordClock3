
#include "display.h"
#include "config.h"
#include "gamma.h"
#include "time.h"

#include <avr/io.h>
#include <avr/interrupt.h>




const unsigned char Display::digits[12] = {
	DISPLAYDIGIT_ONE,DISPLAYDIGIT_TWO,DISPLAYDIGIT_THREE,DISPLAYDIGIT_FOUR,
	DISPLAYDIGIT_FIVE,DISPLAYDIGIT_SIX,DISPLAYDIGIT_SEVEN, DISPLAYDIGIT_EIGHT,
	DISPLAYDIGIT_NINE,DISPLAYDIGIT_TEN,DISPLAYDIGIT_ELEVEN,DISPLAYDIGIT_TWELVE
};











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
	data = 0x00000000UL;
}

void Display::Setup()
{
	DISPLAY_DDR |= (1 << DISPLAY_DATA) | (1 << DISPLAY_CLOCK) | (1 << DISPLAY_LATCH);

	DISPLAY_PORT &= ~((1 << DISPLAY_CLOCK) | (1 << DISPLAY_LATCH));

	unsigned long t = 0UL;

	shiftout(&t);


	updateBrightness();

	//Set PWM output
	DISPLAY_PWM_DDR |= (1 << DISPLAY_PWM);

	TCCR1A |= (1 << COM1B1) | (1 << COM1B0);
	// set none-inverting mode

	TCCR1A |= (1 << WGM11) | (1 << WGM10);
	// set fast PWM Mode

	TCCR1B |= (1 << CS11);
	// set prescaler to 8 and starts PWM
}


void Display::SetRaw(unsigned char value)
{
	data |= (1UL << value);
}

void Display::SetDigit(unsigned char digit)
{
	if (digit == 0 || digit > 12)
		return;

	digit--;


	SetRaw(digits[digit]);

}

void Display::SetTime(Time *time)
{
	unsigned char hour = time->Hour;

	unsigned char clock = time->Minute / 5;

	//Show "It's olock"
	SetRaw(DISPLAYSTATUS_CLOCK);

	switch (clock)
	{
	case 0:
		break;
	case 1:
		SetRaw(DISPLAYTIME_FIVE);
		SetRaw(DISPLAYTIME_PAST);
		break;
	case 2:
		SetRaw(DISPLAYTIME_TEN);
		SetRaw(DISPLAYTIME_PAST);
		break;
	case 3:
		SetRaw(DISPLAYTIME_QUARTER);
		SetRaw(DISPLAYTIME_PAST);
		break;
	case 4:
		SetRaw(DISPLAYTIME_TWENTY);
		SetRaw(DISPLAYTIME_PAST);
		break;
	case 5:
		SetRaw(DISPLAYTIME_FIVE);
		SetRaw(DISPLAYTIME_TO);
		SetRaw(DISPLAYTIME_HALF);
		hour++;
		break;
	case 6:
		SetRaw(DISPLAYTIME_HALF);
		hour++;
		break;
	case 7:
		SetRaw(DISPLAYTIME_FIVE);
		SetRaw(DISPLAYTIME_PAST);
		SetRaw(DISPLAYTIME_HALF);
		hour++;
		break;
	case 8:
		SetRaw(DISPLAYTIME_TWENTY);
		SetRaw(DISPLAYTIME_TO);
		hour++;
		break;
	case 9:
		SetRaw(DISPLAYTIME_QUARTER);
		SetRaw(DISPLAYTIME_TO);
		hour++;
		break;
	case 10:
		SetRaw(DISPLAYTIME_TEN);
		SetRaw(DISPLAYTIME_TO);
		hour++;
		break;
	case 11:
		SetRaw(DISPLAYTIME_FIVE);
		SetRaw(DISPLAYTIME_TO);
		hour++;
		break;
	default:
		break;
	}


	//Reduse number of hours from 0-24 to 0-12
	if (hour > 12)
		hour-=12;


	//Display 12 instead of 0
	if (hour == 0)
		hour = 12;

	SetDigit(hour);
}





void Display::updateBrightness()
{
	OCR1B = Gamma::b10s10(brightness);
}

void Display::IncreaseBrightness()
{
	if (brightness < BRIGHTNESS_MAX)
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