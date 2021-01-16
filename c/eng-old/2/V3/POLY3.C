#include <math.h>

#include "defines.h"
#include "types.h"
#include "vars.h"
#include "gfx.h"

// --------------------------------------------------------------------------

/*
typedef struct sEdge
{
 int x1,x2;
 int u,v;
 int c;
} sEdge;

sEdge Edge[200];
*/

#define S 65536.0
#define MAGIC (((S*S*16)+(S*.5))*S)
#define MAGICfixed (MAGIC/65536.0)

// 컴컴컴컴컴

#pragma inline float2int;
static long float2int( float d )
{
 double dtemp = MAGIC+d;
 return (*(long *)&dtemp)-0x80000000;
}

// 컴컴컴컴컴

#pragma inline float2fixed;
static long float2fixed( float d )
{
 double dtemp = MAGICfixed+d;
 return (*(long *)&dtemp)-0x80000000;
}

// 컴컴컴컴컴

#pragma inline CEIL;
static int CEIL(float f)
{
 int i,j;

 i = float2fixed(f);
 j = i >> 16;
 if ((i&0xffff) != 0) j+=1;
 return j;
}


// --------------------------------------------------------------------------

void PolyWireFrame( sPolygon *p )
{
 sVertex *v[3];

 v[0] = p->v[0];
 v[1] = p->v[1];
 v[2] = p->v[2];

 Line(v[0]->PosR.x,v[0]->PosR.y,v[1]->PosR.x,v[1]->PosR.y,15);
 Line(v[0]->PosR.x,v[0]->PosR.y,v[2]->PosR.x,v[2]->PosR.y,15);
 Line(v[1]->PosR.x,v[1]->PosR.y,v[2]->PosR.x,v[2]->PosR.y,15);
}

// --------------------------------------------------------------------------

// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

int _y1,_y2;
volatile int _inneru,_innerv,_inneruadd,_innervadd;

typedef struct sEdge
{
 int x1,x2;
 int u,v;
 int c;
} sEdge;

sEdge Edge[200];

// --------------------------------------------------------------------------

void ScanRight( sVertex *v1, sVertex *v2 )
{
 int x,y1,y2,j;
 int skip;
 float height,width;
 float xslope,prestep;
 int   ixslope;
 sEdge *EdgePtr;

 y1 = CEIL( v1->PosR.y );
 y2 = CEIL( v2->PosR.y );
 if ( y1 == y2 ) return;

 xslope   = (v2->PosR.x - v1->PosR.x) / (v2->PosR.y - v1->PosR.y);
 ixslope  = float2fixed(xslope);
 prestep = y1 - v1->PosR.y;
 x = float2fixed(v1->PosR.x + (xslope*prestep)) + (1<<15);

 if (y1 > 199) return;
 if (y2 < 0) return;
 if (y1 < 0)
 {
	skip = (0-y1);
  x  += (ixslope*skip);
  y1 = 0;
 }
 if (y2 > 199) y2 = 199;

 if (y1 < _y1) _y1 = y1;
 if (y2 > _y2) _y2 = y2;

 EdgePtr = &(Edge[y1]);
 for (j=y1 ; j<y2 ; j++)
 {
  EdgePtr->x2 = x >> 16;
  x += ixslope;
  EdgePtr++;
 }
}

// --------------------------------------------------------------------------

