#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

//#include "tlc5940.h"
#include "animation.h"
#include "config.h"

#include "display.h"


Display display;





//Animation animation;


//unsigned long data = 0xff000001;

//
//void Clock()
//{
//	PORTC |= (1 << PC0);
//	_delay_ms(1);
//	PORTC &= ~(1 << PC0);
//}
//void Latch()
//{
//	PORTC |= (1 << PC1);
//	_delay_ms(1);
//	PORTC &= ~(1 << PC1);
//}
//
//
//
//
//void Shiftout()
//{
//	unsigned long mask = 1;
//
//	while (1)
//	{
//		if (mask&data)
//			PORTC |= (1 << PC2);
//		else
//			PORTC &= ~(1 << PC2);
//
//		_delay_ms(1);
//		Clock();
//
//		_delay_ms(1);
//
//		if (mask & 0x80000000)
//			break;
//
//		mask = mask << 1;
//
//
//	}
//
//
//	Latch();
//
//
//}
//









int main()
{


	Display::Setup();
	display.Clear();
	display.SetRaw(9);
	display.Show();


	unsigned int counter = 0;
	while (1)
	{
		counter = 0;
		while (counter < 12)
		{
			counter++;
			Display::IncreaseBrightness();
			_delay_ms(300);
		}

		counter = 0;
		while (counter < 12)
		{
			counter++;
			Display::DecreaseBrightness();
			_delay_ms(300);
		}
	}





	/*unsigned char counter = 0;
	while (++counter<32)
	{
		display.Clear();
		display.SetRaw(counter);
		display.Show();
		_delay_ms(6000);
	}
*/











//DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2);

//PORTC &= ~(1 << PC2);

/*unsigned char counter = 0;
while (counter < 32)
{
	Clock();
	Latch();


	counter++;
}
*/




//DDRB |= (1 << PB2);
//PORTB &= ~(1 << PB2);

//Shiftout();
//while (1)
//{

//}












//PORTC |= (1 << PC2);
//Clock();
//Latch();
//PORTC &= ~(1 << PC2);

//while (1)
//{
//	Clock();
//	Latch();
//	_delay_ms(1000);
//}




/*
	animation.Setup();

	while (1)
	{
		animation.Update();
		_delay_ms(30);
	}
*/

}