

#include "birthday.h"
#include "time.h"
#include "config.h"

#include <avr/eeprom.h>





void Birthday::Set(Time *time, unsigned char value)
{
	if (value != 0)
		value = 1;

	unsigned int addr = ((time->Month - 1) * 31) + (time->Date - 1);
	addr += BIRTHDAY_EEPROM_BASE_ADDR;

	eeprom_update_byte((uint8_t*)addr, value);
}

unsigned char Birthday::IsBirthday(Time *time)
{
	unsigned int addr= ((time->Month - 1) * 31) + (time->Date - 1);
	addr += BIRTHDAY_EEPROM_BASE_ADDR;

	return eeprom_read_byte((uint8_t*)addr) == 1;
}



void Birthday::ClearAll()
{

}
