// y+=v;
// v-=y/k;      <- pr›v † finne shift.

/*
 y = 1;
 v = 0;

// fixed
// k m† v‘re stor nok til † fylle alle "hull" i tabellen
 for (i=0;i<tabell_size;i++)
 {
  cos(i) = y;
  y+=v;
  v-=y/k;
 }
 sjekk at y = 1; /* cos(0) */

// start:
//  x = "topp"
//  v = 0
//  k = konstant (oppl›sning nok for † fylle 100%)

//           sin(v)
// atan(v) = ------
//           cos(v)

#include <conio.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#define USE_CLIPPING

// -----

void SetMode(short int mode);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [eax];

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

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void main(void)
{
 float i,k,v,y;
 SetMode(0x13);

// k m† v‘re stor nok til † fylle alle "hull" i tabellen
 y = 160;
 v = 0;
 k = 1650;
 for (i=0;i<256;i++)
 {
  PutPixel(i,100-(y/2),15);
  y+=v;
  v-=y/k;
 }
// sjekk at y = 1; /* cos(0) */

 getch();
 SetMode(3);

}
