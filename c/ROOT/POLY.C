#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

typedef struct sVertex
{
 float x,y,z;
 float u,v;
} sVertex;

typedef struct sPolygon
{
 sVertex *v[3];
} sPolygon;

typedef struct sEdge
{
 int x1,x2;
 int u,v,c;
} sEdge;

sEdge Edge[200];
char *Texture;
char *_Buffer;
char palette[768];
int _y1,_y2;
float sine[4096];
float cosine[4096];

// --------------------------------------------------------------------------

void PutPixel(int x, int y, char color)
{
 if (((unsigned)x <= 319) && ((unsigned)y <= 199))
 {
  *(char *)(0xA0000 + y*320+x) = color;
 }
}

void Line( int x0, int y0, int x1, int y1, char color )
{
 int dx,ix,cx,dy,iy,cy,m,i;

 if (x0<x1) { dx=x1-x0; ix=1; }
       else { dx=x0-x1; ix=-1; }
 if (y0<y1) { dy=y1-y0; iy=1; }
       else { dy=y0-y1; iy=-1; }
 if (dx>dy) m=dx; else m=dy;
 cx=cy=(m>>1);
 for (i=0;i<m;i++)
 {
  PutPixel(x0,y0,color);
  if ((cx+=dx)>m) { cx-=m; x0+=ix; }
  if ((cy+=dy)>m) { cy-=m; y0+=iy; }
 }
}

// --------------------------------------------------------------------------

void ScanRight( sVertex *v1, sVertex *v2 )
{
 int x,y1,y2,j;
 float height,width;
 float xslope,prestep;
 int   ixslope;
 sEdge *EdgePtr;

 y1 = (int)ceil( v1->y );
 y2 = (int)ceil( v2->y );
 if ( y1 == y2 ) return;

 if (y1 < _y1) _y1 = y1;
 if (y2 > _y2) _y2 = y2;

 EdgePtr = &(Edge[y1]);
 xslope   = (v2->x - v1->x) / (v2->y - v1->y);
 ixslope  = (int)(xslope * 65536.0);
 prestep = y1 - v1->y;

 x = (int)( (v1->x + (xslope * prestep) ) * 65536.0) + (1<<15)/*((1<<16)-1)*/;

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

 y1 = (int)ceil( v1->y );
 y2 = (int)ceil( v2->y );
 if ( y1 == y2 ) return;
 prestep = y1 - v1->y;
 EdgePtr = &(Edge[y1]);

 xslope   = (v2->x - v1->x) / (v2->y - v1->y);
 ixslope  = (int)(xslope * 65536.0);
 x = (int)( (v1->x + (xslope * prestep) ) * 65536.0) + (1<<15)/*((1<<16)-1)*/;

 uslope   = (v2->u - v1->u) / (v2->y - v1->y);
 iuslope  = (int)(uslope * 65536.0);
 u = (int)( (v1->u + (uslope * prestep) ) * 65536.0) + (1<<15)/*((1<<16)-1)*/;

 vslope   = (v2->v - v1->v) / (v2->y - v1->y);
 ivslope  = (int)(vslope * 65536.0);
 v = (int)( (v1->v + (vslope * prestep) ) * 65536.0) + (1<<15)/*((1<<16)-1)*/;

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
   *ptr++ = Texture[((v>>8)&0xff00)+((u>>16)&0xff)];
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

 a = 0;
 if (v[1]->y < v[a]->y) a = 1;
 if (v[2]->y < v[a]->y) a = 2;
 b = a+1; if (b>2) b=0;
 c = a-1; if (c<0) c=2;
//  a     a   a b  c a
//   b   c     c    b    a
// c       b            c b
 TexScanLeft( v[a],v[c] );
 ScanRight  ( v[a],v[b] );
 if (v[b]->y < v[c]->y)
 {
  ScanRight(v[b],v[c]);
 }
 else
 {
  TexScanleft(v[c],v[b]);
 }

 delta =   (v[1]->x - v[0]->x) * (v[2]->y - v[0]->y)
         - (v[2]->x - v[0]->x) * (v[1]->y - v[0]->y);
 divdelta = 1/delta;
 uadd =  ( (v[1]->u - v[0]->u) * (v[2]->y - v[0]->y)
          -(v[2]->u - v[0]->u) * (v[1]->y - v[0]->y) )*divdelta;
 vadd =  ( (v[1]->v - v[0]->v) * (v[2]->y - v[0]->y)
          -(v[2]->v - v[0]->v) * (v[1]->y - v[0]->y) )*divdelta;
 TexFillPoly(_y1,_y2,(int)(uadd*65536.0),(int)(vadd*65536.0));
}
// --------------------------------------------------------------------------

// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께
// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께
// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께
// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께
// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

// Crappy test routines

#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

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

#define PI 3.1415926535

// Calculate sin/cos tables.
void calc_sincos(void)
{
 int i;
 float realangle = -PI;

 for (i = 0; i < 4096; i++)
 {
  sine[i]   = sin(realangle);
  cosine[i] = cos(realangle);
  realangle = realangle + ((PI*2)/4096);
 }
}

// --------------------------------------------------------------------------

void rotate(float *xx, float *yy, float *zz, int xangle, int yangle, int zangle)
{
 float xt,yt,zt;
 float x,y,z;

 xangle &= 4095;
 yangle &= 4095;
 zangle &= 4095;

 x = *xx;
 y = *yy;
 z = *zz;

 yt = y*cosine[xangle] - z*sine[xangle];
 zt = y*sine[xangle]   + z*cosine[xangle];
 y = yt;
 z = zt;
 xt = x*cosine[yangle] - z*sine[xangle];
 zt = x*sine[yangle]   + z*cosine[xangle];
 x = xt;
 z = zt;
 xt = x*cosine[zangle] - y*sine[zangle];
 yt = x*sine[zangle]   + y*cosine[zangle];
 x = xt;
 y = yt;

 z += 256;

 *xx = (x*256/z) + 160;
 *yy = (y*256/z) + 100;
 *zz = z;
}


void main(void)
{
 FILE *fp;
 int i;
 sVertex v1,v2,v3;
 float x,y,z;
 int xa,ya,za;
 sVertex vv1,vv2,vv3;
 sPolygon p;
 float delta,divdelta,uadd,vadd;

 _Buffer = (char *)malloc(64000);//0xA0000;
 Texture = (char *)malloc(65536);
 fp = fopen("texture.pix","rb");
 fread(Texture,1,64000,fp);
 fclose(fp);

 fp = fopen("texture.pal","rb");
 fread(palette,1,768,fp);
 fclose(fp);

 vv1.x = -50;
 vv1.y = -50;
 vv1.z = 1;
 v1.u = 0;
 v1.v = 0;

 vv2.x = 50;
 vv2.y = -50;
 vv2.z = 1;
 v2.u = 15;
 v2.v = 0;

 vv3.x = 0;
 vv3.y = 50;
 vv3.z = 1;
 v3.u = 0;
 v3.v = 15;

 p.v[0] = &v1;
 p.v[1] = &v2;
 p.v[2] = &v3;

 SetMode(0x13);
// for (i=0;i<256;i++) setrgb(i,palette[i*3],palette[i*3+1],palette[i*3+2]);

 calc_sincos();
 xa = 0;
 ya = 0;
 za = 0;

 while(!kbhit())
 {
  x = vv1.x;
  y = vv1.y;
  z = vv1.z;
  rotate(&x,&y,&z,xa,ya,za);
  p.v[0]->x = x;
  p.v[0]->y = y;
  p.v[0]->z = z;
  x = vv2.x;
  y = vv2.y;
  z = vv2.z;
  rotate(&x,&y,&z,xa,ya,za);
  p.v[1]->x = x;
  p.v[1]->y = y;
  p.v[1]->z = z;
  x = vv3.x;
  y = vv3.y;
  z = vv3.z;
  rotate(&x,&y,&z,xa,ya,za);
  p.v[2]->x = x;
  p.v[2]->y = y;
  p.v[2]->z = z;

  memset(_Buffer,0,64000);
  TexPoly(&p);
  memcpy((char *)0xA0000,_Buffer,64000);

  xa += 0;
  ya += 0;
  za += 1;
 }
 getch();
 SetMode(3);
 free(Texture);
 free(_Buffer);
}
