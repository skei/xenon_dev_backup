#include <stdlib.h>
#include <string.h>
#include "map.hpp"

// Default constructor. Init a 256x256 1-byte map, allocates memory
c_MAP::c_MAP()
 {
  width = 256;
  height = 256;
  bpp = 8;
  map_size = width*height;
  map = malloc(map_size);
 };

 // ---

// User specified size and bits per pixel (8,15,16,24,32)
c_MAP::c_MAP(int w, int h, int b)
 {
  width = w;
  height = h;
  bpp = b;

  switch(b)
  {
   case  8 : map_size = w*h;   break;
   case 15 : map_size = w*h*2; break;
   case 16 : map_size = w*h*2; break;
   case 24 : map_size = w*h*3; break;
   case 32 : map_size = w*h*4; break;
  }
  map = malloc(map_size);
 };

 // ---

// User specified size, bpp and pointer to existing map (screen, etc)
c_MAP::c_MAP(int w,int h,int b, void *ptr)
 {
  width = w;
  height = h;
  bpp = b;

  // change these... VESA/MCGA initialization, etc...
  switch(b)
  {
   case  8 : map_size = w*h;   break;
   case 15 : map_size = w*h*2; break;
   case 16 : map_size = w*h*2; break;
   case 24 : map_size = w*h*3; break;
   case 32 : map_size = w*h*4; break;
  }

  //map = malloc(bitmap_size);
  map = ptr;
 };

 // ---

c_MAP::~c_MAP()
 {
  free(map);
 };

 // ---

void c_MAP::Clear(void)
 {
  memset(map,0,map_size);
 };

 // ---

void c_MAP::BlitTo(c_MAP &dest)
 {
  int temp;

  if (dest.map_size == map_size)
  {
   temp = (dest.bpp<<8)+bpp;
   switch(temp)
   {
    case 0x0808 : memcpy(dest.map,map,map_size);
				  break;
    //case 0x080f : 8->15
    //case 0x0810 : 8->16
    //case 0x0818 : 8->24
    //case 0x0820 : 8->32
   }
  }
  else
  {
   // scale source map to fit dest...
  }
 };

 // ---

void *c_MAP::GetPtr(void)
 {
  return map;
 };

