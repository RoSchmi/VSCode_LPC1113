//#include "pch.h"
#include "parse_for_response.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <isNumber.h>

//#include <iostream>
//#include <sstream>
//#include <cctype>
//#include <algorithm>

//#define memcpy std::memcpy

//using namespace std;

// UART functions

const char * help = "Possible commands:\r\nDump, [par1[auto] | [Address like 0x00341234], [cnt]\r\n";



extern void UART_puts(char * mytext);
extern uint32_t htoi(char * strIn);

extern bool isNumberMax_8(char number[], numBase select);
extern bool uint32_t_ToHex(uint32_t num, void * hexStr, uint32_t hexStrLength);

bool parse_for_resp(char * cmd, char * par1, char * par2, char * par3, void * returnString, uint32_t bufLen)
{
	volatile uint8_t * byteMemPtr = 0;
	
	volatile bool returnResult = true;

	char theAnswer[100] = "";

	char * command = cmd;

	for (int i = 0; i < strlen(cmd); i++)   // Set cmd to lowercase
	{
		command[i] = tolower(command[i]);
	}
	
	if (!strcmp(command, "?"))
	{       
	    UART_puts((char*)help);
		return false;
	}	   
	else if (!strcmp(command, "dump"))
			{
				char adr[11] = "";
				if (!strcmp(par1, "this"))
				{
					byteMemPtr = (uint8_t*)cmd;									
				}
				else
				{			    
					// Solution for C++
					// https://en.cppreference.com/w/cpp/string/basic_string/stoul
					// https://www.geeksforgeeks.org/stringstream-c-decimal-hexadecimal-back/
					
					if (isNumberMax_8(par1, Hex))
					{				
						byteMemPtr = (uint8_t*)htoi(par1);															
					}
					else
					{
						byteMemPtr = 0;
						returnResult = false;
					}					
				}

				if (uint32_t_ToHex((uint32_t)byteMemPtr, &adr, sizeof(adr)))
				{					
					sprintf(theAnswer,"%s  0x%02x  %c",adr, *byteMemPtr, ((*byteMemPtr >= 32) && (*byteMemPtr <= 127)) ? (char)*byteMemPtr : ' ');                    
				}
				else
				{
					strcpy(theAnswer, "Internal Return Array was to small");
					returnResult = false;
				}
			}
		
	uint32_t allowedLength = bufLen -1 > strlen(theAnswer) ? strlen(theAnswer) : bufLen -1;
    if (allowedLength < strlen(theAnswer))
	{
		returnResult = false;
	}		
	memcpy(returnString, theAnswer, allowedLength);
			
	return returnResult;
}					

	
	
