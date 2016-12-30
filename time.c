
#include "time.h"
#include "string.h"
#include "ds1302.h"



Time::Time()
{

}

void Time::GetCurrent(Time *time)
{
	DS1302::GetTime(time);
}


void Time::SetCurrent(Time *time)
{
	DS1302::SetTime(time);
}

void Time::SetupRTC()
{
	DS1302::Setup();
}



