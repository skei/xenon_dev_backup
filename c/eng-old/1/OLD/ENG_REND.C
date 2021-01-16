#include <assert.h>
#include <math.h>

#include "eng_def.h"
#include "eng_type.h"
#include "eng_vars.h"

#include "eng_math.h"
#include "eng_poly.h"

// --------------------------------------------------------------------------

void Handle_Light(sLight *l, sCamera *c)
{
 switch(l->Flags)
 {
  case LIGHT_point : // ----- Point light source light -----
    Matrix_Identity(l->m);
    l->m[ 3] += l->Pos.x;
    l->m[ 7] += l->Pos.y;
    l->m[11] += l->Pos.z;
    Matrix_Mult(l->m,c->m);
    Vector_Trans( &l->PosR, l->Pos, l->m );
    break;
  case LIGHT_point_dist : // ----- Point light source light w/distance factor -----
    Matrix_Identity(l->m);
    l->m[ 3] += l->Pos.x;
    l->m[ 7] += l->Pos.y;
    l->m[11] += l->Pos.z;
    Matrix_Mult(l->m,c->m);
    Vector_Trans( &l->PosR, l->Pos, l->m );
    break;
  case LIGHT_directional : // ----- Directional light source -----
    Vector_inv_Trans3x3( &l->DirectionR, l->Direction, c->m );
    break;
 }
}

// --------------------------------------------------------------------------

void CalcLight(sObject *o,sWorld *w)
{
 int i,j;
 int flag;
 float c,cc;
 float dist;
 sVector vertexnormal;
 sVector lightvector;
 sVector tempvector;
 sLight *L;
 int xx,yy;


 for (i=0;i<o->NumVertices;i++)
 {
  Vector_Trans3x3(&vertexnormal,  o->Vertices[i].Normal, &o->m);

  c = 0;
  for (j=0;j<w->NumLights;j++)
  {
   L = &w->Lights[j];
   flag = L->Flags;
   cc = 0;

   switch(flag)
   {
    case LIGHT_directional : // ----- Directional light -----
      cc = Vector_Dot(vertexnormal,L->DirectionR) * L->Strength;
      if (cc<0) cc = 0;
      c+=cc;
      break;

    case LIGHT_point : // ----- point light source -----
      // tempvector = Light-Vertex
      Vector_Sub(&tempvector, L->PosR, o->Vertices[i].PosR3D);
      // normalize tempvector to get Unit Light normal
      Vector_Normalize(&lightvector,&tempvector);
      // dot product metween Light/Vertex
      cc = Vector_Dot(vertexnormal,lightvector) * L->Strength;
      if (cc<0) cc = 0;
      c+=cc;
      break;

    case LIGHT_point_dist : // ----- point light source w/distance factor -----
      // tempvector = Light-Vertex
      Vector_Sub(&tempvector, L->PosR, o->Vertices[i].PosR3D);
      // normalize tempvector to get Unit Light normal
      Vector_Normalize(&lightvector,&tempvector);
      // dot product metween Light/Vertex
      cc = Vector_Dot(vertexnormal,lightvector) * L->Strength;
      if (cc<0) cc = 0;
      dist = (Vector_Distance(&L->PosR,&o->Vertices[i].PosR3D));
      dist = (L->MaxRange-dist);
      if (dist > 0)
      {
       cc *= (dist/L->MaxRange);
      }
      c+=cc;
      break;
   }
  }
  c = c * 127;
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

 Up = c->UpVector;
// Up.x = c->Upvector.x
// Up.x = 0;
// Up.y = -1;
// Up.z = 0;
 Vector_Sub(&Vtemp,c->Target,c->Pos);
 Vector_Normalize(&N,&Vtemp);
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
 sVector s1;
 float Radius;
 float xcompare,ycompare;

 Vector_Trans(&s1, o->Pos, &o->m);
 Radius = (o->BSphereRadius * viewdistance)/s1.z;
 Radius = o->BSphereRadius;

 if (s1.z + Radius < min_clip_z) return FALSE;
 if (s1.z - Radius > max_clip_z) return FALSE;

 if (s1.x + Radius < -s1.z) return FALSE;
 if (s1.x - Radius >  s1.z) return FALSE;

 if (s1.y + Radius < -s1.z) return FALSE;
 if (s1.y - Radius >  s1.z) return FALSE;

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
  if ((O->Flags&OBJECT_inherit)!=0) Object_CreateMatrix_Inherit(O,C,O->Parent);
  else Object_CreateMatrix(O,C);

  if (Object_Visible(O,C) == TRUE)
  {
   W->VisibleObjects++;
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

//     O->Vertices[i].PosR.x = ((tempvector.x * PROJ_VALUE2) /*/ (tempvector.z) */ * inv_z) + 160;
//     O->Vertices[i].PosR.y = ((tempvector.y * PROJ_VALUE2) /*/ (tempvector.z) */ * inv_z) + 100;

//     O->Vertices[i].PosR.x = ((tempvector.x * viewdistance) /*/ (tempvector.z) */ * inv_z) + 160;
//     O->Vertices[i].PosR.y = ((tempvector.y * viewdistance) /*/ (tempvector.z) */ * inv_z) + 100;

     O->Vertices[i].PosR.x = ((tempvector.x * viewdistance) * inv_z) + 160;
     O->Vertices[i].PosR.y = ((tempvector.y * viewdistance) * inv_z) + 100;


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
 World->VisibleObjects = 0;
 switch(World->Camera->Flags)
 {
  case CAMERA_free : Camera_CreateMatrix_Free( World->Camera );
                     break;
  case CAMERA_aim  : Camera_CreateMatrix_Aim( World->Camera );
                     break;
 }

 for (i=0;i<World->NumLights;i++)
 {
  Handle_Light( &World->Lights[i], World->Camera );
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
 memset(_Buffer+(320*198),1,World->VisibleObjects);
}

