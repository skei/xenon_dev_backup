#include <stdio.h>
#include <conio.h>

int NewAbs(int);
#pragma aux NewAbs =\
 "cdq",\
 "sbb eax,0",\
 "xor eax,edx",\
 parm [eax] value [eax] modify [edx];
// #define NewAbs(a) (a>0?a:-a)

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

void DrawBump(int cx, int cy, char *BumpMap)
{
 int i,j;
 int di;
 int lx,ly;
 int nx,ny;
 char c,c1,c2;
 char *scrpos;

 di=321;
 scrpos = (char *)(0xA0000+di);
 ly=1-cy;
 for (i=1;i<199;i++)
 {
  lx=1-cx;
  for (j=1;j<319;j++)
  {
   nx=BumpMap[di+1] - BumpMap[di-1];
   c1 = 127-NewAbs(nx-lx);
   if (c1<0) c1 = 0;
   ny = BumpMap[di+320] - BumpMap[di-320];
   c2 = 127-NewAbs(ny-ly);
   if (c2<0) c2 = 0;
   c = (c1*c2) >> 8;
   *scrpos++ = c;
   di++;
   lx++;
  }
  di+=2;
  scrpos+=2;
  ly++;
 }
}

void main(int argc, const char *argv[])
{
 FILE *fp;
 char *bump;
 int i;

 if (argc != 2)
 {
  printf("BUMP <file>\n");
  exit(1);
 }
 else
 {
  SetMode(0x13);
  for (i=0;i<64;i++)   setrgb(i,i, 0, 0);
  for (i=64;i<256;i++) setrgb(i,63,63,63);
  bump = (char *)malloc(64000);
  fp = fopen(argv[1],"rb");
  fread(bump,1,64000,fp);
  fclose(fp);
  DrawBump(160,100,bump);
  getch();
  SetMode(0x3);
  free(bump);
 }
}

