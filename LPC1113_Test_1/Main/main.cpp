// A simple program that computes the squareroot of a number
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "LPC1113_Test_1Config.h"
#ifdef USE_MYMATH
#include "MathFunctions.h"
#endif

int main(int argc, char *argv[]) 
{
    argc = 9;

    if (argc < 2)
    {
        
    fprintf(stdout,"%s Version %d.%d\n",
            argv[0],
            LPC1113_Test_1_VERSION_MAJOR,
            LPC1113_Test_1_VERSION_MINOR);

   fprintf(stdout,"Usage: %s number\n",argv[0]);
return 1;
}

  double inputValue = atof(argv[1]);
  

//#ifdef USE_MYMATH
   double outputValue = sqrt(inputValue);

//#else

  //double outputValue = sqrt(inputValue);

//#endif
 
  fprintf(stdout,"The square root of %g is %g\n",
          inputValue, outputValue);
  return 0;
}
    