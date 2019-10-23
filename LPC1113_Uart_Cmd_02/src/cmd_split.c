#include <stdint.h>
#include "cmd_split.h"
#include <string.h>

int32_t SplitCommand(char * InBuffer, void * OutCmdBuffer, void * OutPar_1_Buffer, void * OutPar_2_Buffer, void * OutPar_3_Buffer)
{
	char inpStr[51];
	uint32_t  lenInBuff = strlen(InBuffer);
	memcpy(inpStr, InBuffer, lenInBuff + 1);
	const char *delim = " ";
	uint32_t currTokenCount = 0;	
	char *token = strtok(inpStr, delim);

	while (token && (currTokenCount < 4)) 
	{	
			switch (currTokenCount)
			{
			case 0:
			{
				memcpy(OutCmdBuffer, token, strlen(token));
			}
			break;
			case 1:
			{
				memcpy(OutPar_1_Buffer, token, strlen(token));
			}
			break;
			case 2:
			{
				memcpy(OutPar_2_Buffer, token, strlen(token));
			}
			break;
			case 3:
			{
				memcpy(OutPar_3_Buffer, token, strlen(token));
			}
			break;
			}
			currTokenCount++;			
			token = strtok(NULL, delim);
	}
	return currTokenCount;
}