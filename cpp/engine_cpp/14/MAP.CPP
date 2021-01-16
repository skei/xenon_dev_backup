#include <stdlib.h>
#include <string.h>

#include "map.hpp"

 // -------------------------------------------------------------------------

c_MAP::c_MAP()
 {
  m_width = 256;
  m_height = 256;
  m_bpp = 8;
  m_mapsize = m_width*m_height;
  m_map = malloc(m_mapsize);
 };

 // ---

c_MAP::c_MAP(int width, int height, int bpp)
 {
  m_width = width;
  m_height = height;
  m_bpp = bpp;

  switch(m_bpp)
  {
   case  8 : m_mapsize = m_width*m_height;   break;
   case 15 : m_mapsize = m_width*m_height*2; break;
   case 16 : m_mapsize = m_width*m_height*2; break;
   case 24 : m_mapsize = m_width*m_height*3; break;
   case 32 : m_mapsize = m_width*m_height*4; break;
  }
  m_map = malloc(m_mapsize);
 };

 // ---

c_MAP::c_MAP(int width,int height,int bpp, void *ptr)
 {
  m_width = width;
  m_height = height;
  m_bpp = bpp;
  // change these... VESA/MCGA initialization, etc...
  switch(m_bpp)
  {
   case  8 : m_mapsize = m_width*m_height;   break;
   case 15 : m_mapsize = m_width*m_height*2; break;
   case 16 : m_mapsize = m_width*m_height*2; break;
   case 24 : m_mapsize = m_width*m_height*3; break;
   case 32 : m_mapsize = m_width*m_height*4; break;
  }
  //bitmap = malloc(bitmap_size);
  m_map = ptr;
 };

 // ---

c_MAP::~c_MAP()
 {
  free(m_map);
 };

 // ---

void c_MAP::Clear(void)
 {
  memset(m_map,0,m_mapsize);
 };

 // ---

void c_MAP::BlitTo(c_MAP &dest)
 {
  int temp;

  if (dest.m_mapsize == m_mapsize)
  {
   temp = (dest.m_bpp<<8)+m_bpp;
   switch(temp)
   {
    case 0x0808 : memcpy(dest.m_map,m_map,m_mapsize);
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
