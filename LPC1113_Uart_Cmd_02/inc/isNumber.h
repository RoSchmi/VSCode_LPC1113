#ifndef __IS_HEX_NUMBER_H_
#define __IS_HEX_NUMBER_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

enum NumberBase {Hex, Dec};
typedef enum NumberBase numBase;

bool isNumberMax_8(char number[], numBase select);

#endif