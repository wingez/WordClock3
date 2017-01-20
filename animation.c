
#define F_CPU 8000000UL
#include <util/delay.h>

#include "animation.h"
#include "config.h"
#include "gamma.h"

#include <avr/io.h>

#define NUMREGISTERS TLC5940_NUMREGISTERS
#define NUMCHANNELS NUMREGISTERS*16
#define NUMLAMPS NUMCHANNELS/3





void Animation::Setup()
{
	_delay_ms(1);


	tlc.Setup();
	DDRC |= (1 << PC3) ;
	PORTC &= ~((1 << PC3));

	tlc.Start();


	data.rainbow.hue = 0;
}

void Animation::Update()
{

	unsigned char lamp=0;
	while(lamp<21)
	{
		unsigned int hue = data.rainbow.hue;
		
		hue+=lamp*10;
		
		hue%=360;

		RGB color;
		HueToRGB(hue, &color);

		SetLampRGB(lamp,&color);

		lamp++;
	}




	//SetAllHSV(data.rainbow.hue);
	tlc.Update();
	data.rainbow.hue+=2;
	if (data.rainbow.hue >= 360)
		data.rainbow.hue -= 360;

	//unsigned char a=0;
	//while(a<4*16)
	//{
	//	tlc.BrightnessValues[a++]=0x100;
	//}
	//tlc.Update();
	
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

void Animation::SetLampRGB(unsigned char lamp, RGB* color)
{
	unsigned char counter=lamp*3;
	tlc.BrightnessValues[counter++]=color->Red;
	tlc.BrightnessValues[counter++]=color->Green;
	tlc.BrightnessValues[counter]=color->Blue;
}

void Animation::HueToRGB(unsigned int hue,RGB* color)
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
	default:
		r=0;
		g=0;
		b=0;
		break;
	}


	color->Red = Gamma::b12s256(r);
	color->Green = Gamma::b12s256(g);
	color->Blue = Gamma::b12s256(b);
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
	unsigned int R = Gamma::b12s256(r);
	unsigned int G = Gamma::b12s256(g);
	unsigned int B = Gamma::b12s256(b);




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