void TexScanLeft( sVertex *v1, sVertex *v2 )
{
 int x,u,v;
 int skip;
 int j,y1,y2;
 float height;
 float  xslope, uslope, vslope;
 int   ixslope,iuslope,ivslope;
 float prestep;
 sEdge *EdgePtr;

 y1 = CEIL( v1->PosR.y );
 y2 = CEIL( v2->PosR.y );
 if ( y1 == y2 ) return;
 prestep = y1 - v1->PosR.y;

 height = (v2->PosR.y - v1->PosR.y);
 xslope   = (v2->PosR.x - v1->PosR.x) / height;//(v2->PosR.y - v1->PosR.y);
 ixslope  = float2fixed(xslope);
 x = float2fixed(v1->PosR.x + (xslope*prestep)) + (1<<15);

 uslope   = (v2->u - v1->u) / height;//(v2->PosR.y - v1->PosR.y);
 iuslope  = float2fixed(uslope);
 u = float2fixed(v1->u + (uslope*prestep)) + (1<<15);

 vslope   = (v2->v - v1->v) / height;//(v2->PosR.y - v1->PosR.y);
 ivslope  = float2fixed(vslope);
 v = float2fixed(v1->v + (vslope*prestep)) + (1<<15);
// ----- clipping -----
 if (y1 > 199) return;
 if (y2 < 0) return;
 if (y1 < 0)
 {
	skip = (0-y1);
  x  += (ixslope*skip);
  u  += (iuslope*skip);
  v  += (ivslope*skip);
  y1 = 0;
 }
 if (y2 > 199) y2 = 199;
// --------------------
 EdgePtr = &(Edge[y1]);
 for (j=y1 ; j<y2 ; j++)
 {
  EdgePtr->x1 = x >> 16;
  EdgePtr->u  = u;
  EdgePtr->v  = v;
  x += ixslope;
  u += iuslope;
  v += ivslope;
  EdgePtr++;
 }
}

// --------------------------------------------------------------------------

void GTexScanLeft( sVertex *v1, sVertex *v2 )
{
 int x,u,v,c;
 int skip;
 int j,y1,y2;
 float height;
 float  xslope, uslope, vslope, cslope;
 int   ixslope,iuslope,ivslope, icslope;
 float prestep;
 sEdge *EdgePtr;

 y1 = CEIL( v1->PosR.y );
 y2 = CEIL( v2->PosR.y );
 if ( y1 == y2 ) return;
 prestep = y1 - v1->PosR.y;

 height = (v2->PosR.y - v1->PosR.y);
 xslope   = (v2->PosR.x - v1->PosR.x) / height;//(v2->PosR.y - v1->PosR.y);
 ixslope  = float2fixed(xslope);
 x = float2fixed(v1->PosR.x + (xslope*prestep)) + (1<<15);

 uslope   = (v2->u - v1->u) / height;//(v2->PosR.y - v1->PosR.y);
 iuslope  = float2fixed(uslope);
 u = float2fixed(v1->u + (uslope*prestep)) + (1<<15);

 vslope   = (v2->v - v1->v) / height;//(v2->PosR.y - v1->PosR.y);
 ivslope  = float2fixed(vslope);
 v = float2fixed(v1->v + (vslope*prestep)) + (1<<15);

 cslope   = (v2->c - v1->c) / height;//(v2->PosR.y - v1->PosR.y);
 icslope  = float2fixed(cslope);
 c = float2fixed(v1->c + (cslope*prestep)) + (1<<15);

// ----- clipping -----
 if (y1 > 199) return;
 if (y2 < 0) return;
 if (y1 < 0)
 {
	skip = (0-y1);
  x  += (ixslope*skip);
  u  += (iuslope*skip);
  v  += (ivslope*skip);
  v  += (icslope*skip);
  y1 = 0;
 }
 if (y2 > 199) y2 = 199;
// --------------------
 EdgePtr = &(Edge[y1]);
 for (j=y1 ; j<y2 ; j++)
 {
  EdgePtr->x1 = x >> 16;
  EdgePtr->u  = u;
  EdgePtr->v  = v;
  EdgePtr->c  = c;
  x += ixslope;
  u += iuslope;
  v += ivslope;
  c += icslope;
  EdgePtr++;
 }
}


// --------------------------------------------------------------------------

