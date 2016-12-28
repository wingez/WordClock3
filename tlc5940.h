
#ifndef TLC5940_H
#define TLC5940_H

#include "config.h"

extern volatile char needXLAT;


extern const char xlatPins[4];


class TLC5940
{
public:
	unsigned int BrightnessValues[TLC5940_NUMREGISTERS*16];

	void Setup();

	void Start();
	void Stop();

	void Update();

private:
	void SetDOT();
};





#endif // !TLC5940_H


