/* LPC1113_Test_2  */
/* Working C blinky example Visual Studio Code, CMake, GNU-Compiler Collection */
/* Board: GHI Electronics DL40 Module with NXP LPC1113F/301 MCU                */

#include "lpc11xx.h"
#include "cmsis_gcc.h"
#include "LPC1113_Test_2Config.h"
#include <stdint.h>
#include <stdio.h>

const uint32_t led_mask[] = {1UL << 18, 1UL << 19, 1UL << 1};
// LED #0, #1 are port B, LED #2 is port D

void   LED_Config(void);
void   LED_Set(void);
void   LED_Clear(void);

void   Delay(uint32_t nCount);

int32_t dummy;

/* Main */
int main(void)
{
   
  // Configure LED outputs
  LED_Config();

#define LOOP_COUNT 0x04800

  while(1)
  {
      __NOP();
      __NOP();
      
      __asm("nop");
       
	   Delay(LOOP_COUNT);
		
      LED_Set();
		
      Delay(LOOP_COUNT);
		
      LED_Clear();
   };
 
   int theInt = 1;
   if (theInt == 1)
   {
      return 0;
   }
   else
   {
     return 0; 
   }  
}

void Delay(uint32_t nCount)
{
  while(nCount--)
  {  
      
      __NOP();
      __NOP();
      __NOP();
      __NOP();
      __NOP();
      __NOP();
      __NOP();

      __asm("nop");  // I the same as __NOP()    
  }
}

void LED_Config(void)
{
  // Enable clocks to GPIO and IO config block
  // Bit 6: GPIO, bit 16: IO config
  LPC_SYSCON->SYSAHBCLKCTRL |=  ((1<<16) | (1<<6)); 

  __NOP(); // Short time delay
  __NOP();
  __NOP();
	
  // PIO1_5 IO output config
  //  bit[10]  - Open drain (0 = standard I/O, 1 = open drain)
  //  bit[5]   - Hysteresis (0=disable, 1 =enable)
  //  bit[4:3] - MODE(0=inactive, 1 =pulldown, 2=pullup, 3=repeater)
  //  bit[2:0] - Function (0 = IO, 1=~RTS, 2=CT32B0_CAP0)
  LPC_IOCON->PIO1_5 = (0<<10) | (0<<5) | (0<<3) | (0x0);

  // Optional: Turn off clock to I/O Config block to save power
  // LPC_SYSCON->SYSAHBCLKCTRL &=  ~(1<<16);

  // Set pin 8 as output
  LPC_GPIO1->DIR = LPC_GPIO1->DIR | (1<<5);	
  return;
}

/*------------------------------------------------------------------------------
  Switch on LEDs
 *------------------------------------------------------------------------------*/
void LED_Set(void)
{
  // Set bit 5 output to 1
  LPC_GPIO1->MASKED_ACCESS[1<<5] = (1<<5);
  return;
}

/*------------------------------------------------------------------------------
  Switch off LEDs
 *------------------------------------------------------------------------------*/

void LED_Clear(void)
{
  // Clear bit 5 output to 1
  LPC_GPIO1->MASKED_ACCESS[1<<5] = 0;
  return;	
}
