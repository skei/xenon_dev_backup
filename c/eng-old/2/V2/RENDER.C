#include <stdio.h>    // sprintf
#include <string.h>   // memset
#include <conio.h>    // getch

#include "defines.h"
#include "types.h"
#include "vars.h"

#include "math3d.h"
#include "gfx.h"
#include "poly2.h"

#include "light.h"
// #include "texpoly.h"

char tekst[256];

// --------------------------------------------------------------------------

// --------------------------------------------------------------------------

// Returns 1 if the object's bounding-box is visible in ViewFrustum, else 0
int Object_CheckBoundingSphere(sObject *O, sCamera *C)
{
 sVector s1,s2;
 float xcompare,ycompare;

 s1.x = O->Pos.x;
 s1.y = O->Pos.y;
 s1.z = O->Pos.z;

 s2 = Vector_Trans(&s1, C->m);

 if (s2.z + O->BSphereRadius < minclip_z) return 0;
 if (s2.z - O->BSphereRadius > maxclip_z) return 0;

 xcompare = halfscreenwidth_vd * s2.z;
 if (s2.x + O->BSphereRadius < -xcompare) return 0;
 if (s2.x - O->BSphereRadius >  xcompare) return 0;
 ycompare = halfscreenheight_vd * s2.z;
 if (s2.y + O->BSphereRadius < -ycompare) return 0;
 if (s2.y - O->BSphereRadius >  ycompare) return 0;

 return 1;
}

// --------------------------------------------------------------------------
// ByteSort the "world"-object
// --------------------------------------------------------------------------
void SortWorldObject( void )
{
 int i;
 short int a,b,d;
 int Numbers[256];

 // clear numbers array
 memset( Numbers,0,sizeof (Numbers));
// Count number of each byte-value
 for ( i=0;i<World->VisiblePolys;i++ )
 {
  Numbers[ World->RenderList[i]->z & 255 ]++;
 }
// Convert from numbers to offset in list to sort
 a = 0;
 for ( i=0;i<256;i++ )
 {
  b = Numbers[i];
  Numbers[i] = a;
  a+=b;
 }
// Sort according to LOW byte
 for ( i=0;i<World->VisiblePolys;i++ )
 {
  a = World->RenderList[i]->z & 255; //sorttable[i].sortvalue & 255;
  d = Numbers[a];
  World->Sorted[d] = World->RenderList[i];
  Numbers[a]++;
 }
// -
// Now, do the same, but with high byte...
// -
 // clear numbers array
 memset( Numbers,0,sizeof (Numbers));
// Count number of each byte-value
 for ( i=0;i<World->VisiblePolys;i++ )
 {
  Numbers[ (World->Sorted[i]->z & 0xff00) >> 8 ]++;
 }
// Convert from numbers to offset in list to sort
 a = 0;
 for ( i=0;i<256;i++ )
 {
  b = Numbers[i];
  Numbers[i] = a;
  a+=b;
 }
// Sort according to HIGH byte
 for ( i=0;i<World->VisiblePolys;i++ )
 {
  a = (World->Sorted[i]->z & 0xff00) >> 8; //sorttable[i].sortvalue & 255;
  d = Numbers[a];
  World->RenderList[d] = World->Sorted[i];
  Numbers[a]++;
 }
}

// --------------------------------------------------------------------------

void Camera_CreateMatrix_Free(sCamera *c)
{
 float mm[16];
 Matrix_Translation(c->m,-(c->Pos.x),-(c->Pos.y),-(c->Pos.z));
 Matrix_Rotation(&mm,-(c->Rot.x),-(c->Rot.y),-(c->Rot.z));
 Matrix_Mult(c->m,&mm);
}

// --------------------------------------------------------------------------

void Camera_CreateMatrix_Aim(sCamera *c)
{
 float mm[16];
 float x,y,z;
 float focus;

 x = c->Target.x - c->Pos.x;
 y = c->Target.y - c->Pos.y;
 z = c->Target.z - c->Pos.z;
 focus = sqrt(x*x + y*y + z*z);

 c->Rot.x = -atan(x/z);
 c->Rot.y = asin(y/focus);
 c->Rot.z = c->Roll;

 Matrix_Translation(c->m,-(c->Pos.x),-(c->Pos.y),-(c->Pos.z));
 Matrix_Rotation(&mm,-(c->Rot.x),-(c->Rot.y),-(c->Rot.z));
 Matrix_Mult(c->m,&mm);
}


// --------------------------------------------------------------------------

