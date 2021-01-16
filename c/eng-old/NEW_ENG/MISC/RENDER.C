#include <math.h>
#include "engine.h"


//
// ±±±±± Render ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void _Camera_CreateMatrix_Aim(sCamera *c)
 {
  float mm[16];
  sVector Vtemp,Up,U,V,N;

  Matrix_Translation(c->m,-(c->Pos.x),-(c->Pos.y),-(c->Pos.z));
  Up = c->UpVector;
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

void _Camera_CreateMatrix_Roll( sCamera *C )
 {
  float m1[16];
  sVector focus;
  float d1,d2;
  float ax,ay,az;

  Vector_Sub(&focus,C->Target,C->Pos);
  d1 = sqrt((focus.x * focus.x) + (focus.y * focus.y) + (focus.z * focus.z));
  d2 = sqrt((focus.x * focus.x) + (focus.z * focus.z));
  ax = (asin(focus.y / d1) * 180 / PI );
  ay = (acos(focus.z / d2) * 180 / PI );
  az = (C->Bank);
  Matrix_Translation(&m1,-(C->Pos.x),-(C->Pos.y),-(C->Pos.z));
  Matrix_Rotation(C->m,-ax,-ay,-az);
  Matrix_Mult(C->m,&m1);
 }

 // -------------------------------------------------------------------------

void _Object_CreateMatrix( sObject *o, sWorld *World )
 {
  float mm[16];
  sVector temp;

  Matrix_Rotation(o->m,o->Rot.x,o->Rot.y,o->Rot.z);
  o->m[ 3] += o->Pos.x;
  o->m[ 7] += o->Pos.y;
  o->m[11] += o->Pos.z;
  if (o->Parent == NULL) Matrix_Mult(o->m,World->Camera->m);
  else Matrix_Mult(o->m,o->Parent->m);
 }

 // --------------------------------------------------------------------------

void _Handle_Object( sObject *O, sWorld *World )
 {
  int i;
  sVector t1,t2;
  sVector tempvector;
  sPolygon *poly;
  sVertex *v1,*v2,*v3;
  float inv_z;

  if ( O == NULL) return;
  if ((O->Flags & OBJECT_active)!=0)
  {
//   if (Object_Visible(O,C) == TRUE)
   {
    World->VisibleObjects++;
    // --- rotate all vertices of object ------------------------------------
    for (i=0;i<O->NumVertices;i++)
    {
     O->Vertices[i].Flags |= -VERTEX_visible; // blank visible flag
     Vector_Trans(&tempvector, (O->Vertices[i].Pos), &O->m);
     if (tempvector.z <= 1)// vector.z = 1;
     {
      O->Vertices[i].PosR.z = -1;
     }
     else
     {
      O->Vertices[i].PosR.z = tempvector.z;
      inv_z = 1/tempvector.z;

      O->Vertices[i].PosR.x = ((tempvector.x * C->viewdistance) * inv_z) + 160;
      O->Vertices[i].PosR.y = ((tempvector.y * C->viewdistance) * inv_z) + 100;

      O->Vertices[i].ix = float2int(O->Vertices[i].PosR.x);
      O->Vertices[i].iy = float2int(O->Vertices[i].PosR.y);
      O->Vertices[i].iu = float2int(O->Vertices[i].u) << 10;
      O->Vertices[i].iv = float2int(O->Vertices[i].v) << 10;
      O->Vertices[i].ic = 63;//float2int(O->Vertices[i].c);

     }
    }

    // ------------------------------
    // EnvMap
    // ------------------------------
    if ((O->Flags & OBJECT_envmap)!=0)
    {
     for (i=0;i<O->NumVertices;i++)
     {
      if ((O->Vertices[i].Flags & VERTEX_visible)!=0)
      {

       Vector_Trans3x3(&tempvector, (O->Vertices[i].Normal), &O->m);

       O->Vertices[i].u = tempvector.x * 127 + 128;
       O->Vertices[i].v = tempvector.y * 127 + 128;

       O->Vertices[i].iu = float2int(tempvector.x * 127 + 128);
       O->Vertices[i].iv = float2int(tempvector.y * 127 + 128);
       O->Vertices[i].ic = 63;
      }
     }
    }

//    Light_Calc( O, W );


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
       v1->Flags |= VERTEX_visible;
       v2->Flags |= VERTEX_visible;
       v3->Flags |= VERTEX_visible;
       poly->z = (v1->PosR.z + v2->PosR.z + v3->PosR.z);
       World->RenderList[ World->VisiblePolys ] = poly;
       World->VisiblePolys++;
       poly->Object = O;
      }
     }
     poly++;
    }
   }
  }
  Handle_Object( O->Next, C/*, W */);
 }


void _Render_Setup( sWorld *World)
 {
  int i;
  float mm[16];
 // int cc;
  sObject *O;
  sCamera *C;

  C = World->Camera;
  World->VisiblePolys = 0;
  World->VisibleObjects = 0;

  Camera_CreateMatrix_Aim( C );
  #define aFov (2400/C->FOV)
  C->viewdistance = 160 / ( sin((aFov/360)*PI) / cos((aFov/360)*PI));

  O = World->Objects;
  while (O!=NULL)
  {
   // Create object matrix
   // Object_CreateMatrix_VUE(O,World->Camera,World->FrameNum);
   Object_CreateMatrix(O,World->Camera);
   O = O->Next;
  }
  _Handle_Object( World->Objects, World->Camera );
  SortWorldObject( World );
 }

 // -------------------------------------------------------------------------

void _Render_Draw( sWorld *World)
 {
  int i;

  for (i=World->VisiblePolys-1 ; i>=0 ; i-- )
  {
  }
 }

 // --------------------------------------------------------------------------

  // Animation (in separete function?)
  // Frustum_planes
  // Camera matrix (FOV)
  // objects:
  //  create object matrix
  //  transform bounding sphere, check if within frustum
  //
  // vertices:
  //  transform vertices
  //  if within frustum/screen, project 3D->2D, mark visible, ceil etc
  // polys:
  //  backface cull, Z-cull
  //  check for necessary clipping (Z, 2D), mark polygons
  //  inset in global render list

  // sort global list
  // draw:
  //   clip
  //   triangularize & draw

