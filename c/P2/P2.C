// --------------------------------------------------------------------------
// PolyT:  5.17 (to buffer)..... 193424 pr sec.
//        17.?? (directly to screen).. No VRAM :-/
//         9.?? (directly to LFB/VBE2)
// --------------------------------------------------------------------------

// #include <stdlib.h>     // malloc,exit
#include <stdio.h>      // printf, file
// #include <conio.h>      // getch

#include "vbe2.h"

// --------------------------------------------------------------------------

// fixed point multiplication...
int fixedmul2(int, int);
#pragma aux fixedmul2=\
 "imul ebx",\
 "shrd eax,edx,16",\
 parm [eax] [ebx] value [eax] modify [edx];

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

// --------------------------------------------------------------------------

typedef struct sVertex
{
 float x,y,z;           // world coordinates
 int ix,iy,iz;          // screen coordinates
 int u,v;               // 8.8 FP
 int c;                 // 8.8 FP
} sVertex;

// --------------------

typedef struct sPolygon
{
 sVertex *v1,*v2,*v3;
} sPolygon;

typedef unsigned short int WORD;

#define NULL 0

// --------------------------------------------------------------------------

char palette[768];
WORD *VesaPtr;

WORD *_Buffer;
char *_Texture;
WORD *_ShadeTable;

int _outerUadd,_outerVadd,_outerCadd;
int _innerUadd,_innerVadd,_innerCadd;
int Lslope,Rslope;
int _x1,_x2,_u1,_v1,_c1;
int Clipping,xclip,yclip;

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void Calc_ShadeHighLight(char *palette, WORD *ShadeTable,int diffuse,
                         float rr, float gg, float bb)
{
 int x,y;
 float rv,gv,bv;
 float radd,gadd,badd;

 for (x=0;x<256;x++)
 {
  rv = rr;
  gv = gg;
  bv = bb;
  radd = ((float)palette[x*3]   - rr)/ diffuse;
  gadd = ((float)palette[x*3+1] - gg)/ diffuse;
  badd = ((float)palette[x*3+2] - bb)/ diffuse;
  for (y=0;y<diffuse;y++)
  {
   ShadeTable[y*256+x] = ((int)(rv/2)<<10)+((int)(gv/2)<<5)+(int)(bv/2);
   rv += radd;
   gv += gadd;
   bv += badd;
  }
 }
 for (x=0;x<256;x++)
 {
  rv = (float)palette[x*3];
  gv = (float)palette[x*3+1];
  bv = (float)palette[x*3+2];
  radd = (63-(float)palette[x*3]+1)   / (63-diffuse+1);
  gadd = (63-(float)palette[x*3+1]+1) / (63-diffuse+1);
  badd = (63-(float)palette[x*3+2]+1) / (63-diffuse+1);
  for (y=diffuse;y<64;y++)
  {
   ShadeTable[y*256+x] = ((int)(rv/2)<<10)+((int)(gv/2)<<5)+(int)(bv/2);
   rv += radd;
   gv += gadd;
   bv += badd;
  }
 }
}

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define Adjust(src,dest) {         \
 if ((src&&0xffff)==0x8000) src++; \
 dest = ((src+0x8000) >> 16); }

// --------------------------------------------------------------------------

void InnerLoopGT(WORD *pos, int width, int u, int v, int c)
{
 while (width>0)
 {
  *pos++ = _ShadeTable[ ((c>>16)<<8) + _Texture[ ( ((v>>16)<<8) + (u>>16) ) & 0xffff ] ];
  u+=_innerUadd;
  v+=_innerVadd;
  c+=_innerCadd;
  width--;
 }
}

// --------------------------------------------------------------------------

