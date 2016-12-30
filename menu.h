
#ifndef MENU_H
#define MENU_H

#include "display.h"
#include "time.h"

class Menu
{
public:

	Menu();

	static void Setup();


	void Update();

	unsigned char IsSelecting();

	void SetDisplay(Display *display);





private:
	unsigned int idlecounter;

	unsigned char blinkcounter;

	unsigned char state;
	unsigned char debounced;

	Time time;

	void start();
	void apply();
	void abort();

	unsigned char get_button();

	static unsigned int adc_read(unsigned char channel);
	static void adc_setup();

	static const unsigned char daysinmonth[12];
};













#endif // !MENU_H
