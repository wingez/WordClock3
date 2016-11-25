#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

const unsigned int gammaValues[256]  = {
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2
	, 2, 3, 3, 3, 4, 4, 5, 5, 6, 7, 7, 8, 9, 10, 11, 12
	, 13, 14, 15, 16, 18, 19, 20, 22, 23, 25, 27, 29, 30, 32, 34, 36
	, 39, 41, 43, 46, 48, 51, 54, 56, 59, 62, 65, 68, 72, 75, 79, 82
	, 86, 90, 94, 98, 102, 106, 110, 115, 119, 124, 129, 134, 139, 144, 149, 154
	, 160, 166, 171, 177, 183, 189, 196, 202, 209, 215, 222, 229, 236, 244, 251, 258
	, 266, 274, 282, 290, 298, 307, 315, 324, 333, 342, 351, 360, 370, 380, 389, 399
	, 410, 420, 430, 441, 452, 463, 474, 485, 497, 508, 520, 532, 544, 557, 569, 582
	, 595, 608, 621, 635, 648, 662, 676, 691, 705, 720, 734, 749, 765, 780, 795, 811
	, 827, 843, 860, 876, 893, 910, 927, 945, 962, 980, 998, 1016, 1035, 1054, 1072, 1092
	, 1111, 1130, 1150, 1170, 1190, 1211, 1231, 1252, 1273, 1295, 1316, 1338, 1360, 1382, 1405, 1428
	, 1451, 1474, 1497, 1521, 1545, 1569, 1593, 1618, 1643, 1668, 1693, 1719, 1745, 1771, 1797, 1824
	, 1851, 1878, 1905, 1933, 1961, 1989, 2017, 2046, 2075, 2104, 2133, 2163, 2193, 2223, 2254, 2284
	, 2315, 2347, 2378, 2410, 2442, 2475, 2507, 2540, 2573, 2607, 2641, 2675, 2709, 2744, 2778, 2814
	, 2849, 2885, 2921, 2957, 2994, 3031, 3068, 3105, 3143, 3181, 3220, 3258, 3297, 3337, 3376, 3416
	, 3456, 3497, 3537, 3578, 3620, 3662, 3704, 3746, 3788, 3831, 3875, 3918, 3962, 4006, 4051, 4095
};

#define PORT PORTD
#define DDR DDRD

#define VPRG	0b00000001
#define SIN		0b00000010
#define SCLK	0b00000100
#define GSCLK	0b00001000
#define XLAT	0b00010000
#define BLANK	0b00100000



volatile char needXLAT = 0;

unsigned int values[16];

inline void BlinkRedLamp()
{
	DDRC |= 0b00100000;
	PORTC |= 0b00100000;
	_delay_ms(100);
	PORTC &= ~0b00100000;
	_delay_ms(100);
	PORTC |= 0b00100000;
	_delay_ms(100);
	PORTC &= ~0b00100000;
}

void SetAll(unsigned int value)
{
	char i = 0;
	while (i < 16)
	{
		values[i] = value;
		i++;
	}
}

void SetLamp(char lamp, unsigned int value)
{
	values[lamp] = value;
}

void SetAllRGB(unsigned int R, unsigned int G, unsigned int B)
{
	char i = 0;
	char remainder;
	while (i < 15)
	{
		remainder = i % 3;
		if (remainder == 0)
			values[i] = R;
		else if (remainder == 1)
			values[i] = G;
		else
			values[i] = B;

		i++;
	}
}
void SetAllHSV(unsigned int hue)
{
	unsigned char r, g, b;
	unsigned int accent = hue / 60;
	unsigned char bottom = 0;
	unsigned char top = 255;
	unsigned char rising = (255 * (hue % 60)) / 60;
	unsigned char falling = (255 * (60 - hue % 60)) / 60;

	switch (accent)
	{
	case 0:
		r = top;
		g = rising;
		b = bottom;
		break;
	case 1:
		r = falling;
		g = top;
		b = bottom;
		break;
	case 2:
		r = bottom;
		g = top;
		b = rising;
		break;
	case 3:
		r = bottom;
		g = falling;
		b = top;
		break;
	case 4:
		r = rising;
		g = bottom;
		b = top;
		break;
	case 5:
		r = top;
		g = bottom;
		b = falling;
		break;
	}
	unsigned int R = gammaValues[r];
	unsigned int G = gammaValues[g];
	unsigned int B = gammaValues[b];

	SetAllRGB(R, G, B);
}

void UpdateDisplay()
{
	char lampcounter = 15;
	unsigned int lampvalue;
	char bit;

	PORT &= ~VPRG;

	while (1)
	{
		lampvalue = values[lampcounter];
		bit = 0;
		while (bit < 12)
		{
			if (lampvalue & 0x800)
				PORT |= SIN;
			else
				PORT &= ~SIN;


			PORT |= SCLK;
			PORT &= ~SCLK;

			lampvalue = lampvalue << 1;
			bit++;
		}

		if (lampcounter == 0)
			break;
		lampcounter--;
	}
	needXLAT = 1;
	while (needXLAT) {}
}

void SetDOT()
{
	PORT |= VPRG;
	PORT |= SIN;
	char counter = 0;
	while (counter < 96)
	{
		PORT |= SCLK;
		PORT &= ~SCLK;
		counter++;
	}

	needXLAT = 1;
	while (needXLAT) {}
}


int main()
{
	cli();

	DDR |= (VPRG | SIN | SCLK | GSCLK | XLAT | BLANK);

	PORT |= BLANK;


	TCCR2A = 0b00010010;
	TCCR2B = 0b00000001;

	TCCR1A = 0b00000000;
	TCCR1B = 0b00001100;
	TIMSK1 = 0b00000010;
	OCR1A = 31;

	sei();

	SetDOT();
	SetAll(0x0);

	BlinkRedLamp();


	/*while (1)
	{
		unsigned char i = 0;
		while (1)
		{
			values[0] = gammaValues[i];
			UpdateDisplay();
			_delay_ms(1);


			if (i == 0xff)
				break;
			i++;
		}




	}

*/

	while (1)
	{
		unsigned int hue = 0;
		while (hue < 360)
		{
			SetAllHSV(hue);
			UpdateDisplay();
			_delay_ms(30);


			hue++;
		}



	}

}



ISR(TIMER1_OVF_vect) {}
ISR(TIMER1_COMPB_vect) {}
ISR(TIMER1_COMPA_vect)
{
	PORT |= BLANK;

	if (needXLAT)
	{
		PORT |= XLAT;
		PORT &= ~XLAT;
		PORT |= SCLK;
		PORT &= ~SCLK;
		needXLAT = 0;
	}

	PORT &= (~BLANK);

}