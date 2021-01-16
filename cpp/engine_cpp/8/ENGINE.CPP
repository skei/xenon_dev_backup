 #include <math.h>
 #include "engine.hpp"
 #include "math3d.hpp"

//
// ÄÄÄ Camera ÄÄÄ
//

void cCamera::CreateMatrix_Free(void)
 {
  static matrix mm;

  mm.Translation(-mPos);
  mm.RotationXYZonly(-mRot);
  mM *= mm;
 }

 // ÄÄÄ

void cCamera::CreateMatrix_UVN(void)
 {
  float TempFloat;
  matrix mm;
  vector Vtemp,Up,U,V,N,tv;

  mM.Translation(-mPos);
  Up = mUpVector;
  N = mTarget-mPos;
  N.Normalize();
  U = N^Up;   // Vector_Cross(&U,&N,&Up);
  V = N^U;    // Vector_Cross(&V,&N,&U);
  mm.Identity();
  mm.M[ 0] = U.X;
  mm.M[ 1] = U.Y;
  mm.M[ 2] = U.Z;
  mm.M[ 4] = V.X;
  mm.M[ 5] = V.Y;
  mm.M[ 6] = V.Z;
  mm.M[ 8] = N.X;
  mm.M[ 9] = N.Y;
  mm.M[10] = N.Z;
  mM *= mm;
 }

 // ÄÄÄ

void cCamera::CreateMatrix_3DS(void)
 {
  matrix mm;
  vector c,pivot;
  float   tanfov,focus;
  float   ax, ay, az;
  float   sinx, siny, sinz,cosx, cosy, cosz;

  mM.Translation(-mPos);
  mm.Identity();
  pivot = -mPos;
  c = mTarget-mPos;
  focus = c.Length();
  tanfov = tan ((mFov / 2) / 180 * PI);
  mPerspX = mSizeX / 2 / tanfov;
  mPerspY = mSizeY / 2 / mAspectRatio / tanfov;
  ax = -atan2 (c.X, c.Z);
  ay = asin (c.Y / focus);
  az = -mRoll * PI / 180;
  sinx = sin (ax); cosx = cos (ax); siny = sin (ay);
  cosy = cos (ay); sinz = sin (az); cosz = cos (az);
  mm.M[ 0] =  sinx * siny * sinz + cosx * cosz;
  mm.M[ 1] =  cosy * sinz;
  mm.M[ 2] =  sinx * cosz - cosx * siny * sinz;
  mm.M[ 4] =  sinx * siny * cosz - cosx * sinz;
  mm.M[ 5] =  cosy * cosz;
  mm.M[ 6] = -cosx * siny * cosz - sinx * sinz;
  mm.M[ 8] = -sinx * cosy;
  mm.M[ 9] =  siny;
  mm.M[10] =  cosx * cosy;
  mM *= mm;
 }

//
// ÄÄÄ Object ÄÄÄ
//

void     Object_CreateMatrix_Free( sObject *O, sScene *Scene )
 {
  Matrix_RotationXYZ(O->m_o2w,O->Rot.x,O->Rot.y,O->Rot.z);
  O->m_o2w[ 3] += O->Pos.x;
  O->m_o2w[ 7] += O->Pos.y;
  O->m_o2w[11] += O->Pos.z;
  Matrix_Copy(O->m_o2c,O->m_o2w);
  Matrix_Mul(O->m_o2c,Scene->Camera->m);
 }

 // ÄÄÄ

void     Object_Handle( sObject *O, sScene *Scene )
 {
  int i;
  sVector t1,t2,tempvector;
  sPolygon *poly;
  sVertex *v1,*v2,*v3;
  float inv_z;

  if ( O == NULL) return;
  // if (O->OBJECT_active==1)
  {
   // if (Object_Visible(O,C) == TRUE)
   {
    Scene->VisibleObjects++;
    // --- vertices ---
    for (i=0;i<O->NumVertices;i++)
    {
     Vector_Trans(&tempvector, (O->Vertices[i].Pos), &O->m_o2c);
     if (tempvector.z <= 1)// vector.z = 1;
     {
      O->Vertices[i].PosR.z = -1;
     }
     else
     {
      O->Vertices[i].PosR.z = tempvector.z;
      inv_z = 1/tempvector.z;
      O->Vertices[i].PosR.x = ((tempvector.x * Scene->ViewDistanceX) * inv_z) + 160;
      O->Vertices[i].PosR.y = ((tempvector.y * Scene->ViewDistanceY) * inv_z) + 100;

     }
    }
    // Light_Calc( O, W );
    // --- polygons ---
    poly = O->Polygons;
    for (i=0 ; i<O->NumPolygons ; i++)
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
      if ( (((v2->PosR.x - v1->PosR.x) * (v3->PosR.y - v1->PosR.y))
         - ((v3->PosR.x - v1->PosR.x) * (v2->PosR.y - v1->PosR.y)) >= 0))
      {
       poly->Z = (v1->PosR.z + v2->PosR.z + v3->PosR.z);
       Scene->RenderList[ Scene->VisiblePolys ] = poly;
       Scene->VisiblePolys++;
      }
     }
     poly++;
    }

   }
  }
  Object_Handle( O->Next, Scene );
 }


//
// ÄÄÄ Scene ÄÄÄ
//

void cScene::Render(void)
 {
  // 1. Set up camera matrix
  switch(mCameraType)
  {
   case 0 : mCamera->CreateMatrix_Free(); break;
   case 1 : mCamera->CreateMatrix_UVN();  break;
   case 2 : mCamera->CreateMatrix_3DS();  break;
  }
  // 2. set up camera view frustum planes
  // for all objects:
  // {
  //  calc matrix
  //  inherit matrix from parent's o2w ( = o2w)
  //  multiply w/camera matrix ( = o2c )
  // }
  // for all objects:
  // {
  //  if active, continue
  //  bounding sphere test (A)
  //  for all vertices:
  //  {
  //   transform o2c
  //   project 3D->2D
  //  }
  //  for all polygons:
  //  {
  //   backface cull
  //   check if z-clipping needed (B)
  //   calc z for sorting
  //   insert in global renderlist
  //  }
  // }
  // sort renderlist
  // draw all polys. (C)
  //
 }

 /*

  (A) Check distance from center of bounding sphere (object pos, or pivot?)
      against all view frustum planes...  mark object INSIDE, OUTSIDE or
      NEED_CLIP if OUTSIDE, skip entire object - since it's not visible on
      screen Mark with a bit for each clip plane... So that we know which side
      of the screen and eventual z-clip we need to test for later on...

  (B) If bounding sphere intersects near Z plane, Check Z for all three
      vertices, if all beyond screen plane, skip poly If one or two, mark as
      Z-clipping necessary

  (C) 1. Calc constant gradients
      2. If z-clipping necessary, clip against view plane
      3. if NEED_CLIP - usual 2D clipping
      4. Draw poly, according to shading/mapping flags & material, etc...

 */

