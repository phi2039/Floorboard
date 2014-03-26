
// Address Bus Definitions
#define A0_PIN       5
#define A1_PIN       6
#define A2_PIN       7
#define ADDR_EN_PIN  8 // Active low

#define BASE_INPUT_MODULE_ID   0
#define MAX_INPUT_MODULES      4
#define BASE_OUTPUT_MODULE_ID  4
#define MAX_OUTPUT_MODULES     4
#define MODULE_ID_NONE         0xFF

#define IO_WIDTH  8 // Bits handled by each I/O module (so we can chain shift registers later, if we want to)

#define MODID_TO_INDEX(type, id) ((type == INPUT) ? (id - BASE_INPUT_MODULE_ID) : (id - BASE_OUTPUT_MODULE_ID))

// SPI Bus Definitions
//#define SLOAD_PIN   10  // Shift register load/latch
#define MOSI_PIN    11  // Master Out/Slave In
#define MISO_PIN    12  // Master In/Slave out 
#define SCK_PIN     13  // Clock
#define SLOAD_PIN   10  // Serial Load (Inputs only)

// Global Variables
byte g_DigitalInputStates[MAX_INPUT_MODULES];
byte g_DigitalOutputStates[MAX_OUTPUT_MODULES];

// Utility Functions ///////////////////////

void PrintInputs(int moduleId)
{
  byte values = g_DigitalInputStates[MODID_TO_INDEX(INPUT, moduleId)];
  Serial.print("Input States[");
  Serial.print(moduleId);
  Serial.print("]: ");
  for(int i = 0; i < 8; i++)
  {
      if((values >> i) & 1)
          Serial.print("1 ");
      else
          Serial.print("0 ");
  }

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
      SetOutputByte(5, 0x10 >> i);
      SetOutputByte(4, 0x01 << i);
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

void SetOutputByte(int moduleId, byte vals)
{
  // Take the latch/load pin low (the address bus is active LOW) so the output states don't change until we send all the data
//  SelectIOModule(moduleId);

  // Shift out the data
  shiftOut(MOSI_PIN, SCK_PIN, MSBFIRST, ~vals); // We have to invert this, since the output module is acvite LOW

  // Pulse the latch/load pin to latch the last 8 bits shifted...
  SelectIOModule(moduleId);
  delayMicroseconds(5);  
  SelectIOModule(MODULE_ID_NONE);
}

byte GetInputByte(int moduleId)
{
  // Latch the input states
  digitalWrite(SLOAD_PIN, LOW);
  // let the values latch in the shift register
  delayMicroseconds(5);
  // Reset latch pin (data has been captured)
  digitalWrite(SLOAD_PIN, HIGH);
  
  // Take the enable pin(Clock Inhibit) low (the address bus is active LOW) to begin getting the input states
  SelectIOModule(moduleId);
 
    /* Loop to read each bit value from the serial out line
     * of the SN74HC165N.
    */
    byte bitVal;
    unsigned int bytesVal = 0;
    for(int i = 0; i < 8; i++)
    {
        bitVal = digitalRead(MISO_PIN);

        //Set the corresponding bit in value byte
        bytesVal |= (bitVal << ((8-1) - i));

        /* Pulse the Clock (rising edge shifts the next bit).
        */
        digitalWrite(SCK_PIN, HIGH);
        delayMicroseconds(5);
        digitalWrite(SCK_PIN, LOW);
    }
    
 // Set the enable pin HIGH to stop retrieving data (sets all modules HIGH)
 SelectIOModule(MODULE_ID_NONE);
 
 if (bytesVal != g_DigitalInputStates[MODID_TO_INDEX(INPUT, moduleId)])
 {
   // TODO: Trigger OnChange Event...
   g_DigitalInputStates[MODID_TO_INDEX(INPUT, moduleId)] = bytesVal; // Store new state
   PrintInputs(moduleId);
 }
 
 return bytesVal; 
}

void UpdateIO(int moduleId)
{
  // Latch input states
  digitalWrite(SLOAD_PIN, LOW);
  // let the values latch in the shift register
  delayMicroseconds(5);
  // Reset latch pin (data has been captured)
  digitalWrite(SLOAD_PIN, HIGH);  
}

void loop() 
{
//  SetOutputByte(5, 0x55);
//  
//  for (int i = 0; i < 5; i++)
//  {
//    SetOutputByte(5, 0x10 >> i);
//    SetOutputByte(4, 0x01 << i);
//    GetInputByte(0);
//    delay(500);
//  }
  SetOutputByte(4,GetInputByte(0) >> 1);
}
