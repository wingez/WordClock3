
#ifndef DS1302_H
#define DS1302_H

#include "time.h"




class DS1302
{
public:
	static void GetTime(Time *time);
	static void SetTime(Time *time);

	static void Setup();
	static unsigned char read(unsigned char address);
	static void write(unsigned char address, unsigned char value);

private:
	static void start(void);
	static void stop(void);
	static unsigned char toggleread(void);
	static void togglewrite(unsigned char data, unsigned char release);
};






#endif // !DS1302_H


