 // #include <stdio.h>
 // #include <math.h>

 #include "engine.h"

//
// ±±±±± Variables ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

 #define VISIBLE     0
 #define TOP         1
 #define BOT         2
 #define LEFT        4
 #define RIGHT       8

 char *destptr;
 char *_PolyTexture;

 sVertex TempVertices[12];    // array 12 of sVertex
 sVertex *ClipPoly[12];       // array 12 of pointers to sVertex
 sVertex *ClipTemp[12];       // array 12 of pointers to sVertex

 int max_vtx;                 // Max y vertex (ending vertex)
 int start_vtx, end_vtx;     // First and last vertex in array
 int right_vtx, left_vtx;    // Current right and left vertex

 int right_x_slope,right_x,right_height;
 int  left_x_slope, left_x, left_height;

 int left_u,left_v;
 int left_u_slope, left_v_slope, left_c_slope;
 int inner_u_slope,inner_v_slope,inner_c_slope;

//
// ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

//
// --- CLIPPING -------------------------------------------------------------
//


int cliptest( sVertex *v)
 {
  int bits = 0;
  if(v->PosR.y < TopClip)   bits |= TOP;
  if(v->PosR.y > BotClip)   bits |= BOT;
  if(v->PosR.x < LeftClip)  bits |= LEFT;
  if(v->PosR.x > RightClip) bits |= RIGHT;
  return bits;
 }

 // -----

int cliptestx( sVertex *v)
 {
  int bits = 0;
  if(v->PosR.y < LeftClip)     bits |= LEFT;
  if(v->PosR.y > RightClip)    bits |= RIGHT;
  return bits;
 }

 // -----

int cliptesty( sVertex *v)
 {
  int bits = 0;
  if(v->PosR.y < TopClip)      bits |= TOP;
  if(v->PosR.y > BotClip)      bits |= BOT;
  return bits;
 }


 // -------------------------------------------------------------------------

 // Clips polygon in _Poly, returns final poly in _Clipped
 // returns number of visible vertices
