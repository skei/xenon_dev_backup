#include <conio.h>

// set video mode
void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

void spline(int x1, int y1, int x2, int y2, int x3, int y3, int steps)
{
 int sx,sy,dx1,dx2,dy1,dy2,sdx,sdy,l;
 int deep,deep2,ox,oy;
 int _x1,_y1,_x2,_y2,_x3,_y3;

 _x1 = x1;
 _y1 = y1;
 _x2 = x2;
 _y2 = y2;
 _x3 = x3;
 _y3 = y3;

 dx1   = (x2-x1);
 dx2   = (x3-x2);
 dy1   = (y2-y1);
 dy2   = (y3-y2);
 deep  = steps;
 deep2 = deep*deep;
 sdx = dx1*(deep*2-1)-dx2;
 sdy = dy1*(deep*2-1)-dy2;
 dx1 += dx1;
 dx2 += dx2;
 dy1 += dy1;
 dy2 += dy2;
 sx = 0; sy = 0; x2 = x1; y2 = y1;

 for (l=0;l<deep;l++)
 {
  ox = x2;
  oy = y2;
  x2 = x1+sx / deep2;
  y2 = y1+sy / deep2;

  *(char *)(0xA0000+y2*320+x2) = 15;
  *(char *)(0xA0000+oy*320+ox) = 15;

  sx += sdx;
  sy += sdy;
  sdx += dx2-dx1;
  sdy += dy2-dy1;
 }
 *(char *)(0xA0000+_y1*320+_x1) = 1;
 *(char *)(0xA0000+_y2*320+_x2) = 1;
 *(char *)(0xA0000+_y3*320+_x3) = 1;
}

void main(void)
{
 SetMode(0x13);
 Spline(10,10,160,190,310,100,64);
 getch();
 SetMode(3);
}

