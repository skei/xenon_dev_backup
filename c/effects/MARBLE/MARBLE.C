// #include "subplasm.h"
#include <conio.h>
#include <math.h>
#include <mem.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

unsigned char test[65536];
unsigned char source[65536];
unsigned char *video = (unsigned char *) 0xa0000;

 // -------------------------------------------------------------------------

void SetBiosMode (short int);
 #pragma aux SetBiosMode=   \
 "int 0x10"         \
 parm [ax]          \
        modify [ax];


// ebx = "seed"
int Random(int);
#pragma aux Random=\
 " mov eax, 015a4e35h",\
 " imul eax, edx",\
 " inc  eax",\
 " mov  edx, eax",\
 " sar eax, 16",\
 " imul eax, ebx",\
 parm [ebx] value [eax] modify [edx];


 // -------------------------------------------------------------------------

#define RANDOM(a) (rand()/(RAND_MAX/(a)))         // a simple random function
#define MAPSHIFT   8                              // 2^MAPSHIFT = size of map
#define MAPSIZE    (1<<MAPSHIFT)                  // size of one side of the map
#define BYTE(a)    (a&(MAPSIZE-1))                // put the range within MAPSIZE
#define INDEX(x,y) ((BYTE(y)<<MAPSHIFT)+BYTE(x))  // index calculation

// ---

unsigned char newcolor(int mc, int n, int dvd)
{
 int loc;

 loc = (mc + n - RANDOM(n << 1)) / dvd - 1;
 if (loc > 255) loc = 255;
 if (loc < 10) loc = 10;
 return(loc);
}

// ---

void createfractalmap(unsigned char *heightmap,int x1, int y1, int x2, int y2)
{
 int p1, p2, p3, p4;
 int xn, yn, dxy;

 if (((x2-x1) < 2) && ((y2-y1) < 2)) return;
 p1 = heightmap[INDEX(x1,y1)];
 p2 = heightmap[INDEX(x1,y2)];
 p3 = heightmap[INDEX(x2,y1)];
 p4 = heightmap[INDEX(x2,y2)];
 xn = (x2+x1) >> 1;
 yn = (y2+y1) >> 1;
 dxy = 5*(x2 - x1 + y2 - y1) / 3;
 if (heightmap[INDEX(xn,y1)] == 0) heightmap[INDEX(xn,y1)] = newcolor(p1+p3, dxy, 2);
 if (heightmap[INDEX(x1,yn)] == 0) heightmap[INDEX(x1,yn)] = newcolor(p2+p4, dxy, 2);
 if (heightmap[INDEX(x2,yn)] == 0) heightmap[INDEX(x2,yn)] = newcolor(p3+p4, dxy, 2);
 if (heightmap[INDEX(xn,y2)] == 0) heightmap[INDEX(xn,y2)] = newcolor(p1+p2, dxy, 2);
 heightmap[INDEX(xn,yn)] = newcolor(p1+p2+p3+p4, dxy, 4);
 createfractalmap(heightmap,x1, y1, xn, yn);
 createfractalmap(heightmap,xn, y1, x2, yn);
 createfractalmap(heightmap,x1, yn, xn, y2);
 createfractalmap(heightmap,xn, yn, x2, y2);
}

// ---

void smoothmap(unsigned char *heightmap)
{
 int x,y;

 for (x = 0; x < MAPSIZE; x++)
 {
  for (y = 0; y < MAPSIZE; y++)
  {
   heightmap[INDEX(x,y)] = (heightmap[INDEX(x-1,y-1)] +
                            heightmap[INDEX(x-1,y+1)] +
                            heightmap[INDEX(x+1,y-1)] +
                            heightmap[INDEX(x+1,y+1)] +
                            heightmap[INDEX(x,  y-1)] +
                            heightmap[INDEX(x,  y+1)] +
                            heightmap[INDEX(x-1,y)] +
                            heightmap[INDEX(x+1,y)]) >> 3;
  }
 }
}


 // -------------------------------------------------------------------------

// This is the Marble-Generator
void marble (unsigned char *source, unsigned char *dest, float scale,
             float turbulence, unsigned char *xrpage, unsigned char *yrpage)
{
 int i,x,y;
 unsigned char xx,yy;

 int cval[256];
 int sval[256];
 for (i=0; i<256; i++)
 {
  sval[i]=-scale*sin((float)i/(float)turbulence);
  cval[i]=scale*cos((float)i/(float)turbulence);
 }
 for (x=0; x<256; x++)
 for (y=0; y<200; y++)
 {
  xx=(x+sval[xrpage[x+(y<<8)]]);
  yy=(y+cval[yrpage[x+(y<<8)]]);
  dest[x+(y<<8)]=source[xx+(yy<<8)];
 }
}

 // -------------------------------------------------------------------------

// --------------------------------------------------------------------------

// Code to blit a marble to the screen </font>
void display (void)
{
 int y;

 for (y=0; y<200; y++)
 {
  memmove(&video[320*y], &test[y*256], 256);
  memmove(&video[320*y+256], &test[y*256], (320-256));
 }
}

// --------------------------------------------------------------------------

// and the main program </font>
void main (void)
{
 int c,quit;
 float scale,turbulence;

 unsigned char *xrpage = (unsigned char *)malloc(0x10000);
 unsigned char *yrpage = (unsigned char *)malloc(0x10000);

 memset(xrpage,0,65536);
 memset(yrpage,0,65536);
 memset(source,0,65536);

 // Generate Source Images (we use a different plasma for x and y
 // displacement. That makes the marbles more weired!
 xrpage[0] = 127;//Random(24401);
 createfractalmap(xrpage,0,0,255,255);
 yrpage[0] = 85;//Random(55400);
 createfractalmap(yrpage,0,0,255,255);
 source[0] = 170;//Random(63703);
 createfractalmap(source,0,0,255,255);

 smoothmap(xrpage);
 smoothmap(xrpage);
 smoothmap(xrpage);

 smoothmap(yrpage);
 smoothmap(yrpage);
 smoothmap(yrpage);

 smoothmap(source);
 smoothmap(source);
 smoothmap(source);

 // makeplasma(/*(void *)*/xrpage, 24401, 420);
 // makeplasma_(/*(void *)*/yrpage, 55400, 420);
 // makeplasma(/*(void *)*/source, 63703, 750);
 // Set Video Mode and a grayscale Palette
 SetBiosMode (0x13);
 outp (0x3c8, 0);
 for (c=0; c<256; c++)
 {
  outp (0x3c9, c>>2);
  outp (0x3c9, c>>2);
  outp (0x3c9, c>>2);
 }
 // Animate the Marble Textures

 scale = 14;
 turbulence = 30;

 quit = 0;

 while (quit == 0)
 {

  memcpy (test, source, 0x10000);
  marble ((unsigned char *) source,
          (unsigned char *) test,
          scale,
          turbulence,
          (unsigned char *) xrpage,
          (unsigned char *) yrpage);
  display();
  if (kbhit())
  {
   c = getch();
   switch(toupper(c))
   {
    case ' ' : quit = 1;
               break;
    case 'Q' : scale += 1;
               break;
    case 'A' : scale -= 1;
               break;
    case 'W' : turbulence += 1;
               break;
    case 'S' : turbulence -= 1;
               break;
   }
   c = 0;
  }
 }
 SetBiosMode (0x03);
 free (xrpage);
 free (yrpage);
 SetBiosMode (0x03);
 printf("scale: %f... turbulence: %f...\n",scale,turbulence);
}

// -------------------------------------------------------------------------

