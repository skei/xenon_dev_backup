 #include <math.h>
 #include <stdlib.h>    // getmem
 #include "engine.h"
 // #include "poly2.h"

 #include "math3d.c"
 #include "spline.c"
 #include "api.c"
 #include "render.c"

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

//
// ±±±±± Global variables :-/ ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

char *_Buffer;    // Background Buffer to draw to
char *_Screen;    // _the_ screen

int MinX,MaxX,MinY,MaxY;      // Clipping rectangle
int ScreenWidth,ScreenHeight;  // ViewPort size

sWorld *World;

int TopClip;
int BotClip;
int LeftClip;
int RightClip;

//
// ±±±±± Misc / Help / Support ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void *GetMem( unsigned int Size )
 {
  void *n;

  n = malloc(Size);
  memset(n,0,Size);
  return n;
 }

 // -----

sVector *NewVector(int num)
 { return (sVector *)GetMem(num*sizeof(sVector)); }

sKey *NewKey(int num)
 { return (sKey *)GetMem(num*sizeof(sKey)); }

sMaterial *NewMaterial(int num)
 { return (sMaterial *)GetMem(num*sizeof(sMaterial)); }

sVertex *NewVertex(int num)
 { return (sVertex *)GetMem(num*sizeof(sVertex)); }

sPolygon *NewPolygon(int num)
 { return (sPolygon *)GetMem(num*sizeof(sPolygon)); }

sObject *NewObject(int num)
 { return (sObject *)GetMem(num*sizeof(sObject)); }

sLight *NewLight(int num)
 { return (sLight *)GetMem(num*sizeof(sLight)); }

sCamera *NewCamera(int num)
 { return (sCamera *)GetMem(num*sizeof(sCamera)); }

sWorld *NewWorld(int num)
 { return (sWorld *)GetMem(num*sizeof(sWorld)); }

int *NewInt(int num)
 { return (int *)GetMem(num*sizeof(int)); }

//
// ±±±±± Init ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void Engine_Init(void)
 {
  World = (sWorld *)malloc(sizeof(sWorld));
  _Buffer = (char *)malloc(64000);
  _Screen = (char *)0xA0000;
  MinX = 0;  MaxX = 319;
  MinY = 0;  MaxY = 199;
  ScreenWidth = 320;
  ScreenHeight = 200;
  TopClip   = 0;
  BotClip   = 199;
  LeftClip  = 0;
  RightClip = 319;
  #define maxpolys 10000
  World->RenderList = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
  World->Sorted = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
 }

 // -------------------------------------------------------------------------

void Engine_Exit(void)
 {
 }

//
// ±±±±± Yucku GFX ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void YuckuLine(int x0, int y0, int x1, int y1, char color, char *_B)
 {
  int dx,ix,cx,dy,iy,cy,m,i,pos;

  if (x0<x1) { dx=x1-x0; ix=1; } else { dx=x0-x1; ix=-1; }
  if (y0<y1) { dy=y1-y0; iy=1; } else { dy=y0-y1; iy=-1; }
  if (dx>dy) m=dx; else m=dy;
  cx=cy=(m>>1);
  for (i=0;i<m;i++)
  {
   if ((x0>=MinX)&&(x0<=MaxX)&&(y0>=MinY)&&(y0<=MaxY))
   {
    _B[y0*ScreenWidth+x0] = color;
   }
   if ((cx+=dx)>=m) { cx-=m; x0+=ix; }
   if ((cy+=dy)>=m) { cy-=m; y0+=iy; }
  }
 }

 // -------------------------------------------------------------------------

void YuckuPolyWireFrame( sPolygon *p )
 {
  sVertex *v[3];

  v[0] = p->v[0];
  v[1] = p->v[1];
  v[2] = p->v[2];

  YuckuLine( (int)(v[0]->PosR.x),
             (int)(v[0]->PosR.y),
             (int)(v[1]->PosR.x),
             (int)(v[1]->PosR.y),15,_Buffer);
  YuckuLine( (int)(v[0]->PosR.x),
             (int)(v[0]->PosR.y),
             (int)(v[2]->PosR.x),
             (int)(v[2]->PosR.y),15,_Buffer);
  YuckuLine( (int)(v[1]->PosR.x),
             (int)(v[1]->PosR.y),
             (int)(v[2]->PosR.x),
             (int)(v[2]->PosR.y),15,_Buffer);
 }


