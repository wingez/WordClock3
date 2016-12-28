
#define F_CPU 8000000UL
#include <util/delay.h>

#include "animation.h"

#include "config.h"

#include <avr/pgmspace.h>
#include <avr/io.h>

#define NUMREGISTERS TLC5940_NUMREGISTERS
#define NUMCHANNELS NUMREGISTERS*16
#define NUMLAMPS NUMCHANNELS/3



const unsigned int gammaValues[256] PROGMEM = {
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

void Animation::Setup()
{

	DDRC |= (1 << PC5) | (1 << PC4);
	PORTC &= ~((1 << PC5) | (1 << PC4));

	tlc.Setup();
	tlc.Start();
	hue = 0;
}

void Animation::Update()
{
	SetAllHSV(hue);
	tlc.Update();
	hue++;
	if (hue >= 360)
		hue = 0;
}



//void Animation::Run()
//{
//
//	
//
//
//
//	while (1)
//	{
//		unsigned int hue = 0;
//		while (hue < 360)
//		{
//			//unsigned int value = gammaValues[hue];
//			SetAllHSV(hue);
//			tlc.Update();
//			//UpdateDisplay();
//			_delay_ms(30);
//			//wait();
//			hue++;
//		}
//
//		/*
//		animation.Update();
//		_delay_ms(30);
//		*/
//
//	}
//
//
//}


void Animation::SetAllRGB(unsigned int R, unsigned int G, unsigned int B)
{
	unsigned char lampcounter = 0;
	while (1)
	{

		tlc.BrightnessValues[lampcounter++] = R;
		tlc.BrightnessValues[lampcounter++] = G;
		tlc.BrightnessValues[lampcounter++] = B;

		if (lampcounter >= NUMREGISTERS * 16 - 3)
			break;

	}


}

void Animation::SetAllHSV(unsigned int hue)
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
	unsigned int R = pgm_read_dword(&(gammaValues[r]));
	unsigned int G = pgm_read_dword(&(gammaValues[g]));
	unsigned int B = pgm_read_dword(&(gammaValues[b]));




	SetAllRGB(R, G, B);
}
















//void Animation::Setup()
//{
//	//Setup pins used for animation
//
//	//active low
//	///ANIMATION_PORT |= (1 << ANIMATION_BDAY) | (1 << ANIMATION_LDAY);
//	///ANIMATION_DDR |= (1 << ANIMATION_BDAY) | (1 << ANIMATION_LDAY);
//
//	///SetMode(ANIMATION_MODE_NONE);
//
//	DDRC |= (1 << PC5) | (1 << PC4);
//	PORTC &= ~((1 << PC5) | (1 << PC4));
//
//	tlc.Setup();
//	tlc.Start();
//
//	data = 0;
//
//}
//
//void Animation::Update()
//{
//	/*HSV hsv;
//	hsv.Hue = data;
//	SetAllHSV(hsv);
//	*/
//	
//	ASetAllHSV(data);
//
//	tlc.Update();
//
//
//	data++;
//	if (data >= 360)
//		data = 0;
//}
//
//void Animation::Start()
//{
//	tlc.Start();
//}
//
//void Animation::Stop()
//{
//	tlc.Stop();
//}
//
//////////////////////////////////
//
//void Animation::ASetAllRGB(unsigned int R, unsigned int G, unsigned int B)
//{
//	char lampcounter = 0;
//	while (1)
//	{
//
//		tlc.BrightnessValues[lampcounter++] = R;
//		tlc.BrightnessValues[lampcounter++] = G;
//		tlc.BrightnessValues[lampcounter++] = B;
//
//		if (lampcounter >= NUMREGISTERS * 16 - 3)
//			break;
//
//	}
//
//
//}
// 
//void Animation::ASetAllHSV(unsigned int hue)
//{
//	unsigned char r, g, b;
//	unsigned int accent = hue / 60;
//	unsigned char bottom = 0;
//	unsigned char top = 255;
//	unsigned char rising = (255 * (hue % 60)) / 60;
//	unsigned char falling = (255 * (60 - hue % 60)) / 60;
//
//	switch (accent)
//	{
//	case 0:
//		r = top;
//		g = rising;
//		b = bottom;
//		break;
//	case 1:
//		r = falling;
//		g = top;
//		b = bottom;
//		break;
//	case 2:
//		r = bottom;
//		g = top;
//		b = rising;
//		break;
//	case 3:
//		r = bottom;
//		g = falling;
//		b = top;
//		break;
//	case 4:
//		r = rising;
//		g = bottom;
//		b = top;
//		break;
//	case 5:
//		r = top;
//		g = bottom;
//		b = falling;
//		break;
//	}
//	unsigned int R = gammaValues[r];
//	unsigned int G = gammaValues[g];
//	unsigned int B = gammaValues[b];
//
//	ASetAllRGB(R, G, B);
//}
//
//
//
//




























