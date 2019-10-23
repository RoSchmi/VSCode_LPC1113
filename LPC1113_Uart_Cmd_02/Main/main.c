// Copyright RoSchmi 2019
// This program runs on the NXP Lpc113 MCU. 
// The program uses the uart, where it opens a Command Line. 
// You can enter commands through a terminal program (e.g. teraterm) on your PC which
// is connected through a FTDI USB/Serial adapter.
// You can enter a command with max. 3 parameters separated by one or more spaces.
// 
// Actually the only realized command is 'dump', followed by 'this' or a memory address in hex like '0x00541234' and the number of addresses to dump
// 'this' means that the values of the first 16 memory addresses starting with the address of the Command variable cmd are printed.

// The following functions can be realized
// Blinking of a LED (GPIO)
// Uses a timer interrupt
// Receives and returns messages via Uart
// Receive uses interrupts

// UART TXD is PIO_1_7
// UART RXD is PIO_1_6
// System running at 48MHz
// Baud rate 38400 at system clock 48MHz

#include <LPC11xx.h>
#include <cmsis_gcc.h>
#include <stdio.h>

//#include <lpc_types.h>

#include <ring_buffer.h>

#include <uart_cmd.h>
#include <uart_line.h>
#include <cmd_split.h>
#include <parse_for_response.h>
#include <uint32_t_ToHexString.h>

#include <stdint.h>
#include <stdio.h>
#include "string.h"
#include "charToInt.h"
#include "isNumber.h"


/* Transmit and receive ring buffers */
// STATIC RINGBUFF_T txring, rxring;
static RINGBUFF_T txring, rxring;

/* Transmit and receive ring buffer sizes */
#define UART_SRB_SIZE 128	/* Send */
#define UART_RRB_SIZE 32	/* Receive */

/* Transmit and receive buffers */
static uint8_t rxbuff[UART_RRB_SIZE], txbuff[UART_SRB_SIZE];

// UART_CMD

// LED functions
extern void LED_Config(void);
extern void LED_Set(void);
extern void LED_Clear(void);

// UART functions
extern void UART_puts(char * mytext);


extern int Uart_Line_Append(UART_LINE_T *uartLine, char appChar);
extern int UART_Line_GetInpStr(UART_LINE_T *uartLine, void *inpStr);
extern int Uart_Line_Reset(UART_LINE_T *uartLine, void *buffer);



extern uint32_t uart_get_intr_IIR(void);
extern void uart_setup(void);
extern void uart_sendch(int);
extern int uart_status_rxd_error(void);
extern int  uart_status_rxd(void);
extern char uart_getch(void);

extern void timer32_0_config(void);
extern void misc_IO_config(void);

// volatile uint32_t  time_counter;      // Timer for creating time delay

int UART_ReadRB(RINGBUFF_T *pRB, void *data, int bytes);

void printAddresses(char * StartAddress, char *  cnt);

void   Delay(uint32_t nCount);
void time_delay(int loop);
bool ledState = false;

extern bool isNumberMax_8(char number[], numBase select);
extern uint32_t htoi(char * strIn);
extern bool uint32_t_ToHex(uint32_t num, void * hexStr, uint32_t hexStrLength);
int char2int(char * array, int n);
extern int32_t SplitCommand(char * InBuffer, void * OutCmdBuffer, void * OutPar_1_Buffer, void * OutPar_2_Buffer, void * OutPar_3_Buffer);

// int32_t ctr;

int32_t key;
int32_t bytes;

const int32_t iNumChars = 50;
const char iNull_Char = 1;

//char buffer[iNumChars + iNull_Char];

char buffer[51];
char outPutText[50];
int32_t bufCnt = 0;

char cmdName[10];
char par_1[10];
char par_2[10];
char par_3[10];

//char readStr[iNumChars + iNull_Char];
char readStr[51];

uint32_t readStrIndex = 0;

// STATIC UART_CMD_T uartCmd;

static UART_LINE_T uartLine;

