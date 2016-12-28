#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "tlc5940.h"

TLC5940 tlc;



#define TLC_PORT PORTD
#define TLC_DATAPORT PORTB
#define TLC_DDR DDRD
#define TLC_DATADDR DDRB

#define VPRG	PD6
#define SIN		PB6
#define SCLK	PB7
#define BLANK	PD5

#define GSCLK	PD3

#define XLAT1	PD2
#define XLAT2	PD1
#define XLAT3	PD0
#define XLAT4	PD4

//const char xlatPins[4] = { (1 << XLAT1),(1 << XLAT2),0,0 };



#define NUM_REGISTERS 1


const unsigned int gammaValues[256] = {
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





//volatile char needXLAT = 0;
//unsigned int brightnessValues[16 * NUM_REGISTERS];

//
//void SetAll(unsigned int value)
//{
//	char counter = 0;
//	while (counter < NUM_REGISTERS * 16)
//	{
//		brightnessValues[counter] = value;
//		counter++;
//	}
//}
//
//void SetLamp(char lamp, unsigned int value)
//{
//	brightnessValues[lamp] = value;
//}

void SetAllRGB(unsigned int R, unsigned int G, unsigned int B)
{
	char lampcounter = 0;
	while (1)
	{
		
		tlc.BrightnessValues[lampcounter++] = R;
		tlc.BrightnessValues[lampcounter++] = G;
		tlc.BrightnessValues[lampcounter++] = B;

		if (lampcounter >= NUM_REGISTERS * 16 - 3)
			break;

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




//void UpdateDisplay()
//{
//	char chipCounter = 0;
//
//	char lampcounter;
//
//	unsigned int lampvalue;
//	unsigned int mask;
//
//	TLC_PORT &= ~(1 << VPRG);
//
//	while (chipCounter < NUM_REGISTERS)
//	{
//		lampcounter = chipCounter * 16 + 15;

//		while (1)
//		{
//			lampvalue = brightnessValues[lampcounter];
//			mask = 0x800;
//			while (1)
//			{
//				if (lampvalue & mask)
//					TLC_DATAPORT |= (1 << SIN);
//				else
//					TLC_DATAPORT &= ~(1 << SIN);
//
//				TLC_DATAPORT |= (1 << SCLK);
//				TLC_DATAPORT &= ~(1 << SCLK);
//
//				mask = mask >> 1;
//
//				if (mask == 0)
//					break;
//			}
//
//			if (lampcounter == lastLamp)
//				break;
//			lampcounter--;
//
//		}
//		needXLAT = xlatPins[chipCounter];
//		while (needXLAT) {}
//
//		chipCounter++;
//	}
//
//
//}


//void SetDOT()
//{
//	TLC_PORT |= (1 << VPRG);
//	TLC_DATAPORT |= (1 << SIN);
//	char counter;
//	char chipcounter = 0;
//	while (chipcounter < NUM_REGISTERS)
//	{
//		counter = 0;
//
//		while (counter < (16 * 6))
//		{
//			TLC_DATAPORT |= (1 << SCLK);
//			TLC_DATAPORT &= ~(1 << SCLK);
//			counter++;
//		}
//
//		needXLAT = xlatPins[chipcounter];
//		while (needXLAT) {}
//
//		chipcounter++;
//	}
//
//}


int main()
{
	/*cli();

	TLC_DDR |= (1 << VPRG) | (1 << GSCLK) | (1 << BLANK);


	char xlatcounter = NUM_REGISTERS;
	do
	{
		xlatcounter--;
		TLC_DDR |= (xlatPins[xlatcounter]);
	} while (xlatcounter);

	TLC_DATADDR |= (1 << SIN) | (1 << SCLK);


	TLC_PORT |= (1 << BLANK);


	TCCR2A = 0b00010010;
	TCCR2B = 0b00000001;

	TCCR1A = 0b00000000;
	TCCR1B = 0b00001100;
	TIMSK1 = 0b00000010;
	OCR1A = 31;

	sei();

	TLC_PORT &= ~(1 << BLANK);








	SetDOT();
	SetAll(0);
	UpdateDisplay();
*/

	DDRC |= (1 << PC5) | (1 << PC4);
	PORTC &= ~((1 << PC5) | (1 << PC4));

	tlc.Setup();


	while (1)
	{
		unsigned int hue = 0;
		while (hue < 360)
		{
			//unsigned int value = gammaValues[hue];
			SetAllHSV(hue);
			tlc.Update();
			//UpdateDisplay();
			_delay_ms(30);
			//wait();
			hue++;
		}


	}

}

//
//
//ISR(TIMER1_OVF_vect) {}
//ISR(TIMER1_COMPB_vect) {}
//ISR(TIMER1_COMPA_vect)
//{
//	TLC_PORT |= (1 << BLANK);
//
//	if (needXLAT)
//	{
//		TLC_PORT |= needXLAT;
//		TLC_PORT &= ~needXLAT;
//		TLC_DATAPORT |= (1 << SCLK);
//		TLC_DATAPORT &= ~(1 << SCLK);
//		needXLAT = 0;
//	}
//
//	TLC_PORT &= ~(1 << BLANK);
//
//}