void Object_CreateMatrix(sObject *o, sCamera *c)
{
 Matrix_Rotation(o->m,o->Rot.x,o->Rot.y,o->Rot.z);
 o->m[ 3] += o->Pos.x;
 o->m[ 7] += o->Pos.y;
 o->m[11] += o->Pos.z;
 Matrix_Mult(o->m,c->m);
}

// --------------------------------------------------------------------------

void HandleGroup( sObject *O,sGroup *G)
{
 int i;
 sPolygon *poly;
 sVertex *v[3];

 while (G!=NULL)
 {
/*
  switch(g->RenderMethod)
  {
   case RENDER_GouraudTexture : CalcLight_Directional(o,g);
                                break;
   case RENDER_EnvMap         : CalcLight_EnvMap(o,g);
                                break;
  }
*/
  poly = G->Polygons;
  for (i=0 ; i<G->NumPolys ; i++)
  {
   v[0] = poly->v[0];
   v[1] = poly->v[1];
   v[2] = poly->v[2];
   // if any of the Z-coordinates are zero or less, skip the entire poly
   if (    (v[0]->PosR.z != -1)
        && (v[1]->PosR.z != -1)
        && (v[2]->PosR.z != -1))
   {
    // check z-component of face-normal
    if ( ((v[1]->PosR.x - v[0]->PosR.x) * (v[2]->PosR.y - v[0]->PosR.y))
       - ((v[2]->PosR.x - v[0]->PosR.x) * (v[1]->PosR.y - v[0]->PosR.y)) >= 0 )
    {
     poly->z = (v[0]->PosR.z + v[1]->PosR.z + v[2]->PosR.z)/* *Z_MULT */;
     World->RenderList[ World->VisiblePolys ] = poly;
     World->VisiblePolys++;
     poly->Group = G;
    }
   }
   poly++;
  }
  G = G->Next;
 }
}

// --------------------------------------------------------------------------
// Recursively handle an object and all child- and branch-objects
// to render "world", start with:
//   HandleObject(World->Object);
// --------------------------------------------------------------------------
void HandleObject( sObject *O)
{
 int i,j;
 sVector vector;

 if ( O == NULL) return;
 if (Object_CheckBoundingSphere( O, World->Camera) == 1)
 {
  if (O->Flags & OBJECT_Active == OBJECT_Active)
  {
   // around here.... Object matrix inheritance... Inherit from parent???
   Object_CreateMatrix( O, World->Camera );
  // ------------------------------
  // rotate all vertices of object
  // ------------------------------
   for (i=0;i<O->NumVertices;i++)
   {
    vector = Vector_Trans( &(O->Vertices[i].Pos), &O->m);

    if (vector.z <= 0)// vector.z = 1;
     {
     O->Vertices[i].PosR.z = -1;
    }
    else
    {
     O->Vertices[i].PosR.z = vector.z;
     vector.z += PROJ_VALUE;
     O->Vertices[i].PosR.x = ((vector.x * PROJ_VALUE2)  / (vector.z)) + 160;
     O->Vertices[i].PosR.y = ((vector.y * PROJ_VALUE2)  / (vector.z)) + 100;
    }
   }
//   CalcLight_Camera(O);
   // ------------------------------
   // EnvMap
   // ------------------------------
   if ((O->Flags & 0x10000)==0x10000)
   {
    for (i=0;i<O->NumVertices;i++)
    {
     vector = Vector_Trans3x3( &(O->Vertices[i].Normal), &O->m);
     O->Vertices[i].u = vector.x * 63 + 63;
     O->Vertices[i].v = vector.y * 63 + 63;
    }
   }
// --------------------
  HandleGroup( O,O->Groups );
  }
 }
 HandleObject( O->Child  );
 HandleObject( O->Branch );
}

// --------------------------------------------------------------------------

void Render( void )
{
 float x,y,z;
 int i;
 sGroup *g;

 World->VisiblePolys = 0;
 switch (World->Camera->Flags)
 {
  case 0 : Camera_CreateMatrix_Free( World->Camera );
           break;
  case 1 : Camera_CreateMatrix_Aim( World->Camera );
           break;
 }
 HandleObject( World->Objects );
 SortWorldObject();
 for (i=World->VisiblePolys-1 ; i>=0 ; i-- )
 {
  TexPoly( World->RenderList[i] );
 }
// sprintf(tekst,"Visible polys %i",World->VisiblePolys);
// drawstring(0,0,tekst,14);
}
