#include <stdlib.h>

 void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

 void setrgb(char color, char r,char g, char b);
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


char lines[256*320];


void CalcSkurr(void)
{
 int x;

 for (x=1;x<320*256;x++) lines[x] = (rand()%31);
}

// -----

void DrawSkurr(void)
{
 int y;
 char *ptr;

 ptr = (char *)0xA0000;
 for (y=0;y<200;y++)
 {
  memcpy(ptr,&lines[(rand()%255) * 320],320);
  ptr+=320;
 }
}

// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

void main(void)
{
 int i;

 CalcSkurr();

 SetMode(0x13);
 for (i=0;i<64;i++)
 {
  setrgb(i,i,i,i);
 }


 while(!kbhit())
 {
  waitvr();
  DrawSkurr();
 }

 SetMode(0x3);

}

