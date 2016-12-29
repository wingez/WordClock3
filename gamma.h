
#ifndef GAMMA_H
#define GAMMA_H

#include <avr/pgmspace.h>

const unsigned int _gamma10b10s[10] PROGMEM =
{
	100,200,300,400,500,600,700,800,900,1000
}

unsigned int Gamma10b10s(unsigned char value)
{
	return pgm_read_dword(&(_gamma10b10s[value]));
}












#endif // !GAMMA_H