int ClipUV( void )
 {
  int i;
  int n,num,dstnum;
  int dstptr;
  float a,ima;
  sVertex *v1,*v2;

  dstptr = 0;       // number of NEW vertices (number in TempVertices
  num = 3;          // Number of vertices in resulting poly
  dstnum = 0;       // number of vertices in dest.poly (temp)

  // --- left clip ---

  for(n=0; n<num; n++)
  {
   v1 = ClipPoly[n];
   v2 = ClipPoly[n+1];

   if ((v1->clip_visible & LEFT) == VISIBLE)
   {
    ClipTemp[dstnum++] = v1;//ClipPoly[n];
    if((v2->clip_visible & LEFT) == VISIBLE) continue;
   }
   else
   if ((v2->clip_visible & LEFT) != VISIBLE) continue;
   a = (LeftClip - v1->PosR.x) / (v2->PosR.x - v1->PosR.x);
   ima = 1.0 - a;
   ClipTemp[dstnum] = &TempVertices[dstptr++];
   ClipTemp[dstnum]->PosR.x = LeftClip;
   ClipTemp[dstnum]->PosR.y = v1->PosR.y*ima + v2->PosR.y*a;
   ClipTemp[dstnum]->u      = v1->u*ima + v2->u*a;
   ClipTemp[dstnum]->v      = v1->v*ima + v2->v*a;
   ClipTemp[dstnum]->clip_visible = cliptesty(ClipTemp[dstnum]);
   dstnum++;
  }
  ClipTemp[dstnum] = ClipTemp[0];
  num = dstnum;
  dstnum = 0;

  // --- right clip ---

  for(n=0; n<num; n++)
  {
   v1 = ClipTemp[n];             // current vertex
   v2 = ClipTemp[n+1];           // next vertex

   if((v1->clip_visible & RIGHT) == VISIBLE)
   {
    ClipPoly[dstnum++] = v1;                 // add visible vertex to list
    if((v2->clip_visible & RIGHT) == VISIBLE) continue;
   }
   else
   if ((v2->clip_visible & RIGHT) != VISIBLE) continue;
   a = (RightClip - v1->PosR.x) / (v2->PosR.x - v1->PosR.x);
   ima = 1.0-a;
   ClipPoly[dstnum] = &TempVertices[dstptr++];
   ClipPoly[dstnum]->PosR.x = RightClip;
   ClipPoly[dstnum]->PosR.y = v1->PosR.y*ima + v2->PosR.y*a;
   ClipPoly[dstnum]->u      = v1->u*ima + v2->u*a;
   ClipPoly[dstnum]->v      = v1->v*ima + v2->v*a;
   ClipPoly[dstnum]->clip_visible = cliptesty(ClipPoly[dstnum]);
   dstnum++;
  }
  ClipPoly[dstnum] = ClipPoly[0];
  num = dstnum;
  dstnum = 0;

  // --- top clip ---

  for(n=0; n<num; n++)
  {
   v1 = ClipPoly[n];             // current vertex
   v2 = ClipPoly[n+1];           // next vertex
   if((v1->clip_visible & TOP) == VISIBLE)
   {
    ClipTemp[dstnum++] = v1;                 // add visible vertex to list
    if((v2->clip_visible & TOP) == VISIBLE) continue;
   }
   else
   if((v2->clip_visible & TOP) != VISIBLE) continue;
   a = (TopClip - v1->PosR.y) / (v2->PosR.y - v1->PosR.y);
   ima = 1.0-a;
   ClipTemp[dstnum] = &TempVertices[dstptr++];
   ClipTemp[dstnum]->PosR.x = v1->PosR.x*ima + v2->PosR.x*a;
   ClipTemp[dstnum]->PosR.y = TopClip;
   ClipTemp[dstnum]->u      = v1->u*ima + v2->u*a;
   ClipTemp[dstnum]->v      = v1->v*ima + v2->v*a;
   ClipTemp[dstnum]->clip_visible = cliptestx(ClipTemp[dstnum]);
   dstnum++;
  }
  ClipTemp[dstnum] = ClipTemp[0];
  num = dstnum;
  dstnum = 0;

  // --- bot clip ---

  for(n=0; n<num; n++)
  {
   v1 = ClipTemp[n];             // current vertex
   v2 = ClipTemp[n+1];           // next vertex
   if((v1->clip_visible & BOT) == VISIBLE)
   {
    ClipPoly[dstnum++] = v1;                 // add visible vertex to list
    if((v2->clip_visible & BOT) == VISIBLE) continue;
   }
   else
   if((v2->clip_visible & BOT) != VISIBLE) continue;
   a = (BotClip - v1->PosR.y) / (v2->PosR.y - v1->PosR.y);
   ima = 1.0-a;
   ClipPoly[dstnum] = &TempVertices[dstptr++];
   ClipPoly[dstnum]->PosR.x = v1->PosR.x*ima + v2->PosR.x*a;
   ClipPoly[dstnum]->PosR.y = BotClip;
   ClipPoly[dstnum]->u = v1->u*ima + v2->u*a;
   ClipPoly[dstnum]->v = v1->v*ima + v2->v*a;
   dstnum++;
  }
  return dstnum;
 }


//
// --- ALL ------------------------------------------------------------------
//

void CalcRight(void)
 {
  sVertex *v1,*v2;
  float height;
  float xslope;

  // Walk forward trough the vertex array
  v1 = ClipPoly[right_vtx];
  if(right_vtx < end_vtx)
  {
   v2 = ClipPoly[right_vtx+1];
   right_vtx += 1;
  }
  else
  {
   v2 = ClipPoly[start_vtx];                     // Wrap to start of array
   right_vtx = start_vtx;
  }

  // v1 = top vertex
  // v2 = bottom vertex
  right_height = v2->ceil_y - v1->ceil_y;
  if (right_height <= 0) return;
  height = v2->PosR.y - v1->PosR.y;
  xslope = (v2->PosR.x - v1->PosR.x)/height;
  right_x_slope = float2fixed(xslope);
  //prestep = CEIL(v1->PosR.y) - v1->PosR.y;
  right_x = (1<<15) + float2fixed( v1->PosR.x + (v1->prestep * xslope));
 }