// eax   vfracADD  temp
// ebx   ufracADD  vadd:uadd
// ecx * ufrac
// edx * TEXTURE    v:u
// esi * vfrac
// edi   screen/buffer
// ebp   length/counter
void TexInner(int length, char *scrpos, char *texture);
#pragma aux TexInner=\
 " push ebp",\
 " mov ebp,eax",\
 " mov edx,[_Texture]",\
 " mov eax,[_inneru]",\
 " rol eax,16",\
 " mov ecx,eax",\
 " mov dl,al",\
 " mov eax,[_innerv]",\
 " rol eax,16",\
 " mov esi,eax",\
 " mov dh,al",\
 " mov ebx,[_inneruadd]",\
 " rol ebx,16",\
 " mov eax,[_innervadd]",\
 " rol eax,16",\
 " mov bh,al",\
 "L1:",\
 " mov al,[edx]",\
 " add ecx,ebx",\
 " adc dl,bl",\
 " mov [edi],al",\
 " add esi,eax",\
 " adc dh,bh",\
 " inc edi",\
 " dec ebp",\
 " jnz L1",\
 " pop ebp",\
 parm [eax][edi][esi] modify [eax ebx ecx edx esi edi];

/*
....................
Gouraud texture

eax   0        temp
ebx            temp
ecx ufrac   cint:cfrac
edx   0        v:u
esi vfrac
edi screen/buffer
ebp length/counter

 L1:
  mov al,[0x12345678h+edx]  ; texture
  mov ah,ch
  mov bl,[0x12345678h+eax]  ; shadetable
  mov [edi],bl

  add ecx,0x11112222h
  adc dl,0x11h
  add esi,0x11110000h
  adc dh,0x11h

  dec ebp
  jnz L1
*/

// --------------------------------------------------------------------------

void TexFillPoly( int y1, int y2, int uadd, int vadd )
{
 int i,j;
 int length,skip;
 int x1,x2;
 int u,v;
 char *ptr;
 char *test;

 if (y1==y2) return;
 if (y1 > 199) return;
 if (y2 < 0) return;
 for ( i=y1;i<y2;i++)
 {
  x1 = Edge[i].x1;
  x2 = Edge[i].x2;
//  x = 0;
  u = Edge[i].u;
  v = Edge[i].v;

  if (x1 > 319) goto skipthisline;
	if (x2 < 0) goto skipthisline;
  if (x2 > 319) x2 = 319;
	if (x1 < 0)
	{
	 skip = (0-x1);
   u += (uadd*skip);
   v += (vadd*skip);
	 x1 = 0;
	}
  length = x2 - x1;
  if (length > 0)
  {
//   _inneru = u;
//   _innerv = v;
//   _inneruadd = uadd;
//   _innervadd = vadd;
   ptr = &(_Buffer[i*320+x1]);
//   TexInner(length,ptr,_Texture);
   for (j=0;j<length;j++)
   {
    *ptr++ = _Texture[((v>>8)&0xff00)+((u>>16)&0xff)];
    u+=uadd;
    v+=vadd;
   }
  }
  skipthisline:;
 }
}

// --------------------------------------------------------------------------

void GTexFillPoly( int y1, int y2, int uadd, int vadd, int cadd)
{
 int i,j;
 int length,skip;
 int x1,x2;
 int u,v,c;
 char *ptr;
 char *test;

 if (y1==y2) return;
 if (y1 > 199) return;
 if (y2 < 0) return;
 for ( i=y1;i<y2;i++)
 {
  x1 = Edge[i].x1;
  x2 = Edge[i].x2;
//  x = 0;
  u = Edge[i].u;
  v = Edge[i].v;
  c = Edge[i].c;

  if (x1 > 319) goto skipthisline;
	if (x2 < 0) goto skipthisline;
  if (x2 > 319) x2 = 319;
	if (x1 < 0)
	{
	 skip = (0-x1);
   u += (uadd*skip);
   v += (vadd*skip);
   c += (cadd*skip);
	 x1 = 0;
	}
  length = x2 - x1;
  if (length > 0)
  {
//   _inneru = u;
//   _innerv = v;
//   _inneruadd = uadd;
//   _innervadd = vadd;
   ptr = &(_Buffer[i*320+x1]);
//   TexInner(length,ptr,_Texture);
   for (j=0;j<length;j++)
   {
    *ptr++ = _ShadeTable[  (c>>8)&0xff00)
                         +_Texture[((v>>8)&0xff00)+((u>>16)&0xff)] ];
    u+=uadd;
    v+=vadd;
    c+=cadd;
   }
  }
  skipthisline:;
 }
}


