#include "defines.h"
#include "types.h"
#include "vars.h"

void Line(int x0, int y0, int x1, int y1, char color)
{
 int dx,ix,cx,dy,iy,cy,m,i,pos;

 if (x0<x1) { dx=x1-x0; ix=1; } else { dx=x0-x1; ix=-1; }
 if (y0<y1) { dy=y1-y0; iy=1; } else { dy=y0-y1; iy=-1; }
 if (dx>dy) m=dx; else m=dy;
 cx=cy=(m>>1);
 for (i=0;i<m;i++)
 {
  if ( ((unsigned)x0<320) && ((unsigned)x0<320) ) _Screen[y0*320+x0] = color;
  if ((cx+=dx)>=m) { cx-=m; x0+=ix; }
  if ((cy+=dy)>=m) { cy-=m; y0+=iy; }
 }
}

// --------------------------------------------------------------------------

void PolyWireFrame( sPolygon *p )
{
 sVertex *v1,*v2,*v3;

 v1 = p->v1;
 v2 = p->v2;
 v3 = p->v3;

 Line(v1->Pos.x,v1->Pos.y,v2->Pos.x,v2->Pos.y,15);
 Line(v1->Pos.x,v1->Pos.y,v3->Pos.x,v3->Pos.y,15);
 Line(v2->Pos.x,v2->Pos.y,v3->Pos.x,v3->Pos.y,15);
}

