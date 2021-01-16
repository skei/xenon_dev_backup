#include <assert.h>
#include <math.h>

#include "defines.h"
#include "types.h"
#include "vars.h"

#include "math3d.h"
#include "poly.h"

// --------------------------------------------------------------------------

void CalcLight(sObject *o,sWorld *w)
{
 int i,j;
 float c,cc;
 sVector vertexnormal;
 sVector lightvector;
 sVector tempvector;
 int xx,yy;

 for (i=0;i<o->NumVertices;i++)
 {
  Vector_Trans3x3(&vertexnormal,  o->Vertices[i].Normal, &o->m);
  c = 0;
  for (j=0;j<w->NumLights;j++)
  {
   if ((w->Lights[j].Flags & LIGHT_directional)!=0)
   {
    cc = Vector_Dot(vertexnormal,w->Lights[j].Rot);
    if (cc<0) cc = 0;
    c+=cc;
   }
   else
   if ((w->Lights[j].Flags & LIGHT_point)!=0)
   {
    // tempvector = Light-Vertex
    Vector_Sub(&tempvector, w->Lights[j].Pos, o->Vertices[i].PosR3D);
    tempvector.x -= w->Camera->Pos.x;
    tempvector.y -= w->Camera->Pos.y;
    tempvector.z -= w->Camera->Pos.z;
    // normalize tempvector to get Unit Light normal
    Vector_Normalize(&lightvector,&tempvector);
    // dot product metween Light/Vertex
    cc = Vector_Dot(vertexnormal,lightvector);
    if (cc<0) cc = 0;
    c+=cc;
   }

  }
  c = c*63 + 64;
  if (c>127) c = 127;
  if (c<0) c = 0;
  o->Vertices[i].c = PhongIntensity[float2int(c)];
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
 sVector Vtemp,Up,U,V,N;

 Matrix_Translation(c->m,-(c->Pos.x),-(c->Pos.y),-(c->Pos.z));

 Up.x = 0;
 Up.y = -1;
 Up.z = 0;
 Vector_Sub(&Vtemp,c->Target,c->Pos);
 Vector_Normalize(&N,&Vtemp);
// U.x = 0; U.y = 0; U.z = 0;
// V.x = 0; V.y = 0; V.z = 0;
 Vector_Cross(&U,N,Up);
 Vector_Cross(&V,N,U);
 Matrix_Identity(&mm);
 mm[ 0] = U.x;
 mm[ 1] = U.y;
 mm[ 2] = U.z;

 mm[ 4] = V.x;
 mm[ 5] = V.y;
 mm[ 6] = V.z;

 mm[ 8] = N.x;
 mm[ 9] = N.y;
 mm[10] = N.z;

 Matrix_Mult(c->m,&mm);
}

// --------------------------------------------------------------------------

void Object_CreateMatrix(sObject *o, sCamera *c)
{
// Create Object -> Camera matrix
 Matrix_Rotation(o->m,o->Rot.x,o->Rot.y,o->Rot.z);
 o->m[ 3] += o->Pos.x;
 o->m[ 7] += o->Pos.y;
 o->m[11] += o->Pos.z;
 Matrix_Mult(o->m,c->m);
}

// --------------------------------------------------------------------------

void Object_CreateMatrix_Inherit(sObject *o, sCamera *c, sObject *ParentO)
{
// Create Object -> Camera matrix
 Matrix_Rotation(o->m,o->Rot.x,o->Rot.y,o->Rot.z);
 o->m[ 3] += o->Pos.x;
 o->m[ 7] += o->Pos.y;
 o->m[11] += o->Pos.z;
 Matrix_Mult(o->m,ParentO->m);
}

// --------------------------------------------------------------------------

// Returns 1 if the object's bounding-box is visible in ViewFrustum, else 0
int Object_Visible(sObject *o, sCamera *c)
{
 sVector s1,s2,s3;
 float xcompare,ycompare;

 s1.x = o->Pos.x;
 s1.y = o->Pos.y;
 s1.z = o->Pos.z;

 Vector_Trans(&s3, s1, o->m);
 Vector_Trans(&s2, s2, c->m);

 if (s2.z + o->BSphereRadius < min_clip_z) return FALSE;
 if (s2.z - o->BSphereRadius > max_clip_z) return FALSE;

 xcompare = halfscreenwidth_vd * s2.z;
 if (s2.x + o->BSphereRadius < -xcompare) return FALSE;
 if (s2.x - o->BSphereRadius >  xcompare) return FALSE;

 ycompare = halfscreenheight_vd * s2.z;
 if (s2.y + o->BSphereRadius < -ycompare) return FALSE;
 if (s2.y - o->BSphereRadius >  ycompare) return FALSE;

 return TRUE;
}

// --------------------------------------------------------------------------
// ByteSort the "world"-object
// --------------------------------------------------------------------------
void SortWorldObject( sWorld *World )
{
 static int Numbers[256];

 int i;
 short int a,b,d;

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
// Recursively handle an object and all child- and branch-objects
// to render "world", start with:
//   HandleObject(World->Object);
// --------------------------------------------------------------------------

void Handle_Object( sObject *O, sCamera *C, sWorld *W )
{
 int i;
 sVector tempvector;
 sPolygon *poly;
 sVertex *v1,*v2,*v3;
 float inv_z;

 if ( O == NULL) return;
 if ((O->Flags & OBJECT_active)!=0)
 {
  // Create object matrix
//   if (InheritMatrix == TRUE) Object_CreateMatrix_Inherit(O,C,ParentO);
//   else Object_CreateMatrix(O,C);
  if ((O->Flags&OBJECT_inherit)!=0) Object_CreateMatrix_Inherit(O,C,O->Parent);
  else Object_CreateMatrix(O,C);

//  if (Object_Visible(O,C) == TRUE)
  {
   // ------------------------------
   // rotate all vertices of object
   // ------------------------------

   for (i=0;i<O->NumVertices;i++)
   {
    Vector_Trans(&tempvector, (O->Vertices[i].Pos), &O->m);
    if (tempvector.z <= 1)// vector.z = 1;
    {
     O->Vertices[i].PosR.z = -1;
    }
    else
    {
     O->Vertices[i].PosR3D.x = tempvector.x;
     O->Vertices[i].PosR3D.y = tempvector.y;
     O->Vertices[i].PosR3D.z = tempvector.z;
     O->Vertices[i].PosR.z = tempvector.z;
//     tempvector.z += PROJ_VALUE;
     inv_z = 1/tempvector.z;
     O->Vertices[i].PosR.x = ((tempvector.x * PROJ_VALUE2) /*/ (tempvector.z) */ * inv_z) + 160;
     O->Vertices[i].PosR.y = ((tempvector.y * PROJ_VALUE2) /*/ (tempvector.z) */ * inv_z) + 100;
    }
   }

   // ------------------------------
   // EnvMap
   // ------------------------------
   if ((O->Flags & OBJECT_envmap)!=0)
   {
    for (i=0;i<O->NumVertices;i++)
    {
     Vector_Trans3x3(&tempvector, (O->Vertices[i].Normal), &O->m);
     O->Vertices[i].u = tempvector.x * 63 + 63;
     O->Vertices[i].v = tempvector.y * 63 + 63;
    }
   }

   CalcLight(O,W);

   // ------------------------------
   // Go through all objects, check if visible, and add to world.renderlist
   // ------------------------------
   poly = O->Polygons;
   for (i=0 ; i<O->NumPolys ; i++)
   {
    v1 = poly->v[0];
    v2 = poly->v[1];
    v3 = poly->v[2];
    // if any of the Z-coordinates are zero or less, skip the entire poly
    // CHANGE THIS!!!! Clip poly if any of the vertices crosses the
    // camera/view plane!!!
    if (    (v1->PosR.z != -1)
         && (v2->PosR.z != -1)
         && (v3->PosR.z != -1))
    {
     // check z-component of face-normal
     if ( (O->Flags & OBJECT_twosided) ||
          (((v2->PosR.x - v1->PosR.x) * (v3->PosR.y - v1->PosR.y))
        - ((v3->PosR.x - v1->PosR.x) * (v2->PosR.y - v1->PosR.y)) <= 0))
     {
      poly->z = (v1->PosR.z + v2->PosR.z + v3->PosR.z);
      W->RenderList[ W->VisiblePolys ] = poly;
      W->VisiblePolys++;
      poly->Object = O;
     }
    }
    poly++;
   }
  }
 }
 // FIX... Matrix inheritance.... Maybe we have to include current matrix
 // in parameter list, to indicate matrix to inherit
// Handle_Object( O->Child , C, W, O,  TRUE );
// Handle_Object( O->Branch, C, W, O, FALSE );
 Handle_Object( O->Child, C, W );

}

// --------------------------------------------------------------------------

/*
// Debug version... Wireframe only..... :-/
void PolyWireFrame( sPolygon *p )
{
 int x1,y1,x2,y2,x3,y3;

 x1 = p->v[0]->PosR.x;
 y1 = p->v[0]->PosR.y;
 x2 = p->v[1]->PosR.x;
 y2 = p->v[1]->PosR.y;
 x3 = p->v[2]->PosR.x;
 y3 = p->v[2]->PosR.y;

 Line(x1,y1,x2,y2,15,_Buffer);
 Line(x2,y2,x3,y3,15,_Buffer);
 Line(x1,y1,x3,y3,15,_Buffer);
}
*/

// Renders the entire scene
void Render( sWorld *World)
{
 int i;
 int cc;


 World->VisiblePolys = 0;
 switch(World->Camera->Flags)
 {
  case CAMERA_free : Camera_CreateMatrix_Free( World->Camera );
                     break;
  case CAMERA_aim  : Camera_CreateMatrix_Aim( World->Camera );
                     break;
 }

 Handle_Object( World->Objects, World->Camera, World );
 SortWorldObject( World );
// for (i=0 ; i<World.VisiblePolys ; i++ )
 for (i=World->VisiblePolys-1 ; i>=0 ; i-- )
 {
//  PolyWireFrame( World->RenderList[i] );
  GTexPoly(World->RenderList[i]);
//  switch(World->RenderList[i]->group->RenderMethod)
 }
}

