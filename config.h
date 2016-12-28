
/*
+-------------------------------------------------------------------------+
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|           reset           PC6 +------------+                            |
|           TLC5940-XLAT3   PD0 |            |                            |
|           TLC5940|XLAT2   PD1 |            |                            |
|           TLC5940|XLAT1   PD2 |            |                            |
|           TLC5940|GSCLK   PD3 |            |                            |
|           TLC5940-XLAT4   PD4 | atmega328p |                            |
|                           VCC |            |                            |
|                           GND |            |                            |
|           TLC5940-SIN     PB6 |            |                            |
|           TLC5940|SCLK    PB7 |            |                            |
|           TLC5940|BLANK   PD5 |            |                            |
|           TLC5940-VPRG    PD6 |            |                            |
|                           PD7 |            |                            |
|                           PB0 +------------+                            |
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

