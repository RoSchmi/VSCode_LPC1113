

#include "LPC11xx.h"
#include "uart_line.h"
#include <string.h>
#include <stdint.h>

char * inpStr;

char inpBuffer[50];

int Uart_Line_Reset(UART_LINE_T *uartLine, void *buffer)
{
	uartLine->data = buffer;
	memset(&uartLine->inputString[0], 0, sizeof(uartLine->inputString));	
	uartLine->cmdIsReady = false;
	uartLine->inputIndex = 0;
	return 0;
}

int Uart_Line_Append(UART_LINE_T *uartLine, char appChar)
{
	volatile char theComp = '\r';
	if (appChar != theComp)
	{	
		uartLine->inputString[uartLine->inputIndex] = appChar;		
		uartLine->inputIndex = uartLine->inputIndex + 1;		
	}
	else
	{
		if (uartLine->inputIndex != 0)
		{			
			uartLine->inputString[uartLine->inputIndex] = appChar;
			uartLine->cmdIsReady = true;			
		}
	}
	return (uint32_t)appChar;
}

int UART_Line_GetInpStr(UART_LINE_T *uartLine, void * inpStr)
{
	
	const void *ptr = uartLine->inputString;
	volatile uint8_t cnt = uartLine->inputIndex;
	memcpy(inpStr, ptr, cnt);
	
	if (!uartLine->cmdIsReady)
	{
		cnt = 0;
	}
	else
	{
		uartLine->cmdIsReady = false;
		uartLine->inputIndex = 0;		
		memset(&uartLine->inputString[0], 0, strlen(uartLine->inputString));
	}
	return (cnt);
}

bool Uart_Line_IsReady(UART_LINE_T *uartLine)
{
	return (uartLine->cmdIsReady);
}



	