
#include <avr/pgmspace.h>
#include "gamma.h"


const unsigned int Gamma::_gammab10s10[10] PROGMEM =
{
	500,1023,16,48,106,198,329,507,736,1023
};

unsigned int Gamma::b10s10(unsigned char value)
{
	return pgm_read_dword(&(_gammab10s10[value]));
}