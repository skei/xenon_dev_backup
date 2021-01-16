#include <math.h>   // math yucku
#include <stdlib.h> // malloc,rand
#include <string.h> // memset,memcpy
#include <stdio.h>  // file yucku
#include <conio.h>  // kbhit

// --------------------------------------------------------------------------

// set video mode
void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

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


int *Height;;
char *tempBitMap;
char *BitMap;
char *BackgroundBuffer;
unsigned char pal[768];

char *kulemap;

// --------------------------------------------------------------------------

void WarpBlob(int x, int y, int radius, int height, int page)
{
 int cx, cy;
 int left,top,right,bottom;
 int square;
 int radsquare = radius * radius;
 radsquare = (radius*radius);
 height /= 64;
 left=-radius; right = radius;
 top=-radius; bottom = radius;
 if(x - radius < 1) left -= (x-radius-1);
 if(y - radius < 1) top  -= (y-radius-1);
 if(x + radius > 320-1) right -= (x+radius-320+1);
 if(y + radius > 200-1) bottom-= (y+radius-200+1);
 for (cy = top; cy < bottom; cy++)
 {
  for(cx = left; cx < right; cx++)
  {
   square = cy*cy + cx*cx;
//   square <<= 8;
   if(square < radsquare)
   {
//    Height[page][320*(cy+y) + cx+x] += (sqrt(radsquare)-sqrt(square))*height;
    Height[ page*64000 + 320*(cy+y) + cx+x] += (radius-sqrt(square))*(float)(height);
   }
  }
 }
}

// --------------------------------------------------------------------------

void DrawWaterNoLight(int page)
{
//  int ox, oy;
 int dx, dy;
 int x, y;
 int c;
 int offset=320+1;
 int *ptr = &(Height[page*64000]);

 for (y = (200-1)*320; offset < y; offset += 2)
 {
  for (x = offset+320-2; offset < x; offset++)
  {
   dx = ptr[offset] - ptr[offset+1];
   dy = ptr[offset] - ptr[offset+320];
   c = BitMap[offset + 320*(dy>>3) + (dx>>3)];
   BackgroundBuffer[offset] = (c < 0) ? 0 : (c > 255) ? 255 : c;
   offset++;
   dx = ptr[offset] - ptr[offset+1];
   dy = ptr[offset] - ptr[offset+320];
   c = BitMap[offset + 320*(dy>>3) + (dx>>3)];
   BackgroundBuffer[offset] = (c < 0) ? 0 : (c > 255) ? 255 : c;
  }
 }
 memcpy((char *)0xA0000,BackgroundBuffer,64000);
}

// --------------------------------------------------------------------------

void DrawWaterWithLight(int page, int LightModifier)
{
//  int ox, oy;
 int dx, dy;
 int x, y;
 int c;
 int offset=320 + 1;
 int *ptr = &(Height[page*64000]);

 for (y = (200-1)*320; offset < y; offset += 2)
 {
  for (x = offset+320-2; offset < x; offset++)
  {
   dx = ptr[offset] - ptr[offset+1];
   dy = ptr[offset] - ptr[offset+320];
   c = BitMap[offset + 320*(dy>>3) + (dx>>3)] - (dx>>LightModifier);
   BackgroundBuffer[offset] = (c < 0) ? 0 : (c > 255) ? 255 : c;
   offset++;
   dx = ptr[offset] - ptr[offset+1];
   dy = ptr[offset] - ptr[offset+320];
   c = BitMap[offset + 320*(dy>>3) + (dx>>3)] - (dx>>LightModifier);
   BackgroundBuffer[offset] = (c < 0) ? 0 : (c > 255) ? 255 : c;
  }
 }
 memcpy((char *)0xA0000,BackgroundBuffer,64000);
}

// --------------------------------------------------------------------------

void CalcWater(int npage, int density)
{
 int newh;
 int count = 320 + 1;
 int *newptr = &(Height[npage*64000]);
 int *oldptr = &(Height[(npage^1)*64000]);
 int x, y;

 for (y = (200-1)*320; count < y; count += 2)
 {
  for (x = count+320-2; count < x; count++)
  {
   newh = ((oldptr[count + 320]
          + oldptr[count - 320]
          + oldptr[count + 1]
          + oldptr[count - 1]
          + oldptr[count - 320 - 1]
          + oldptr[count - 320 + 1]
          + oldptr[count + 320 - 1]
          + oldptr[count + 320 + 1]
          ) >> 2 )
          - newptr[count];
   newptr[count] =  newh - (newh >> density);
  }
 }
}

// --------------------------------------------------------------------------

void main(void)
{
 FILE *fp;

 int density = 8;
 int pheight = 200;
 int radius = 60;

 int Hpage = 0;
 int x,y;


// kulemap = (char *)malloc(128*128);
// fp = fopen("kule.raw","rb");
// fread(kulemap,1,128*128,fp);
// fclose(fp);

 tempBitMap = (char *)malloc(64000*3);
 BitMap = tempBitMap+64000;
 BackgroundBuffer = (char *)malloc(64000);
 Height = (int *)malloc(64000*2*sizeof(int));

 memset(Height, 0, sizeof(int)*64000*2);

 fp = fopen("tmk_bw.pix","rb");
 fread(BitMap,1,64000,fp);
 fclose(fp);
 fp = fopen("tmk_bw.pal","rb");
 fread(pal,1,768,fp);
 fclose(fp);

 memcpy(tempBitMap, BitMap, 320*200);
 memcpy(tempBitMap+128000, BitMap, 320*200);
 memcpy(BackgroundBuffer, BitMap, 320*200);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

 SetMode(0x13);

 for (x=0;x<256;x++) setrgb(x,pal[x*3],pal[x*3+1],pal[x*3+2]);

 while (!kbhit())
 {
  x = rand()%(320-radius*2) + radius;
  y = rand()%(200-radius*2) + radius;

/*
  for (x=0;x<128;x++)
  {
   for (y=0;y<128;y++)
   {
    Height[Hpage*64000 + 320*y + x] = kulemap[y*128+x];
   }
  }
*/

  WarpBlob(x,y, radius, pheight, Hpage);
//  DrawWaterWithLight(Hpage, 2);
  DrawWaterNoLight(Hpage);
  CalcWater(Hpage^1, density);
  Hpage ^= 1;
 }

 SetMode(3);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

// free(kulemap);

 free(Height);
 free(BackgroundBuffer);
 free(tempBitMap);
}

