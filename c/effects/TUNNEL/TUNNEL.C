#include <math.h>
#include <string.h>
#include <stdio.h>

// --------------------------------------------------------------------------

void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

#define PI 3.1415926535

char *Xtable;
char *Ytable;
char *Xtable2;
char *Ytable2;

// --------------------------------------------------------------------------

void CalcTables( float r,float sd)
{
 float x,y;
 int blah;
 float yp;
 float xy2,l,z,t;
 int zc;
 unsigned char col;

 blah = 0;
 for ( y=-100; y<=99; y++ )
 {
  yp = y;
  for ( x=-160; x<=159; x++ )
  {
   xy2 = fabs(( x * x ) + ( y * y )/0.1);
   xy2 = sqrt( xy2 );
   if (xy2 != 0) l = r/xy2;
   z = l * sd;
   if ((x != 0) && (y != 0))
   {
    t = atan( y / x );
    t = 256 * t / ( 2 * PI );
    if ((x <= 0) && (y <= 0)) t++;
    if ((x > 0) && (y > 0)) t++;
    if (x >= 0) t = t+128;
   }
   zc = ceil( z / 4 );
   zc = zc % 256;
   col = zc;
   Xtable[blah] = col;
   col = t;
   if ((y == 0) && (x < 0)) col = 1;
   Ytable[blah] = col;
   blah++;
  }
 }
}

// -----

void CalcTables2( float r,float sd)
{
 float x,y;
 int blah;
 float yp;
 float xy2,l,z,t;
 int zc;
 unsigned char col;

 blah = 0;
 for ( y=-100; y<=99; y++ )
 {
  yp = y;
  for ( x=-160; x<=159; x++ )
  {
   xy2 = fabs(( x * x ) + ( y * y )/0.8);
   xy2 = sqrt( xy2 );
   if (xy2 != 0) l = r/xy2;
   z = l * sd;
   if ((x != 0) && (y != 0))
   {
    t = atan( y / x );
    t = 256 * t / ( 2 * PI );
    if ((x <= 0) && (y <= 0)) t++;
    if ((x > 0) && (y > 0)) t++;
    if (x >= 0) t = t+128;
   }
   zc = ceil( z / 4 );
   zc = zc % 256;
   col = zc;
   Xtable2[blah] = col;
   col = t;
   if ((y == 0) && (x < 0)) col = 1;
   Ytable2[blah] = col;
   blah++;
  }
 }
}


// --------------------------------------------------------------------------

void DrawTunnel(char *Screen, char *BitMap,char xadd, char yadd)
{
 int i;
 unsigned char x,y;

 for (i=0;i<64000;i++)
 {
  x = Xtable[i] + xadd;
  y = Ytable[i] + yadd;
  Screen[i] = BitMap[(y<<8)+x];
 }
}

// -----

void DrawTunnel2(char *Screen, char *BitMap,char xadd, char yadd, int panx, int pany)
{
 int i,ii;
 int xx,yy;
 unsigned char x,y;

 i = 0;
 for (yy=0;yy<200;yy++)
 {
  for (xx=0;xx<320;xx++)
  {
   ii = ((yy+pany)*640)+(xx+panx);
   x = Xtable[ii] + xadd;
   y = Ytable[ii] + yadd;
   Screen[i++] = BitMap[(y<<8)+x];
  }
 }
}

// -----

void DrawTunnel2_2(char *Screen, char *BitMap,
                   char xadd1, char yadd1, int panx1, int pany1,
                   char xadd2, char yadd2, int panx2, int pany2)
{
 int i,i1,i2;
 int xx,yy;
 int x,y;
 unsigned char x1,y1;
 unsigned char x2,y2;

 i = 0;
 for (yy=0;yy<200;yy++)
 {
  for (xx=0;xx<320;xx++)
  {
   i1 = ((yy+pany1)*640)+(xx+panx1);
   i2 = ((yy+pany2)*640)+(xx+panx2);
   x1 = Xtable2[i1] + xadd1;
   y1 = Ytable2[i1] + yadd1;
   x2 = Xtable2[i2] + xadd2;
   y2 = Ytable2[i2] + yadd2;

   x = (Xtable2[i1] + xadd1) + (Xtable2[i2] + xadd2) >> 1;
   y = (Ytable2[i1] + yadd1) + (Ytable2[i2] + yadd2) >> 1;

   if ( Ytable2[i1] > Ytable[i2] )
   {
    Screen[i++] = BitMap[(y2<<8)+x2];
   }
   else
   {
    Screen[i++] = BitMap[(y2<<8)+x2];
   }
  }
 }
}


// --------------------------------------------------------------------------

void main(void)
{
 FILE *fp;
 char x1,y1;
 float px1,py1;
 char *tex;

 Xtable = (char *)malloc(320*200);
 Ytable = (char *)malloc(320*200);
 Xtable2 = (char *)malloc(640*400);
 Ytable2 = (char *)malloc(640*400);

 tex = (char *)malloc(65536);

 fp = fopen("tex.pix","rb");
 fread(tex,1,65536,fp);
 fclose(fp);

 // radius, dist between viewer & screen
 CalcTables(200,200);
 SetMode(0x13);

 x1 = 0;
 y1 = 0;
 px1 = 0;
 py1 = 0;
 while (!kbhit())
 {
  DrawTunnel((char *)0xA0000,tex,x1,y1);
  x1++;
  y1++;
  px1+=0.1;
  py1+=0.1;
 }

 SetMode(3);

 free(tex);
 free(Xtable);
 free(Ytable);
 free(Xtable2);
 free(Ytable2);
}



















