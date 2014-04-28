
#include <SPI.h>
#include <LCD.h>
#include <Time.h>

#include "93L46B.h"
#include "BusControl.h"
#include "LiquidCrystal_CAL.h"
#include "MAX3100.h"

// Address Bus Definitions
// There are two registers (LOW/HIGH)
// Digital I/O Modules are arranged in "banks" attached to the LOW register (addresses 0-7)
// Each bank consists of one input module and one output module (creating a single, virtual I/O device)
// Other peripherals are attached to the HIGH register (addresses 8-15)
// All SPI devices share the same serial bus (MOSI[11], MISO[12], SCK[13])
#define ADDR0_PIN       2
#define ADDR1_PIN       3 
#define ADDR2_PIN       4
#define ADDR3_PIN       5 // Register Select 
#define ADDR_EN_PIN  9 // Active LOW

// I/O Control Definitions
#define SLOAD_PIN   10  // Serial Load (Inputs only)

// External I/O Module Configuration
#define IO_BANK_COUNT       2 // MAX 8
#define ANALOG_INPUT_PIN    0
#define ANALOG_INTPUT_COUNT 2
#define ANALOG_SENSITIVIY   3

// I/O Scanning
#define MODULE_IO_WIDTH  8 // Number of bits handled by each I/O module (so we can chain shift registers later, if we want to, for a wider I/O range)

#define RISING_EDGE  0
#define FALLING_EDGE 1

// Forward Declarations
unsigned int ScanIOBank(int bankIndex, unsigned int outputVal, int* pAnalogInputValue = NULL);

// Global Variables
unsigned int g_DigitalInputStates[IO_BANK_COUNT];
unsigned int g_DigitalOutputStates[IO_BANK_COUNT];
unsigned int g_ActiveSPISlave;
unsigned int g_AnalogInputStates[ANALOG_INTPUT_COUNT];
int g_AddressPins[] = {ADDR0_PIN,ADDR1_PIN,ADDR2_PIN,ADDR3_PIN}; // 4-bit addressing
BusControl g_BusControl(g_AddressPins, 4, ADDR_EN_PIN);
LiquidCrystal_CAL g_LCD(g_BusControl, 9);

#define DEBUG_MON // Enables serial port monitoring of input/output state changes
 
// Utility Functions ///////////////////////
#ifdef DEBUG_MON
  #define PrintInputStates(b) _PrintInputStates(b)
  #define PrintOutputStates(b) _PrintOutputStates(b)
  #define PrintBuffer(a,b) _PrintBuffer(a,b)
#else
  #define PrintInputStates(b)
  #define PrintOutputStates(b)
  #define PrintBuffer(a,b)
#endif

void _PrintInputStates(int bank)
{
  unsigned int values = g_DigitalInputStates[bank];
  Serial.print("Input States[bank ");
  Serial.print(bank);
  Serial.print("]: ");
  for(int i = 0; i < MODULE_IO_WIDTH; i++)
  {
      if((values >> ((MODULE_IO_WIDTH - 1) - i)) & 1)
          Serial.print("1 ");
      else
          Serial.print("0 ");
  }
  Serial.print(" (");
  Serial.print(values);
  Serial.print(")");

  Serial.print("\r\n");  
}

void _PrintOutputStates(int bank)
{
  unsigned int values = g_DigitalOutputStates[bank];
  Serial.print("Output States[bank ");
  Serial.print(bank);
  Serial.print("]: ");
  for(int i = 0; i < MODULE_IO_WIDTH; i++)
  {
      if((values >> ((MODULE_IO_WIDTH - 1) - i)) & 1)
          Serial.print("1 ");
      else
          Serial.print("0 ");
  }
  Serial.print(" (");
  Serial.print(values);
  Serial.print(")");
  
  Serial.print("\r\n");  
}

void _PrintBuffer(uint8_t* pBuf, unsigned int len)
{
  Serial.print("[0x");
  Serial.print((unsigned int)pBuf, HEX);
  Serial.print("]: ");

  for (int i = 0; i < len; i++)
  {
    Serial.print(pBuf[i], HEX);
    Serial.print(" ");
  }
  Serial.print("\r\n");  
}

void _PrintBuffer(uint16_t* pBuf, unsigned int len)
{
  Serial.print("[0x");
  Serial.print((unsigned int)pBuf, HEX);
  Serial.print("]: ");

  for (int i = 0; i < len; i++)
  {
    Serial.print(pBuf[i], HEX);
    Serial.print(" ");
  }
  Serial.print("\r\n");  
}
///////////////////////////////////////////

// DUMMY VARIABLES/FUNCTIONS FOR TESTING ///////////////////////////////////////////
byte latch[IO_BANK_COUNT];

//////////////////////////////////////////////////////////////////////////

