
#include "ds1302.h"
#include "config.h"

#include <avr/io.h>
#include <util/delay.h>

//#define DS1302_SCLK_PIN   7    // Arduino pin for the Serial Clock
//#define DS1302_IO_PIN     6    // Arduino pin for the Data I/O
//#define DS1302_CE_PIN     5    // Arduino pin for the Chip Enable
//
//#define PORT PORTD
//#define DDR DDRD
//#define ENABLE PD5
//#define DATA PD6
//#define CLOCK PD7
//
//#define bcd2bin(h,l)    (((h)*10) + (l))
//#define bin2bcd_h(x)   ((x)/10)
//#define bin2bcd_l(x)    ((x)%10)

//#define DS1302_SECONDS           0x80
//#define DS1302_MINUTES           0x82
//#define DS1302_HOURS             0x84
//#define DS1302_DATE              0x86
//#define DS1302_MONTH             0x88
//#define DS1302_DAY               0x8A
//#define DS1302_YEAR              0x8C
//#define DS1302_ENABLE            0x8E
//#define DS1302_TRICKLE           0x90
//#define DS1302_CLOCK_BURST       0xBE
//#define DS1302_CLOCK_BURST_WRITE 0xBE
//#define DS1302_CLOCK_BURST_READ  0xBF
//#define DS1302_RAMSTART          0xC0
//#define DS1302_RAMEND            0xFC
//#define DS1302_RAM_BURST         0xFE
//#define DS1302_RAM_BURST_WRITE   0xFE
//#define DS1302_RAM_BURST_READ    0xFF


#define DS1302_SECONDS           0x80
#define DS1302_MINUTES           0x82
#define DS1302_HOURS             0x84
#define DS1302_DATE              0x86
#define DS1302_MONTH             0x88
//#define DS1302_DAY               0x8A
#define DS1302_YEAR              0x8C
#define DS1302_ENABLE            0x8E
#define DS1302_TRICKLE           0x90



#define DS1302_D0 0
#define DS1302_D1 1
#define DS1302_D2 2
#define DS1302_D3 3
#define DS1302_D4 4
#define DS1302_D5 5
#define DS1302_D6 6
#define DS1302_D7 7


// Bit for reading (bit in address)
#define DS1302_READBIT DS1302_D0 // READBIT=1: read instruction

// Bit for clock (0) or ram (1) area, 
// called R/C-bit (bit in address)
#define DS1302_RC DS1302_D6

// Seconds Register
#define DS1302_CH DS1302_D7   // 1 = Clock Halt, 0 = start

// Hour Register
#define DS1302_AM_PM DS1302_D5 // 0 = AM, 1 = PM
#define DS1302_12_24 DS1302_D7 // 0 = 24 hour, 1 = 12 hour

// Enable Register
#define DS1302_WP DS1302_D7   // 1 = Write Protect, 0 = enabled


void DS1302::Setup()
{


	/*Serial.begin(9600);
	Serial.println(F("DS1302 Real Time Clock"));
	Serial.println(F("Version 2, March 2013"));*/


	// Start by clearing the Write Protect bit
	// Otherwise the clock data cannot be written
	// The whole register is written, 
	// but the WP-bit is the only bit in that register.
	write(DS1302_ENABLE, 0);

	// Disable Trickle Charger.
	write(DS1302_TRICKLE, 0x00);

	//write(DS1302_SECONDS, 0x00);
}

void DS1302::GetTime(DateTime *time)
{
	//Year
	unsigned int year = read(DS1302_YEAR);
	unsigned char month = read(DS1302_MONTH);
	unsigned char date = read(DS1302_DATE);
	unsigned char hour = read(DS1302_HOURS);
	unsigned char minute = read(DS1302_MINUTES);
	unsigned char seconds = read(DS1302_SECONDS);

	time->Year = 10 * ((year & 0xf0) >> 4) + (year & 0x0f);

	if (month & 0x10)
		time->Month = 10 + (month & 0x0f);
	else
		time->Month = month;

	time->Date = 10 * ((date & 0xf0) >> 4) + (date & 0x0f);

	time->Hour = 10 * ((hour & 0x30) >> 4) + (hour & 0x0f);

	time->Minute = 10 * ((minute & 0x70) >> 4) + (minute & 0x0f);

	time->Second = 10 * ((seconds & 0x70) >> 4) + (seconds & 0x0f);

}

void DS1302::SetTime(DateTime *time)
{
	write(DS1302_YEAR, ((time->Year / 10) << 4) | (time->Year % 10));
	write(DS1302_MONTH, ((time->Month / 10) << 4) | (time->Month % 10));
	write(DS1302_DATE, ((time->Date / 10) << 4) | (time->Date % 10));
	write(DS1302_HOURS, ((time->Hour / 10) << 4) | (time->Hour % 10));
	write(DS1302_MINUTES, ((time->Minute / 10) << 4) | (time->Minute % 10));
	write(DS1302_SECONDS, ((time->Second / 10) << 4) | (time->Second % 10));





}



//
//void loop()
//{
//
//
//	unsigned char val = DS1302_read(DS1302_SECONDS);
//
//	Serial.println(val, BIN);
//
//
//	//ds1302_struct rtc;
//	//char buffer[80];     // the code uses 70 characters.
//
//	//					 // Read all clock data at once (burst mode).
//	//DS1302_clock_burst_read((uint8_t *)&rtc);
//
//	//sprintf(buffer, "Time = %02d:%02d:%02d, ", \
//					//	bcd2bin(rtc.h24.Hour10, rtc.h24.Hour), \
//	//	bcd2bin(rtc.Minutes10, rtc.Minutes), \
//	//	bcd2bin(rtc.Seconds10, rtc.Seconds));
////Serial.print(buffer);
//
////sprintf(buffer, "Date(day of month) = %d, Month = %d, " \
//	//	"Day(day of week) = %d, Year = %d", \
//	//	bcd2bin(rtc.Date10, rtc.Date), \
//	//	bcd2bin(rtc.Month10, rtc.Month), \
//	//	rtc.Day, \
//	//	2000 + bcd2bin(rtc.Year10, rtc.Year));
////Serial.println(buffer);
//
//	delay(1000);
//}

