// Mode 12h gfx routines

#include <conio.h>

// --------------------------------------------------------------------------

// ----- Plane tables -----

unsigned char StartMask[8] = { 255,127, 63, 31, 15,  7,  3,  1 };
unsigned char EndMask[8]   = { 128,192,224,240,248,252,254,255 };
//unsigned char EndMask[8]   = { 255,254,252,248,240,224,192,128 };
unsigned char SameMask[64] =
 { 128,192,224,240,248,252,254,255,
     0, 64, 96,112,120,124,126,127,
     0,  0, 32, 48, 56, 60, 62, 63,
     0,  0,  0, 16, 24, 28, 30, 31,
     0,  0,  0,  0,  8, 12, 14, 15,
     0,  0,  0,  0,  0,  4,  6,  7,
     0,  0,  0,  0,  0,  0,  2,  3,
     0,  0,  0,  0,  0,  0,  0,  1 };
// --------------------------------------------------------------------------

// ----- set video mode -----

void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

// ----- set one palette entry -----

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

// ----- Vaits for Vertical retrace to START -----
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

// ----- Set write plane (0..15 - 4 bits -> 4 planes) -----

void SetPlane(unsigned char BitP);
#pragma aux SetPlane=\
 " mov dx,3c4h",\
 " mov al,2",\
 " out dx,ax",\
 parm [ah] modify [eax edx];

// ----- HorizLine -----

void HorizLine(int x1, int x2, int y, int plane)
{
 int leftp,rightp;
 int bytelength;
 int i,temp;
 unsigned char *screenpos;

 if (x1 == x2) return;
 if (x1 > x2)
 {
  temp = x1; x1 = x2; x2 = temp;
 }
 SetPlane(plane);
 leftp = (x1 & 7);
 rightp = (x2 & 7);
 x1 = (x1 >> 3)+1;
 (unsigned char *)screenpos = (unsigned char *)(0xA0000 + y*80+x1);
 x2 >>= 3;
 bytelength = x2-x1;
 if (bytelength < 0)
 {
  *screenpos = SameMask[(leftp << 3) + rightp];
 }
 else
 {
  *screenpos++ = StartMask[leftp];
  if (bytelength !=0)
  {
   for (i=0;i<bytelength;i++) *screenpos++ = 255;
  }
  *screenpos++ = EndMask[rightp];
 }
}

// อออออ Entrypoint from DOS ออออออออออออออออออออออออออออออออออออออออออออออออ

void main(void)
{
 int i;

 SetMode(0x12);
 for (i=0;i<450;i++)
 {
  HorizLine(i,630-i,i,7);
 }
 getch();
 SetMode(3);
}








