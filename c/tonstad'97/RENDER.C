#include "engine.h"

void Update_Animation( sWorld *W )
 {
  sVector TempVector;
  float TempFloat;
  sCamera *C;
  sObject *O;

  // --- camera ---
  C = W->Camera;

  // --- camera position ---
  if (C->KF_Pos.NumKeys != 0)
  {
   TempFloat = Spline_GetValue( &C->KF_Pos, &C->Pos);
   Spline_Update( &C->KF_Pos );
  }

  // --- camera target ---
  if (C->KF_Target.NumKeys != 0)
  {
   TempFloat = Spline_GetValue( &C->KF_Target, &C->Target);
   Spline_Update( &C->KF_Target );
  }

  // --- camera bank ---
  if (C->KF_Bank.NumKeys != 0)
  {
   C->Bank = Spline_GetValue( &C->KF_Bank, &TempVector);
   Spline_Update( &C->KF_Bank );
  }

  // --- camera fov ---
  if (C->KF_Fov.NumKeys != 0)
  {
   C->FOV = Spline_GetValue( &C->KF_Fov, &TempVector);
   Spline_Update( &C->KF_Fov );
  }

  // --- objects ---
  O = W->Objects;

  while ( O != NULL )
  {
   // --- object rotation ---
   if (O->KF_Rot.NumKeys != 0)
   {
    TempFloat = Spline_GetValue( &O->KF_Rot, &TempVector);
    Spline_Update( &O->KF_Rot );
    O->q[qW] = TempFloat;
    O->q[qX] = TempVector.x;
    O->q[qY] = TempVector.y;
    O->q[qZ] = TempVector.z;
   }

   // --- object position ---
   if (O->KF_Pos.NumKeys != 0)
   {
    TempFloat = Spline_GetValue( &O->KF_Pos, &O->Pos);
    Spline_Update( &O->KF_Pos );
   }
   O = O->Next;
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

void Camera_CreateMatrix_Roll( sCamera *C )
 {
 /*
  sVector view,temp;
  float m1[16];
  float ca,sa;
  float a,b,c,cc,n;

  Matrix_Translation(C->m,-(C->Pos.x),-(C->Pos.y),-(C->Pos.z));

  Vector_Sub(&temp,C->Target,C->Pos);
  Vector_Normalize(&view,&temp);
  a = view.x;
  b = view.y;
  c = view.z;
  cc = c*c;
  if (cc == 1) n = 1; else n = sqrt(1-cc);
  Matrix_Identity(&m1);
  m1[0] =      b/n; m1[4] =     -a/n; m1[ 8] = a;
  m1[1] = -(a*c)/n; m1[5] = -(b*c)/n; m1[ 9] = b;
                    m1[6] =        n; m1[10] = c;
  Matrix_Mult(C->m, &m1);

  ca = (cos(2*PI*C->Roll));
  sa = (sin(2*PI*C->Roll));
  Matrix_Identity(&m1);
  m1[0] =  ca; m1[4] =  sa;
  m1[1] = -sa; m1[5] =  ca;
  Matrix_Mult(C->m, &m1);
 */

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

 // --------------------------------------------------------------------------

void Camera_CreateMatrix_UVN(sCamera *c)
 {
  float TempFloat;
  float mm[16];
  sVector Vtemp,Up,U,V,N,tv;

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

void Object_CreateMatrix_Quat( sObject *O, sWorld *World )
 {
  float m1[16];
  sVector temp1,temp2;
  float TempFloat;

  Matrix_Identity(O->o2w);
  // --- rotation ---
  Quat_Normalize(O->q);
  Matrix_Identity(&m1);
  Quat_ToMatrix( O->q, &m1 );        // ???
  Matrix_Mult(O->o2w,&m1);
  // --- translation ---
  Matrix_Translation(&m1,O->Pos.x,O->Pos.y,O->Pos.z);
  Matrix_Mult(O->o2w,&m1);
  // --- parent ---
  if (O->Parent != NULL) Matrix_Mult(O->o2w,O->Parent->o2w);
  Matrix_Copy(O->m,O->o2w);
  Matrix_Mult(O->m,World->Camera->m);
 }

 // --------------------------------------------------------------------------

void Object_CreateMatrix_Free( sObject *O, sWorld *World )
 {
  Matrix_Rotation(O->o2w,O->Rot.x,O->Rot.y,O->Rot.z);
  O->o2w[ 3] += O->Pos.x;
  O->o2w[ 7] += O->Pos.y;
  O->o2w[11] += O->Pos.z;
  if (O->Parent != NULL) Matrix_Mult(O->o2w,O->Parent->o2w);
  Matrix_Copy(O->m,O->o2w);
  Matrix_Mult(O->m,World->Camera->m);
 }

 // --------------------------------------------------------------------------

void Handle_Object( sObject *O, sWorld *World )
 {
  int i;
  sVector t1,t2,tempvector;
  sPolygon *poly;
  sVertex *v1,*v2,*v3;
  float inv_z;

  if ( O == NULL) return;
  if (O->OBJECT_active==1)
  {
   // if (Object_Visible(O,C) == TRUE)
   {
    World->VisibleObjects++;
    // --- vertices ---
    for (i=0;i<O->NumVertices;i++)
    {
     O->Vertices[i].VERTEX_visible = 0; // blank visible flag
     Vector_Trans(&tempvector, (O->Vertices[i].Pos), &O->m);
     if (tempvector.z <= 1)// vector.z = 1;
     {
      O->Vertices[i].PosR.z = -1;
     }
     else
     {
      O->Vertices[i].PosR.z = tempvector.z;
      inv_z = 1/tempvector.z;
      //O->Vertices[i].PosR.x = ((tempvector.x * World->viewdistance) * inv_z) + 160;
      //O->Vertices[i].PosR.y = ((tempvector.y * World->viewdistance) * inv_z) + 100;
      O->Vertices[i].PosR.x = ((tempvector.x * World->ViewDistanceX) * inv_z) + 160;
      O->Vertices[i].PosR.y = ((tempvector.y * World->ViewDistanceY) * inv_z) + 100;

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
       v1->VERTEX_visible = 1;
       v2->VERTEX_visible = 1;
       v3->VERTEX_visible = 1;
       poly->z = (v1->PosR.z + v2->PosR.z + v3->PosR.z);
       World->RenderList[ World->VisiblePolys ] = poly;
       World->VisiblePolys++;
      }
     }
     poly++;
    }
   }
  }
  Handle_Object( O->Next, World );
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

void Render_Setup( sWorld *World)
 {
  sObject *O;
  sCamera *C;
  float TempFloat;

  C = World->Camera;
  World->VisiblePolys = 0;
  World->VisibleObjects = 0;

  if (C->Type == 0) Camera_CreateMatrix_Free( C );
  else if (C->Type == 1) Camera_CreateMatrix_UVN( C );

  // The screen dimensions
  #define SCREEN_WIDTH 320
  #define SCREEN_HEIGHT 200
  // The aspect ratio of the screen
  // (value of 2.0 makes everything look twice as high)
  #define ASPECT_RATIO 1.2
  // Angle of the camera lense
  // #define FIELD_OF_VISION 45.0
  /// --- The rest is all code and calculations --- ///
  // Conversion from degrees to radians
  #define DEG2RAD(deg) ((deg)*PI/180.0)
  // Calculate focal distance from field of vision
  #define FOV2FOC(width,angle) (((width)*cos((angle)/2.0))/(sin((angle)/2.0)*2))
  // Global variables
  #define FocalDistaceX = FOV2FOC( SCREEN_WIDTH, DEG2RAD(C->FOV))
  #define FocalDistaneY = FOV2FOC( SCREEN_WIDTH, DEG2RAD(C->FOV))

 // World->viewdistance = 160 / ( sin(  ((2400/C->FOV)/360)*PI  )
 //                           /   cos(  ((2400/C->FOV)/360)*PI) );
 //  World->viewdistance = 256;

  World->ViewDistanceX = FOV2FOC( SCREEN_WIDTH, DEG2RAD(C->FOV));
  World->ViewDistanceY = FOV2FOC( SCREEN_WIDTH, DEG2RAD(C->FOV));

  O = World->Objects;
  while (O!=NULL)
  {
   if (World->WORLD_anim == 1)
   {
    Object_CreateMatrix_Quat(O,World);
   }
   else
   {
    Object_CreateMatrix_Free(O,World);
   }
   O = O->Next;
  }
  Handle_Object( World->Objects, World );
  SortWorldObject( World );
 }

 // -------------------------------------------------------------------------

void Render_Draw( sWorld *World)
 {
  int i;

  for (i=World->VisiblePolys-1 ; i>=0 ; i-- )
  {
   // DrawTextureTriangle( World->RenderList[i] );
   // YuckuPolyWireFrame( World->RenderList[i] );
   Poly_Tex( World->RenderList[i] );
  }
 }

