//  Convert a hexadecimal number (0xABCD or ABCD) to its respective integer
//  Changed from the original version of Louie Shi.
// https://github.com/louiejshi/HextoInteger/blob/master/HextoInteger.c


#include <stdio.h>
#include <string.h>
#include <stdint.h>
//#include <math.h>

#define MAXSIZE 1000

uint32_t htoi(char * strIn)
{
    volatile uint32_t i = 0;
    volatile uint32_t power = 0;
    volatile uint32_t length = 0;
    volatile uint32_t sum = 0;
    volatile uint32_t digit = 0;

    const uint32_t posValues[8] = {1, 16, 256, 4096, 65536, 1048576, 16777216, 268435456};

char str[11] = "";
memcpy(&str, strIn, 10); 
   
    if(str[i] == '0')           //Case starting with 0X
    {
        i++;
        if(str[i] == 'x' || str[i] == 'X')
        {
            i++;
            length = strlen(str) - 2;
            for(int j = length - 1 + 2; j >= 2; j--)
            {
                if(str[j] >= '0' && str[j] <= '9')
                {
                    digit = str[j] - '0';
                }
                else if(str[j] >= 'a' && str[j] <= 'f')
                {
                    digit = str[j] - 'a' + 10;
                }
                else if(str[j] >= 'A' && str[j] <= 'F')
                {
                    digit = str[j] - 'A' + 10;
                }
                
                volatile double res = posValues[power];
                
                sum = sum + (digit * res);
                power++;
            }
        }
    }
    else                        //Case without 0X
    {
        length = strlen(str);
        for(int j = length - 1; j >= 0; j--)
        {
            if(str[j] >= '0' && str[j] <= '9')
            {
                digit = str[j] - '0';
            }
            else if(str[j] >= 'a' && str[j] <= 'f')
            {
                digit = str[j] - 'a' + 10;
            }
            else if(str[j] >= 'A' && str[j] <= 'F')
            {
                digit = str[j] - 'A' + 10;
            }
                       
            volatile double res = posValues[power];   

                sum = sum + (digit * res);           
            power++;
        }
    }
    
    return sum;
}
