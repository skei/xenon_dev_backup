#include "defines.h"
#include "types.h"
#include "vars.h"
#include "poly.h"
#include "math3d.h"
// 哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪哪

void Camera_CreateMatrix_Free(sCamera *c)
{
 float mm[16];

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

// Returns 1 if the object's bounding-box is visible in ViewFrustum, else 0
int Object_CheckBoundingSphere(sObject *o, sCamera *c)
{
 sVector s1,s2;
 float xcompare,ycompare;
 int r;

 r = 1;

 s1.x = o->Pos.x;
 s1.y = o->Pos.y;
 s1.z = o->Pos.z;

 s2 = Vector_Trans(s1, c->m);

 if (s2.z + o->BSphereRadius < minclip_z) r = 0;
 if (s2.z - o->BSphereRadius > maxclip_z) r = 0;

 xcompare = halfscreenwidth_vd * s2.z;
 if (s2.x + o->BSphereRadius < -xcompare) r = 0;
 if (s2.x - o->BSphereRadius >  xcompare) r = 0;

 ycompare = halfscreenheight_vd * s2.z;
 if (s2.y + o->BSphereRadius < -ycompare) r = 0;
 if (s2.y - o->BSphereRadius >  ycompare) r = 0;

 return r;
}

// --------------------------------------------------------------------------
// ByteSort the "world"-object
// --------------------------------------------------------------------------
void SortWorldObject( void )
{
 int Numbers[256];
 int i;
 short int a,b,d;

 // clear numbers array
 memset( Numbers,0,sizeof (Numbers));
// Count number of each byte-value
 for ( i=0;i<World.VisiblePolys;i++ )
 {
  Numbers[ World.RenderList[i]->z & 255 ]++;
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
 for ( i=0;i<World.VisiblePolys;i++ )
 {
  a = World.RenderList[i]->z & 255; //sorttable[i].sortvalue & 255;
  d = Numbers[a];
  World.Sorted[d] = World.RenderList[i];
  Numbers[a]++;
 }
// -
// Now, do the same, but with high byte...
// -
 // clear numbers array
 memset( Numbers,0,sizeof (Numbers));
// Count number of each byte-value
 for ( i=0;i<World.VisiblePolys;i++ )
 {
  Numbers[ (World.Sorted[i]->z & 0xff00) >> 8 ]++;
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
 for ( i=0;i<World.VisiblePolys;i++ )
 {
  a = (World.Sorted[i]->z & 0xff00) >> 8; //sorttable[i].sortvalue & 255;
  d = Numbers[a];
  World.RenderList[d] = World.Sorted[i];
  Numbers[a]++;
 }
}

// --------------------------------------------------------------------------
// Recursively handle a group and all sub-groups
// --------------------------------------------------------------------------
void HandleGroup( sObject *o,sGroup *g)
{
 int i,n;
 sPolygon *poly;
 sVertex *v1,*v2,*v3;
 sVector vector;

 if (g == NULL) return;
// --------------------
//  switch(g->RenderMethod)
//  {
//   case RENDER_GouraudTexture : CalcLight_Directional(o,g);
//                                break;
//   case RENDER_EnvMap         : CalcLight_EnvMap(o,g);
//                                break;
//  }
// --------------------
 poly = g->Polygons;
 for (i=0 ; i<g->NumPolys ; i++)
 {
  v1 = poly->v1;
  v2 = poly->v2;
  v3 = poly->v3;
  // if any of the Z-coordinates are zero or less, skip the entire poly
  if (    (v1->PosR.z != -1)
       && (v2->PosR.z != -1)
       && (v3->PosR.z != -1))
  {
   // check z-component of face-normal
   if (   (v2->PosR.x - v1->PosR.x) * (v3->PosR.y - v1->PosR.y)
        - (v3->PosR.x - v1->PosR.x) * (v2->PosR.y - v1->PosR.y) <= 0 )
    {
    poly->z = (int)((v1->PosR.z + v2->PosR.z + v3->PosR.z)*Z_MULT);
    World.RenderList[ World.VisiblePolys ] = poly;
    World.VisiblePolys++;
// *****     poly->group = g;
   }
  }
  poly++;
 }
 HandleGroup(o,g->Next);
}

// --------------------------------------------------------------------------
// Recursively handle an object and all child- and branch-objects
// to render "world", start with:
//   HandleObject(World->Object);
// --------------------------------------------------------------------------
void HandleObject( sObject *o)
{
 int i,j;
 sVector vector;

 if ( o == NULL) return;
// if (Object_CheckBoundingSphere( o, World.Camera) == 1)
 {
  if ((o->Flags & OBJECT_Active) != 0)
  {
   Object_CreateMatrix( o, World.Camera );
// --------------------
// rotate all vertices
   for (i=0;i<o->NumVertices;i++)
   {
    vector = Vector_Trans( o->Vertices[i].Pos, &o->m);
    if (vector.z <= 0)
    {
     o->Vertices[i].PosR.z = -1;
    }
    else
    {
     o->Vertices[i].PosR.z = vector.z;
     vector.z += DISTANCE;
     o->Vertices[i].PosR.x = ((vector.x * PROJ_PARAM) / (vector.z)) + (ScreenWidth/2);
     o->Vertices[i].PosR.y = ((vector.y * PROJ_PARAM) / (vector.z)) + (ScreenHeight/2);
    }
   }
   HandleGroup( o,o->Groups );
  }
 }
 HandleObject( o->Child  );
 HandleObject( o->Branch );
}

// --------------------------------------------------------------------------

// Renders the entire scene
void Render(void)
{
 int i;
 int cc;
// sCamera *c;
// float mm[16];

 World.VisiblePolys = 0;
 Camera_CreateMatrix_Free( World.Camera );
 HandleObject( World.Objects );
 SortWorldObject();
 for (i=World.VisiblePolys-1 ; i>=0 ; i-- )
 {
  PolyWireFrame(World.RenderList[i]);
 }
}

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北