unsigned char DS1302::read(unsigned char address)
{
	unsigned char data;

	// set lowest bit (read bit) in address
	/*bitSet(address, DS1302_READBIT);*/
	address |= (1 << DS1302_READBIT);

	start();
	// the I/O-line is released for the data
	togglewrite(address, 1);
	data = toggleread();
	stop();

	return (data);
}


void DS1302::write(unsigned char address, unsigned char data)
{
	// clear lowest bit (read bit) in address
	//bitClear(address, DS1302_READBIT);
	address &= ~(1 << DS1302_READBIT);

	start();
	// don't release the I/O-line
	togglewrite(address, 0);
	// don't release the I/O-line
	togglewrite(data, 0);
	stop();
}

void DS1302::start(void)
{
	DS1302_CE_PORT &= ~(1 << DS1302_CE);
	DS1302_CE_DDR |= (1 << DS1302_CE);

	DS1302_SCLK_PORT &= ~(1 << DS1302_SCLK);
	DS1302_SCLK_DDR |= (1 << DS1302_SCLK);

	DS1302_IO_DDR |= (1 << DS1302_IO);

	DS1302_CE_PORT |= (1 << DS1302_CE);
	_delay_us(4);


	//digitalWrite(DS1302_CE_PIN, LOW); // default, not enabled
	//pinMode(DS1302_CE_PIN, OUTPUT);

	//digitalWrite(DS1302_SCLK_PIN, LOW); // default, clock low
	//pinMode(DS1302_SCLK_PIN, OUTPUT);

	//pinMode(DS1302_IO_PIN, OUTPUT);

	//digitalWrite(DS1302_CE_PIN, HIGH); // start the session
	//delayMicroseconds(4);           // tCC = 4us


}

void DS1302::stop(void)
{
	DS1302_CE_PORT &= ~(1 << DS1302_CE);
	_delay_us(4);


	//// Set CE low
	//digitalWrite(DS1302_CE_PIN, LOW);

	//delayMicroseconds(4);           // tCWH = 4us
}


unsigned char DS1302::toggleread(void)
{
	unsigned char i, data;

	data = 0;
	for (i = 0; i <= 7; i++)
	{
		// Issue a clock pulse for the next databit.
		// If the 'togglewrite' function was used before 
		// this function, the SCLK is already high.
		DS1302_SCLK_PORT |= (1 << DS1302_SCLK);
		_delay_us(1);
		//digitalWrite(DS1302_SCLK_PIN, HIGH);
		//delayMicroseconds(1);

		// Clock down, data is ready after some time.


		DS1302_SCLK_PORT &= ~(1 << DS1302_SCLK);
		_delay_us(1);
		//digitalWrite(DS1302_SCLK_PIN, LOW);
		//delayMicroseconds(1);        // tCL=1000ns, tCDD=800ns

		// read bit, and set it in place in 'data' variable

		if (DS1302_IO_PIN&(1 << DS1302_IO))
			data |= (1 << i);

		//bitWrite(data, i, digitalRead(DS1302_IO_PIN));
	}
	return(data);
}


// --------------------------------------------------------
// _DS1302_togglewrite
//
// A helper function for writing a byte with bit toggle
//
// The 'release' parameter is for a read after this write.
// It will release the I/O-line and will keep the SCLK high.
//
void DS1302::togglewrite(unsigned char data, unsigned char release)
{
	unsigned char i;

	for (i = 0; i <= 7; i++)
	{
		// set a bit of the data on the I/O-line

		if (data & (1 << i))
			DS1302_IO_PORT |= (1 << DS1302_IO);
		else
			DS1302_IO_PORT &= ~(1 << DS1302_IO);
		_delay_us(1);

		//digitalWrite(DS1302_IO_PIN, bitRead(data, i));
		//delayMicroseconds(1);     // tDC = 200ns



		DS1302_SCLK_PORT |= (1 << DS1302_SCLK);
		_delay_us(1);

		// clock up, data is read by DS1302
		//digitalWrite(DS1302_SCLK_PIN, HIGH);
		//delayMicroseconds(1);     // tCH = 1000ns, tCDH = 800ns

		if (release && i == 7)
		{
			// If this write is followed by a read, 
			// the I/O-line should be released after 
			// the last bit, before the clock line is made low.
			// This is according the datasheet.
			// I have seen other programs that don't release 
			// the I/O-line at this moment,
			// and that could cause a shortcut spike 
			// on the I/O-line.
			DS1302_IO_DDR &= ~(1 << DS1302_IO);
			//pinMode(DS1302_IO_PIN, INPUT);

			// For Arduino 1.0.3, removing the pull-up is no longer needed.
			// Setting the pin as 'INPUT' will already remove the pull-up.
			// digitalWrite (DS1302_IO, LOW); // remove any pull-up  
		}
		else
		{
			DS1302_SCLK_PORT &= ~(1 << DS1302_SCLK);
			_delay_us(1);

			//digitalWrite(DS1302_SCLK_PIN, LOW);
			//delayMicroseconds(1);       // tCL=1000ns, tCDD=800ns
		}
	}
}