int main(void)
{
    __NOP();
    __NOP();
    Uart_Line_Reset(&uartLine, &buffer);
  
    #define LOOP_COUNT 0x80000
    uint32_t loop_Counter = LOOP_COUNT;
   
    // Configure LED outputs
    LED_Config();

    /* Before using the ring buffers, initialize them using the ring
	   buffer init function */
	  RingBuffer_Init(&rxring, rxbuff, 1, UART_RRB_SIZE);
	  RingBuffer_Init(&txring, txbuff, 1, UART_SRB_SIZE);

    uart_setup();
  
    UART_puts("\r\nEnter a Command: CommandName and max. 3 parameters. \r\n");
	  UART_puts("Actually the only realized command is 'dump', followed by 'this' as second parameter\r\n");
    UART_puts("or a memory address in hex like '0x00541234' followed by the number of addresses to dump\r\n");
	  UART_puts("'this' means that the values of the first 16 memory addresses starting with the address\r\n");
    UART_puts("of the command-name ('dump') are printed. \r\n>");
  
    misc_IO_config();
	
    timer32_0_config(); // Enable timer interrupt

  while(1)
  { 
    time_delay(loop_Counter);	  
    key = 0;
	  while (key != 27) 
    {
      time_delay(loop_Counter);    			          
      bytes = UART_ReadRB(&rxring, &key, 1);                          
		  if (bytes > 0) 
      {
          uart_sendch(key); 
          Uart_Line_Append(&uartLine, (char)key);         
          bufCnt = UART_Line_GetInpStr(&uartLine, &buffer);    // returns with bufcnt > 0 when string is complete
		      if (bufCnt > 0)
          {
            SplitCommand(buffer, &cmdName, &par_1, &par_2, &par_3);
            if (!strcmp(cmdName, "dump"))
            {
                memset(buffer, 0, sizeof(buffer));
                char startAddr[11] = "";               
                if (!strcmp(par_1, "this"))          
                {
			              parse_for_resp(cmdName, par_1, par_2, par_3, &buffer, sizeof(buffer));                   
                    memcpy(startAddr, buffer, strlen(buffer) < 10 ? strlen(buffer) : 10);
                    char count[]= "16";                 
                    printAddresses(startAddr, count);
                }
                else
                {
                    parse_for_resp(cmdName, par_1, par_2 , par_3, &buffer, sizeof(buffer));
                    memcpy(startAddr, buffer, strlen(buffer) < 10 ? strlen(buffer) : 10);                                    
                    printAddresses(startAddr, strcmp(par_2, "") ? par_2 : "1\0");
                }                              
            }
         
            // Reset all buffers
			      memset(buffer, 0, sizeof(buffer));
			      memset(cmdName, 0, sizeof(cmdName));
			      memset(par_1, 0, sizeof(par_1));
			      memset(par_2, 0, sizeof(par_2));
			      memset(par_3, 0, sizeof(par_3));        
          }                        		     
		}
    time_delay(loop_Counter);
   }
  }
}

//void printAddresses(char * StartAddress, uint32_t  cnt)
void printAddresses(char * StartAddress, char * counter)
{
  char dumpCmd[6] = "dump";
  char actHexAddress[11] = "";
  int cnt = 1;
  
  if (isNumberMax_8(counter, Hex))
  {
       cnt = htoi(counter);
  }
  else
  {
    if (isNumberMax_8(counter, Dec))
    {    
     cnt = char2int((char*)counter, strlen(counter));
    }
  }
    
  volatile uint32_t actIntAddress = 0;
  
  if (isNumberMax_8(StartAddress, Hex))
  {
       actIntAddress = htoi((char*)StartAddress);
  }
  UART_puts("\r\n");
  for (int i = 0; i < cnt; i++)
  {
    //memset(actHexAddress, 0, sizeof(actHexAddress));
    // memset(buffer, 0, sizeof(buffer));
    uint32_t_ToHex(actIntAddress, actHexAddress, sizeof(actHexAddress));
    memset(buffer, 0, sizeof(buffer));
    parse_for_resp(dumpCmd, actHexAddress, "", "", &buffer, sizeof(buffer));
    UART_puts(buffer);
    UART_puts("\r\n");
    actIntAddress++;
  } 
  UART_puts("\r\n>");
}


/* Copy data from a receive ring buffer */

// int Chip_UART_ReadRB(LPC_USART_T *pUART, RINGBUFF_T *pRB, void *data, int bytes)
int UART_ReadRB(RINGBUFF_T *pRB, void *data, int bytes)
{
   volatile int result = RingBuffer_PopMult(pRB, (uint8_t *) data, bytes);
   return result;
}


/* Read data through the UART peripheral (non-blocking) */
//int UART_Receive(LPC_USART_T *pUART, void *data, int numBytes)
int UART_Receive( void *data, int numBytes)
{
	int readBytes = 0;
	uint8_t *p8 = (uint8_t *) data;

	/* Send until the transmit FIFO is full or out of bytes */
	while ((readBytes < numBytes) && (uart_status_rxd()) != 0)
  {    	
    *p8 = uart_getch();
		p8++;
		readBytes++;
	}

	return readBytes;
}

