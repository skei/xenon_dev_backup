#include <stdlib.h>

// -----

#define SkurrNum 10
#define SkurrMax 20

// -----

int Skurr_Array[200][SkurrNum];

// -----

void Skurr_Init(void)
 {
  int i,j;
  float displace;
  float delta;

  for (i=0;i<200;i++)
  {
   displace = (rand()%(SkurrMax*2))-SkurrMax;
   delta = displace/SkurrNum;
   for (j=SkurrNum;j>=0;j--)
   {
    Skurr_Array[i][j] = (int)displace;
    displace-=delta;
   }
  }
 }

// -----

void Skurr_Draw(char *dst, char *src, int num)
 {
  int i;
  int displace;

  if (num == 0)
  {
   memcpy (dst,src,64000);
   return;
  }
  if ((num >= SkurrNum) || (num < 0)) return;
  memcpy(&dst[0],&src[0],320);
  memcpy(&dst[199*320],&src[199*320],320);
  for (i=1;i<199;i++)
  {
   displace = Skurr_Array[i][num];
   memcpy(&dst[i*320],&src[(i*320)+displace],320);
  }
 }

