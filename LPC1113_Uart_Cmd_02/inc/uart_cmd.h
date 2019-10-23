#include "lpc_types.h"
#include <stdio.h>

#ifndef __UART_CMD_H_
#define __UART_CMD_H_

typedef struct {
	void *data;  
    char inputString [40];
    bool cmdIsReady;
    bool cmdIsEmpty;
    int inputIndex;
	char cmdName [10];
    char par1 [8];
    char par2 [8];
    char par3 [8];   
} UART_CMD_T;

int Uart_Cmd_Reset(UART_CMD_T *uartCmd, void *buffer);
//int Uart_Cmd_Reset(void*);
int Append(UART_CMD_T *uartCmd, char appChar);
int UART_Cmd_GetInpStr(UART_CMD_T *uartCmd, void *inpStr);
//int UART_Cmd_GetInpStr(UART_CMD_T *uartCmd, char *inpStr);

bool Uart_Cmd_IsReady(UART_CMD_T *uartCmd);

#endif /* __UART_CMD_H_ */