

#include "config.h"
#include "menu.h"
#include "time.h"
#include "display.h"
#include "birthday.h"

#define BUTTON_NONE 0
#define BUTTON_MODE 1
#define BUTTON_INCREASE 2
#define BUTTON_DECREASE 3



#define adc_threshhold(v,c) ((v>c-MENU_ADC_THRESHHOLD)&&(v<c+MENU_ADC_THRESHHOLD))

const unsigned char Menu::daysinmonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };


Menu::Menu()
{
	idlecounter = 0;
	state = 0;
	debounced = 0;
	blinkcounter = 0;
}


void Menu::Setup()
{
	adc_setup();
}


void Menu::Update()
{
	blinkcounter++;




	unsigned char button = get_button();


	//If no button is pressed
	if (button == BUTTON_NONE)
	{
		//increase but dont overflow
		if (idlecounter < 0xffff)
			idlecounter++;

		//If no button has been pressed for X up that means button is debounced and we can detect next rising edge
		if (idlecounter > MENU_BUTTONDEBOUNCECOUNT)
			debounced = 1;
	}
	else
	{
		//Reset and wait for al buttons to be released X updates
		idlecounter = 0;
	}


	if (button != BUTTON_NONE && debounced)
	{
		//Keep button and clear dedounsed so we dont detect it nest update
		debounced = 0;
	}
	else
	{
		//Remove button since we cannot asume it is debounced yet
		button = BUTTON_NONE;
	}



	if (state && (idlecounter > MENU_IDLEUPDATES))
	{
		//If no buttons has been pressed for a while exit the menu and return to normal
		abort();

	}



	//button variable now only contain data if it is debounced and on the rising edge and we can safely use it


	if (button == BUTTON_MODE)
	{
		//Continue to next mode
		state += 1;

		switch (state)
		{
		case 1:
			//Entering menu
			start();
			break;

		case (MENUSTATE_COUNT):
			//Leaving menu
			apply();
			state = 0;
			break;

		default:
			break;
		}
	}

	if (button == BUTTON_INCREASE)
	{

		switch (state)
		{
		case MENUSTATE_BRIGHTNESS:

			Display::IncreaseBrightness();

			break;


		case MENUSTATE_TIME:
			//Add 5 min and round down to nearest 5-min period
			time.Minute = (time.Minute / 5 + 1) * 5;
			if (time.Minute >= 60)
				time.Minute -= 60;
			break;


		case MENUSTATE_HOUR:
			time.Hour++;
			if (time.Hour == 24)
				time.Hour = 0;

			break;


		case MENUSTATE_YEAR:
			time.Year++;
			if (time.Year == 254)
				time.Year = 1;
			break;


		case MENUSTATE_MONTH:
			time.Month++;
			if (time.Month == 13)
				time.Month = 1;

			if (time.Date > daysinmonth[time.Month - 1])
				time.Date = daysinmonth[time.Month - 1];

			break;


		case MENUSTATE_DATE:
			time.Date++;
			if (time.Date > daysinmonth[time.Month - 1])
				time.Date = 1;
			
			break;


		case MENUSTATE_BDAY:
			Birthday::Set(&time, 1);

			break;


		default:
			break;
		}

	}


	if (button == BUTTON_DECREASE)
	{


		switch (state)
		{

		case MENUSTATE_BRIGHTNESS:

			Display::DecreaseBrightness();

			break;


		case MENUSTATE_TIME:
			//Reduce 5 min and round down to nearest 5-min period
			if (time.Minute < 5)
				time.Minute = 60;

			time.Minute = (time.Minute / 5 - 1) * 5;

			break;


		case MENUSTATE_HOUR:

			if (time.Hour == 0)
				time.Hour = 24;

				time.Hour--;

			break;


		case MENUSTATE_YEAR:

			if (time.Year == 1)
				time.Year = 254;
			time.Year--;

			break;


		case MENUSTATE_MONTH:

			if (time.Month == 1)
				time.Month = 13;

			time.Month--;

			if (time.Date > daysinmonth[time.Month - 1])
				time.Date = daysinmonth[time.Month - 1];

			break;


		case MENUSTATE_DATE:
			if (time.Date == 1)
				time.Date = daysinmonth[time.Month - 1];
			else
				time.Date--;

			break;

		case MENUSTATE_BDAY:
			Birthday::Set(&time, 0);

		default:
			break;
		}
	}




}