// --------------------------------------------------------------------------

void TexPoly( sPolygon *p )
{
 int a,b,c;
 sVertex *v[3];
 float delta,divdelta,uadd,vadd;
 float y2_y0,y1_y0;

 float minu,maxu,minv,maxv,d;

 _y1 =  9999;
 _y2 = -9999;

 v[0] = p->v[0];
 v[1] = p->v[1];
 v[2] = p->v[2];

 a = 0;
 if (v[1]->PosR.y < v[a]->PosR.y) a = 1;
 if (v[2]->PosR.y < v[a]->PosR.y) a = 2;
 b = a+1; if (b>2) b=0;
 c = a-1; if (c<0) c=2;
//  a     a   a b  c a
//   b   c     c    b    a
// c       b            c b
 TexScanLeft( v[a],v[c] );
 ScanRight  ( v[a],v[b] );
 if (v[b]->PosR.y < v[c]->PosR.y)
 {
  ScanRight(v[b],v[c]);
 }
 else
 {
  TexScanleft(v[c],v[b]);
 }

 y1_y0 = (v[1]->PosR.y - v[0]->PosR.y);
 y2_y0 = (v[2]->PosR.y - v[0]->PosR.y);

 delta =   (v[1]->PosR.x - v[0]->PosR.x) * y2_y0
         - (v[2]->PosR.x - v[0]->PosR.x) * y1_y0;
 divdelta = 1/delta;
 uadd =  ( (v[1]->u - v[0]->u) * y2_y0
          -(v[2]->u - v[0]->u) * y1_y0 )*divdelta;
 vadd =  ( (v[1]->v - v[0]->v) * y2_y0
          -(v[2]->v - v[0]->v) * y1_y0 )*divdelta;
 TexFillPoly(_y1,_y2,float2fixed(uadd),float2fixed(vadd));
}

// --------------------------------------------------------------------------

void GTexPoly( sPolygon *p )
{
 int a,b,c;
 sVertex *v[3];
 float delta,divdelta,uadd,vadd;
 float y2_y0,y1_y0;

 float minu,maxu,minv,maxv,d;

 _y1 =  9999;
 _y2 = -9999;

 v[0] = p->v[0];
 v[1] = p->v[1];
 v[2] = p->v[2];

 a = 0;
 if (v[1]->PosR.y < v[a]->PosR.y) a = 1;
 if (v[2]->PosR.y < v[a]->PosR.y) a = 2;
 b = a+1; if (b>2) b=0;
 c = a-1; if (c<0) c=2;
//  a     a   a b  c a
//   b   c     c    b    a
// c       b            c b
 TexScanLeft( v[a],v[c] );
 ScanRight  ( v[a],v[b] );
 if (v[b]->PosR.y < v[c]->PosR.y)
 {
  ScanRight(v[b],v[c]);
 }
 else
 {
  TexScanleft(v[c],v[b]);
 }

 y1_y0 = (v[1]->PosR.y - v[0]->PosR.y);
 y2_y0 = (v[2]->PosR.y - v[0]->PosR.y);

 delta =   (v[1]->PosR.x - v[0]->PosR.x) * y2_y0
         - (v[2]->PosR.x - v[0]->PosR.x) * y1_y0;
 divdelta = 1/delta;
 uadd =  ( (v[1]->u - v[0]->u) * y2_y0
          -(v[2]->u - v[0]->u) * y1_y0 )*divdelta;
 vadd =  ( (v[1]->v - v[0]->v) * y2_y0
          -(v[2]->v - v[0]->v) * y1_y0 )*divdelta;
 cadd =  ( (v[1]->c - v[0]->c) * y2_y0
          -(v[2]->c - v[0]->c) * y1_y0 )*divdelta;

 TexFillPoly(_y1,_y2,float2fixed(uadd),float2fixed(vadd),float2fixed(cadd));
}

