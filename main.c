#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

//#include "tlc5940.h"
#include "animation.h"

Animation animation;









int main()
{

	animation.Setup();

	while (1)
	{
		animation.Update();
		_delay_ms(30);
	}


}