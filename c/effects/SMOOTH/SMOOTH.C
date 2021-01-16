#include <dos.h>
// #include <io.h>
#include <stdio.h>
// #include <fcntl.h>
#include <conio.h>

// #include "smooth.h"

#define _IMG_WIDTH 256
#define _IMG_HEIGHT 53
#define _IMG_SIZE (_IMG_WIDTH * _IMG_HEIGHT)

// --------------------------------------------------------------------------

union REGS r;

int bLoadImage(void);
void ColorAdjust(void);
void CalcContour(void);
void CalcSmooth(int pos);
void Draw(void);

unsigned char acPalette[768];
unsigned char acImage[_IMG_SIZE];
unsigned char acContour[_IMG_SIZE];
unsigned char acSmooth[_IMG_SIZE];

// --------------------------------------------------------------------------

void bLoadImage(void)
{
 FILE *fp;
 fp = fopen("nemesis.raw","rb");
 fread(acPalette,1,32,fp);
 fread(acPalette,1,768,fp);
 fread(acImage,1,_IMG_SIZE,fp);
 fclose(fp);
}

// --------------------------------------------------------------------------

void ColorAdjust(void)
{
 int i;
 int uPalOff;

 for(i=0; i < 768; i++) acPalette[i]>>=2;
 for(i=0; i < 64; i++)
 {
  uPalOff=i * 3;
  acPalette[uPalOff]=acPalette[uPalOff + 192 * 3];
  acPalette[uPalOff + 1]=acPalette[uPalOff + 192 * 3 + 1];
  acPalette[uPalOff + 2]=acPalette[uPalOff + 192 * 3 + 2];
 }
 for(i=64; i < 256; i++)
 {
  uPalOff=i * 3;
  acPalette[uPalOff]=acPalette[63 * 3];
  acPalette[uPalOff + 1]=acPalette[63 * 3 + 1];
  acPalette[uPalOff + 2]=acPalette[63 * 3 + 2];
 }
 for(i=0; i < _IMG_SIZE; i++)
 {
  acImage[i]=(acImage[i] >= 192) ? (acImage[i] - 192) : 0;
 }
}

// --------------------------------------------------------------------------

#define _Image(x, y) acImage[(x) + _IMG_WIDTH * (y)]
#define _D 6

// --------------------------------------------------------------------------

int Median(int cx, int cy, int d)
{
 int Sum=0, Count=0;
 int x, y;

 for(y=cy - d; y <= cy + d; y++)
 {
  for(x=cx - d; x <= cx + d; x++)
  {
   Sum+=_Image(x, y);
   Count++;
  }
 }
 return Sum/Count;
}

// --------------------------------------------------------------------------

void CalcContour(void)
{
 int x, y, col;

 for(y=0; y < _IMG_HEIGHT; y++)
 {
  for(x=0; x < _IMG_WIDTH; x++)
  {
   if((x<_D)||(y<_D)||(x>=(_IMG_WIDTH-_D))||(y>=(_IMG_HEIGHT-_D))) col=0;
   else col=Median(x, y, _D);
   acContour[x + _IMG_WIDTH * y]=col;
  }
 }
}

// --------------------------------------------------------------------------

void CalcSmooth( int pos)
{
 int i, npos;

 npos = 128-pos;

 for(i=0; i < _IMG_SIZE; i++)
 {
  acSmooth[i]=((int)acContour[i] * pos + (int)acImage[i] * npos) >> 7;
 }
}

// --------------------------------------------------------------------------

#define _VID_GAP (320 - _IMG_WIDTH);

void Draw(void)
{
 char *pcVideo;
 int x, y, uOff, vOff;

 pcVideo=(char *)0xA0000;
 uOff = 0;
 vOff = 0;
 while(!(inp(0x3da) & 8));
 for(y=0; y <_IMG_HEIGHT; y++)
 {
  for (x=0; x<_IMG_WIDTH; x++) pcVideo[vOff++]=acSmooth[uOff++];
  vOff+=_VID_GAP;
 }
}

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void main(void)
{
 int iPos;

 iPos = 0;
 bLoadImage();
 ColorAdjust();
 CalcContour();

 r.w.ax = 0x13;
 int386(0x10,&r,&r);

 outp(0x3c8, 0);
 for(iPos=0; iPos < 768; iPos++) outp(0x3c9, acPalette[iPos]);
 iPos=0;
 while(!kbhit())
 {
  for(iPos=0; (iPos < 128) && !kbhit(); iPos++)
  {
   CalcSmooth(iPos);
   Draw();
  }
  for(iPos=128; (iPos > 0) && !kbhit(); iPos--)
  {
   CalcSmooth(iPos);
   Draw();
  }
 }
 getch();

 r.w.ax = 0x3;
 int386(0x10,&r,&r);

}

