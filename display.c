
#include "display.h"
#include "config.h"
#include "gamma.h"
#include "time.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>



const unsigned char Display::digits[12] = {
	DISPLAYDIGIT_ONE,DISPLAYDIGIT_TWO,DISPLAYDIGIT_THREE,DISPLAYDIGIT_FOUR,
	DISPLAYDIGIT_FIVE,DISPLAYDIGIT_SIX,DISPLAYDIGIT_SEVEN, DISPLAYDIGIT_EIGHT,
	DISPLAYDIGIT_NINE,DISPLAYDIGIT_TEN,DISPLAYDIGIT_ELEVEN,DISPLAYDIGIT_TWELVE
};











/*------------------------------------------

				Display

--------------------------------------------*/

unsigned char Display::brightness = 0;


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




	loadBrightness();





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

//1-31
void Display::SetNumber(unsigned char number)
{
	if (number == 0 || number > 31)
		return;

	//1-12
	if (number < 13)
		SetDigit(number);
	//10-19
	else if (number < 20)
	{
		SetRaw(DISPLAYTIME_TEN);
		SetDigit(number - 10);
	}
	//20-29
	else if (number < 30)
	{
		SetRaw(DISPLAYTIME_TWENTY);
		SetDigit(number - 20);
	}
	//30
	else if (number == 30)
	{
		SetDigit(3);
		SetDigit(10);
	}
	else if (number == 31)
	{
		SetDigit(3);
		SetDigit(10);
		SetDigit(1);
	}
}


void Display::SetMinute(unsigned char minute)
{
	unsigned char clock = minute / 5;

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
		break;
	case 6:
		SetRaw(DISPLAYTIME_HALF);
		break;
	case 7:
		SetRaw(DISPLAYTIME_FIVE);
		SetRaw(DISPLAYTIME_PAST);
		SetRaw(DISPLAYTIME_HALF);
		break;
	case 8:
		SetRaw(DISPLAYTIME_TWENTY);
		SetRaw(DISPLAYTIME_TO);
		break;
	case 9:
		SetRaw(DISPLAYTIME_QUARTER);
		SetRaw(DISPLAYTIME_TO);
		break;
	case 10:
		SetRaw(DISPLAYTIME_TEN);
		SetRaw(DISPLAYTIME_TO);
		break;
	case 11:
		SetRaw(DISPLAYTIME_FIVE);
		SetRaw(DISPLAYTIME_TO);
		break;
	default:
		break;
	}





}
void Display::SetHour(unsigned char hour)
{
	//Reduse number of hours from 0-24 to 0-12
	if (hour > 12)
		hour -= 12;


	//Display 12 instead of 0
	if (hour == 0)
		hour = 12;




	SetDigit(hour);
}



void Display::SetTime(Time *time)
{
	//Show "It's olock"
	SetRaw(DISPLAYSTATUS_CLOCK);

	unsigned char minute = time->Minute;
	unsigned char hour = time->Hour;

	if (minute >= 25)
		hour++;



	//Show hour
	SetHour(hour);

	//Show minute
	SetMinute(minute);

	/*
		unsigned char hour = time->Hour;

	unsigned char clock = time->Minute / 5;*/




	//SetDigit(hour);
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

void Display::SaveBrightness()
{
	//unsigned char addr = /*DISPLAY_EEPROM_BRIGHTNESS_ADDR*/;
	eeprom_write_byte((uint8_t*)DISPLAY_EEPROM_BRIGHTNESS_ADDR, brightness);
}
void Display::loadBrightness()
{
	//unsigned char addr = /*DISPLAY_EEPROM_BRIGHTNESS_ADDR*/;
	unsigned char val = eeprom_read_byte((uint8_t*)DISPLAY_EEPROM_BRIGHTNESS_ADDR);
	if (val > BRIGHTNESS_MAX)
		val = BRIGHTNESS_DEFAULT;

	brightness = val;
	updateBrightness();
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