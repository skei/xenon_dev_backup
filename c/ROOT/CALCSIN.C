#include <math.h>

volatile float floatvar;

void _sin(void);
#pragma aux _sin=\
 "fld [floatvar]",\
 "fsin",\
 "fstp [floatvar]";

void main(void)
{
 float test;


 for (test=0;test<5;test+=0.2)
 {
  floatvar = test;
  _sin();
  printf("_sin(%5.2f) = %5.2f  þ  sin(%5.2f) = %5.2f\n",test,floatvar,
                                                        test,sin(test));
 }
}
