// Blinks a LED (GPIO)
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

#include <ring_buffer.h>

/* Transmit and receive ring buffers */
STATIC RINGBUFF_T txring, rxring;

/* Transmit and receive ring buffer sizes */
#define UART_SRB_SIZE 128	/* Send */
#define UART_RRB_SIZE 32	/* Receive */

/* Transmit and receive buffers */
static uint8_t rxbuff[UART_RRB_SIZE], txbuff[UART_SRB_SIZE];

// LED functions
extern void LED_Config(void);
extern void LED_Set(void);
extern void LED_Clear(void);

// UART functions
extern void UART_puts(char * mytext);

extern uint32_t uart_get_intr_IIR(void);
extern void uart_setup(void);
extern void uart_sendch(int);
extern int uart_status_rxd_error(void);
extern int  uart_status_rxd(void);
extern char uart_getch(void);

extern void timer32_0_config(void);
extern void misc_IO_config(void);

volatile uint32_t  time_counter;      // Timer for creating time delay

int UART_ReadRB(RINGBUFF_T *pRB, void *data, int bytes);
void   Delay(uint32_t nCount);
void time_delay(int loop);
bool ledState = false;

int32_t ctr;

int32_t key;
int32_t bytes;

int main(void)
{
    __NOP();
    __NOP();
	  ctr = 0;
  #define LOOP_COUNT 0x80000
  uint32_t loop_Counter = LOOP_COUNT;
   
  // Configure LED outputs
  LED_Config();

  /* Before using the ring buffers, initialize them using the ring
	   buffer init function */
	RingBuffer_Init(&rxring, rxbuff, 1, UART_RRB_SIZE);
	RingBuffer_Init(&txring, txbuff, 1, UART_SRB_SIZE);

  uart_setup();
  
  UART_puts("\r\nSimple UART echo test\r\n");

  misc_IO_config();
	
  	time_counter = 20; // Stop for 1 second

  timer32_0_config(); // Enable timer interrupt

  while(1)
  { 
    time_delay(loop_Counter);	  
	  ctr++;

    

    key = 0;
	  while (key != 27) 
    {
      time_delay(loop_Counter);    			          
      bytes = UART_ReadRB(&rxring, &key, 1);                          
		  if (bytes > 0) 
      {
          volatile uint32_t result = key;                             
          uart_sendch(result);                      
			}
      ctr++;
      char counter [16];
      sprintf(counter, "%d", (int)ctr);
      // UART_puts(counter);
      // UART_puts("\r\n");
		}
    time_delay(loop_Counter);
   }
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






