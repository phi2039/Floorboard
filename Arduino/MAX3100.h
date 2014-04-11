//   Bit Definitions (Configuration Mode - Write/Read)
//0:  B0/TEST - Baud-rate divisor select (bit 0) -/- Test mode
//1:  B1 - Baud-rate divisor select (bit 1)
//2:  B2 - Baud-rate divisor select (bit 2)
//3:  B3 - Baud-rate divisor select (bit 3)
//4:  L - Word Length (0 = 8/9-bit, 1 = 7-bit)
//5:  PE - Parity Enable (0 = parity bit not appended, 1 = parity bit appended). Can be used to emulate 9-bit word length
//6:  ST - Stop Bit (0 = 1 bit, 1 = 2 stop bits)
//7:  IR - IrDA Timing Mode (0 = disable, 1 = enable)
//8:  ~RAM - Mask for RA_FE bit. IRQ is asserted if ~RAM = 1 and RA_FE = 1
//9:  ~PM -  Mask for Pr bit. IRQ is asserted if ~PM = 1 and Pr = 1
//10: ~RM -  Mask for R bit. IRQ is asserted if ~RM = 1 and R = 1
//11: ~TM -  Mask for T bit. IRQ is asserted if ~TM = 1 and T = 1
//12: SHDNo/SHDNi - Software Shutdown Enable/Indicator
//13: ~FEN - FIFO Enable (0 = enable, 1 = disable)
//14: 1 (Configuration Mode)
//15: READ/WRITE

//   Bit Definitions (Data Mode - Write/Read)
//0:  D0t/r - Data in/out (bit 0)
//1:  D1t/r - Data in/out (bit 0)
//2:  D2t/r - Data in/out (bit 0)
//3:  D3t/r - Data in/out (bit 0)
//4:  D4t/r - Data in/out (bit 0)
//5:  D5t/r - Data in/out (bit 0)
//6:  D6t/r - Data in/out (bit 0)
//7:  D7t/r - Data in/out (bit 0)
//8:  Pt/Pr - Transmit/Receive Parity Bit (Ignored if PE = 0)
//9:  RTS/CTS - Ready-to-send/Clear-to-send
//10: ~TE/RA_FE - Transmit Enable (0 = enable, 1 = disable) -/- Receiver-Activity(shutdown mode)/Framing Error(normal operation) Bit
//11: 0
//12: 0
//13: 0
//14: 0 (Data Mode)
//15: READ/WRITE

// NOTE: Bits 14 and 15 also represent the T and R bits, respectively, on the MISO line
// R - RX Data Available
// T - TX Buffer Empty

// A Read Data Operation Clears ~IRQ

#include <stdint.h>
#include "BusControl.h"

#define BAUD_DIV_1 0x0
#define BAUD_DIV_2 0x1
#define BAUD_DIV_4 0x2
#define BAUD_DIV_8 0x3
#define BAUD_DIV_16 0x4
#define BAUD_DIV_32 0x5
#define BAUD_DIV_64 0x6
#define BAUD_DIV_128 0x7
#define BAUD_DIV_3 0x8
#define BAUD_DIV_6 0x9
#define BAUD_DIV_12 0xA
#define BAUD_DIV_24 0xB
#define BAUD_DIV_48 0xC
#define BAUD_DIV_96 0xD
#define BAUD_DIV_192 0xE
#define BAUD_DIV_384 0xF

#define TEST_MODE_OFF (0x0)
#define TEST_MODE_ON  (0x1)

#define WORD_LEN_8 (0x0 << 4)
#define WORD_LEN_7 (0x1 << 4)

#define PARITY_OFF (0x0 << 5)
#define PARITY_ON  (0x1 << 5)

#define STOP_BITS_1 (0x0 << 6)
#define STOP_BITS_2 (0x1 << 6)

#define IRDA_DISABLE (0x0 << 7)
#define IRDA_ENABLE  (0x1 << 7)

#define IRQ_RA_ON        (0x1 << 8)
#define IRQ_RA_OFF       (0x0 << 8)

#define IRQ_PARITY_ON    (0x1 << 9)
#define IRQ_PARITY_OFF   (0x0 << 9)

#define IRQ_RXDATA_ON    (0x1 << 10)
#define IRQ_RXDATA_OFF   (0x0 << 10)

#define IRQ_TXEMPTY_ON   (0x1 << 11)
#define IRQ_TXEMPTY_OFF  (0x0 << 11)

#define SHUTDOWN_ENABLE  (0x1 << 12)
#define SHUTDOWN_DISABLE (0x0 << 12)

#define FIFO_ENABLE  (0x0 << 13)
#define FIFO_DISABLE (0x1 << 13)

#define RW_MODE_WRITE (0x1 << 15)
#define RW_MODE_READ  (0x0 << 15)

#define MSG_MODE_CONFIG (0x1 << 14)
#define MSG_MODE_DATA   (0x0 << 14)

#define TX_BUF_EMPTY  (0x1 << 14)
#define RX_DATA_READY (0x1 << 15)

#define TX_PARITY_1   (0x1 << 8)
#define TX_PARITY_0   (0x0 << 8)

#define RX_PARITY_1   (0x1 << 8)
#define RX_PARITY_0   (0x0 << 8)

#define CTS_RESET (0x0 << 9)
#define CTS_SET   (0x1 << 9)

#define RTS_RESET (0x0 << 9)
#define RTS_SET   (0x1 << 9)

#define TX_ENABLE  (0x0 << 10)
#define TX_DISABLE (0x1 << 10)

#define FRAMING_ERROR (0x1 << 10)
#define RCV_ACTIVITY  (0x1 << 10)

bool MAX3100_SendReceiveData(BusControl& busControl, int address, uint8_t dataOut, uint8_t* pDataIn = 0, bool printOK = true);
bool MAX3100_ReceiveData(BusControl& busControl, int address, uint8_t* pDataIn = 0, bool printOK = true);
void MAX3100_SetRTS(BusControl& busControl, int address);
void MAX3100_ResetRTS(BusControl& busControl, int address);
void MAX3100_ConfigureUART(BusControl& busControl, int address, uint16_t config);
uint16_t MAX3100_ReadUARTConfig(BusControl& busControl, int address);
