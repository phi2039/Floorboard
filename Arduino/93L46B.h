#include "BusControl.h"

#define READ  0x02 // 10 00ABCD - 0x02 ADDR
#define EWEN1 0x00 // 00 11XXXX
#define EWEN2 0x30 //           - 0x00 0x30
#define ERASE 0x03 // 11 00ABCD - 0x03 ADDR
#define ERAL1 0x00 // 00 10XXXX
#define ERAL2 0x20 //           - 0x00 0x20
#define WRITE 0x01 // 01 00ABCD - 0x01 ADDR
#define WRAL1 0x00 // 00 01XXXX
#define WRAL2 0x10 //           - 0x00 0x10
#define EWDS1 0x00 // 00 00XXXX
#define EWDS2 0x00 //           - 0x00 0x00


#define DO 3
#define DI 2
#define PIN93C46  PINB
#define PORT93C46 PORTB

class SPI_93C46B
{
public:
  SPI_93C46B(unsigned int busAddress, BusControl& busControl) :
    m_BusControl(busControl),
    m_BusAddress(busAddress)
  {      
  }

  uint16_t Read(uint8_t address) 
  {
    uint16_t data = 0;

    Opcode(READ, address);

    // "0" Dummy Bit
    digitalWrite(11, LOW);
    digitalWrite(13, HIGH);
    digitalWrite(13, LOW);

    SPCR |=  (1 << CPHA);               // Invert CPHA, very important!

    data = SPI.transfer(0);
    data = (data << 8) | (SPI.transfer(0));
    delayMicroseconds(1);
    m_BusControl.SelectSlave(0xFF);
    digitalWrite(11, LOW);
    SPCR &= ~(1 << CPHA);    

    return data;
  }
  
  void EraseWriteEnable(void) 
  {
    Opcode(EWEN1, EWEN2);
    m_BusControl.SelectSlave(0xFF);
    digitalWrite(11, LOW);
    delayMicroseconds(0.1);
    m_BusControl.SelectSlave(m_BusAddress);
    while(!(PIN93C46 & (1 << DO))); // Wait 1
    m_BusControl.SelectSlave(0xFF);
  }
  
  void EraseWriteDisable(void) 
  {
    Opcode(EWDS1, EWDS2);
    m_BusControl.SelectSlave(0xFF);
    digitalWrite(11, LOW);
    delayMicroseconds(0.1);
    m_BusControl.SelectSlave(m_BusAddress);
    while(!(PIN93C46 & (1 << DO))); // Wait 1
    m_BusControl.SelectSlave(0xFF);
  }
  
  void Erase(uint8_t address) 
  {
    Opcode(ERASE, address);   
    digitalWrite(11, LOW);
    m_BusControl.SelectSlave(0xFF);
    m_BusControl.SelectSlave(m_BusAddress);
    while(!(PIN93C46 & (1 << DO))); // Wait 1
    delayMicroseconds(0.1);
    m_BusControl.SelectSlave(0xFF);
  }
  
  void EraseAll(void) 
  {
    Opcode(ERAL1, ERAL2);   
    digitalWrite(11, LOW);
    m_BusControl.SelectSlave(0xFF);
    m_BusControl.SelectSlave(m_BusAddress);
    while(!(PIN93C46 & (1 << DO))); // Wait 1
    delayMicroseconds(0.1);
    m_BusControl.SelectSlave(0xFF);
  }
  
  void Write(uint8_t address, uint16_t data) 
  {
    Opcode(WRITE, address);
    SPI.transfer(data >> 8);            // Write data high byte to address
    SPI.transfer(data & 0xFF);          // Write data low byte to address

    digitalWrite(11, LOW);
    m_BusControl.SelectSlave(0xFF);
    m_BusControl.SelectSlave(m_BusAddress);
    delayMicroseconds(0.1);
    while(!(PIN93C46 & (1 << DO))); // Wait "1"
    m_BusControl.SelectSlave(0xFF);
  }
  
  void WriteAll(uint16_t data) 
  {
    Opcode(WRAL1, WRAL2);
    SPI.transfer(data >> 8);            // Write data high byte to address
    SPI.transfer(data & 0xFF);          // Write data low byte to address

    digitalWrite(11, LOW);
    m_BusControl.SelectSlave(0xFF);
    m_BusControl.SelectSlave(m_BusAddress);
    delayMicroseconds(0.1);
    while(!(PIN93C46 & (1 << DO))); // Wait "1"
    m_BusControl.SelectSlave(0xFF);
  }

private:
  void Opcode(uint8_t opcode, uint8_t address) 
  {
    m_BusControl.SelectSlave(m_BusAddress);

    // Take manual control to send the start bit
    SPCR &= ~(1 << SPE);               // SPI disable
    delayMicroseconds(1);

    // Send the start bit
    digitalWrite(13, LOW);
    digitalWrite(11, HIGH);
    digitalWrite(13, HIGH);
    digitalWrite(13, LOW);

    SPCR |= (1 << SPE);                // SPI enable

    SPI.transfer((opcode << 6) | address); // Transmit byte
  }

  BusControl& m_BusControl;
  unsigned int m_BusAddress;
};
