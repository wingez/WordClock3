

#include "tlc5940.h"
#include "config.h"
#include <avr/interrupt.h>
#include <avr/io.h>

volatile  char needXLAT = 0;


//unsigned int TLC5940::BrighnessValues[TLC5940_NUMREGISTERS * 16];

const char /*TLC5940::*/xlatPins[4] = {
	(1 << TLC5940_XLAT1),
	(1 << TLC5940_XLAT2),
	(1 << TLC5940_XLAT3),
	(1 << TLC5940_XLAT4) };

void TLC5940::Start()
{
	cli();
	//Setup timer2 to toggle OC2B each clockcycle. At8MHz this gives a 4MHz square wave
	TCCR2A = 0b00010010;
	TCCR2B = 0b00000001;

	//Setup timer1 to call an interupt every 8192 clockcycles. 8192 clockcycles is equal to 4096 puleson OC2B frpm timer 2
	TCCR1A = 0b00000000;
	TCCR1B = 0b00001100;
	TIMSK1 = 0b00000010;
	OCR1A = 31;
	TLC5940_BLANK_PORT &= ~(1 << TLC5940_BLANK);

	sei();
}

void TLC5940::Stop()
{
	cli();


	//Setup timer2 to toggle OC2B each clockcycle. At8MHz this gives a 4MHz square wave
	TCCR2A = 0;
	TCCR2B = 0;

	//Setup timer1 to call an interupt every 8192 clockcycles. 8192 clockcycles is equal to 4096 puleson OC2B frpm timer 2
	TCCR1A = 0;
	TCCR1B = 0;
	TIMSK1 = 0;
	OCR1A = 0;
	TLC5940_BLANK_PORT |= (1 << TLC5940_BLANK);

	sei();
}



void TLC5940::Setup()
{
	cli();

	TLC5940_VPRG_DDR |= (1 << TLC5940_VPRG);
	TLC5940_SIN_DDR |= (1 << TLC5940_SIN);
	TLC5940_SCLK_DDR |= (1 << TLC5940_SCLK);
	TLC5940_BLANK_DDR |= (1 << TLC5940_BLANK);
	TLC5940_GSCLK_DDR |= (1 << TLC5940_GSCLK);

	TLC5940_BLANK_PORT |= (1 << TLC5940_BLANK);

	char xlatcounter = TLC5940_NUMREGISTERS;
	do
	{
		xlatcounter--;
		TLC5940_XLAT_DDR |= (xlatPins[xlatcounter]);
	} while (xlatcounter);


	//Setup timer2 to toggle OC2B each clockcycle. At8MHz this gives a 4MHz square wave
	TCCR2A = 0b00010010;
	TCCR2B = 0b00000001;

	//Setup timer1 to call an interupt every 8192 clockcycles. 8192 clockcycles is equal to 4096 puleson OC2B frpm timer 2
	TCCR1A = 0b00000000;
	TCCR1B = 0b00001100;
	TIMSK1 = 0b00000010;
	OCR1A = 31;

	sei();


	SetDOT();

	TLC5940_BLANK_PORT &= ~(1 << TLC5940_BLANK);

	char counter = 0;
	while (counter < TLC5940_NUMREGISTERS * 16)
	{
		BrightnessValues[counter] = 0;
		counter++;
	}

	Update();

	Stop();
}

void TLC5940::Update()
{
	char chipCounter = 0;

	char lampcounter;

	unsigned int lampvalue;
	unsigned int mask;

	TLC5940_VPRG_PORT &= ~(1 << TLC5940_VPRG);

	while (chipCounter < TLC5940_NUMREGISTERS)
	{
		lampcounter = chipCounter * 16 + 15;
		char lastLamp = chipCounter * 16;
		while (1)
		{
			lampvalue = BrightnessValues[lampcounter];
			mask = 0x800;
			while (1)
			{
				if (lampvalue & mask)
					TLC5940_SIN_PORT |= (1 << TLC5940_SIN);
				else
					TLC5940_SIN_PORT &= ~(1 << TLC5940_SIN);

				TLC5940_SCLK_PORT |= (1 << TLC5940_SCLK);
				TLC5940_SCLK_PORT &= ~(1 << TLC5940_SCLK);

				mask = mask >> 1;

				if (mask == 0)
					break;
			}

			if (lampcounter == lastLamp)
				break;
			lampcounter--;

		}

		//Wait for data to be latched in
		needXLAT = xlatPins[chipCounter];
		while (needXLAT) {}

		chipCounter++;
	}

}

void TLC5940::SetDOT()
{
	TLC5940_VPRG_PORT |= (1 << TLC5940_VPRG);
	TLC5940_SIN_PORT |= (1 << TLC5940_SIN);
	char counter;
	char chipcounter = 0;
	while (chipcounter < TLC5940_NUMREGISTERS)
	{
		counter = 0;

		while (counter < (16 * 6))
		{
			TLC5940_SCLK_PORT |= (1 << TLC5940_SCLK);
			TLC5940_SCLK_PORT &= ~(1 << TLC5940_SCLK);
			counter++;
		}

		needXLAT = xlatPins[chipcounter];
		while (needXLAT) {}

		chipcounter++;
	}



}



ISR(TIMER1_OVF_vect) {}
ISR(TIMER1_COMPB_vect) {}
ISR(TIMER1_COMPA_vect)
{
	TLC5940_BLANK_PORT |= (1 << TLC5940_BLANK);

	if (needXLAT)
	{
		TLC5940_XLAT_PORT |= needXLAT;
		TLC5940_XLAT_PORT &= ~needXLAT;
		TLC5940_SCLK_PORT |= (1 << TLC5940_SCLK);
		TLC5940_SCLK_PORT &= ~(1 << TLC5940_SCLK);
		needXLAT = 0;
	}

	TLC5940_BLANK_PORT &= ~(1 << TLC5940_BLANK);
}