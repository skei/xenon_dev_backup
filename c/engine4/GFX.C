// Low level graphics primitives

#include "gfx.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#define USE_CLIPPING

// -----

void PutPixel(int x, int y, char color)
{
 #ifdef USE_CLIPPING
  if ( (x<SCREEN_WIDTH) && (x>=0) && (y<SCREEN_HEIGHT) && (y>=0) )
 #endif
 {
  *(char *)(0xA0000+y*SCREEN_WIDTH+x) = color;
 }
}

// -----

void SlowLine(int x1, int y1, int x2, int y2, char color)
{
 int slope,i,xx,yy;
 int temp;

 if ( (y1==y2) || (x1==x2) ) return;
 if ( y1 > y2 )
 {
  temp = y1; y1 = y2; y2 = temp;
  temp = x1; x1 = x2; x2 = temp;
 }
 if ((x2-x1) < (y2-y1))
 {
  slope = ((x2-x1)<<16)/(y2-y1);
  xx = x1 << 16;
  for (i=y1;i<=y2;i++)
  {
   PutPixel(xx >> 16,i,color);
   xx += slope;
  }
 }
 else
 {
  slope = ((y2-y1)<<16)/(x2-x1);
  yy = y1 << 16;
  for (i=x1;i<=x2;i++)
  {
   PutPixel(i, yy >> 16,color);
   yy += slope;
  }
 }
}
