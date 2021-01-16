#include <conio.h>
#include "d:\axon\axon.c"

char *back;

/* CALCULATE BACKGROUND */
void CalcBackground(char *buffer)
{
 short unsigned int from,to;
 int counter1,counter2;
 unsigned short data;
 from=0;
 to=0;
 for (counter1=0;counter1!=10;counter1++)
 {
  for (counter2=0;counter2!=65536;counter2++)
  {
   data = buffer[from+=2];      /* screen */
   if ((data&1)==1) (to+=321); else (to+=320);
   buffer[to]++;                /* screen */
  }
 }
}

/* SMOOTH BACKGROUND */
void Smooth(char *buffer)
{
 short unsigned int from;
 unsigned short data;
 for (from=1;from<318;from++)
 {
  buffer[from] = (buffer[from-1]+buffer[from+1]) >> 1;
 }
 for (from=320;from<64000;from++)
 {
  buffer[from] = (buffer[from-320]+buffer[from-1]+buffer[from+1]+buffer[from+320]) >> 2;
 }
}

// --------------------------------------------------------------------------

void DrawBump(int x1, int y1, char *scr, char *map)
{
 int i,j;
 int c11,c12;
 int nx1,ny1;
 int lx1,ly1;
 int c21,c22;
 int nx2,ny2;
 int lx2,ly2;

 int di;
 int c;

 scr += 321;
 di = 321;
 ly1 = 1-y1;
 for (i=1;i<199;i++)
 {
  lx1 = 1-x1;
  for (j=1;j<319;j++)
  {
   nx1 = map[di+1] - map[di-1];
   c11 = 127-AsmAbs((nx1-lx1));
   if (c11 < 0) c11 = 0;

   ny1 = map[di+320] - map[di-320];
   c12 = 127-AsmAbs(ny1-ly1);
   if (c12 < 0) c12 = 0;

   c = (c11*c12) >> 7;//(128);
   if (c > 63) c = 63;
   *scr++ = c*4;
   di++;
   lx1++;
  }
  di+=2;
  scr+=2;
  ly1++;
 }
}

// -----

void main(void)
{
 int i;
 back = (char *)malloc(65536);
 CalcBackGround(back);
 Smooth(back);
 SetMode(0x13);
 for (i=0;i<256;i++) setrgb(i,i/4,i/4,i/4);
 DrawBump(100,100,(char *)0xA0000,back);
 getch();
 SetMode(0x3);
 free(back);
}
