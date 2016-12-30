
#ifndef TIME_H
#define TIME_H






class Time
{
public:
	unsigned char Second;
	unsigned char Minute;
	unsigned char Hour;
	unsigned char Date;
	unsigned char Month;
	unsigned int Year;

	static void GetCurrent(Time *time);
	static void SetCurrent(Time *time);

	static void SetupRTC();

};







#endif // !TIME_H

