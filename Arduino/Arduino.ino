
// Address Bus Definitions
#define A0_PIN       5
#define A1_PIN       6
#define A2_PIN       7
#define ADDR_EN_PIN  8 // Active low

// SPI/Module Bus Definitions
#define MOSI_PIN    11  // Master Out/Slave In
#define MISO_PIN    12  // Master In/Slave out 
#define SCK_PIN     13  // Clock
#define SLOAD_PIN   10  // Serial Load (Inputs only)

// External I/O Module Configuration
#define BASE_INPUT_MODULE_ID   0
#define MAX_INPUT_MODULES      4
#define BASE_OUTPUT_MODULE_ID  4
#define MAX_OUTPUT_MODULES     4
#define MODULE_ID_NONE         0xFF

#define SCANMODE_INPUT 0x01
#define SCANMODE_OUTPUT 0x02
#define SCANMODE_BOTH  (SCANMODE_INPUT | SCANMODE_OUTPUT)

#define MODULE_IO_WIDTH  8 // Bits handled by each I/O module (so we can chain shift registers later, if we want to)

#define MODID_TO_INDEX(type, id) ((type == INPUT) ? (id - BASE_INPUT_MODULE_ID) : (id - BASE_OUTPUT_MODULE_ID))
#define INDEX_TO_MODID(type, idx) ((type == INPUT) ? (idx + BASE_INPUT_MODULE_ID) : (idx + BASE_OUTPUT_MODULE_ID))

// Global Variables
unsigned int g_DigitalInputStates[MAX_INPUT_MODULES];
unsigned int g_DigitalOutputStates[MAX_OUTPUT_MODULES];

// Forward Declarations
unsigned int ScanIOBank(int bankIndex, unsigned int outputVal, byte scanMode = SCANMODE_BOTH);

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
  Serial.begin(9600);   
    
// Configure internal digital I/O pins
  // I/O Module Address Bus
  pinMode(A0_PIN, OUTPUT);
  pinMode(A1_PIN, OUTPUT);
  pinMode(A2_PIN, OUTPUT);    
  pinMode(ADDR_EN_PIN, OUTPUT);
  
  // SPI Bus
  pinMode(SCK_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(SLOAD_PIN, OUTPUT);
  
// Initialize SPI Bus
  digitalWrite(SCK_PIN, LOW);
  digitalWrite(MOSI_PIN, LOW);
  digitalWrite(SLOAD_PIN, HIGH);

// TODO: Replace this with something more elegant...
// Do something fun with the LEDs during initialization(flash them in a spinning loop) :-) 
  for (int a = 0; a < 5; a++)
  {
    for (int i = 0; i < 5; i++)
    {
      ScanIOBank(0, (0x10 >> i), SCANMODE_OUTPUT);
      ScanIOBank(1, (0x01 << i), SCANMODE_OUTPUT);
//      SetOutputByte(5, 0x10 >> i);
//      SetOutputByte(4, 0x01 << i);
      delay(50);
    }
  }
  
// Set Default State
  memset(g_DigitalInputStates, 0, sizeof(g_DigitalInputStates)); // Initialize all external inputs LOW
  memset(g_DigitalOutputStates, 0, sizeof(g_DigitalOutputStates)); // Initialize all external outputs LOW

// Initialize I/O Modules
  // TODO: The state values have been initialized...just do an I/O scan...
  for (int i = 0; i < MAX_OUTPUT_MODULES; i++)
    SetOutputByte(MODID_TO_INDEX(OUTPUT,i),0);
}

// I/O Module Handling ///////////////////////////////////////
void SelectIOModule(int moduleId)
{
    // Disable demux output
    digitalWrite(ADDR_EN_PIN, HIGH);
    if (moduleId != MODULE_ID_NONE) // If MODULE_ID_NONE is specified, disable all modules
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

// Writes outputVal to selected bank and returns input value from selected bank
unsigned int ScanIOBank(int bankIndex, unsigned int outputVal, byte scanMode /*= SCANMODE_BOTH*/)
{
  // Reset clock
  digitalWrite(SCK_PIN, LOW);

  if (scanMode & SCANMODE_INPUT)
  {
    // Pulse the LOAD pin (inputs) to store input states
    digitalWrite(SLOAD_PIN, LOW); // Copy input states into shift register
    delayMicroseconds(5); // let the values settle in the shift register (TODO: Do we really need this? The datasheet says it only takes 15ns to load...)
    digitalWrite(SLOAD_PIN, HIGH); // Reset latch pin (data has been captured)
    SelectIOModule(INDEX_TO_MODID(INPUT, bankIndex)); // Take the input enable pin (Clock Inhibit) low (the address bus is active LOW) to enable shifting input states
  }
  
  unsigned int inputVal = 0;
  for(int i = 0; i < MODULE_IO_WIDTH; i++) // Perform I/O for each bit in bank
  {
    if (scanMode & SCANMODE_INPUT)
    {
      byte bitVal = digitalRead(MISO_PIN); // Read bit from serial input
      inputVal |= (bitVal << ((MODULE_IO_WIDTH - 1) - i)); // Set the corresponding bit in value byte (MSB)
    }

    if (scanMode & SCANMODE_OUTPUT)
    {
      digitalWrite(MOSI_PIN, ((outputVal >> ((MODULE_IO_WIDTH - 1) - i)) & 0x1) ? LOW : HIGH); // Invert bit (active LOW) and write to serial output
    }
    
    // Pulse clock to advance shift registers
    digitalWrite(SCK_PIN, HIGH);
    delayMicroseconds(5); // TODO: Do we really need this? The datasheet says it only requires a 25ns pulse...
    digitalWrite(SCK_PIN, LOW);
  }
  
  if (scanMode & SCANMODE_INPUT)
  {
     // Reset (HIGH) the clock inhibit pin on input register to stop shifting
     SelectIOModule(MODULE_ID_NONE);
  }
  
  if (scanMode & SCANMODE_OUTPUT)
  {
     // Pulse the latch/load pin on output register to latch the last 8 bits shifted to the outputs...
     SelectIOModule(INDEX_TO_MODID(OUTPUT, bankIndex)); // TODO: Can we just switch to the output module without going to NULL first?
     delayMicroseconds(5); // TODO: Do we really need this? The datasheet says it only requires a 22ns pulse...
     SelectIOModule(MODULE_ID_NONE);
  }
  
 return inputVal;
}

//////////////////////////////////////////////////////////////////////////////

void loop() 
{
  for (int bank = 0; bank < 2; bank++)
  {
    unsigned int inputVal = ScanIOBank(bank, g_DigitalOutputStates[bank]);
  
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
