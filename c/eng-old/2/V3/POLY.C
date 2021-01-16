#include <math.h>

#include "defines.h"
#include "types.h"
#include "vars.h"
#include "gfx.h"
#include "math3d.h"

// --------------------------------------------------------------------------

void PolyWireFrame( sPolygon *p )
{
 sVertex *v[3];

 v[0] = p->v[0];
 v[1] = p->v[1];
 v[2] = p->v[2];

 Line(v[0]->PosR.x>>16,v[0]->PosR.y>>16,v[1]->PosR.x>>16,v[1]->PosR.y>>16,15);
 Line(v[0]->PosR.x>>16,v[0]->PosR.y>>16,v[2]->PosR.x>>16,v[2]->PosR.y>>16,15);
 Line(v[1]->PosR.x>>16,v[1]->PosR.y>>16,v[2]->PosR.x>>16,v[2]->PosR.y>>16,15);
}


// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

int fill_y1,fill_y2;

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
 int height;
 int xslope;
 sEdge *EdgePtr;

 y1 = CEIL(v1->PosR.y);
 y2 = CEIL(v2->PosR.y);
 if ( y1==y2 ) return;
 height = v2->PosR.y - v1->PosR.y;
 if ( height <= 0 ) return;

 fxslope   = (v2->PosR.x-v1->PosR.x) / height;
 xslope = float2fixed(fxslope);
 if (v1->prestep != 0)
 {
  x = float2fixed(v1->PosR.x + (xxslope*v1->prestep)) + (1<<15);
 }
 else
 {
  x = float2fixed(v1->PosR.x) + (1<<15));
 }

 if (y1 > 199) return;
 if (y2 < 0) return;
 if (y1 < 0)
 {
	skip = (0-y1);
  x  += (xslope*skip);
  y1 = 0;
 }
 if (y2 > 199) y2 = 199;

 if (y1 < fill_y1) fill_y1 = y1;
 if (y2 > fill_y2) fill_y2 = y2;

 EdgePtr = &(Edge[y1]);
 for (j=y1 ; j<y2 ; j++)
 {
  EdgePtr->x2 = x >> 16;
  x += xslope;
  EdgePtr++;
 }
}

// --------------------------------------------------------------------------

void TexScanLeft( sVertex *v1, sVertex *v2 )
{
 int x,u,v;
 int y1,y2,j;
 int skip;
 int height;
 int xslope,uslope,vslope;
 sEdge *EdgePtr;

 y1 = v1->ceil_y >> 16;
 y2 = v2->ceil_y >> 16;
 if ( y1==y2 ) return;
 height = v2->PosR.y - v1->PosR.y;
 if ( height < 0x10000 ) return;

 xslope = fixeddiv(v2->PosR.x-v1->PosR.x , height);
 uslope = fixeddiv(v2->u-v1->u , height);
 vslope = fixeddiv(v2->v-v1->v , height);
 if (v1->prestep != 0)
 {
  x = v1->PosR.x + fixedmul(xslope,v1->prestep) + (1<<15);
  u = v1->u      + fixedmul(uslope,v1->prestep) + (1<<15);
  v = v1->v      + fixedmul(vslope,v1->prestep) + (1<<15);
 }
 else
 {
  x = v1->PosR.x + (1<<15);
  u = v1->u + (1<<15);
  v = v1->v + (1<<15);
 }
 if (y1 > 199) return;
 if (y2 < 0) return;
 if (y1 < 0)
 {
	skip = (0-y1);
  x  += (xslope*skip);
  u  += (uslope*skip);
  v  += (vslope*skip);
  y1 = 0;
 }
 if (y2 > 199) y2 = 199;

 if (y1 < fill_y1) fill_y1 = y1;
 if (y2 > fill_y2) fill_y2 = y2;

 EdgePtr = &(Edge[y1]);
 for (j=y1 ; j<y2 ; j++)
 {
  EdgePtr->x1 = x >> 16;
  EdgePtr->u  = u;
  EdgePtr->v  = v;
  x += xslope;
  u += uslope;
  v += vslope;
  EdgePtr++;
 }
}


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
  x1 = Edge[i].x1;                    // indeksering.. -> pointers
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
   ptr = &(_Buffer[i*320+x1]);        // fsj... en *
   for (j=0;j<length;j++)
   {
    *ptr++ = _Texture[((v>>8)&0xff00)+((u>>16)&0xff)];    // shifting i hytt og pine
    u+=uadd;
    v+=vadd;
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
 int fpdelta,fpdivdelta;
 int fpuadd,fpvadd;
 int y2_y0,y1_y0;

 fill_y1 =  9999;
 fill_y2 = -9999;

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



 fpdelta = p->delta;

 fpuadd = fixeddiv( (   fixedmul(v[1]->u-v[0]->u,y2_y0)
                      - fixedmul(v[2]->u-v[0]->u,y1_y0)),fpdelta);

 fpvadd = fixeddiv( (   fixedmul(v[1]->v-v[0]->v,y2_y0)
                      - fixedmul(v[2]->v-v[0]->v,y1_y0)),fpdelta);

 TexFillPoly(fill_y1,fill_y2,fpuadd,fpvadd);
}
// --------------------------------------------------------------------------


