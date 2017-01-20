

#include "config.h"
//#include <avr/io.h>
#include <util/delay.h>
//#include <avr/interrupt.h>
//#include <avr/pgmspace.h>

//#include "tlc5940.h"
#include "animation.h"
#include "display.h"
#include "time.h"
#include "menu.h"
#include "birthday.h"







Time time;
Display display;
Menu menu;
Animation animation;




int main()
{
	Display::Setup();
	Time::SetupRTC();
	Menu::Setup();
	animation.Setup();

	display.Clear();
	display.Show();


	while (1)
	{
		/*display.Clear();

		menu.Update();

		if (menu.IsSelecting())
		{
			menu.SetDisplay(&display);
		}
		else
		{

			Time::GetCurrent(&time);

			if (Birthday::IsBirthday(&time))
			{

				display.SetRaw(DISPLAYSTATUS_ACTIVEBDAY);

				animation.Update();



			}
			else
			{
				display.SetTime(&time);
			}


		}*/

		animation.Update();

		_delay_ms(1);









		//display.Show();

		_delay_ms(30);
	}




}