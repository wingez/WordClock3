
#ifndef BIRTHDAY_H
#define BIRTHDAY_H

#include "time.h"

class Birthday
{
public:
	static void Set(Time *time, unsigned char value);

	static void ClearAll();

	static unsigned char IsBirthday(Time *time);


};











#endif // !BIRTHDAY_H
