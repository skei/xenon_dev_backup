#include <math.h>

#include "defines.h"
#include "types.h"
#include "vars.h"

// #include "gfx.h"

// --------------------------------------------------------------------------

int _y1,_y2;

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
 float height,width;
 float xslope,prestep;
 int   ixslope;
 sEdge *EdgePtr;

 y1 = (int)ceil( v1->PosR.y );
 y2 = (int)ceil( v2->PosR.y );
 if ( y1 == y2 ) return;

 if (y1 < _y1) _y1 = y1;
 if (y2 > _y2) _y2 = y2;

 EdgePtr = &(Edge[y1]);
 xslope   = (v2->PosR.x - v1->PosR.x) / (v2->PosR.y - v1->PosR.y);
 ixslope  = (int)(xslope * 65536.0);
 prestep = y1 - v1->PosR.y;
 x = (int)( (v1->PosR.x + (xslope * prestep) ) * 65536.0) + ((1<<16)-1);
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
 int j,y1,y2;
 float  xslope, uslope, vslope;
 int   ixslope,iuslope,ivslope;
 float prestep;
 sEdge *EdgePtr;

 y1 = (int)ceil( v1->PosR.y );
 y2 = (int)ceil( v2->PosR.y );
 if ( y1 == y2 ) return;
 prestep = y1 - v1->PosR.y;
 EdgePtr = &(Edge[y1]);

 xslope   = (v2->PosR.x - v1->PosR.x) / (v2->PosR.y - v1->PosR.y);
 ixslope  = (int)(xslope * 65536.0);
 x = (int)( (v1->PosR.x + (xslope * prestep) ) * 65536.0) + ((1<<16)-1);

 uslope   = (v2->u - v1->u) / (v2->PosR.y - v1->PosR.y);
 iuslope  = (int)(uslope * 65536.0);
 u = (int)( (v1->u + (uslope * prestep) ) * 65536.0) + ((1<<16)-1);

 vslope   = (v2->v - v1->v) / (v2->PosR.y - v1->PosR.y);
 ivslope  = (int)(vslope * 65536.0);
 v = (int)( (v1->v + (vslope * prestep) ) * 65536.0) + ((1<<16)-1);

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

void TexFillPoly( int y1, int y2, int uadd, int vadd )
{
 int i,j;
 int length;
 int x,u,v;
 char *ptr;

 for ( i=y1;i<y2;i++)
 {
  x = Edge[i].x1;
  u = Edge[i].u;
  v = Edge[i].v;
  length = Edge[i].x2 - x;
  ptr = &(_Buffer[i*320+x]);
//  ptr = (char *)(0xA0000+i*320+x);
  for (j=0;j<length;j++)
  {
   *ptr++ = _Texture[((v>>8)&0xff00)+((u>>16)&0xff)];
   u+=uadd;
   v+=vadd;
  }
 }
}

// --------------------------------------------------------------------------

void TexPoly( sPolygon *p )
{
 int a,b,c;
 sVertex *v[3];
 float delta,divdelta,uadd,vadd;

 _y1 =  9999;
 _y2 = -9999;

 v[0] = p->v[0];
 v[1] = p->v[1];
 v[2] = p->v[2];

 v[0]->u = 0;
 v[0]->v = 0;

 v[1]->u = 31;
 v[1]->v = 0;

 v[2]->u = 0;
 v[2]->v = 31;

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

 delta =   (v[1]->PosR.x - v[0]->PosR.x) * (v[2]->PosR.y - v[0]->PosR.y)
         - (v[2]->PosR.x - v[0]->PosR.x) * (v[1]->PosR.y - v[0]->PosR.y);
 divdelta = 1/delta;
 uadd =  ( (v[1]->u - v[0]->u) * (v[2]->PosR.y - v[0]->PosR.y)
          -(v[2]->u - v[0]->u) * (v[1]->PosR.y - v[0]->PosR.y) )*divdelta;
 vadd =  ( (v[1]->v - v[0]->v) * (v[2]->PosR.y - v[0]->PosR.y)
          -(v[2]->v - v[0]->v) * (v[1]->PosR.y - v[0]->PosR.y) )*divdelta;
 TexFillPoly(_y1,_y2,(int)(uadd*65536.0),(int)(vadd*65536.0));
}
// --------------------------------------------------------------------------

