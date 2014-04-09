#include <SPI.h>
#include "BusControl.h"

BusControl::BusControl(int* pPins, int addressBits, unsigned int enablePin) :
  m_EnablePin(enablePin)
{
  if (addressBits > 5) // What hardware is this?!?
    addressBits = 5;
  else if (!pPins)
    addressBits = 0;

  m_AddressBits = addressBits;
  if (pPins)
    memcpy(m_AddressPins, pPins, addressBits * sizeof(int));
  else
    memset(m_AddressPins, 0, sizeof(m_AddressPins));
}

void BusControl::SelectSlave(unsigned int address, bool noBusEnable /*=false*/)
{
  if (address != m_ActiveSlave)
  {
    if ((m_EnablePin != NO_BUS_ENABLE) && !noBusEnable)
      digitalWrite(m_EnablePin, HIGH); // Disable output (Active LOW)

    if (address != BUS_SLAVE_NONE)
    {
      for (int bit = 0; bit < m_AddressBits; bit++)
      {
        digitalWrite(m_AddressPins[bit], (address & (0x1 << bit)) ? HIGH : LOW);
      }
    if ((m_EnablePin != NO_BUS_ENABLE) && !noBusEnable)
        digitalWrite(m_EnablePin, LOW); // Enable output    
    }
    m_ActiveSlave = address;  
  } 
}
