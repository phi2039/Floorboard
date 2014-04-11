// ---------------------------------------------------------------------------
// Created by Chris Lance 04/03/2014
// Copyright 2014 - Under creative commons license 3.0:
//        Attribution-ShareAlike CC BY-SA
//
// This software is furnished "as is", without technical support, and with no 
// warranty, express or implied, as to its usefulness for any purpose.
//
// @file LiquidCrystal_CAL.h
//  Connects an LCD using 2 or 3 pins from the Arduino, via an 8-bit 
// ShiftRegister (SR from now on).
// 
// @brief 
// This is a library for the use of an HD44780-compatible LCD via
// a (non-)latching shift register (e.g. 74LS164)
//
// Limited LCD Pinout:
//    1.  Ground
//    2.  VCC (+3.3 to +5V)
//    3.  Contrast adjustment (VO)
//    4.  Register Select (RS). RS=0: Command, RS=1: Data
//    5.  Read/Write (R/W). R/W=0: Write, R/W=1: Read (In general use, this pin will be permanently connected directly to ground.)
//    6.  Clock (Enable). Falling edge triggered
//    7.  Bit 0 (Not used in 4-bit operation)
//    8.  Bit 1 (Not used in 4-bit operation)
//    9.  Bit 2 (Not used in 4-bit operation)
//    10. Bit 3 (Not used in 4-bit operation)
//    11. Bit 4
//    12. Bit 5
//    13. Bit 6
//    14. Bit 7
//    15. Backlight Anode (+) (If applicable)
//    16. Backlight Cathode (-) (If applicable)
//
// SR output:
// Bit  #0   - N/C - not connected, used to hold a zero
// Bit  #1   - N/C
// Bit  #2   - connects to RS (Register Select) on the LCD
// Bits #3-6 - connects to LCD data inputs D4 - D7.
// Bit  #7   - enables the LCD enable-puls (via the diode-resistor AND "gate")
//
// 2 or 3 Pins required from the Arduino for Data, Clock and (optional) Enable
// If not using Enable, the Data pin is used for the enable signal by defining
// the same pin for Enable as for Data. Data and Clock outputs/pins goes to
// the shiftregister.
// LCD RW-pin hardwired to LOW (only writing to LCD).
// Busy Flag (BF, data bit D7) is not read.
//
//  Original project homepage: http://code.google.com/p/arduinoshiftreglcd/

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <inttypes.h>
#include <SPI.h>

#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif


#include "LiquidCrystal_CAL.h"
#include <Time.h>

// CONSTRUCTORS
// ---------------------------------------------------------------------------
// Assuming 1 line 8 pixel high font
LiquidCrystal_CAL::LiquidCrystal_CAL (BusControl& busControl, int busAddress) :
  m_BusControl(busControl),
  m_BusAddress(busAddress)
{
  init();
}

// PRIVATE METHODS
// ---------------------------------------------------------------------------

//
// init
void LiquidCrystal_CAL::init()
{
   _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
}

//
// shiftIt
void LiquidCrystal_CAL::shiftIt(uint8_t val)
{
  m_BusControl.SelectSlave(m_BusAddress);
  SPI.transfer(val);
   
  // LCD ENABLE PULSE
  m_BusControl.SelectSlave(BUS_SLAVE_NONE);
}

// PUBLIC METHODS
// ---------------------------------------------------------------------------


/************ low level data pushing commands **********/
//
// send
void LiquidCrystal_CAL::send(uint8_t value, uint8_t mode)
{
   // Divide byte in two nibbles include the RS signal
   // and format it for shiftregister output wiring to the LCD
   // We are only interested in my COMMAND or DATA for myMode
   uint8_t myMode = ( mode == DATA ) ? SR_RS_BIT : 0; // RS bit; LOW: command.  HIGH: character.
   
   if ( mode != FOUR_BITS )
   {
      shiftIt(myMode | SR_EN_BIT | ((value >> 1) & 0x78)); // upper nibble
   }

   shiftIt(myMode | SR_EN_BIT | ((value << 3) & 0x78)); // lower nibble
   /*
    * Add some delay since this code is so fast it needs some added delay
    * even on AVRs because the shiftout is shorter than the LCD command execution time.
    */
#if (F_CPU <= 16000000)
 	delayMicroseconds ( 17 ); // 3 wire mode is faster so it must delay longer
#else
   delayMicroseconds ( 37 );      // commands & data writes need > 37us to complete
#endif

}

//
// setBacklightPin
void LiquidCrystal_CAL::setBacklightPin ( uint8_t pin, t_backlighPol pol )
{ }

//
// setBacklight
void LiquidCrystal_CAL::setBacklight ( uint8_t mode ) 
{ }

void LiquidCrystal_CAL::PrintClockDigits(int digits)
{
  // utility function for digital clock display: prints preceding colon and leading 0
  print(":");
  if(digits < 10)
    print('0');
  print(digits);
}

void LiquidCrystal_CAL::PrintClock(int row, int col)
{
  setCursor (col, row);
  // digital clock display of the time
  print(hour());
  PrintClockDigits(minute());
  PrintClockDigits(second());
  print(" ");
  print(day());
  print(" ");
  print(month());
  print(" ");
  print(year());   
}
