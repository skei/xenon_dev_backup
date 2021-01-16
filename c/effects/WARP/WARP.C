#include <math.h>
#include <stdio.h>

#define PI 3.1415926535
#define TWO_PI (PI+PI)

unsigned char *P2R;
unsigned char *R2P;
unsigned char *bitmap;
unsigned char pal[768];

void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

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

void waitvr (void);
#pragma aux waitvr =\
 " mov dx,3dah",\
 "wait1:",\
 " in al,dx",\
 " test al,8",\
 " jnz wait1",\
 "wait2:",\
 " in al,dx",\
 " test al,8",\
 " jz wait2",\
 modify [al dx];


void MakeTables(void)
 {
  // make polar-to-rectangular
  int x,y;

  P2R = (unsigned char *)malloc(320*200*2);
  for (y=0;y<200;y++)
  {
   for (x=0;x<320;x++)
   {
    P2R[ (y*320+x)*2   ] = (atan2( (y-100),(x-160) ) * 256)/TWO_PI;
    P2R[((y*320+x)*2)+1] = sqrt( (y-100)*(y-100)+(x-160)*(x-160) );
   }
  }
  // make rectangular-to-polar
  R2P = (unsigned char *)malloc(256*256*2);
  for (y=0;y<256;y++)
  {
   for (x=0;x<256;x++)
   {
    R2P[(y*256+x)*2+1] = (cos(y*TWO_PI/256)*x)+160;
    R2P[(y*256+x)*2  ] = (sin(y*TWO_PI/256)*x)+100;
   }
  }
 }

void main(void)
{
 unsigned char xadd,yadd;
 unsigned char x,y;
 unsigned char *screen;
 int i;
 FILE *fp;

 bitmap = (unsigned char *)malloc(65536);
 MakeTables();

 fp = fopen("bitmap.pix","rb");
 fread(bitmap,1,65536,fp);
 fclose(fp);
 fp = fopen("bitmap.pal","rb");
 fread(pal,1,768,fp);
 fclose(fp);
 SetMode(0x13);
 for (i=0;i<256;i++) setrgb(i,pal[i*3],pal[i*3+1],pal[i*3+2]);

 xadd = 0;
 yadd = 0;
 while(!kbhit())
 {
  screen = (unsigned char *)0xA0000;
  for (i=0;i<64000;i++)
  {
   x = P2R[i*2  ] + xadd;
   y = P2R[i*2+1] + yadd;
   *screen++ = bitmap[ R2P[ (y*256+x)*2 ] ];
  }
  // yadd += 1;
  // yadd += 1;
 }
 SetMode(3);
 free(bitmap);
 free(P2R);
 free(R2P);
}
