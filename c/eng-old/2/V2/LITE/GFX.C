#include <string.h>       // strchr
#include "defines.h"
#include "types.h"
#include "vars.h"

#include "font.inc"
#define fontwidth  1
#define fontheight 8
unsigned char s[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!.'\"?:";

// --------------------------------------------------------------------------

void Line(int x0, int y0, int x1, int y1, char color)
{
 int dx,ix,cx,dy,iy,cy,m,i,pos;

 if (x0<x1) { dx=x1-x0; ix=1; } else { dx=x0-x1; ix=-1; }
 if (y0<y1) { dy=y1-y0; iy=1; } else { dy=y0-y1; iy=-1; }
 if (dx>dy) m=dx; else m=dy;
 cx=cy=(m>>1);
 for (i=0;i<m;i++)
 {
  if ((x0>=MinX)&&(x0<=MaxX)&&(y0>=MinY)&&(y0<=MaxY))
  {
   _Buffer[y0*ScreenWidth+x0] = color;
  }
  if ((cx+=dx)>=m) { cx-=m; x0+=ix; }
  if ((cy+=dy)>=m) { cy-=m; y0+=iy; }
 }
}

// --------------------------------------------------------------------------

void DrawChar( int x, int y, unsigned char c, short int color)
{
 int i,j;
 unsigned char n;
 unsigned char cc;
 unsigned char *pscr;

 if (c==' ') return;
 cc = (int)strchr(s,c) - (int)s;
 pscr = &(_Buffer[(y*ScreenWidth)+x]);
// (short int *)( (int)VesaPtr + ( y * 640 ) * page + x*2 );
 for (i=0;i<fontheight;i++)
 {
  n = FontData[(cc*8)+i];
  if ((n & 128) != 0) pscr[0] = color;
  if ((n &  64) != 0) pscr[1] = color;
  if ((n &  32) != 0) pscr[2] = color;
  if ((n &  16) != 0) pscr[3] = color;
  if ((n &   8) != 0) pscr[4] = color;
  if ((n &   4) != 0) pscr[5] = color;
  if ((n &   2) != 0) pscr[6] = color;
  if ((n &   1) != 0) pscr[7] = color;
  pscr+=ScreenWidth;
 }
}

// --------------------------------------------------------------------------

void DrawString(int x1, int y1, char *s, short int color)
{
 int i;

 i=0;
 while (s[i] != 0)
 {
  DrawChar(x1,y1,s[i],color);
  x1+=fontwidth*8;
  i++;
 }
}

// --------------------------------------------------------------------------

