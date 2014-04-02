
#include <SPI.h>

// Address Bus Definitions
#define A0_PIN       2
#define A1_PIN       4
#define A2_PIN       7
#define ADDR_EN_PIN  8 // Active low

// SPI/Module Bus Definitions
// NOTE: This is just for SW SPI (delete when we feel OK with it)
// #define MOSI_PIN    11  // Master Out/Slave In
// #define MISO_PIN    12  // Master In/Slave out 
// #define SCK_PIN     13  // Clock

#define SLOAD_PIN   10  // Serial Load (Inputs only)

// External I/O Module Configuration
#define IO_BANK_COUNT          4
#define BANK_NONE         0xFF

// I/O Scanning
#define MODULE_IO_WIDTH  8 // Number of bits handled by each I/O module (so we can chain shift registers later, if we want to, for a wider IO range)

// Global Variables
unsigned int g_DigitalInputStates[IO_BANK_COUNT];
unsigned int g_DigitalOutputStates[IO_BANK_COUNT];

// Forward Declarations
unsigned int ScanDigitalIOBank(int bankIndex, unsigned int outputVal);

//#define DEBUG_MON // Enables serial port monitoring of input/output state changes

// Utility Functions ///////////////////////
#ifdef DEBUG_MON
  #define PrintInputStates(b) _PrintInputStates(b)
  #define PrintOutputStates(b) _PrintOutputStates(b)
#else
  #define PrintInputStates(b)
  #define PrintOutputStates(b)
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
///////////////////////////////////////////

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
  pinMode(ADDR_EN_PIN, OUTPUT);
  
  // I/O Control
  pinMode(SLOAD_PIN, OUTPUT);

  // SPI Bus
  // NOTE: This is just for SW SPI (delete when we feel OK with it)
  // pinMode(SCK_PIN, OUTPUT);
  // pinMode(MOSI_PIN, OUTPUT);
  // pinMode(MISO_PIN, INPUT);

SPI.setBitOrder(MSBFIRST);
// CPOL = 0 (rising edge)
// CPHA = 0 (first edge)
SPI.setDataMode(SPI_MODE0);
SPI.setClockDivider(SPI_CLOCK_DIV4); // System Clock / 4
SPI.begin();

// NOTE: This is just for SW SPI (delete when we feel OK with it)
// Initialize SPI Bus (Outputs Only)
  // digitalWrite(SCK_PIN, LOW);
  // digitalWrite(MOSI_PIN, LOW);

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
  memset(g_DigitalInputStates, 0, sizeof(g_DigitalInputStates)); // Initialize all external inputs LOW
  memset(g_DigitalOutputStates, 0, sizeof(g_DigitalOutputStates)); // Initialize all external outputs LOW
  for (int bank = 0; bank < IO_BANK_COUNT; bank++)
    ScanDigitalIOBank(bank, g_DigitalOutputStates[bank]);
}

// I/O Module Handling ///////////////////////////////////////
void SelectDigitalIOBank(int moduleId)
{
    // Disable demux output
    digitalWrite(ADDR_EN_PIN, HIGH);
    if (moduleId != BANK_NONE) // If BANK_IDX_NONE is specified, disable all modules
    {
      // Write address bits
      // TODO: Is there a function to write all these at once?
      digitalWrite(A0_PIN, (moduleId & 0x1) ? HIGH : LOW);
      digitalWrite(A1_PIN, (moduleId & 0x2) ? HIGH : LOW);
      digitalWrite(A2_PIN, (moduleId & 0x4) ? HIGH : LOW);
      
      // Enable demux output
      digitalWrite(ADDR_EN_PIN, LOW);  
    }
}

// Scans one bank of digital I/O (one input, oue output). Writes outputVal to selected output module and returns input value from selected input module
unsigned int ScanDigitalIOBank(int bankIndex, unsigned int outputVal)
{
// NOTE: This is just for SW SPI (delete when we feel OK with it)
  // Reset clock
//  digitalWrite(SCK_PIN, LOW);

  // Pulse the LOAD pin (inputs) to store input states in shift registers
  digitalWrite(SLOAD_PIN, LOW); // Copy input states into shift register
  delayMicroseconds(5); // let the values settle in the shift register (TODO: Do we really need this? The datasheet says it only takes 15ns to load...)
  digitalWrite(SLOAD_PIN, HIGH); // Reset latch pin (data has been captured)
  
  // Set BUS_ENABLE LOW (inputs) and STCP LOW (outputs) for the desired modules
  SelectDigitalIOBank(bankIndex);

  // TODO: Consider using SPI library (might not handle clocks correctly for both inputs and outputs...out of phase?)
  unsigned int inputVal = 0;
 
  inputVal = SPI.transfer(~outputVal);

  // NOTE: This is just for SW SPI (delete when we feel OK with it)
  // for(int i = 0; i < MODULE_IO_WIDTH; i++) // Perform I/O for each bit in bank
  // {

  //   byte bitVal = digitalRead(MISO_PIN); // Read bit from serial input
  //   inputVal |= (bitVal << ((MODULE_IO_WIDTH - 1) - i)); // Set the corresponding bit in value byte (MSB)
  //   digitalWrite(MOSI_PIN, ((outputVal >> ((MODULE_IO_WIDTH - 1) - i)) & 0x1) ? LOW : HIGH); // Invert bit (active LOW) and write to serial output
    
  //   // Pulse clock to advance shift registers
  //   digitalWrite(SCK_PIN, HIGH);
  //   delayMicroseconds(5); // TODO: Do we really need this? The datasheet says it only requires a 25ns pulse...
  //   digitalWrite(SCK_PIN, LOW);
  // }
  
  // Reset BUS_ENABLE (stop shifting inputs) and STCP (latch outputs)
  SelectDigitalIOBank(BANK_NONE);
  
 return inputVal;
}

//////////////////////////////////////////////////////////////////////////////

void loop() 
{
  for (int bank = 0; bank < IO_BANK_COUNT; bank++)
  {
    unsigned int inputVal = ScanDigitalIOBank(bank, g_DigitalOutputStates[bank]);
    if (g_DigitalInputStates[bank] != inputVal) // Detect change
    {
      g_DigitalInputStates[bank] = inputVal; // Store new state
      PrintInputStates(bank);
    }

    if (g_DigitalOutputStates[bank] != g_DigitalInputStates[bank] >> 1) // Detect change
    {
      g_DigitalOutputStates[bank] = g_DigitalInputStates[bank] >> 1;  // Store new state
      PrintOutputStates(bank);      
    }
  }
}
