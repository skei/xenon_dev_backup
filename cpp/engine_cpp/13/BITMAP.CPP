#include <stdlib.h>
#include <string.h>
#include "bitmap.hpp"

c_BITMAP::c_BITMAP()
 {
  width = 256;
  height = 256;
  bpp = 8;
  bitmap_size = width*height;
  bitmap = malloc(bitmap_size);
 };

 // ---

c_BITMAP::c_BITMAP(int w, int h, int b)
 {
  width = w;
  height = h;
  bpp = b;

  switch(b)
  {
   case  8 : bitmap_size = w*h;   break;
   case 15 : bitmap_size = w*h*2; break;
   case 16 : bitmap_size = w*h*2; break;
   case 24 : bitmap_size = w*h*3; break;
   case 32 : bitmap_size = w*h*4; break;
  }
  bitmap = malloc(bitmap_size);
 };

 // ---

c_BITMAP::c_BITMAP(int w,int h,int b, void *ptr)
 {
  width = w;
  height = h;
  bpp = b;

  // change these... VESA/MCGA initialization, etc...
  switch(b)
  {
   case  8 : bitmap_size = w*h;   break;
   case 15 : bitmap_size = w*h*2; break;
   case 16 : bitmap_size = w*h*2; break;
   case 24 : bitmap_size = w*h*3; break;
   case 32 : bitmap_size = w*h*4; break;
  }

  //bitmap = malloc(bitmap_size);
  bitmap = ptr;
 };

 // ---

c_BITMAP::~c_BITMAP()
 {
  free(bitmap);
 };

 // ---

void c_BITMAP::Clear(void)
 {
  memset(bitmap,0,bitmap_size);
 };

 // ---

void c_BITMAP::BlitTo(c_BITMAP &dest)
 {
  int temp;

  if (dest.bitmap_size == bitmap_size)
  {
   temp = (dest.bpp<<8)+bpp;
   switch(temp)
   {
    case 0x0808 : memcpy(dest.bitmap,bitmap,bitmap_size);
				  break;
    //case 0x080f : 8->15
    //case 0x0810 : 8->16
    //case 0x0818 : 8->24
    //case 0x0820 : 8->32
   }
  }
  else
  {
   // scale source bitmap to fit dest...
  }
 };

 // ---

void *c_BITQMAP::GetPtr(void)
 {
  return bitmap;
 };

