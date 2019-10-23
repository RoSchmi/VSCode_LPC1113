#include "lpc_types.h"
#include <stdio.h>

#ifndef __UART_LINE_H_
#define __UART_LINE_H_

typedef struct {
	void *data;
	char inputString[50];
	bool cmdIsReady;
	int inputIndex;
} UART_LINE_T;

int Uart_Line_Reset(UART_LINE_T *uartLine, void *buffer);
int Uart_Line_Append(UART_LINE_T *uartLine, char appChar);
int UART_Line_GetInpStr(UART_LINE_T *uartLine, void *inpStr);
bool Uart_Line_IsReady(UART_LINE_T *uartLine);

#endif /* __UART_LINE_H_ */