void setup() 
{
#ifdef DEBUG_MON  
  Serial.begin(9600); // Text output
#else
  Serial.begin(115200); // USB MIDI (Hairless Bridge)
#endif

// Configure internal digital I/O pins
  // I/O Module Address Bus
  pinMode(ADDR0_PIN, OUTPUT);
  pinMode(ADDR1_PIN, OUTPUT);
  pinMode(ADDR2_PIN, OUTPUT);    
  pinMode(ADDR3_PIN, OUTPUT);    
  pinMode(ADDR_EN_PIN, OUTPUT);
  
  // I/O Control
  pinMode(SLOAD_PIN, OUTPUT);

  // Analog I/O
  pinMode(A0, INPUT);

  // SPI Bus
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0); // POL = 0 (rising edge), CPHA = 0 (first edge)
  SPI.setClockDivider(SPI_CLOCK_DIV4); // System Clock / 4 (4MHz on Arduino UNO)
  SPI.begin();

// Initialize I/O Control
  digitalWrite(SLOAD_PIN, HIGH);

// Do something fun with the LEDs during initialization (flash them in a spinning loop) :-) 
// TODO: Handle this more generically, since the configuration is variable (e.g. might not always have LEDs on these banks)
  for (int a = 0; a < 5; a++)
  {
    for (int i = 0; i < 5; i++)
    {
      ScanIOBank(0, (0x10 >> i));
      ScanIOBank(1, (0x01 << i));
      delay(50);
    }
  }
  
// Set Default Hardware State
  g_ActiveSPISlave = 0xFF;
  memset(g_DigitalOutputStates, 0, sizeof(g_DigitalOutputStates)); // Initialize all external outputs LOW
  for (int bank = 0; bank < IO_BANK_COUNT; bank++) // Set output states
    ScanIOBank(bank, g_DigitalOutputStates[bank]);
  memset(g_DigitalInputStates, 0, sizeof(g_DigitalInputStates)); // Initialize all external inputs LOW

// External device initialization
// Initialize the external UART
uint16_t uartConfig =   BAUD_DIV_8 \
                  | WORD_LEN_8 \ 
                  | PARITY_OFF \ 
                  | STOP_BITS_1 \
                  | IRDA_DISABLE \ 
                  | IRQ_RA_OFF \
                  | IRQ_PARITY_OFF \ 
                  | IRQ_RXDATA_OFF \ 
                  | IRQ_TXEMPTY_OFF \
                  | SHUTDOWN_DISABLE \
                  | FIFO_ENABLE \
                  | RW_MODE_WRITE \
                  | MSG_MODE_CONFIG;
MAX3100_ConfigureUART(g_BusControl, 10, uartConfig);
int baudRate = 4000000 / (16 * (0x1 << (uartConfig & 0x7)));

// Initialize LCD
g_LCD.begin(40,2);               // initialize the lcd

g_LCD.home ();                   // go home
g_LCD.print("LiquidCrystal_CALifragilistic");


// TEMP TESTING STUFF TO CLEAN UP LATER
  memset(latch, 0, sizeof(latch));
}

// Persistent Storage Handling ///////////////////////////////////////

SPI_93C46B g_EEPROM(8, g_BusControl);
void WriteEEPROM(uint8_t address, uint16_t val)
{
  g_EEPROM.EraseWriteEnable();
  g_EEPROM.Write(address, val);
  g_EEPROM.EraseWriteDisable();
}

void WriteEEPROM(uint8_t address, uint8_t len, uint16_t* pBuf)
{
  g_EEPROM.EraseWriteEnable();
 // TODO: Do Something
  g_EEPROM.EraseWriteDisable();
}

void EraseEEPROM()
{
  g_EEPROM.EraseWriteEnable();
  g_EEPROM.EraseAll();
  g_EEPROM.EraseWriteDisable();
}

void EraseEEPROM(uint8_t address)
{
  g_EEPROM.EraseWriteEnable();
  g_EEPROM.Erase(address);
  g_EEPROM.EraseWriteDisable();
}

uint16_t ReadEEPROM(uint8_t address)
{
  return g_EEPROM.Read(address);
}

uint16_t* ReadEEPROM(uint8_t address, uint8_t len)
{
 // TODO: Do Something
  return NULL;
}

// Test Functions
void FillEEPROM()
{
  g_EEPROM.EraseWriteEnable();
  for (uint8_t i = 0; i < 64; i++)
    g_EEPROM.Write(i, i); // TODO: How about a more interesting/useful pattern...

  g_EEPROM.EraseWriteDisable();
}

void DumpEEPROM()
{
  uint16_t data[4];
  for (int i = 0; i < 64; i+=4)
  {
    for (int a = 0; a < 4; a++)
      data[a] = g_EEPROM.Read(i+a);
    
    PrintBuffer(data, 4);
  }
}
///////////////////////////////////////////////////////////////////////////

// MIDI Handling /////////////////////////////////////////////////////////

