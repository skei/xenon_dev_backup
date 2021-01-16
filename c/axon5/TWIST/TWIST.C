#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#pragma aux twist "_*" parm caller [];
extern void twist(void);

// set video mode
void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

// -----

// Vaits for Vertical retrace to START
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


unsigned char *pic1;
unsigned char *pic2;
unsigned char pal[768];

unsigned short int sqrtd[320][200];
short int sind[256];
short int cosd[256];
int maindegree;
unsigned char *twisttarget;
unsigned char *twistsource;

// -----

void twistinit(void)
{
 unsigned short int x;
 unsigned short int y;

 for(x=0; x<320; x++)
 {
  for(y=0; y<200; y++)
  {
   sqrtd[x][y]=sqrt((160-x)*(160-x)+(100-y)*(100-y));
  }
 }
 sqrtd[160][100]=1;

 for(x=0; x<256; x++)
 {
  sind[x]=sin(x*3.14159/128)*32767;
  cosd[x]=cos(x*3.14159/128)*32767;
 }
}

// -----

void dotwist()
{
 int u,n,m;

 twistsource=pic1;
 twisttarget=pic2;

 maindegree=0;
 n=0;
 m=0;
 u=1;

 while (!kbhit())
 {
  m=16384; // 10000;
  maindegree=u*sind[n];
  maindegree=u*cosd[n] + maindegree+m;
  n=(n+1)&127;
  twist();
  memcpy((char *)0xA0000,pic2,64000);
 }
}

// --------------------------------------------------------------------------

void main(void)
{
 FILE *fp;
 int i;

 pic1 = (unsigned char *)malloc(65536);
 pic2 = (unsigned char *)malloc(65536);
 memset(pic1,0,65536);
 memset(pic2,0,65536);

 fp = fopen("pic.raw","rb");
 fread(pic1,1,64000,fp);
 fclose(fp);
 fp = fopen("pal.raw","rb");
 fread(pal,1,768,fp);
 fclose(fp);

 twistinit();


 SetMode(0x13);
 for (i=0;i<256;i++) setrgb(i,pal[i*3],pal[i*3+1],pal[i*3+2]);
 dotwist();
 SetMode(3);
}


