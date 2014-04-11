#include "MAX3100.h"
#include <SPI.h>

void PrintHexByte(uint8_t b)
{
  if (b < 0x10)
    Serial.print("0");
  Serial.print(b, HEX);
}

void PrintBinByte(uint8_t b)
{
  for (int i = 0; i < 8; i++)
  {
    Serial.print((b >> 7) & 0x1 ? "1" : "0");
    b <<= 1;
  }
}

void PrintConfig(uint16_t config)
{
  Serial.print("{(");
  PrintBinByte(config >> 8);
  PrintBinByte(config & 0xFF);
  Serial.print(") ");
  
  uint8_t baudDiv = config & 0x7;
  baudDiv = (0x1 << baudDiv);

  Serial.print(baudDiv);
  Serial.print(" (");
  Serial.print(4000000 / (16 * baudDiv));
  Serial.print(")");
  
  Serial.print(" | ");
  Serial.print(config & WORD_LEN_7 ? "7 BITS" : "8 BITS");

  Serial.print(" | ");
  Serial.print(config & PARITY_ON ? "PARITY_ON" : "PARITY_OFF");

  Serial.print(" | ");
  Serial.print(config & STOP_BITS_2 ? "2 SB" : "1 SB");
  
  Serial.print(" | ");
  Serial.print(config & IRDA_ENABLE ? "IRDA_ON" : "IRDA_OFF");
  
  Serial.print(" | ");
  Serial.print(config & IRQ_RA_ON ? "RA_FE" : "X");
  
  Serial.print("/");
  Serial.print(config & IRQ_PARITY_ON ? "Pr" : "X");  
  Serial.print("/");
  Serial.print(config & IRQ_RXDATA_ON ? "R" : "X");
  Serial.print("/");
  Serial.print(config & IRQ_TXEMPTY_ON ? "T" : "X");
  
  Serial.print(" | ");
  Serial.print(config & SHUTDOWN_ENABLE ? "SHUTDOWN" : "X");

  Serial.print(" | ");
  Serial.print(config & FIFO_DISABLE ? "FIFO_OFF" : "FIFO_ON");
  
  Serial.print("}");
}

void PrintDataCommand(uint16_t cmd)
{
  Serial.print("{(");
  PrintBinByte(cmd >> 8);
  PrintBinByte(cmd & 0xFF);
  Serial.print(") ");
  
  if (cmd & RW_MODE_WRITE)
  {
    Serial.print("WRITE | ");
    Serial.print(cmd & TX_DISABLE ? "TX_OFF | " : "TX_ON | ");
    Serial.print(cmd & RTS_SET ? "RTS_SET | " : "RTS_RESET | ");
    Serial.print(cmd & TX_PARITY_1 ? "TX_PARITY=1 | 0x" : "TX_PARITY=0 | 0x");    
    PrintHexByte(cmd & 0xFF);
  }  
  else
    Serial.print("READ | <no flags>");  
  
  Serial.print("}");  
}

void PrintDataResponse(uint16_t resp)
{
  Serial.print("{(");
  PrintBinByte(resp >> 8);
  PrintBinByte(resp & 0xFF);
  Serial.print(") ");
  
  Serial.print(resp & RX_DATA_READY ? "RX_READY | " : "NO_DATA | ");
  Serial.print(resp & TX_BUF_EMPTY ? "TX_EMPTY | " : "TX_FULL | ");
  Serial.print(resp & FRAMING_ERROR ? "FRAMING_ERROR | " : "NO_ERROR | ");
  Serial.print(resp & CTS_SET ? "CTS_SET | " : "CTS_RESET | ");
  Serial.print(resp & RX_PARITY_1 ? "RX_PARITY=1 | 0x" : "RX_PARITY=0 | 0x");    
  PrintHexByte(resp & 0xFF);
  
  Serial.print("}");
}

uint16_t SendReceive(BusControl& busControl, int address, uint16_t outVal)
{
  busControl.SelectSlave(address);
  uint16_t inVal = (SPI.transfer(((outVal >> 8) & 0xFF)) << 8); // MSB First
  // TODO: Check TX_EMPTY bit to make sure the transfer was successful
  inVal |= (SPI.transfer(outVal & 0xFF) & 0xFF); // LSB Second
  busControl.SelectSlave(BUS_SLAVE_NONE);
  return inVal;
}

