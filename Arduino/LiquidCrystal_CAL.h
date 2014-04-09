// ---------------------------------------------------------------------------
// Created by Chris Lance 04/03/2014
// Copyright 2014 - Under creative commons license 3.0:
//        Attribution-ShareAlike CC BY-SA
//
// This software is furnished "as is", without technical support, and with no 
// warranty, express or implied, as to its usefulness for any purpose.

#ifndef _LIQUIDCRYSTAL_CAL_
#define _LIQUIDCRYSTAL_CAL_

#include <inttypes.h>
#include <LCD.h>

#include "BusControl.h"

// two-wire indicator constant
// ---------------------------------------------------------------------------
#define TWO_WIRE  204
#define SR_RS_BIT 0x04
#define SR_EN_BIT 0x80

class LiquidCrystal_CAL : public LCD
{
public:
   LiquidCrystal_CAL (BusControl& busControl, int busAddress);
   
   /*!
    @function
    @abstract   Send a particular value to the LCD.
    @discussion Sends a particular value to the LCD for writing to the LCD or
    as an LCD command using the shift register.
    
    Users should never call this method.
    
    @param      value[in] Value to send to the LCD.
    @result     mode LOW - write to the LCD CGRAM, HIGH - write a command to
    the LCD.
    */
   virtual void send(uint8_t value, uint8_t mode);
   
// Required Overloads
   /*!
    @function
    @abstract   Sets the pin to control the backlight.
    @discussion Sets the pin in the device to control the backlight.
    @warning    Currently not supported
    
    @param      mode: backlight mode (HIGH|LOW)
    @param      pol: backlight polarity
    */
   void setBacklightPin ( uint8_t pin, t_backlighPol pol );
   
   /*!
    @function
    @abstract   Switch-on/off the LCD backlight.
    @discussion Switch-on/off the LCD backlight.
    The setBacklightPin has to be called before setting the backlight for
    this method to work. @see setBacklightPin.
    
    @param      mode: backlight mode (HIGH|LOW)
    */
   void setBacklight ( uint8_t mode );
 //////////////////////////

   /*!
    @function
    @abstract   Print a clock display at the specified location
    @discussion Print a clock display at the specified location
    
    @param      row: Row on which to display
    @param      col: Starting Column
    */
    void PrintClock(int row, int col);

    /*!
    @function
    @abstract   Utility function for digital clock display: prints preceding colon and leading 0
    @discussion Utility function for digital clock display: prints preceding colon and leading 0
    
    @param      digits: Number to display
    */
    void PrintClockDigits(int digits);

private:
   
   void init ();
   
   /*!
    * @method
    * @abstract takes care of shifting and the enable pulse
    */
   void shiftIt (uint8_t val);

   BusControl& m_BusControl;
   int m_BusAddress;
};

#endif
