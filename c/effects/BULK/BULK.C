#include <stdio.h>    // file
#include <conio.h>    // kbhit
#include <string.h>   // memcyp
#include <math.h>     // sin,cos

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

unsigned char xmod[128*128];
unsigned char ymod[128*128];
unsigned char background[320*200];
unsigned char pal[768];
unsigned char buffer[320*200];

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void SetMode(unsigned short int mode);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

void SetRGB(char c, char r, char g, char b);
#pragma aux SetRGB=\
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

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void DrawBulk( int x0, int y0 )
{
 int x,y;
 int p;
 int bulkpos;
 int xskip;

 xskip = (x0-(320-128));
 if (xskip < 0) xskip = 0;
 bulkpos = 0;
 p = y0*320+x0;
 for (y=0;y<128;y++)
 {
  for (x=0;x<128;x++)
  {
   buffer[p++] = background[ (y0+ymod[bulkpos])*320 + (x0+xmod[bulkpos])];
   bulkpos++;
  }
  bulkpos += xskip;
  p+= (320-128);
 }
}

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void main(void)
{
 FILE *fp;
 int i;
 float xpos,ypos;

 fp = fopen("xmod.pix","rb");
 fread(xmod,1,(128*128),fp);
 fclose(fp);

 fp = fopen("ymod.pix","rb");
 fread(ymod,1,(128*128),fp);
 fclose(fp);

 fp = fopen("back.pix","rb");
 fread(background,1,(320*200),fp);
 fclose(fp);

 fp = fopen("back.pal","rb");
 fread(pal,1,768,fp);
 fclose(fp);

 SetMode(0x13);
 for (i=0;i<256;i++) SetRGB(i,pal[i*3],pal[i*3+1],pal[i*3+2]);

 xpos = 0;
 ypos = 0;
 while (!kbhit())
 {
  xpos += 0.01;
  ypos += 0.02;
  memcpy(buffer,background,64000);
  DrawBulk(96+(int)(sin(xpos)*96),18+(int)(cos(ypos)*18));
  memcpy((char *)0xA0000,buffer,64000);
 }

 getch();

 SetMode(3);
}

