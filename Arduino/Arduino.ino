
#include <SPI.h>

// Address Bus Definitions
#define A0_PIN       2
#define A1_PIN       4
#define A2_PIN       7
#define ADDR_EN_PIN  8 // Active low

// I/O Control Definitions
#define SLOAD_PIN   10  // Serial Load (Inputs only)

// External I/O Module Configuration
#define IO_BANK_COUNT          4
#define BANK_NONE         0xFF

// I/O Scanning
#define MODULE_IO_WIDTH  8 // Number of bits handled by each I/O module (so we can chain shift registers later, if we want to, for a wider IO range)

#define RISING_EDGE  0
#define FALLING_EDGE 1

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

byte latch[IO_BANK_COUNT];

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
  memset(g_DigitalOutputStates, 0, sizeof(g_DigitalOutputStates)); // Initialize all external outputs LOW
  for (int bank = 0; bank < IO_BANK_COUNT; bank++)
    ScanDigitalIOBank(bank, g_DigitalOutputStates[bank]);
  memset(g_DigitalInputStates, 0, sizeof(g_DigitalInputStates)); // Initialize all external inputs LOW


  memset(latch, 0, sizeof(latch));
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

// Scans one bank of digital I/O (one input module, one output module). Writes outputVal to selected output module and returns input value from selected input module
unsigned int ScanDigitalIOBank(int bankIndex, unsigned int outputVal)
{
  // Pulse the LOAD pin (inputs) to store input states in shift registers
  digitalWrite(SLOAD_PIN, LOW); // Copy input states into shift register
  delayMicroseconds(5); // let the values settle in the shift register (TODO: Do we really need this? The datasheet says it only takes 15ns to load...)
  digitalWrite(SLOAD_PIN, HIGH); // Reset latch pin (data has been captured)
  
  // Set BUS_ENABLE LOW (inputs) and STCP LOW (outputs) for the desired modules
  SelectDigitalIOBank(bankIndex);

  // Read/Write I/O
  unsigned int inputVal = SPI.transfer(~outputVal);
  
  // Reset BUS_ENABLE (stop shifting inputs) and STCP (latch outputs)
  SelectDigitalIOBank(BANK_NONE);
  
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
