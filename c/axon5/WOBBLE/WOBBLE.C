#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define BYTE unsigned char

#define PI 3.1415926535
#define SIN_MULT 7
#define TABLESIZE 256

char *texture;                  // 128x128
char *palette;
BYTE sine[256];
BYTE Xpos,Ypos;
BYTE Xouter,YOuter;
int x,y,c;
short int edi,w;

BYTE xoadd,yoadd,xiadd,yiadd;
BYTE xo,yo;

// -----

// set video mode
void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

// -----

void setrgb (char,char,char,char);
#pragma aux setrgb=\
 "mov dx,3c8h",\
 "out dx,al",\
 "inc dx",\
 "mov al,ah",\
 "out dx,al",\
 "mov al,bl",\
 "out dx,al",\
 "mov al,bh",\
 "out dx,al",\
 parm [al] [ah] [bl] [bh] modify [edx];


// -----

void CalcSinus(void)
{
 int i;
 float realangle = 0/*-PI*/;

 for (i = 0; i < TABLESIZE; i++)
 {
  sine[i] =  SIN_MULT * sin(realangle);
  realangle = realangle + ((PI*2)/TABLESIZE);
 }
}

// -----

void DrawWobble(BYTE xouter,      BYTE youter,
                BYTE xouteradder, BYTE youteradder,
                BYTE xinneradder, BYTE yinneradder)
{
 BYTE xsinus,ysinus;
 char *screen;
 int x,y;
 char c;

 screen = (char *)0xA0000;
 for (y=0;y<200;y++)
 {
  xouter += xouteradder;
  youter += youteradder;
  xsinus  = xouter;
  ysinus  = youter;
  for (x=0;x<320;x++)
  {
   c = texture[(((y+sine[ysinus])*256)&0xff00) + ((x+sine[xsinus])&0xff)];
   *screen++ = c;
   xsinus+=xinneradder;
   ysinus+=yinneradder;
  }
 }
}

// -----

void main(void)
{
 int i;
 FILE *fp;

 texture = (char *)malloc(65536);
 palette = (char *)malloc(768);

 fp = fopen("texture.raw","rb");
 fread(texture,1,65536,fp);
 fclose(fp);
 fp = fopen("palette.raw","rb");
 fread(palette,1,768,fp);
 fclose(fp);
 CalcSinus();
 SetMode(0x13);
 for (i=0;i<256;i++) setrgb(i,palette[i*3],palette[i*3+1],palette[i*3+2]);
 xo = 0;
 yo = 0;
 xoadd = 3;
 yoadd = 2;
 xiadd = 4;
 yiadd = 5;
 while (!kbhit())
 {
  DrawWobble(xo,yo,xoadd,yoadd,xiadd,yiadd);
  xo+=4;
  yo+=7;
 }
 SetMode(3);
 free(texture);
 free(palette);
}