void Menu::SetDisplay(Display *display)
{




	switch (state)
	{
	case MENUSTATE_BRIGHTNESS:
		display->SetRaw(DISPLAYSTATUS_BRIGHTNESS);
		//Show some random text to let the user decide what brightness to use
		display->SetRaw(DISPLAYTIME_TWENTY);
		display->SetRaw(DISPLAYTIME_FIVE);
		display->SetRaw(DISPLAYTIME_HALF);
		display->SetRaw(DISPLAYDIGIT_FOUR);
		display->SetRaw(DISPLAYDIGIT_ELEVEN);
		display->SetRaw(DISPLAYDIGIT_TWELVE);
		display->SetRaw(DISPLAYDIGIT_TWO);
		display->SetRaw(DISPLAYDIGIT_TEN);
		display->SetRaw(DISPLAYDIGIT_FIVE);
		display->SetRaw(DISPLAYDIGIT_ONE);
		break;

	case MENUSTATE_TIME:
		display->SetRaw(DISPLAYSTATUS_TIME);

		display->SetMinute(time.Minute);

		break;


	case MENUSTATE_HOUR:
		display->SetRaw(DISPLAYSTATUS_HOUR);

		if (time.Hour == 0)
			display->SetNumber(24);
		else
			display->SetNumber(time.Hour);

		break;

	case MENUSTATE_YEAR:
		display->SetRaw(DISPLAYSTATUS_YEAR);

		if (time.Year < 32)
			display->SetNumber(time.Year);
		else
		{	//TODO


		}

		break;

	case MENUSTATE_MONTH:
		display->SetRaw(DISPLAYSTATUS_MONTH);

		display->SetNumber(time.Month);

		break;

	case MENUSTATE_DATE:
		display->SetRaw(DISPLAYSTATUS_DATE);

		display->SetNumber(time.Date);

		break;

	case MENUSTATE_BDAY:
		display->SetRaw(DISPLAYSTATUS_BDAY);

		if (Birthday::IsBirthday(&time))
			display->SetRaw(DISPLAYSTATUS_ACTIVEBDAY);

		break;
	default:
		break;
	}

	/*
		if (state)
		{
			display->SetNumber(state);
		}
	*/

}




void Menu::start()
{
	Time::GetCurrent(&time);
	blinkcounter++;
}

void Menu::apply()
{
	time.Second = 1;

	Time::SetCurrent(&time);
	Display::SaveBrightness();
}

void Menu::abort()
{
	//Reset Menu and rturn to normal display mode
	state = 0;
}

unsigned char Menu::IsSelecting()
{
	return state;
}




unsigned char Menu::get_button()
{
	unsigned int adcval = adc_read(MENU_ADC_CHANNEL);

	if (adc_threshhold(adcval, MENU_BUTTON_MODE_VAL))
		return BUTTON_MODE;

	if (adc_threshhold(adcval, MENU_BUTTON_INCREASE_VAL))
		return BUTTON_INCREASE;

	if (adc_threshhold(adcval, MENU_BUTTON_DECREASE_VAL))
		return BUTTON_DECREASE;

	return BUTTON_NONE;
}




void Menu::adc_setup()
{
	// AREF = AVcc
	ADMUX = (1 << REFS0);

	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}


unsigned int Menu::adc_read(unsigned char channel)
{
	// select the corresponding channel 0~7
	// ANDing with ’7′ will always keep the value
	// of ‘ch’ between 0 and 7
	channel &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8) | channel; // clears the bottom 3 bits before ORing

								 // start single convertion
								 // write ’1′ to ADSC
	ADCSRA |= (1 << ADSC);

	// wait for conversion to complete
	// ADSC becomes ’0′ again
	// till then, run loop continuously
	while (ADCSRA & (1 << ADSC));

	return (ADC);
}