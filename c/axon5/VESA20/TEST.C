#include <conio.h>
#include <dos.h>
#include "vbe2.h"

void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

// --------------------------------------------------------------------------

unsigned short int *VESAptr;

// --------------------------------------------------------------------------
// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

void HorizHalf(void)
{
 int a;

 outp(0x3d4,9);
 a = inp(0x3d5);
 a &= 0xffffffe0;   // clear last 5 bits
 a |= 1;            // set 1 bit
 outp(0x3d5,a);
}

void VertHalf(void)
{
 int a;

 outp(0x3d4,0x11);
 a = inp(0x3d5);
 a &= 0x7f;
 outp(0x3d5,a);

 outp(0x3d4,0x00);
 outp(0x3d5,0x2d);

 outp(0x3d4,0x01);
 outp(0x3d5,0x27);

 outp(0x3d4,0x02);
 outp(0x3d5,0x28);

 outp(0x3d4,0x03);
 outp(0x3d5,0x90);

 outp(0x3d4,0x04);
 outp(0x3d5,0x2b);

 outp(0x3d4,0x05);
 outp(0x3d5,0x80);

 outp(0x3d4,0x13);
 outp(0x3d5,0x28);

 outp(0x3c4,0x01);
 outp(0x3c5,0x09);
}


// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

void main(void)
{
 int x,y;

 if (!(VESAptr = (unsigned short int *)VesaInit(640,400,15)))
 {
  printf("ERROR : VESA-Mode not supported! (is UNIVBE installed?)\n\r");
  exit(1);
 }

 getch();
// VertHalf();
// HorizHalf();
// VesaSetScanlines(640);

// for (x=0;x<199;x++)
// {
//  VESAptr[x*320+x]  = 0x7fff;
//  VESAptr[x*320+x+1] = 0x7fff;
// }
 getch();

 VesaUnInit((void *)VESAptr);
}



