
#ifndef _BUS_CONTROL_
#define _BUS_CONTROL_

#define BUS_SLAVE_NONE 0xFF
#define NO_BUS_ENABLE  0xFF

class BusControl
{
public:
  BusControl(int* pPins, int addressBits, unsigned int enablePin);
  void SelectSlave(unsigned int address, bool noBusEnable = false);

private:
  int m_AddressBits;
  int m_AddressPins[5]; // Max of 5-bit addressing
  unsigned int m_ActiveSlave;
  unsigned int m_EnablePin;
};
#endif // _BUS_CONTROL_

void SelectSPISlave(unsigned int address);
