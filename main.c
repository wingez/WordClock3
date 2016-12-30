

#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

//#include "tlc5940.h"
//#include "animation.h"

#include "display.h"
#include "time.h"
#include "menu.h"
//#include "ds1302.h"

Display display;
Menu menu;











int main()
{
	Display::Setup();
	//DS1302::Setup();
	Time::SetupRTC();
	Menu::Setup();

	Time time;

	time.Year = 16;
	time.Month = 12;
	time.Date = 3;
	time.Hour = 1;
	time.Minute = 26;
	time.Second = 30;

	Time::SetCurrent(&time);



	while (1)
	{
		Time t;
		Time::GetCurrent(&t);


		display.Clear();



		menu.Update();


		if (menu.IsSelecting())
		{
			menu.SetDisplay(&display);
		}
		else
		{
			display.SetTime(&t);

		}





		display.Show();

		_delay_ms(30);
	}




}