// fyller en halvdel av et triangel
void FillPolyGT(int y1, int y2)
{
 WORD *pos;
 int ypos;
 int height;
 int x1,x2,u1,v1,c1;
 int xx1,xx2;
 int skip;

// check here if poly needs to be clipped, and if not, jump down to
// NON-clipping version...
// global variable (NeedClipping???)

 if (Clipping == 0)
 {
  // ----- No clipping ------------------------------------------------------
  x1=_x1;
  x2=_x2;
  u1=_u1;
  v1=_v1;
  c1=_c1;
  height = y2-y1;
  ypos = y1*320;
  while (height>0)
  {
   Adjust(x1,xx1);
   Adjust(x2,xx2);
   pos = &(_Buffer[ypos+xx1]);
   InnerLoopGT(pos,(xx2-xx1),u1,v1,c1);
   x1+=Lslope;
   x2+=Rslope;
   u1+=_outerUadd;
   v1+=_outerVadd;
   c1+=_outerCadd;
   ypos+=320;
   height--;
  }
  _x1=x1;
  _x2=x2;
  _u1=u1;
  _v1=v1;
  _c1=c1;
 }
 else
 {
  // ----- Clipping ---------------------------------------------------------
  x1=_x1;
  x2=_x2;
  u1=_u1;
  v1=_v1;
  c1=_c1;
  if (y1>199) return;
  if (y2<0) return;
  if (y2 > 199) y2 = 199;
  if (y1<0)
  {
   skip = 0-y1;
   x1 += (skip*Lslope);
   x2 += (skip*Rslope);
   u1 += (skip*_outerUadd);
   v1 += (skip*_outerVadd);
   c1 += (skip*_outerCadd);
   y1 = 0;
  }
  height = y2-y1;
  ypos = y1*320;
  while (height>0)
  {
   if (x1 > 319<<16) goto skipthisline;
   if (x2 < 0) goto skipthisline;
   if (x2 > 319<<16) x2 = (319<<16);
   if (x1 < 0)
   {
    skip = (0-x1);
    u1 += fixedmul2(skip,_innerUadd);
    v1 += fixedmul2(skip,_innerVadd);
    c1 += fixedmul2(skip,_innerCadd);
    x1 = 0;
   }
   Adjust(x1,xx1);
   Adjust(x2,xx2);
   pos = &(_Buffer[ypos+xx1]);
   InnerLoopGT(pos,(xx2-xx1),u1,v1,c1);
skipthisline:
   x1+=Lslope;
   x2+=Rslope;
   u1+=_outerUadd;
   v1+=_outerVadd;
   c1+=_outerCadd;
   ypos+=320;
   height--;
  }
  _x1=x1;
  _x2=x2;
  _u1=u1;
  _v1=v1;
  _c1=c1;
 }
}

// --------------------------------------------------------------------------

