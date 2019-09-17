// Blinks a LED (GPIO)
// Sends messages via Uart
// Receives and returns messages via Uart



// UART TXD is PIO_1_7
// UART RXD is PIO_1_6
// System running at 48MHz
// Baud rate 38400 at system clock 48MHz

#include <LPC11xx.h>
#include <cmsis_gcc.h>

// LED functions
extern void LED_Config(void);
extern void LED_Set(void);
extern void LED_Clear(void);


// UART functions
extern void UART_config(void);
extern char UART_putc(char ch);
extern char UART_getc(void);
extern void UART_echo(void);
extern void UART_puts(char * mytext);

void   Delay(uint32_t nCount);

int32_t storeVar = 100;
int32_t ctr = 100;


int main(void)
{
    __NOP();
    __NOP();
	  ctr = 0;
#define LOOP_COUNT 0x80000
  uint32_t loop_Counter = LOOP_COUNT;
   
  // Configure LED outputs
  LED_Config();

	// Initialize UART
  UART_config();
  
	UART_puts("Simple UART echo test\r\n");
	
  while(1){

    __NOP();    // To easily recognize this code in assembly
    __NOP();
    __NOP();
    __NOP();
    __NOP();
	  
    Delay(loop_Counter);	  
	  ctr++;
    LED_Set();
	
	  UART_puts("On\r\n");
    Delay(loop_Counter);
			
    LED_Clear();
	  UART_puts("Off\r\n");
   };
}

void Delay(uint32_t nCount)
{
  while(nCount--)
  {
	
    __NOP();
	 // Echo UART inputs if any
		UART_echo();
  }
}