void UART_IntReceive(void)
 {
        uint8_t tmpc;
        volatile uint32_t rLen;       
         while(1)
         {     
                rLen = UART_Receive(&tmpc, 1);
                // If data received
                 if (rLen)
                 {
                    /* Check if buffer is more space */
                    /* If no more space, remaining character will be trimmed out */
                          
                    if (!RingBuffer_IsFull(&rxring))
                    {
                        RingBuffer_Insert(&rxring, &tmpc);                       
                    }                       
                 }
                // no more data
                 else 
                 {                   
                    break;
                 }
        }
 }

void time_delay(int loop)
{
  int i;
  for (i=loop;i>0;i--){
    __NOP();
  }
  return;
}
// ----------------------------------------------
// 32-bit Timer for periodic input and sensor sampling,
// state machine operations
void timer32_0_config(void)
{
  LPC_TMR32B0->PR  = 48;         // Prescale register. Reduce to 1MHz
  LPC_TMR32B0->MR3 = 50000 - 1;  // 20 Hz
  LPC_TMR32B0->MCR = (1<<10)|(1<<9); // Reset & interrupt on MR3 match
	
  LPC_TMR32B0->TCR = 1;   // Enable
  
  NVIC_SetPriority(TIMER_32_0_IRQn, 3); // 3 = Low Prority, 0 = high priority
  NVIC_EnableIRQ(TIMER_32_0_IRQn);
  return;
}
// ----------------------------------------------

void UART_IRQHandler()
{
  __NOP();

  // Read Line Status register
  // Bit 0      =   Receiver data ready
  // Bit 1      =   Overrun Error
  // Bit 2      =   Parity Error
  // Bit 3      =   Framing Error

  // Bit 4      =   Break Interrupt
  // Bit 5      =   Transmit holding register empty
  // Bit 6      =   Transmitter empty
  // Bit 7      =   Error in RX FIFO

  volatile int theLineReceivedStatus = uart_status_rxd();
  if (theLineReceivedStatus == 1)
  {
    volatile int theErrorStatus = uart_status_rxd_error();
    if (theErrorStatus != 0)
    {
      __NOP();
      // Do something to handle the errors
    }
    else
    {
        UART_IntReceive();
    }  
  }
  return;
}

// ----------------------------------------------
// Timer32 0 to handle sensor input sampling, 
// user button sampling
// operation state machine
void TIMER32_0_IRQHandler(void)	
{
  LPC_TMR32B0->IR = (1<<3); // Clear Interrupt request

  if (ledState)
  {
    LED_Set();
  }
  else
  {
    LED_Clear();
  }
  ledState = !ledState; 
  return;
}







// ----------------------------------------------
// LED, button and clock control
void misc_IO_config(void)
{
  // Enable clocks IO config block
  // Bit 16: IO config
  // Bit 13: ADC
  // Bit 12: UART
  // Bit 10: CT32B1
  // Bit 9 : CT32B0
  // Bit 8 : CT16B1
  // Bit 7 : CT16B0
  // Bit 6 : GPIO
  LPC_SYSCON->SYSAHBCLKCTRL |=  ((1<<16) | (1<<9) | (1<<7) | (1<<6));

  // LED       - P1_5
  // PIO1_5 IO output config
  //  bit[10]  - Open Drain
  //  bit[5]   - Hysteresis (0=disable, 1 =enable)
  //  bit[4:3] - MODE(0=inactive, 1 =pulldown, 2=pullup, 3=repeater)
  //  bit[2:0] - Function (0 = IO, 1=RTS, 2=CT32B0_CAP0)
  LPC_IOCON->PIO1_5 = (0<<10) | (0<<5) | (0<<3) | (0x0);
  LPC_GPIO1->DIR    |=  (1<<5); // Output
  
	//set_LED_val(0);
	
  // Button    - P1_8
  // PIO1_8 IO output config
  //  bit[10]  - Open Drain
  //  bit[7]   - Analog mode
  //  bit[5]   - Hysteresis (0=disable, 1 =enable)
  //  bit[4:3] - MODE(0=inactive, 1 =pulldown, 2=pullup, 3=repeater)
  //  bit[2:0] - Function (0 = IO, 1=AD5, 2=CT32B0_MAT3)
  LPC_IOCON->PIO1_8 = (0<<10) | (0<<5) | (0<<3) | (0x0);
	
  return;
}






