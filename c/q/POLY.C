#include "engine.h"

 // 컴컴컴컴컴

#define FIXED_ONE 256

#define POSITIVE  0
#define NEGATIVE  1

 // 컴컴컴컴컴

// (16.16 << 16) / 16.16
// ( 8.8  << 16) / 16.16
int fixdiv16(int,int);
#pragma aux fixdiv16=\
 "cdq",\
 "shld edx,eax,16",\
 "idiv ebx",\
 parm [eax][ebx] value [eax] modify [edx];

 // 컴컴컴컴컴

void WobblePolyGouraudTexture( sPolygon *P )
{
 char *tex;
 sVertex *p[3];
 sVertex *tmp;
 int temp;
 char sign;
 int plus,width;
 int dx,dy,df;
 int  rex, lex, etx, ety, fade;
 int drex,dlex,detx,dety,dfade;
 int u,v,fd;
 int yoffs,yy;
 int begx,endx;
 int sa,sw;
 unsigned short int *ptr;
 int x0,y0,u0,v0,c0;
 int x1,y1,u1,v1,c1;
 int x2,y2,u2,v2,c2;

 p[0]=P->v[0];
 p[1]=P->v[1];
 p[2]=P->v[2];
 // Sort points so that 1 = topmost, 2 = middle, 3 = lowmost
 if (p[1]->iy < p[0]->iy)
 {
  tmp=p[0];
  p[0]=p[1];
  p[1]=tmp;
 }
 if (p[2]->iy < p[0]->iy)
 {
  tmp=p[0];
  p[0]=p[2];
  p[2]=tmp;
 }
 if (p[2]->iy < p[1]->iy)
 {
  tmp=p[1];
  p[1]=p[2];
  p[2]=tmp;
 }

/*

 u0 = 0;
 v0 = 0;
 c0 = 0;

 u1 = 127;
 v1 = 0;
 c1 = 63;

 u2 = 0;
 v2 = 127;
 c2 = 63;

*/

 x0 = p[0]->ix; y0 = p[0]->iy; u0 = p[0]->iu; v0 = p[0]->iv; c0 = p[0]->ic;
 x1 = p[1]->ix; y1 = p[1]->iy; u1 = p[1]->iu; v1 = p[1]->iv; c1 = p[1]->ic;
 x2 = p[2]->ix; y2 = p[2]->iy; u2 = p[2]->iu; v2 = p[2]->iv; c2 = p[2]->ic;

 // If triangle has no height, return
 if (y2==y0) return;
 // Calculate a multiplier
 temp=((y1-y0)<<16)/(y2-y0);
 tex=P->Object->Texture1;
 // Calc width of widest scanline
 width=(temp * (x2 - x0) + ((x0 - x1) << 16 ));
 // If polygon has no width, return
 if (width==0) return;
 if (width<0)
 {
  if (width>-FIXED_ONE) return;
 }
 else
 {
  if (width<FIXED_ONE) return;
 }
 // Calculate dx and dv (du & dv) and dfade for widest scanline. They stay constant
 // over whole triangle.
 dx=fixdiv16((( (temp * (u2 - u0))  ) + ((u0 - u1 ) <<16)) , width)>>8;
 dy=fixdiv16((( (temp * (v2 - v0))  ) + ((v0 - v1 ) <<16)) , width)>>8;
 df=fixdiv16((( (temp * (c2 - c0))  ) + ((c0 - c1 ) <<16)) , width)>>8;
 // Which side is right, and which is left
 if (width>0) sign=NEGATIVE; else sign=POSITIVE;
 width=abs(width);
 // Set up stuff
 rex =x0<<8;
 lex =x0<<8;
 etx =u0<<8;
 ety =v0<<8;
 fade=c0<<8;
 if (sign==POSITIVE)
 {
  dlex=((x2-p[0]->ix)<<8)/(y2-y0);
  if (y1!=y0) drex=((x1-x0)<<8)/(y1-y0);
  else drex=((x1-x2)<<8)/(y1-y2);
  detx =((u2-u0)<<8)/(y2-y0);
  dety =((v2-v0)<<8)/(y2-y0);
  dfade=((c2-c0)<<8)/(y2-y0);
 }
 else
 {
  drex=((x2-x0)<<8)/(y2-y0);
  if (y1!=y0)
  {
   dlex =((x1-x0)<<8)/(y1-y0);
   detx =((u1-u0)<<8)/(y1-y0);
   dety =((v1-v0)<<8)/(y1-y0);
   dfade=((c1-c0)<<8)/(y1-y0);
  }
  else
  {
   dlex =((x1-x2)<<8)/(y1-y2);
   detx =((u1-u2)<<8)/(y1-y2);
   dety =((v1-v2)<<8)/(y1-y2);
   dfade=((c1-c2)<<8)/(y1-y2);
  }
 }
// yoffs = iy*320
 yoffs=y0 * 320;
 // Draw polygon
 for (yy=y0;yy<y2;yy++)
 {
  if (yy>199) return;
  if (sign==POSITIVE)
  {
   if (yy==y1 && y1!=y2)
   {
    rex=x1<<8;
    drex=((x2-x1)<<8)/(y2-y1);
   }
  }
  else
  {
   if (yy==y1 && y1!=y2)
   {
    lex=x1<<8;
    dlex=((x2-x1)<<8)/(y2-y1);
    etx =u1<<8;
    ety =v1<<8;
    fade=c1<<8;
    detx =((u2-u1)<<8)/(y2-y1);
    dety =((v2-v1)<<8)/(y2-y1);
    dfade=((c2-c1)<<8)/(y2-y1);
   }
  }
  plus=0;
  begx=lex>>8;
  endx=rex>>8;
  if (endx>319) endx=319;
  if (begx<0)
  {
   plus=-begx;
   begx=0;
  }
  sa=yoffs+(begx);
  sw=(endx-begx);
  ptr = &(_VesaBuffer[sa]);
  u=etx+plus*dx;
  v=ety+plus*dy;
  fd=fade+plus*df;
  if (yy>=0 && sw>=1)
   while(sw--)
   {
    // If no pixel is drawn to this point, draw a pixel
    *ptr = _VesaShadeTable[ (fd&0xff00) + tex[(v&0xff00)+((u>>8)&0xff)] ];
    u+=dx;
    v+=dy;
    fd+=df;
    ptr++;
   }
   yoffs+=320;
   rex+=drex;
   lex+=dlex;
   etx+=detx;
   ety+=dety;
   fade+=dfade;
 }
}