void PolyGT( sPolygon *p )
{
 sVertex *vv1, *vv2, *vv3, *t;
 int x1,y1,u1,v1,c1;
 int x2,y2,u2,v2,c2;
 int x3,y3,u3,v3,c3;
 int x4,u4,v4,c4;

 int slope12,slope13;
 int y3_y1,y2_y1,y3_y2;
 int x3_x1,x2_x1,x3_x2;
 int temp,width;

 vv1 = p->v1;
 vv2 = p->v2;
 vv3 = p->v3;

 #define X1 (vv1->ix)
 #define Y1 (vv1->iy)
 #define X2 (vv2->ix)
 #define Y2 (vv2->iy)
 #define X3 (vv3->ix)
 #define Y3 (vv3->iy)

 if (Y1 > Y2) { t = vv1; vv1 = vv2; vv2 = t; }
 if (Y1 > Y3) { t = vv1; vv1 = vv3; vv3 = t; }
 if (Y2 > Y3) { t = vv2; vv2 = vv3; vv3 = t; }
 if (Y1==Y3) return;
 if ((Y1==Y2) && (X1>X2)) { t = vv1; vv1 = vv2; vv2 = t; }
 if ((Y2==Y3) && (X2>X3)) { t = vv2; vv2 = vv3; vv3 = t; }

//
 x1=vv1->ix; y1=vv1->iy; u1=vv1->u; v1=vv1->v; c1=vv1->c;
 x2=vv2->ix; y2=vv2->iy; u2=vv2->u; v2=vv2->v; c2=vv2->c;
 x3=vv3->ix; y3=vv3->iy; u3=vv3->u; v3=vv3->v; c3=vv3->c;

//   1     1    1 2    1
//  2       2    3    2 3
//    3   3

 Clipping = 0;
 xclip = ((unsigned)x1>319) + ((unsigned)x2>319) + ((unsigned)x3>319);
 if (xclip == 3) return;
 yclip = ((unsigned)y1>199) + ((unsigned)y2>199) + ((unsigned)y3>199);
 if (yclip == 3) return;
 Clipping = xclip+yclip;

 if ( y1 == y2 ) goto FlatTop;
 if ( y2 == y3 ) goto FlatBottom;
 slope12 = (((x2-x1)<<16) / (y2-y1));
 slope13 = (((x3-x1)<<16) / (y3-y1));
 if ( slope12 == slope13 ) return;
 if ( slope12 > slope13 ) goto FlatLeft;
 if ( slope12 < slope13 ) goto FlatRight;
 return;

// ..........................................................................
// 1 2
//  3
FlatTop:
 {
  y3_y1 = y3-y1;
  Lslope     = ((x3-x1)<<16) / (y3_y1);
  Rslope     = ((x3-x2)<<16) / (y3_y1);
  _outerUadd = (u3-u1) / (y3_y1);
  _outerVadd = (v3-v1) / (y3_y1);
  _outerCadd = (c3-c1) / (y3_y1);
  _innerUadd = (u2-u1) / (x2_x1);
  _innerVadd = (v2-v1) / (x2_x1);
  _innerCadd = (c2-c1) / (x2_x1);
  _x1 = x1<<16;
  _x2 = x2<<16;
  _u1 = u1;
  _v1 = v1;
  _c1 = c1;
  FillPolyGT(y1,y3);
  return;
 }

// ..........................................................................
//  1
// 2 3
FlatBottom:
 {
  return;
 }

// ..........................................................................
//  1
//   2
// 3 (4)
FlatLeft:
 {
  return;
 }

// ..........................................................................
//   1
//  2
//(4) 3
FlatRight:
 {
  return;
 }
}

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void visible(void)
{
 FILE *fp;
 int i,j;
 sVertex v1,v2,v3;
 sPolygon p;

 v1.ix = 10  << 16;
 v1.iy = 10  << 16;
 v1.u  = 0   << 16;
 v1.v  = 0   << 16;
 v1.c  = 0   << 16;

 v2.ix = 310  << 16;
 v2.iy = 10   << 16;
 v2.u  = 255  << 16;
 v2.v  = 0    << 16;
 v2.c  = 31   << 16;

 v3.ix = 160  << 16;
 v3.iy = 190  << 16;
 v3.u  = 0    << 16;
 v3.v  = 255  << 16;
 v3.c  = 63   << 16;

 p.v1 = &v1;
 p.v2 = &v2;
 p.v3 = &v3;

 VesaPtr = (WORD *)VesaInit(320,200,15);
 if (VesaPtr == NULL)
 {
  printf("VBE2 320x200x15 not available!\n");
  exit(1);
 }
 _Buffer = VesaPtr;
// _Buffer = (char *)0xA0000;
 _ShadeTable = (WORD *)malloc(256*64*2);
 _Texture = (char *)malloc(65536);
 fp = fopen("tex.pix","rb");
 fread(_Texture,1,65536,fp);
 fclose(fp);
 fp = fopen("tex.pal","rb");
 fread(palette,1,768,fp);
 fclose(fp);

// Calc_ShadeHighLight(&palette, _ShadeTable,50,0,0,0);
/*
 for (i=0;i<256;i++)
 {
  for (j=0;j<64;j++)
  {
   _ShadeTable[j*256+i] = i;
  }
 }
*/
// SetMode(0x13);
// for (i=0;i<256;i++) setrgb(i,palette[i*3],palette[i*3+1],palette[i*3+2]);
 PolyGT(&p);
 getch();
 VesaUnInit((char *)VesaPtr);
// SetMode(3);
 free(_ShadeTable);
 free(_Texture);
}

