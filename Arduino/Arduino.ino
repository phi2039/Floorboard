
#include <SPI.h>
#include "93L46B.h"

// Address Bus Definitions
// There are two registers (LOW/HIGH)
// Digital I/O Modules are arranged in "banks" attached to the LOW register (addresses 0-7)
// Each bank consists of one input module and one output module (creating a single, virtual I/O device)
// Other peripherals are attached to the HIGH register (addresses 8-15)
// All SPI devices share the same serial bus (MOSI, MISO, SCK)
#define A0_PIN       2
#define A1_PIN       3
#define A2_PIN       4
#define A3_PIN       5 // Bank Select 
#define ADDR_EN_PIN  9 // Active LOW

// I/O Control Definitions
#define SLOAD_PIN   10  // Serial Load (Inputs only)

// External I/O Module Configuration
#define IO_BANK_COUNT     4
#define SLAVE_NONE         0xFF

// I/O Scanning
#define MODULE_IO_WIDTH  8 // Number of bits handled by each I/O module (so we can chain shift registers later, if we want to, for a wider I/O range)

#define RISING_EDGE  0
#define FALLING_EDGE 1

// Forward Declarations
unsigned int ScanDigitalIOBank(int bankIndex, unsigned int outputVal);
void SelectSPISlave(unsigned int index);

// Global Variables
unsigned int g_DigitalInputStates[IO_BANK_COUNT];
unsigned int g_DigitalOutputStates[IO_BANK_COUNT];
unsigned int g_ActiveSPISlave;

SPI_93C46B g_EEPROM(8, SelectSPISlave);

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

// DUMMY VARIABLES FOR TESTING ///////////////////////////////////////////
byte latch[IO_BANK_COUNT];
uint16_t eeprom_data[4];
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
  pinMode(A0_PIN, OUTPUT);
  pinMode(A1_PIN, OUTPUT);
  pinMode(A2_PIN, OUTPUT);    
  pinMode(A3_PIN, OUTPUT);    
  pinMode(ADDR_EN_PIN, OUTPUT);
  
  // I/O Control
  pinMode(SLOAD_PIN, OUTPUT);

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
      ScanDigitalIOBank(0, (0x10 >> i));
      ScanDigitalIOBank(1, (0x01 << i));
      delay(50);
    }
  }
  
// Set Default Hardware State
  g_ActiveSPISlave = 0xFF;
  memset(g_DigitalOutputStates, 0, sizeof(g_DigitalOutputStates)); // Initialize all external outputs LOW
  for (int bank = 0; bank < IO_BANK_COUNT; bank++)
    ScanDigitalIOBank(bank, g_DigitalOutputStates[bank]);
  memset(g_DigitalInputStates, 0, sizeof(g_DigitalInputStates)); // Initialize all external inputs LOW

// TEMP TESTING STUFF TO CLEAN UP LATER
  memset(latch, 0, sizeof(latch));
  memset(eeprom_data, 0, sizeof(eeprom_data));
 }

// Persistent Storage Handling ///////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////

// SPI Bus Selection ///////////////////////////////////////
void SelectSPISlave(unsigned int address)
{
  // See if this slave is already active (don't change it if it's already active)
  if (address != g_ActiveSPISlave)
  {
    // Disable demux output
    digitalWrite(ADDR_EN_PIN, HIGH);
    if (address != SLAVE_NONE) // If BANK_IDX_NONE is specified, disable all modules
    {
      // Write address bits
      // TODO: Is there a function to write all these at once? Ports...
      digitalWrite(A0_PIN, (address & 0x1) ? HIGH : LOW);
      digitalWrite(A1_PIN, (address & 0x2) ? HIGH : LOW);
      digitalWrite(A2_PIN, (address & 0x4) ? HIGH : LOW);
      digitalWrite(A3_PIN, (address & 0x8) ? HIGH : LOW);
      
      // Enable demux output(s)
      digitalWrite(ADDR_EN_PIN, LOW);
    }
    g_ActiveSPISlave = address; // Store the address
  }  
}
//////////////////////////////////////////////////////////////////////////////

// I/O Module Handling ///////////////////////////////////////
void SelectDigitalIOBank(unsigned int bank)
{
  SelectSPISlave(bank & 0x7); // Mask out register bit (A3) (TODO: Does this mask make any sense...?)
}

// Scans one bank of digital I/O (one input module, one output module). Writes outputVal to selected output module and returns input value from selected input module
unsigned int ScanDigitalIOBank(int bankIndex, unsigned int outputVal)
{
  // Make sure the SPI flags are set how we want them for these devices
   SPI.setDataMode(SPI_MODE0); // POL = 0 (rising edge), CPHA = 0 (first edge)

 // Pulse the LOAD pin (inputs) to store input states in shift registers
  digitalWrite(SLOAD_PIN, LOW); // Copy input states into shift register
  delayMicroseconds(5); // let the values settle in the shift register (TODO: Do we really need this? The datasheet says it only takes 15ns to load...)
  digitalWrite(SLOAD_PIN, HIGH); // Reset latch pin (data has been captured)
  
  // Set BUS_ENABLE LOW (inputs) and STCP LOW (outputs) for the desired modules
  SelectDigitalIOBank(bankIndex);

  // Read/Write I/O
  unsigned int inputVal = SPI.transfer(~outputVal);
  
  // Reset BUS_ENABLE (stop shifting inputs) and STCP (latch outputs)
  SelectDigitalIOBank(SLAVE_NONE);
  
 return inputVal;
}

void OnDigitalInputChange(unsigned int bank, unsigned int index, byte type)
{
#ifdef DEBUG_MON
  Serial.print("Input Change [bank ");
  Serial.print(bank);
  Serial.print(", index ");
  Serial.print(index);
  Serial.print("]: ");

  if (type == RISING_EDGE)
    Serial.print("RISING_EDGE");
  else
    Serial.print("FALLING_EDGE");

  Serial.print("\r\n");  
#endif

  if (type == RISING_EDGE)
    latch[bank] ^= (0x1 << index);
}
//////////////////////////////////////////////////////////////////////////////

void loop() 
{
  for (int bank = 0; bank < IO_BANK_COUNT; bank++)
  {
    unsigned int inputVal = ScanDigitalIOBank(bank, g_DigitalOutputStates[bank]);
    unsigned int inputFlags = inputVal ^ g_DigitalInputStates[bank]; // Identify any changes in state
    if (inputFlags) // Detect change
    {
      unsigned int oldVal = g_DigitalInputStates[bank]; // Capture the previous state
      g_DigitalInputStates[bank] = inputVal; // Store new state

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

    g_DigitalOutputStates[bank] = latch[bank] >> 1;
// NOTE: This is just passthrough code to display input changes...
//     if (g_DigitalOutputStates[bank] != g_DigitalInputStates[bank] >> 1) // Detect change
//     {
//       g_DigitalOutputStates[bank] = g_DigitalInputStates[bank] >> 1;  // Store new state
// //      PrintOutputStates(bank);      
//     }
  }
}