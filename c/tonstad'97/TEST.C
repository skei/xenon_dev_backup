#include <stdio.h>

#define SkurrNum 10
#define SkurrMax 20

int Skurr_Array[200][SkurrNum];
char buffer[64000];
char pal[768];
char *screen;

// -----

void Skurr_Init(void)
 {
  int i,j;
  float displace;
  float delta;

  for (i=0;i<200;i++)
  {
   displace = (rand()%(SkurrMax*2))-SkurrMax;
   delta = displace/SkurrNum;
   for (j=SkurrNum;j>=0;j--)
   {
    Skurr_Array[i][j] = (int)displace;
    displace-=delta;
   }
  }
 }

// -----

void Skurr_Draw(char *dst, char *src, int num)
 {
  int i;
  int displace;

  if (num == 0)
  {
   memcpy (dst,src,64000);
   return;
  }
  if ((num >= SkurrNum) || (num < 0)) return;
  memcpy(&dst[0],&src[0],320);
  memcpy(&dst[199*320],&src[199*320],320);
  for (i=1;i<199;i++)
  {
   displace = Skurr_Array[i][num];
   memcpy(&dst[i*320],&src[(i*320)+displace],320);
  }
 }

// -----

void SetMode(unsigned short int);
 #pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

void SetRGB(char color, char r,char g, char b);
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

void WaitVR (void);
 #pragma aux WaitVR =\
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

void main(void)
{
 FILE *fp;
 int num;
 int i,j;

 Skurr_init();

 fp = fopen("van.pix","rb");
 fread(buffer,1,64000,fp);
 fclose(fp);
 fp = fopen("van.pal","rb");
 fread(pal,1,768,fp);
 fclose(fp);
 for (i=0;i<256;i++) SetRGB(i,pal[i*3],pal[i*3+1],pal[i*3+2]);

 SetMode(0x13);

 screen = (char *)0xA0000;
 memcpy(screen,buffer,64000);
 getch();

 for (num=9;num>=0;num--)
 {
  Skurr_Draw(screen,buffer,num);
  getch();
 }
 SetMode(0x3);
}