bool MAX3100_SendReceiveData(BusControl& busControl, int address, uint8_t dataOut, uint8_t* pDataIn /*=NULL*/, bool printOK /*=true*/)
{
  uint16_t cmd =   dataOut \
             | MSG_MODE_DATA \
             | RW_MODE_WRITE \ 
             | TX_ENABLE \
             | RTS_SET \
             | TX_PARITY_0;
             
  if (printOK)
  {
    Serial.print("Sending(");
    Serial.print(address);
    Serial.print(") ");
    PrintDataCommand(cmd);
  }  
  uint16_t resp = SendReceive(busControl, address, cmd);

  // TODO: Process response flags  
  // R, T, FE, CTS, Pr
  if (resp & RX_DATA_READY) // Data was available for reading...it should be here now...
  {
    if (printOK)
    {
      Serial.println();
      Serial.print("Received(");
      Serial.print(address);
      Serial.print(") ");
      PrintDataResponse(resp);
    }

    if (pDataIn)
      *pDataIn = resp & 0xFF;
  }
  
  if (printOK)
  {
    if (resp & FRAMING_ERROR)
      Serial.print(" -> Framing Error");
    else
      Serial.print((resp & CTS_SET) ? " -> Clear" : " -> Not Clear");
    Serial.println();
  }  
  
  return (resp & RX_DATA_READY); // return true if R was set, false otherwise...
}

bool MAX3100_ReceiveData(BusControl& busControl, int address, uint8_t* pDataIn /*=NULL*/, bool printOK /*=true*/)
{
  uint16_t cmd = 0;
  uint16_t resp = SendReceive(busControl, address, cmd);

  // TODO: Process response flags  
  // R, T, FE, CTS, Pr
  if (resp & RX_DATA_READY) // Data was available for reading...it should be here now...
  {
    if (printOK)
    {
      Serial.print("Received(");
      Serial.print(address);
      Serial.print(") ");
      PrintDataResponse(resp);
    }
    
    if (printOK)
    {
      if (resp & FRAMING_ERROR)
        Serial.print(" -> Framing Error");
      else
        Serial.print((resp & CTS_SET) ? " -> Clear" : " -> Not Clear");
      Serial.println();
    }  
      
    if (pDataIn)
      *pDataIn = resp & 0xFF;
  }
  
  return (resp & RX_DATA_READY); // return true if R was set, false otherwise...
}

void MAX3100_SetRTS(BusControl& busControl, int address)
{
  uint16_t cmd =   MSG_MODE_DATA \
             | RW_MODE_WRITE \ 
             | TX_DISABLE \
             | RTS_SET \
             | TX_PARITY_0;
             
  SendReceive(busControl, address, cmd);
}

void MAX3100_ResetRTS(BusControl& busControl, int address)
{
  uint16_t cmd =   MSG_MODE_DATA \
             | RW_MODE_WRITE \ 
             | TX_DISABLE \
             | RTS_RESET \
             | TX_PARITY_0;
             
  SendReceive(busControl, address, cmd);
}

void MAX3100_ConfigureUART(BusControl& busControl, int address, uint16_t config)
{
  Serial.print("Configuring UART (");
  Serial.print(address);
  Serial.print(") - ");
  PrintConfig(config);
  
  // Write configuration                 
  SendReceive(busControl, address, config);
  // Confirm configuration
  uint16_t configConfirm = SendReceive(busControl, address, TEST_MODE_OFF | RW_MODE_READ | MSG_MODE_CONFIG);
  if ((configConfirm & 0x3FFF) != (config & 0x3FFF))
  {
    Serial.print(" -> Error: ");
    PrintConfig(configConfirm);
  }
  else
  {
    Serial.print(" -> Success!");
  }
  Serial.println();
}

uint16_t MAX3100_ReadUARTConfig(BusControl& busControl, int address)
{
  return SendReceive(busControl, address, TEST_MODE_OFF | RW_MODE_READ | MSG_MODE_CONFIG);
}



