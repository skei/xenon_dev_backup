#include <stdio.h>
#include <dos.h>
#include "vbe2.h"

#define WORD unsigned short int

WORD highbyte[32*256];
WORD  lowbyte[32*256];

WORD *bitmap1, *bitmap2, *bitmap3;

WORD *Vesa;

// --------------------------------------------------------------------------

void SetMode( unsigned short int mm)
{
 union REGS r;

 r.w.ax = mm;
 int386(0x10,&r,&r);
}


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

// --------------------------------------------------------------------------

void PrepareAlphaTables(void)
{
 WORD i,j,c;
 WORD r,g,b;

 memset(highbyte,0,32*256*2);
 memset( lowbyte,0,32*256*2);

 for (j=1;j<32;j++)
 {
  for (i=0;i<32768;i++)
  {
   r = ((0         )*j)/31; // 0.00000.00|000.00000
   g = ((i & 224   )*j)/31; // 0.00000.00|111.00000
   b = ((i & 31    )*j)/31; // 0.00000.00|000.11111
   lowbyte[ (j<<8) + (i&0xff) ] = ((r&31744) + (g&992) + (b&31));
   r = ((i & 31744 )*j)/31; // 0.11111.00|000.00000
   g = ((i & 768   )*j)/31; // 0.00000.11|000.00000
   b = ((0         )*j)/31; // 0.00000.00|000.00000
   highbyte[ (j<<8) + (i>>8) ] = ((r&31744) + (g&992) + (b&31));
  }
 }
}

// --------------------------------------------------------------------------

void Load( char *f, WORD *buffer, char *palette)
{
 char *buf,*pal;
 FILE *fp;
 int i;
 char r,g,b,c;

 buf = (char *)malloc(64000);
 pal = (char *)malloc(768);

 fp = fopen(palette,"rb");
 fread(pal,1,768,fp);
 fclose(fp);

 fp = fopen(f,"rb");
 fread(buf,1,64000,fp);
 fclose(fp);
 for (i=0;i<64000;i++)
 {
  c = buf[i];
  r = pal[c*3];
  g = pal[c*3+1];
  b = pal[c*3+2];
  buffer[i] = (WORD)( ((r >> 1)<<10)
                    + ((g >> 1)<< 5)
                    + ((b >> 1)<< 0) );
 }
 free(buf);
 free(pal);
}

// --------------------------------------------------------------------------


void AsmCrossFade(WORD *buffer,WORD *pic1, WORD *pic2, unsigned char alpha);
#pragma aux AsmCrossFade=\
 " push ebp               ",\
 " mov ebp,64000          ",\
 " and ecx,0x00003f00     ",\
 " xor edx,edx            ",\
 "L1:                     ",\
 " mov dh,31              ",\
 " sub dh,ch              ",\
 " mov cl,[esi+1]         ",\
 " mov ax,[highbyte+ecx*2]",\
 " mov cl,[esi]           ",\
 " add ax,[lowbyte+ecx*2] ",\
 " add esi,2              ",\
 " mov dl,[ebx+1]         ",\
 " add ax,[highbyte+edx*2]",\
 " mov dl,[ebx]           ",\
 " add ax,[lowbyte+edx*2] ",\
 " add ebx,2              ",\
 " mov [edi],ax           ",\
 " add edi,2              ",\
 " dec ebp                ",\
 " jnz L1                 ",\
 " pop ebp                ",\
 parm [edi][esi][ebx][ch] modify [eax ebx ecx edx esi edi];

void CrossFade(WORD *buffer, WORD *pic1, WORD *pic2, int alpha)
{
 unsigned char *p1;
 unsigned char *p2;
 WORD *buf;
 int i;
 WORD i1,i2;
 WORD c;
 WORD CC;

 p1 = (char *)pic1;
 p2 = (char *)pic2;
 buf = buffer;

 i1 = (alpha & 31);
 i2 = (31 - i1) << 8;
 i1 <<= 8;


 for (i=0;i<64000;i++)
 {
  c = i1 + *(p1+1);
  CC = highbyte[c];
  c = i1 + *(p1);
  CC += lowbyte[c];
  p1+=2;

  c = i2 + *(p2+1);
  CC += highbyte[c];
  c = i2 + *(p2);
  CC += lowbyte[c];
  p2+=2;

  *buf++ = CC;
 }
}

void main(void)
{
 int alpha,alphaadd;
 int quit;
 char c;

 bitmap1 = (WORD *)malloc(64000*2);
 bitmap2 = (WORD *)malloc(64000*2);
 bitmap3 = (WORD *)malloc(64000*2);
 memset(bitmap3,0,64000*2);

 Load("redhair.pix",bitmap1,"redhair.pal");
 Load("drown2.pix",bitmap2,"drown2.pal");

 PrepareAlphaTables();

 if (VesaInit(320,200,15) != 1)
 {
  printf("Error initializing VBE2 320x200x15\n");
  exit(1);
 }
 Set_Vesa(VesaMode);
 Vesa = (WORD *)VesaPtr;

 alpha = 0;
 alphaadd = 1;
 while (!kbhit())
 {
  AsmCrossFade(Vesa,bitmap1,bitmap2,(alpha&31));
  alpha+=alphaadd;
  if ((alpha == 31) && (alphaadd > 0)) alphaadd = 0 - alphaadd;
  if ((alpha ==  0) && (alphaadd < 0)) alphaadd = 0 - alphaadd;
 }
 getch();
 VesaUnInit();
 Set_Vesa(3);
 SetMode(3);

}
