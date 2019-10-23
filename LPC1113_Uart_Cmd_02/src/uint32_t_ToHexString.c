#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool uint32_t_ToHex(uint32_t num, void * hexStr, uint32_t hexStrLength)
{
   uint32_t testValue = num;
   char result[11];
   char temp[11];
   uint32_t posValue;

   const uint32_t posValues[7] = {268435456, 16777216, 1048576, 65536, 4096, 256, 16};
   const char hexDigits[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    memset(result, 0, sizeof(result));
    memset(temp, 0, sizeof(temp));
    
    int strPos = 0;
    for (strPos = 0; strPos < 7; strPos++)
    {      
       posValue = testValue / posValues[strPos];      
       result[strPos] = hexDigits[posValue];      
       testValue -= (posValue * posValues[strPos]);
    }
    result[strPos] = hexDigits[testValue];
     
    if (hexStrLength < 5)
    {
       return false;
    }
    else
    {   
        for (int i = 0; i < hexStrLength -3; i++)
        {
            temp[i] = result[strlen(result) - (hexStrLength -3) + i];           
        }
        memset(result, 0, sizeof(result));
        strcpy(result, "0x");
        strcat(result, temp);
        memcpy(hexStr, result, strlen(result));
        return true;
    }
}

