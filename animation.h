
#ifndef ANIMATION_H
#define ANIMATION_H

#define ANIMATION_MODE_NONE 0x0
#define ANIMATION_MODE_BDAY 0x1
#define ANIMATION_MODE_LDAY 0x2



#include "tlc5940.h"


typedef char AnimationMode;

struct RGB
{
public:
	unsigned int Red;
	unsigned int Blue;
	unsigned int Green;
};


struct HSV
{
public:
	unsigned int Hue;

};

struct RainbowData
{
	unsigned int hue;
};

union AnimationData
{
	RainbowData rainbow;
};






class Animation
{
public:

	void Setup();

	void Update();

	//void Start();
	//void Stop();

	////TODO
	////void SetAnimation();



	//void SetMode(AnimationMode mode);
	//AnimationMode GetMode();

private:

	TLC5940 tlc;

	void SetAllRGB(unsigned int R, unsigned int G, unsigned int B);
	void SetAllHSV(unsigned int hue);

	void SetLampRGB(unsigned char lamp, RGB* value);

	void HueToRGB(unsigned int hue,RGB* color);

	AnimationData data;

	/*TLC5940 tlc;
	AnimationMode mode;


	void SetChannel(unsigned char channel, unsigned int value);
	void SetLampRGB(unsigned char lamp, RGB color);
	void SetLampHSV(unsigned char lamp, HSV hsv);


	void SetAllChannels(unsigned int value);
	void SetAllRGB(RGB color);
	void SetAllHSV(HSV hsv);

	RGB HsvToRgb(HSV  hsv);

	void ASetAllHSV(unsigned int hue);
	void ASetAllRGB(unsigned int R, unsigned int G, unsigned int B);*/
};






#endif // !ANIMATION_H





