// Accepts a string as char array and returns true if the following conditions are met:
// Not more than 10 characters
// Starts with '0x' or '0X'
// the following characters are '0'-'9' or 'a'-'f' or 'A'-'F' 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "isNumber.h"

bool isNumberMax_8(char number[], numBase select)
{
    int strLength = strlen(number);
    volatile bool returnValue = true;

    if (strLength > 10)
    {
        returnValue = false;
    }
    else
    {  
        if (select == Hex && number[0] != '0' && (number[1] != 'x' || number[1] != 'X'))
        {
            returnValue = false;
        }
        else
        {       
            for (int i = select == Hex ? 2 : 0 ; i < strLength; i++)
            {
                if (!(number[i] >= '0' && number[i] <= '9'))
                {
                    if (select == Dec)
                    {
                        return false;
                    }
                    if (!(number[i] >= 'a' && number[i] <= 'f'))
                    {
                        if (!(number[i] >= 'A' && number[i] <= 'F'))
                        {return false;}
                    } 
                }                          
            }        
        }   
    } 
    return returnValue; 
} 
