
#pragma once
#ifndef __PARSE_FOR_RESPONSE_H_
#define __PARSE_FOR_RESPONSE_H_

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "isNumber.h"

bool parse_for_resp(char * cmd, char * par1, char * par2, char * par3, void * returnString, uint32_t bufLen);

#endif