//
// --- TEXTURE --------------------------------------------------------------
//

void CalcLeftTex(void)
 {
  sVertex *v1,*v2;
  float height;
  float invheight;
  float xslope,uslope,vslope;

  // Walk backwards trough the vertex array
  v1 = ClipPoly[left_vtx];
  if(left_vtx > start_vtx)
  {
   v2 = ClipPoly[left_vtx-1];
   left_vtx -= 1;
  }
  else
  {
   v2 = ClipPoly[end_vtx];                     // Wrap to start of array
   left_vtx = end_vtx;
  }
  // v1 = top vertex
  // v2 = bottom vertex
  left_height = v2->ceil_y - v1->ceil_y;
  if (left_height <= 0) return;
  height = v2->PosR.y - v1->PosR.y;
  invheight = 1.0/height;
  xslope = (v2->PosR.x - v1->PosR.x)*invheight;
  uslope = (v2->u - v1->u)*invheight;
  vslope = (v2->v - v1->v)*invheight;
  left_x_slope = float2fixed(xslope);
  left_u_slope = float2fixed(uslope);
  left_v_slope = float2fixed(vslope);
  //prestep = CEIL(v1->PosR.y) - v1->PosR.y;
  left_x = (1<<15) + float2fixed( v1->PosR.x + (v1->prestep * xslope));
  left_u = (1<<15) + float2fixed( v1->u + (v1->prestep * uslope));
  left_v = (1<<15) + float2fixed( v1->v + (v1->prestep * vslope));
 }

 // -------------------------------------------------------------------------

void FillPolyTex( int NumVert )
 {
  char *ptr;
  char color;
  int min_vtx;
  int vtx;
  int min_y,max_y;
  int n;
  int x1,width,u,v;

  vtx = 0;
  start_vtx = 0;
  min_vtx = 0;
  max_vtx = 0;
  min_y = ClipPoly[0]->ceil_y;
  max_y = ClipPoly[0]->ceil_y;
  vtx++;

  for(n=1; n<NumVert; n++)
  {
   if( ClipPoly[vtx]->ceil_y < min_y)
   {
    min_y = ClipPoly[vtx]->ceil_y;
    min_vtx = vtx;
   }
   else
   if ( ClipPoly[vtx]->ceil_y > max_y)
   {
    max_y = ClipPoly[vtx]->ceil_y;
    max_vtx = vtx;
   }
   vtx++;
  }
  left_vtx  = min_vtx;
  right_vtx = min_vtx;
  end_vtx   = vtx-1;

  do
  {
   if(right_vtx == max_vtx) return;
   CalcRight();
  } while(right_height <= 0);

  do
  {
   if(left_vtx == max_vtx) return;
   CalcLeftTex();
  } while(left_height <= 0);

  destptr = &(_Buffer[min_y*320]);          // buffer

  for(;;)
  {
   x1 = left_x >> 16;
   width = (right_x >> 16) - x1;
   if(width > 0)
   {
    u = left_u;
    v = left_v;
    ptr = destptr+x1;
 // --- inner ---
    while (--width >= 0)
    {
     *ptr++ = _PolyTexture[((v>>8)&0xff00) + ((u>>16)&0xff) ];
     u += inner_u_slope;
     v += inner_v_slope;
/*
 eax u.frac        --- temp ---
 ebx v.frac        (c.int : c.frac)
 ecx u.frac.add    v.int.add : u.int.add
 edx - TEXTURE -       v.int : u.int
 esi counter
 edi --- screen ------------
 ebp v.frac.add    (c.int.add : c.frac.add)

 @L:
  mov al,[edx]
  mov [edi+esi],al
  inc edi
  add eax,ecx
  adc dl,cl
  add ebx,ebp
  adc dh,ch
  inc esi
  jnz @L
*/
    }
 // --- ----- ---
   }
   destptr += 320;                          //
   if(--right_height <= 0)
   {
    do
    {
     if(right_vtx == max_vtx) return;
     CalcRight();
    } while(right_height <= 0);
   }
   else right_x += right_x_slope;

   if(--left_height <= 0)
   {
    do
    {
     if(left_vtx == max_vtx) return;
     CalcLeftTex();
    } while(left_height <= 0);
   }
   else
   {
    left_x += left_x_slope;
    left_u += left_u_slope;
    left_v += left_v_slope;
   }
  }
 }

 // -------------------------------------------------------------------------

 // This is the public one...
