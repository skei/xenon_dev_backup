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

void InitVBE2(void)
{
 if (!(VESAptr = (unsigned short int *)VesaInit(320,200,15)))
 {
  printf("ERROR : VESA-Mode not supported! (is UNIVBE installed?)\n\r");
  exit(1);
 }
}

// --------------------------------------------------------------------------

void UnInitVBE2(void)
{
 VesaUnInit((void *)VESAptr);
}

// --------------------------------------------------------------------------
// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// --------------------------------------------------------------------------

void main(void)
{
 int x,y;

 InitVBE2();

 for (x=0;x<199;x++)
 {
  VESAptr[x*320+x]  = 0x7fff;
  VESAptr[x*320+x+1] = 0x7fff;
 }
 getch();
 UnInitVBE2();
}

