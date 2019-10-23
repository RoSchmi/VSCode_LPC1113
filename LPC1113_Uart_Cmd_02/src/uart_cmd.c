#include "LPC11xx.h"
#include "uart_cmd.h"
#include <string.h>

char * inpStr;

int Uart_Cmd_Reset(UART_CMD_T *uartCmd, void *buffer)
{
      uartCmd->data = buffer;
      memset(&uartCmd->inputString[0], 0, sizeof(uartCmd->inputString));
      uartCmd->cmdIsEmpty = true;
      uartCmd->cmdIsReady = false;
      uartCmd->inputIndex = 0;
      memset(&uartCmd->cmdName[0], 0, sizeof(uartCmd->cmdName));
      memset(&uartCmd->par1[0], 0, sizeof(uartCmd->par1));
      memset(&uartCmd->par2[0], 0, sizeof(uartCmd->par2));
      memset(&uartCmd->par3[0], 0, sizeof(uartCmd->par3));
      
     return 0;  
}

int Uart_Cmd_Append(UART_CMD_T *uartCmd, char appChar)
{
   volatile uint32_t theStop1;
   volatile uint32_t theStop2;
   volatile uint32_t theStop3;
   volatile uint32_t theStop4;
   volatile char theComp = '\r';
   if (appChar != theComp)
   {
      theStop1 = appChar;
      uartCmd->inputString[uartCmd->inputIndex] = appChar;
      uartCmd->cmdIsEmpty = false;
      uartCmd->inputIndex = uartCmd->inputIndex + 1;
      theStop4 = appChar;
   }
   else
   {
      theStop2 = appChar;
      if (uartCmd->inputIndex != 0)
      {
        // UART_puts("\r\nString was received\r\n");
         uartCmd->cmdIsReady = true;
         theStop3 = appChar;
      }     
   }
   volatile uint32_t theReturn = theStop1;
   theReturn = theStop2;
   theReturn = theStop3;
   theReturn = theStop4;
   return theReturn;
}

int UART_Cmd_GetInpStr(UART_CMD_T *uartCmd, void* inpStr)
//int UART_Cmd_GetInpStr(UART_CMD_T *uartCmd, char *inpStr)
{
   //inpStr   = &uartCmd->inputString;
   const void *ptr = uartCmd->data;
   volatile uint8_t cnt = uartCmd->inputIndex;
   memcpy(inpStr, ptr, cnt);
   volatile uint8_t *theWatch = &inpStr;
   volatile uint8_t *theWatch2 = theWatch2;
   volatile uint8_t *theWatch3 = theWatch;
   if (theWatch3)
   {
      theWatch2 = theWatch3;
   }
   theWatch3 = &inpStr;
   //inpStr   = uartCmd->inputString;
   return (cnt);
}

bool Uart_Cmd_IsReady(UART_CMD_T *uartCmd)
{
   return (uartCmd->cmdIsReady);
}