//  plays a MIDI note.  Doesn't check to see that
//  cmd is greater than 127, or that data values are less than 127:
void noteOn(int cmd, int pitch, int velocity) 
{
  MAX3100_SendReceiveData(g_BusControl, 10, cmd, NULL, false);
  delay(1); // Delay more than 705 microsecs to allow transfer to complete
  MAX3100_SendReceiveData(g_BusControl, 10, pitch, NULL, false);
  delay(1); // Delay more than 705 microsecs to allow transfer to complete
  MAX3100_SendReceiveData(g_BusControl, 10, velocity, NULL, false);
  delay(1); // Delay more than 705 microsecs to allow transfer to complete
}
///////////////////////////////////////////////////////////////////////////

// Event Handling /////////////////////////////////////////////////////////

void OnAnalogInputChange(int index, int newValue, int oldValue)
{
#ifdef DEBUG_MON
  Serial.print("Analog Input Change [index ");
  Serial.print(index);
  Serial.print("]: ");
  Serial.print(oldValue);
  Serial.print(" -> ");
  Serial.print(newValue);
  Serial.print("\r\n");  
#endif
}

void OnDigitalInputChange(unsigned int bank, unsigned int index, byte type)
{
#ifdef DEBUG_MON
  Serial.print("Digital Input Change [bank ");
  Serial.print(bank);
  Serial.print(", index ");
  Serial.print(index);
  Serial.print("]: ");

  if (type == RISING_EDGE)
  {
    Serial.print("RISING_EDGE");
  }
  else
  {
    Serial.print("FALLING_EDGE");
  }

  Serial.print("\r\n");
#endif

  if (type == RISING_EDGE)
  {
    noteOn(0x90, 0x2E + (6-index) + (bank*6), 0x45);
  }
  else
  {
    noteOn(0x90, 0x2E + (6-index) + (bank*6), 0x00);
  }

  if (type == RISING_EDGE)
    latch[bank] ^= (0x1 << index);
  else
    latch[bank] ^= (0x1 << index);
}
//////////////////////////////////////////////////////////////////////////////

// I/O Scanning /////////////////////////////////////////////////////////

// TODO: Analog output??
// Scans one bank of I/O (one digital input module, one digital output module, and one analog input). Writes outputVal to selected output module and returns input value from selected input module
unsigned int ScanIOBank(int bankIndex, unsigned int outputVal, int* pAnalogInputValue /*= NULL*/)
{
  // Pulse the LOAD pin (inputs) to store input states in shift registers
  digitalWrite(SLOAD_PIN, LOW); // Copy input states into shift register
  delayMicroseconds(5); // let the values settle in the shift register (TODO: Do we really need this? The datasheet says it only takes 15ns to load...)
  digitalWrite(SLOAD_PIN, HIGH); // Reset latch pin (data has been captured)
  
  // Set BUS_ENABLE LOW (inputs) and STCP LOW (outputs) for the desired modules
  g_BusControl.SelectSlave(bankIndex);

  // Read/Write Digital I/O
  unsigned int inputVal = SPI.transfer(~outputVal);
  
  // Read Analog Input
  if (pAnalogInputValue)
    *pAnalogInputValue = map(analogRead(ANALOG_INPUT_PIN),39,945,0, 127);

  // Reset BUS_ENABLE (stop shifting inputs) and STCP (latch outputs)
  g_BusControl.SelectSlave(BUS_SLAVE_NONE);

 return inputVal;
}

int printCounter = 0;
void loop() 
{
  for (int bank = 0; bank < IO_BANK_COUNT; bank++)
  {
    // Scan/Update Analog/Digital I/O
    int analogInputVal = 0;
    unsigned int digInputVal = ScanIOBank(bank, g_DigitalOutputStates[bank], &analogInputVal);

    // Detect and handle any changes in digital states
    unsigned int inputFlags = digInputVal ^ g_DigitalInputStates[bank]; 
    if (inputFlags) // Detect digital input changes
    {
      unsigned int oldVal = g_DigitalInputStates[bank]; // Capture the previous state
      g_DigitalInputStates[bank] = digInputVal; // Store new state

      for (unsigned int i = 0; i < MODULE_IO_WIDTH; i++) // Check each input bit for changes
      {
        if (inputFlags & 0x1) // This bit changed
        {
          if ((oldVal >> i) & 0x1)
            OnDigitalInputChange(bank, i, FALLING_EDGE);
          else
            OnDigitalInputChange(bank, i, RISING_EDGE);  
        }
        inputFlags >>= 1; // Shift right one bit
      }
    }
    
    // Detect and handle analog input changes
    if (abs(analogInputVal - g_AnalogInputStates[bank]) >= ANALOG_SENSITIVIY)
    {
      OnAnalogInputChange(bank, analogInputVal, g_AnalogInputStates[bank]);
      g_AnalogInputStates[bank] = analogInputVal;
    }

    // TODO: Process UART (MIDI) queues
    // Should we try and use IRQ's for this??

    // Dummy output handling
    g_DigitalOutputStates[bank] = latch[bank] >> 1;
  }
  g_LCD.PrintClock(1,0);
}