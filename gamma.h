
#ifndef GAMMA_H
#define GAMMA_H

#include <avr/pgmspace.h>

const unsigned int _gamma10b10s[10] PROGMEM =
{
	1,3,16,48,106,198,329,507,736,1023
};

unsigned int Gamma10b10s(unsigned char value)
{
	return pgm_read_dword(&(_gamma10b10s[value]));
}












#endif // !GAMMA_H



