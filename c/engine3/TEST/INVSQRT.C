#include <math.h>

int temp;

 // -------------------------------------------------------------------------

float FastSqrt(const float number)
 {
  int i;

  i = 0x5f3759df - ((*((int *)&number))>>1);
  return *(float *)&i;
 }


 // -------------------------------------------------------------------------

float AsmFastSqrt(const float number);
 #pragma aux AsmFastSqrt=\
 " fstp [temp]",\
 " mov eax,[temp]",\
 " mov ebx,0x5f3759df",\
 " shr eax,1",\
 " sub ebx,eax",\
 " mov [temp],ebx",\
 " fld [temp]",\
 parm [8087] value [8087] modify [eax ebx];

 // -------------------------------------------------------------------------

void main(void)
{
 float f;
 float diff,largestdiff;

 largestdiff = 0;
 for (f=0.1;f<100;f+=0.1)
 {
  printf("1/sqrt(%f) = %f\n",f,1/sqrt(f));
  printf("FastSqrt(%f) = %f\n",f,FastSqrt(f));
  printf("AsmFastSqrt(%f) = %f\n",f,AsmFastSqrt(f));
  diff = (1/sqrt(f))-FastSqrt(f);
  if (fabs(diff) > largestdiff) largestdiff = diff;
  printf("Difference = %f   Largest = %f\n",diff,largestdiff);
  printf("-----\n");
 }
}