void Poly_Tex( sPolygon *P )
 {
  float delta,invdelta,Ceil;
  int dudx,dvdx;
  int i,n;
  int andbits,orbits;
  int max;
  int NumV;

  _PolyTexture = P->Material->Tex1;

  delta =   ((P->v[1]->PosR.x - P->v[0]->PosR.x) * (P->v[2]->PosR.y - P->v[0]->PosR.y))
          - ((P->v[2]->PosR.x - P->v[0]->PosR.x) * (P->v[1]->PosR.y - P->v[0]->PosR.y));
  if (delta == 0.0) return;
  invdelta = 1.0/delta;
  inner_u_slope = float2fixed((  ((P->v[1]->u - P->v[0]->u) * (P->v[2]->PosR.y - P->v[0]->PosR.y))
                               - ((P->v[2]->u - P->v[0]->u) * (P->v[1]->PosR.y - P->v[0]->PosR.y)))
                               * invdelta);
  inner_v_slope = float2fixed((  ((P->v[1]->v - P->v[0]->v) * (P->v[2]->PosR.y - P->v[0]->PosR.y))
                               - ((P->v[2]->v - P->v[0]->v) * (P->v[1]->PosR.y - P->v[0]->PosR.y)))
                               * invdelta);
  // --- Z-clip poly ---

  // --- 2D clip polygon ---

  ClipPoly[0] = P->v[0];
  ClipPoly[1] = P->v[1];
  ClipPoly[2] = P->v[2];
  ClipPoly[3] = P->v[0];

  for (i=0;i<3;i++)
  {
   Ceil = CEIL(P->v[i]->PosR.y );
   P->v[i]->ceil_y  = Ceil;
   P->v[i]->prestep = (float)Ceil - P->v[i]->PosR.y;
  }

  ClipPoly[0]->clip_visible = cliptest(ClipPoly[0]);
  ClipPoly[1]->clip_visible = cliptest(ClipPoly[1]);
  ClipPoly[2]->clip_visible = cliptest(ClipPoly[2]);

  andbits = ClipPoly[0]->clip_visible & ClipPoly[1]->clip_visible & ClipPoly[2]->clip_visible;
  if(andbits != 0) return;
  orbits  = ClipPoly[0]->clip_visible | ClipPoly[1]->clip_visible | ClipPoly[2]->clip_visible;
  if((orbits & (TOP | BOT | LEFT | RIGHT)) == VISIBLE)
  {
   FillPolyTex(3);
  }
  else
  {
   NumV = ClipUV();

   if (NumV < 3) return;
   for (i=0;i<NumV;i++)
   {
    // --- test ---
    // ClipPoly[i] = ClipTemp[i];
    // ---
    Ceil = CEIL(ClipPoly[i]->PosR.y );
    ClipPoly[i]->ceil_y  = Ceil;
    ClipPoly[i]->prestep = (float)Ceil - ClipPoly[i]->PosR.y;
   }
   FillPolyTex(NumV);
  }
 }

