
#ifndef DISPLAY_H
#define DISPLAY_H

#define BRIGHTNESS_DEFAULT 8
#define BRIGHTNESS_MAX 9

class Display
{
public:


	Display();


	void SetRaw(unsigned char value);
	void Clear();

	void Show();



	static void Setup();
	static void IncreaseBrightness();
	static void DecreaseBrightness();

private:
	unsigned long data;

	static void shiftout(unsigned long *val);

	static unsigned char brightness;
	static void updateBrightness();

};

#endif // !DISPLAY_H

