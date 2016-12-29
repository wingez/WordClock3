
/*
+-------------------------------------------------------------------------+
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|           reset           PC6 +------------+ PC5 ANIMATE-BDAY           |
|           TLC5940-XLAT3   PD0 |            | PC4 ANIMATE-LDAY           |
|           TLC5940-XLAT2   PD1 |            | PC3 INPUT                  |
|           TLC5940-XLAT1   PD2 |            | PC2 DISPLAY-DATA           |
|           TLC5940-GSCLK   PD3 |            | PC1 DISPLAY-LATCH          |
|           TLC5940-XLAT4   PD4 | atmega328p | PC0 DISPLAY-CLOCK          |
|                           VCC |            | GND                        |
|                           GND |            | AREF                       |
|           TLC5940-SIN     PB6 |            | AVCC                       |
|           TLC5940-SCLK    PB7 |            | PB5 SPI-CLK                |
|           TLC5940-BLANK   PD5 |            | PB4 SPI-MISO               |
|           TLC5940-VPRG    PD6 |            | PB3 SPI-MOSI               |
|                           PD7 |            | PB2 DISPLAY-PWM            |
|                           PB0 +------------+ PB1                        |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
+-------------------------------------------------------------------------+
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <avr/io.h>

/*

	Display

*/

#define DISPLAY_PORT PORTC
#define DISPLAY_DDR DDRC
#define DISPLAY_CLOCK PC0
#define DISPLAY_LATCH PC1
#define DISPLAY_DATA PC2

#define DISPLAY_PWM PB2
#define DISPLAY_PWM_PORT PORTB
#define DISPLAY_PWM_DDR DDRB

#define DISPLAYDIGIT_ONE		1
#define DISPLAYDIGIT_TWO		2
#define DISPLAYDIGIT_THREE		3
#define DISPLAYDIGIT_FOUR		4
#define DISPLAYDIGIT_FIVE		5	
#define DISPLAYDIGIT_SIX		6
#define DISPLAYDIGIT_SEVEN		7
#define DISPLAYDIGIT_EIGHT		8
#define DISPLAYDIGIT_NINE		9
#define DISPLAYDIGIT_TEN		10
#define DISPLAYDIGIT_ELEVEN		11
#define DISPLAYDIGIT_TWELVE		0

#define DISPLAYTIME_FIVE		13
#define DISPLAYTIME_TEN			14
#define DISPLAYTIME_QUARTER		15
#define DISPLAYTIME_TWENTY		16
#define DISPLAYTIME_TO			17
#define DISPLAYTIME_PAST		18
#define DISPLAYTIME_HALF		19

#define DISPLAYSTATUS_CLOCK		12
#define DISPLAYSTATUS_RUNNING		
#define DISPLAYSTATUS_TIME			
#define DISPLAYSTATUS_HOUR			
#define DISPLAYSTATUS_DATE			
#define DISPLAYSTATUS_MONTH			
#define DISPLAYSTATUS_YEAR			
#define DISPLAYSTATUS_BDAY			
#define DISPLAYSTATUS_ACTIVEBDAY	











/*

	Animations

*/
#define ANIMATION_PORT PORTC
#define ANIMATION_DDR DDRC
#define ANIMATION_BDAY PC5
#define ANIMATION_LDAY PC4




/*

	TLC5940

*/

#define TLC5940_NUMREGISTERS 1


//VPRG
#define TLC5940_VPRG PD6
#define TLC5940_VPRG_PORT PORTD
#define TLC5940_VPRG_DDR DDRD

//SIN
#define TLC5940_SIN PB6
#define TLC5940_SIN_PORT PORTB
#define TLC5940_SIN_DDR DDRB

//SCLK
#define TLC5940_SCLK PB7
#define TLC5940_SCLK_PORT PORTB
#define TLC5940_SCLK_DDR DDRB

//BLANK
#define TLC5940_BLANK PD5
#define TLC5940_BLANK_PORT PORTD
#define TLC5940_BLANK_DDR DDRD

//GSCLK
//gsclk is fixed on PD3 due to using a timer
#define TLC5940_GSCLK PD3
#define TLC5940_GSCLK_PORT PORTD
#define TLC5940_GSCLK_DDR DDRD

//XLAT
//all xlats has to use the same port for performance reasons
#define TLC5940_XLAT1 PD2
#define TLC5940_XLAT2 PD1
#define TLC5940_XLAT3 PD0
#define TLC5940_XLAT4 PD4
#define TLC5940_XLAT_PORT PORTD
#define TLC5940_XLAT_DDR DDRD













#endif // !CONFIG_H

