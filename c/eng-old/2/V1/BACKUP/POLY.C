// --------------------------------------------------------------------------
// PolyT:  5.17 (to buffer)..... 193424 pr sec.
//        17.?? (directly to screen).. No VRAM :-/
//         9.?? (directly to LFB/VBE2)
// --------------------------------------------------------------------------

#include <stdlib.h>     // malloc,exit
#include <stdio.h>      // printf
#include <conio.h>      // getch
#include <stdio.h>      // file-stuff

#include "defines.h"
#include "types.h"
#include "vars.h"
#include "vbe2.h"

// ==========================================================================

// fixed point multiplication...
int fixedmul2(int, int);
#pragma aux fixedmul2=\
 "imul ebx",\
 "shrd eax,edx,16",\
 parm [eax] [ebx] value [eax] modify [edx];

// --------------------------------------------------------------------------

#define Adjust(src,dest) { \
 if ((src&&0xffff)==0x8000) src++; \
 dest = ((src+0x8000) >> 16); }

// --------------------------------------------------------------------------

// 께께� GOURAUD TEXTURE 께께께께께께께께께께께께께께께께께께께께께께께께께께


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

 #define X1 (vv1->PosR.x)
 #define Y1 (vv1->PosR.y)
 #define X2 (vv2->PosR.x)
 #define Y2 (vv2->PosR.y)
 #define X3 (vv3->PosR.x)
 #define Y3 (vv3->PosR.y)

 if (Y1 > Y2) { t = vv1; vv1 = vv2; vv2 = t; }
 if (Y1 > Y3) { t = vv1; vv1 = vv3; vv3 = t; }
 if (Y2 > Y3) { t = vv2; vv2 = vv3; vv3 = t; }
 if (Y1==Y3) return;
 if ((Y1==Y2) && (X1>X2)) { t = vv1; vv1 = vv2; vv2 = t; }
 if ((Y2==Y3) && (X2>X3)) { t = vv2; vv2 = vv3; vv3 = t; }

 x1=vv1->PosR.x; y1=vv1->PosR.y; u1=vv1->u; v1=vv1->v; c1=vv1->c;
 x2=vv2->PosR.x; y2=vv2->PosR.y; u2=vv2->u; v2=vv2->v; c2=vv2->c;
 x3=vv3->PosR.x; y3=vv3->PosR.y; u3=vv3->u; v3=vv3->v; c3=vv3->c;

//   1     1    1 2    1
//  2       2    3    2 3
//    3   3

 Clipping = 0;
 xclip = ((unsigned int)x1>319) + ((unsigned int)x2>319) + ((unsigned int)x3>319);
 if (xclip == 3) return;
 yclip = ((unsigned int)y1>199) + ((unsigned int)y2>199) + ((unsigned int)y3>199);
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
  _outerUadd = ((u3-u1)<<16) / (y3_y1);
  _outerVadd = ((v3-v1)<<16) / (y3_y1);
  _outerCadd = ((c3-c1)<<16) / (y3_y1);
  _innerUadd = ((u2-u1)<<16) / (x2_x1);
  _innerVadd = ((v2-v1)<<16) / (x2_x1);
  _innerCadd = ((c2-c1)<<16) / (x2_x1);
  _x1 = x1<<16;
  _x2 = x2<<16;
  _u1 = u1<<16;
  _v1 = v1<<16;
  _c1 = c1<<16;
  FillPolyGT(y1,y3);
  return;
 }

// ..........................................................................
//  1
// 2 3
FlatBottom:
 {
  Lslope     = ((x2-x1)<<16) / (y2-y1);
  Rslope     = ((x3-x1)<<16) / (y3-y1);
  _outerUadd = ((u2-u1)<<16) / (y2-y1);
  _outerVadd = ((v2-v1)<<16) / (y2-y1);
  _outerCadd = ((c2-c1)<<16) / (y2-y1);
  _innerUadd = ((u3-u1)<<16) / (x3-x1);
  _innerVadd = ((v3-v1)<<16) / (x3-x1);
  _innerCadd = ((c3-c1)<<16) / (x3-x1);
  _x1 = x1<<16;
  _x2 = x1<<16;
  _u1 = u1<<16;
  _v1 = v1<<16;
  _c1 = c1<<16;
  FillPolyGT(y1,y3);
  return;
 }

// ..........................................................................
//  1
//   2
// 3 (4)
FlatLeft:
 {
  Lslope     = ((x3-x1)<<16) / (y3-y1);
  Rslope     = ((x2-x1)<<16) / (y2-y1);
  _outerUadd = ((u3-u1)<<16) / (y3-y1);
  _outerVadd = ((v3-v1)<<16) / (y3-y1);
  _outerCadd = ((c3-c1)<<16) / (y3-y1);
  x4 = x1 + (( Rslope                    * (y3-y1))>>16);
  u4 = u1 + (( (((u2-u1)<<16) / (y2-y1)) * (y3-y1))>>16);
  v4 = v1 + (( (((v2-v1)<<16) / (y2-y1)) * (y3-y1))>>16);
  c4 = c1 + (( (((c2-c1)<<16) / (y2-y1)) * (y3-y1))>>16);
  _innerUadd = ((u4-u3)<<16) / (x4-x3);
  _innerVadd = ((v4-v3)<<16) / (x4-x3);
  _innerCadd = ((c4-c3)<<16) / (x4-x3);
  _x1 = x1<<16;
  _x2 = x1<<16;
  _u1 = u1<<16;
  _v1 = v1<<16;
  _c1 = c1<<16;
  FillPolyGT(y1,y2);
  Rslope     = ((x3-x2)<<16) / (y3-y2);
  FillPolyGT(y2,y3);
  return;
 }

// ..........................................................................
//   1
//  2
//(4) 3
FlatRight:
 {
  Lslope     = ((x2-x1)<<16) / (y2-y1);
  Rslope     = ((x3-x1)<<16) / (y3-y1);
  _outerUadd = ((u2-u1)<<16) / (y2-y1);
  _outerVadd = ((v2-v1)<<16) / (y2-y1);
  _outerCadd = ((c2-c1)<<16) / (y2-y1);
  x4 = x1 + (( Lslope                    * (y3-y1))>>16);
  u4 = u1 + (( (((u2-u1)<<16) / (y2-y1)) * (y3-y1))>>16);
  v4 = v1 + (( (((v2-v1)<<16) / (y2-y1)) * (y3-y1))>>16);
  c4 = c1 + (( (((c2-c1)<<16) / (y2-y1)) * (y3-y1))>>16);
  _innerUadd = ((u3-u4)<<16) / (x3-x4);
  _innerVadd = ((v3-v4)<<16) / (x3-x4);
  _innerCadd = ((c3-c4)<<16) / (x3-x4);
  _x1 = x1<<16;
  _x2 = x1<<16;
  _u1 = u1<<16;
  _v1 = v1<<16;
  _c1 = c1<<16;
  FillPolyGT(y1,y2);
  Lslope     = ((x3-x2)<<16) / (y3-y2);
  _outerUadd = ((u3-u2)<<16) / (y3-y2);
  _outerVadd = ((v3-v2)<<16) / (y3-y2);
  _outerCadd = ((c3-c2)<<16) / (y3-y2);
  FillPolyGT(y2,y3);
  return;
 }
}


