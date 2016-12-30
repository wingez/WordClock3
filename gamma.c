
#include <avr/pgmspace.h>
#include "gamma.h"


const unsigned int Gamma::_gammab10s10[10] PROGMEM =
{
	50,100,150,220,329,480,620,736,880,1023
};

unsigned int Gamma::b10s10(unsigned char value)
{
	return pgm_read_dword(&(_gammab10s10[value]));
}