//////////////////////////////////////////////

//void Animation::SetMode(AnimationMode value)
//{
//	mode = value;
//
//	unsigned char port = ANIMATION_PORT;
//	port |= (1 << ANIMATION_BDAY) | (1 << ANIMATION_LDAY);
//
//	if (ANIMATION_MODE_BDAY&value)
//		port &= ~(1 << ANIMATION_BDAY);
//	if (ANIMATION_MODE_LDAY&value)
//		port &= ~(1 << ANIMATION_LDAY);
//
//	ANIMATION_PORT = port;
//
//}
//AnimationMode Animation::GetMode()
//{
//	return mode;
//}
//
//
//
//void Animation::SetChannel(unsigned char channel, unsigned int value)
//{
//	tlc.BrightnessValues[channel] = value;
//}
//void Animation::SetLampRGB(unsigned char lamp, RGB color)
//{
//	unsigned char channel = lamp * 3;
//	tlc.BrightnessValues[channel++] = color.Red;
//	tlc.BrightnessValues[channel++] = color.Green;
//	tlc.BrightnessValues[channel] = color.Blue;
//}
//
//void Animation::SetLampHSV(unsigned char lamp, HSV hsv)
//{
//	RGB rgb = HsvToRgb(hsv);
//	SetLampRGB(lamp, rgb);
//}
//
//void Animation::SetAllChannels(unsigned int value)
//{
//	unsigned char channel = 0;
//	while (channel < NUMCHANNELS)
//	{
//		tlc.BrightnessValues[channel] = value;
//	}
//}
//void Animation::SetAllRGB(RGB color)
//{
//	unsigned char lamp = 0;
//	while (lamp < NUMLAMPS)
//	{
//		SetLampRGB(lamp, color);
//	}
//
//}
//void Animation::SetAllHSV(HSV hsv)
//{
//	RGB rgb = HsvToRgb(hsv);
//
//	SetAllRGB(rgb);
//}
//
//RGB Animation::HsvToRgb(HSV hsv)
//{
//	unsigned char r, g, b;
//	unsigned int accent = hsv.Hue / 60;
//	unsigned char bottom = 0;
//	unsigned char top = 255;
//	unsigned char rising = (255 * (hsv.Hue % 60)) / 60;
//	unsigned char falling = (255 * (60 - hsv.Hue % 60)) / 60;
//
//	switch (accent)
//	{
//	case 0:
//		r = top;
//		g = rising;
//		b = bottom;
//		break;
//	case 1:
//		r = falling;
//		g = top;
//		b = bottom;
//		break;
//	case 2:
//		r = bottom;
//		g = top;
//		b = rising;
//		break;
//	case 3:
//		r = bottom;
//		g = falling;
//		b = top;
//		break;
//	case 4:
//		r = rising;
//		g = bottom;
//		b = top;
//		break;
//	case 5:
//		r = top;
//		g = bottom;
//		b = falling;
//		break;
//	}
//
//	RGB result;
//	result.Red = gammaValues[r];
//	result.Green = gammaValues[g];
//	result.Blue = gammaValues[b];
//
//	return result;
//}
//
//
//
//







