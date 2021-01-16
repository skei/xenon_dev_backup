 #include <stdlib.h>    // malloc
 #include <string.h>    // strcmp
 #include <math.h>      // not needed here!! ?????
 #include <conio.h>     // getch();
 #include "engine.h"
 #include "spline2.h"

// ---

void       Debug(char *s)
 {
  printf("%s\n",s);
  getch();
 }

 // ---

void       Pset(int x, int y, char c, char *buffer)
 {
  if ( (x >= 0) && (x <=319) && (y >= 0) && (y <=199) )
  {
   buffer[y*320+x] = c;
  }
 }

 // ---


sVertex   *Vertex_New(int num)
 {
  sVertex *V;
  int i;

  V = (sVertex *)malloc(sizeof(sVertex)*num);
  //for (i=0;i<num;i++)
  //{
  //}
  return V;
 }

 // ---

sNormal   *Normal_New(int num)
 {
  sNormal *N;
  int i;

  N = (sNormal *)malloc(sizeof(sNormal)*num);
  for (i=0;i<num;i++)
  {
   N[i].u = 0;
   N[i].v = 0;
  }
  return N;
 }

 // ---

sPolygon  *Polygon_New(int num)
 {
  sPolygon *P;
  int i;

  P = (sPolygon *)malloc(sizeof(sPolygon)*num);
  for (i=0;i<num;i++)
  {
   P[i].Vertices[0] = NULL;
   P[i].Vertices[1] = NULL;
   P[i].Vertices[2] = NULL;
   P[i].Normals[0]  = NULL;
   P[i].Normals[1]  = NULL;
   P[i].Normals[2]  = NULL;
   P[i].Material = NULL;
  }
  return P;
 }


 // ---

sMaterial *Material_New(void)
 {
  sMaterial *M;

  M = (sMaterial *)malloc(sizeof(sMaterial));

  M->Flags      = 0;
  M->Name[0]    = 0;
  M->FileNameTexture1[0] = 0;
  M->FileNameTexture2[0] = 0;
  M->FileNameOpacity[0] = 0;
  M->FileNameReflection[0] = 0;
  M->FileNameBump[0] = 0;
  M->Texture1   = NULL;
  M->Texture2   = NULL;
  M->Opacity    = NULL;
  M->Reflection = NULL;
  M->Bump       = NULL;
  memset(&(M->AmbientColor),sizeof(sRGB),0);
  memset(&(M->DiffuseColor),sizeof(sRGB),0);
  memset(&(M->SpecularColor),sizeof(sRGB),0);
  M->Color = 0;

  M->Next       = NULL;
  M->Prev       = NULL;

  return M;
 }

 // ---

sKeyframe *Keyframe_New()
 {
  sKeyframe *K;

  K = (sKeyframe *)malloc(sizeof(sKeyframe));
  K->Frame = 0;
  K->Tens = 1.0;
  K->Bias = 1.0;
  K->Cont = 1.0;
  K->EaseTo = 1.0;
  K->EaseFrom = 1.0;
  K->Next = NULL;
  K->Prev = NULL;
  return K;
 }

void       Track_AddKeyframe( sTrack *T, sKeyframe *K)
 {
  if (T->Keys == NULL)
  {
   K->Prev     = NULL;
   T->Keys     = K;
  }
  else
  {
   K->Prev           = T->LastKey;
   T->LastKey->Next  = K;
  }
  T->Frames = K->Frame;
  T->NumKeys++;
  T->LastKey  = K;
 }

 // ---

sNode     *Node_New(void)
 {
  sNode *N;
  N = (sNode *)malloc(sizeof(sNode));

  N->Type     = 0;
  N->Id       = -1;
  N->ParentId = -1;
  N->Name[0]  = 0;
  N->Next     = NULL;
  N->Prev     = NULL;
  N->Data     = NULL;
  N->Parent   = NULL;
  N->Child    = NULL;
  N->Brother  = NULL;
  return N;
 }

//
// --- Object ---
//

sObject *Object_New(void)
 {
  sObject *O;

  O = (sObject *)malloc(sizeof(sObject));

  O->Flags          = 0;
  O->Type           = 0;

  O->NumVertices   = 0;
  O->Vertices      = NULL;
  O->NumPolygons   = 0;
  O->Polygons      = NULL;
  O->NumNormals    = 0;
  O->Normals       = NULL;

  O->Pos.x = 0;
  O->Pos.y = 0;
  O->Pos.z = 0;

  O->Rot.x = 0;
  O->Rot.y = 0;
  O->Rot.z = 0;

  O->Scale.x = 0;
  O->Scale.y = 0;
  O->Scale.z = 0;

  O->Pivot.x = 0;
  O->Pivot.y = 0;
  O->Pivot.z = 0;

  O->SphereRadius = 0;

  O->Matrix3DS[0] = 0;
  O->Matrix3DS[1] = 0;
  O->Matrix3DS[2] = 0;
  O->Matrix3DS[3] = 0;
  O->Matrix3DS[4] = 0;
  O->Matrix3DS[5] = 0;
  O->Matrix3DS[6] = 0;
  O->Matrix3DS[7] = 0;
  O->Matrix3DS[8] = 0;

  O->Pivot3DS.x = 0;
  O->Pivot3DS.y = 0;
  O->Pivot3DS.z = 0;

  O->kfPos.NumKeys = 0;
  O->kfPos.Frames  = 0;
  O->kfPos.Keys    = NULL;
  O->kfPos.LastKey = NULL;

  O->kfRot.NumKeys = 0;
  O->kfRot.Frames  = 0;
  O->kfRot.Keys    = NULL;
  O->kfRot.LastKey = NULL;

  O->kfScale.NumKeys = 0;
  O->kfScale.Frames  = 0;
  O->kfScale.Keys    = NULL;
  O->kfScale.LastKey = NULL;

  O->kfHide.NumKeys = 0;
  O->kfHide.Frames  = 0;
  O->kfHide.Keys    = NULL;
  O->kfHide.LastKey = NULL;

  O->kfMorph.NumKeys = 0;
  O->kfMorph.Frames  = 0;
  O->kfMorph.Keys    = NULL;
  O->kfMorph.LastKey = NULL;

  return O;
 }

 // ---

void     Object_CalcFaceNormals( sObject *O )
 {
  sVertex *a,*b,*c;
  float ax,ay,az;
  float bx,by,bz;
  float kx,ky,kz;
  float leng;
  int i;

  for (i=0;i<O->NumPolygons;i++)
  {
   a = O->Polygons[i].Vertices[0];
   b = O->Polygons[i].Vertices[1];
   c = O->Polygons[i].Vertices[2];

   ax = b->Pos.x - a->Pos.x;
   ay = b->Pos.y - a->Pos.y;
   az = b->Pos.z - a->Pos.z;

   bx = c->Pos.x - a->Pos.x;
   by = c->Pos.y - a->Pos.y;
   bz = c->Pos.z - a->Pos.z;

   kx = (ay*bz)-(az*by);
   ky = (az*bx)-(ax*bz);
   kz = (ax*by)-(ay*bx);

   leng = (sqrt( kx*kx + ky*ky + kz*kz));
   if (leng!=0)
   {
    O->Normals[i].Pos.x = (kx/leng);
    O->Normals[i].Pos.y = (ky/leng);
    O->Normals[i].Pos.z = (kz/leng);
   }
   else
   {
    O->Normals[i].Pos.x = 0;
    O->Normals[i].Pos.y = 0;
    O->Normals[i].Pos.z = 0;
   }
  }
 }

 // ---

void     Object_CalcVertexNormals( sObject *O )
 {
  int i,a,num;
  sVertex *vi;
  float ax,ay,az;
  float lengd;

  for (i=0;i<O->NumVertices;i++)
  {
   vi = &O->Vertices[i];
   ax = 0;
   ay = 0;
   az = 0;
   num = 0;
   for (a=0;a<O->NumPolygons;a++)
   {
    if (    (O->Polygons[a].Vertices[0] == vi)
         || (O->Polygons[a].Vertices[1] == vi)
         || (O->Polygons[a].Vertices[2] == vi) )
    {
     ax += O->Normals[a].Pos.x;
     ay += O->Normals[a].Pos.y;
     az += O->Normals[a].Pos.z;
     num++;
    }
   }
   if ( num != 0 )
   {
    ax = ax / num;
    ay = ay / num;
    az = az / num;
   }
   lengd = sqrt( ax*ax + ay*ay + az*az);
   if ( lengd != 0)
   {
    O->Normals[i].Pos.x = (ax/lengd);
    O->Normals[i].Pos.y = (ay/lengd);
    O->Normals[i].Pos.z = (az/lengd);
   }
   else
   {
    O->Normals[i].Pos.x = 0;
    O->Normals[i].Pos.y = 0;
    O->Normals[i].Pos.z = 0;
   }
  }
 }

 // ---

void     Object_CalcBoundingSphere( sObject *O )
 {
  int i;
  float sphere,leng;

  sphere = 0;
  for (i=0;i<O->NumVertices;i++)
  {
   leng = Vector_Length(&O->Vertices[i].Pos);
   if ( leng > sphere) sphere = leng;
  }
  O->SphereRadius = sphere;
 }

 // ---

 // Center object around its average X/Y/Z pos
void     Object_Center( sObject *O )
 {
  float xmax,xmin,ymax,ymin,zmax,zmin;
  float xmove, ymove, zmove;
  int i;

  xmax = -32767; xmin = 32767;
  ymax = -32767; ymin = 32767;
  zmax = -32767; zmin = 32767;
  for (i=0;i<O->NumVertices;i++)
  {
   if ( O->Vertices[i].Pos.x > xmax ) xmax = O->Vertices[i].Pos.x;
   if ( O->Vertices[i].Pos.x < xmin ) xmin = O->Vertices[i].Pos.x;
   if ( O->Vertices[i].Pos.y > ymax ) ymax = O->Vertices[i].Pos.y;
   if ( O->Vertices[i].Pos.y < ymin ) ymin = O->Vertices[i].Pos.y;
   if ( O->Vertices[i].Pos.z > zmax ) zmax = O->Vertices[i].Pos.z;
   if ( O->Vertices[i].Pos.z < zmin ) zmin = O->Vertices[i].Pos.z;
  }
 // Sphere = xmax;
 // if (ymax > Sphere) Sphere = ymax;
 // if (zmax > Sphere) Sphere = zmax;
 // if (fabs(xmin) > Sphere) Sphere = fabs(xmin);
 // if (fabs(ymin) > Sphere) Sphere = fabs(ymin);
 // if (fabs(zmin) > Sphere) Sphere = fabs(zmin);

  xmove = xmin+((xmax-xmin) /2 );
  ymove = ymin+((ymax-ymin) /2 );
  zmove = zmin+((zmax-zmin) /2 );
  for (i=0;i<O->NumVertices;i++)
  {
   O->Vertices[i].Pos.x-=xmove;
   O->Vertices[i].Pos.y-=ymove;
   O->Vertices[i].Pos.z-=zmove;
  }
  O->Pos.x += xmove;
  O->Pos.y += ymove;
  O->Pos.z += zmove;
 }

 // ---

 // Creates object matrix from Keyframer splines
void     Object_Update( sObject *O)
 {
  float d[16];

  Quat_Normalize  (&O->kf_Rotate, &O->kf_Rotate);
  // Quat_ToInvMatrix( &O->kf_Rotate, &c);
  Quat_ToMatrix( &O->kf_Rotate, &O->kf_Matrix);
  Matrix_Identity(&d);
  d[ 0] = O->kf_Scale.x;
  d[ 5] = O->kf_Scale.y;
  d[10] = O->kf_Scale.z;
  O->kf_Matrix[ 3] = O->kf_Translate.x;
  O->kf_Matrix[ 7] = O->kf_Translate.y;
  O->kf_Matrix[11] = O->kf_Translate.z;
  Matrix_Mul(&O->kf_Matrix,&d);
 }

 // ---

 // Render object
void     Object_Handle( sObject *O, sScene *Scene )
 {
  int i;
  float m[16];
  float inv_z;
  sCamera *C;
  sVector vec;
  sVertex *v,*v1,*v2,*v3;
  sPolygon *poly;

  C = Scene->ActiveCamera;

  // --- make final transformation matrix:

  Matrix_Identity(&m);
  m[ 3] = -(O->Pivot.x);
  m[ 7] = -(O->Pivot.y);
  m[11] = -(O->Pivot.z);
  Matrix_Mul(&m,&O->kf_Matrix);
  Matrix_Mul(&m,&C->Matrix);

  // --- vertices ---

  for (i=0;i<O->NumVertices;i++)
  {
   v = &(O->Vertices[i]);

   // --- project all vertices

   // Clear render vertex flags
   // v->Visible = 0;
   // v->EnvMapDone = 0;

   Vector_Trans(&vec,&v->Pos,&m);

   if (vec.z <= 1)      // hither z clipping plane
   {
    v->PosRP.z = -1;     // mark as "Crossing Z-plane"
   }
   else
   {
    v->PosRP.z = vec.z;
    inv_z = 1/vec.z;
    v->PosRP.x = 160+(vec.x*C->PerspX)*inv_z;
    v->PosRP.y = 100-(vec.y*C->PerspY)*inv_z;
    //Pset(v->PosRP.x,v->PosRP.y,15,Scene->Buffer);
   }
  }

  // --- polygons ---
  poly = O->Polygons;
  for (i=0 ; i<O->NumPolygons ; i++)
  {
   v1 = poly->Vertices[0];
   v2 = poly->Vertices[1];
   v3 = poly->Vertices[2];

   // if any of the Z-coordinates are zero or less, skip the entire poly
   // CHANGE THIS!!!! Clip poly if any of the vertices crosses the
   // camera/view plane!!!

   if (    (v1->PosRP.z != -1)
        && (v2->PosRP.z != -1)
        && (v3->PosRP.z != -1))
   {

    // check z-component of face-normal
    if (  ((v2->PosRP.x - v1->PosRP.x) * (v3->PosRP.y - v1->PosRP.y))
        - ((v3->PosRP.x - v1->PosRP.x) * (v2->PosRP.y - v1->PosRP.y)) <= 0)
    {
     // mark all vertices as visible (for light etc)
     //v1->Visible = 1;
     //v2->Visible = 1;
     //v3->Visible = 1;

     // --- EnvMap ---

 //     if (poly->Material->ENVMAP == 1)
 //     {
 //
 //      if (v1->EnvMapDone==0)
 //      {
 //       mat_mulvec3 (&m,&v1->Normal,&v1->rNormal);
 //       v1->u = (v1->rNormal.x * EnvMapMultiplier) + (EnvMapMultiplier);
 //       v1->v = (v1->rNormal.y * EnvMapMultiplier) + (EnvMapMultiplier);
 //       v1->EnvMapDone = 1;
 //      }
 //      if (v2->EnvMapDone==0)
 //      {
 //       mat_mulvec3 (&m,&v2->Normal,&v2->rNormal);
 //       v2->u = (v2->rNormal.x * EnvMapMultiplier) + (EnvMapMultiplier);
 //       v2->v = (v2->rNormal.y * EnvMapMultiplier) + (EnvMapMultiplier);
 //       v2->EnvMapDone = 1;
 //      }
 //      if (v3->EnvMapDone==0)
 //      {
 //       mat_mulvec3 (&m,&v3->Normal,&v3->rNormal);
 //       v3->u = (v3->rNormal.x * EnvMapMultiplier) + (EnvMapMultiplier);
 //       v3->v = (v3->rNormal.y * EnvMapMultiplier) + (EnvMapMultiplier);
 //       v3->EnvMapDone = 1;
 //      }
 //     }

     // ---

     // PSET here, for debugging...
     Pset(v1->PosRP.x,v1->PosRP.y,15,Scene->Buffer);
     Pset(v2->PosRP.x,v2->PosRP.y,15,Scene->Buffer);
     Pset(v3->PosRP.x,v3->PosRP.y,15,Scene->Buffer);

     poly->Z = (v1->PosRP.z + v2->PosRP.z + v3->PosRP.z);

     Scene->RenderList[ Scene->VisiblePolys ] = poly;
     Scene->VisiblePolys++;
    }
   }
   poly++;
  }
 }

//
// --- Light ---
//

sLight *Light_New(void)
 {
  sLight *L;

  L = (sLight *)malloc(sizeof(sLight));

  L->Flags         = 0;
  L->Type          = 0;

  L->Strength      = 1;
  L->MaxRange      = 256;
  L->HotSpot       = 0;
  L->FallOff       = 0;
  L->SpotRoll      = 0;

  L->Pos.x = 0;
  L->Pos.y = 0;
  L->Pos.z = 0;

  L->Rot.x = 0;
  L->Rot.y = 0;
  L->Rot.z = 0;

  L->Target.x = 0;
  L->Target.y = 0;
  L->Target.z = 0;

  L->Pivot.x = 0;
  L->Pivot.y = 0;
  L->Pivot.z = 0;

  L->kfPos.NumKeys = 0;
  L->kfPos.Frames  = 0;
  L->kfPos.Keys    = NULL;
  L->kfPos.LastKey = NULL;

  L->kfTarget.NumKeys = 0;
  L->kfTarget.Frames  = 0;
  L->kfTarget.Keys    = NULL;
  L->kfTarget.LastKey = NULL;

  return L;
 }

//
// --- Camera ---
//

sCamera *Camera_New(void)
 {
  sCamera *C;

  C = (sCamera  *)malloc(sizeof(sCamera));

  C->Flags         = 0;
  C->Type          = 0;

  C->AspectRatio = (4/3);

  C->Pos.x = 0;
  C->Pos.y = 0;
  C->Pos.z = 0;

  C->Rot.x = 0;
  C->Rot.y = 0;
  C->Rot.z = 0;

  C->Target.x = 0;
  C->Target.y = 0;
  C->Target.z = 0;

  C->Pivot.x = 0;
  C->Pivot.y = 0;
  C->Pivot.z = 0;

  C->UpVector.x = 0;
  C->UpVector.y = 0;
  C->UpVector.z = 0;

  C->kfPos.NumKeys = 0;
  C->kfPos.Frames  = 0;
  C->kfPos.Keys    = NULL;
  C->kfPos.LastKey = NULL;

  C->kfTarget.NumKeys = 0;
  C->kfTarget.Frames  = 0;
  C->kfTarget.Keys    = NULL;
  C->kfTarget.LastKey = NULL;

  C->kfFov.NumKeys = 0;
  C->kfFov.Frames  = 0;
  C->kfFov.Keys    = NULL;
  C->kfFov.LastKey = NULL;

  C->kfRoll.NumKeys = 0;
  C->kfRoll.Frames  = 0;
  C->kfRoll.Keys    = NULL;
  C->kfRoll.LastKey = NULL;

  return C;
 }

 // ---

 // converts 3dstudio lens to fov.
void     Camera_LensToFov( sCamera *C, float l)
 {
  int  i;

  struct
  {
   float lens, fov;
  } lens_table[] =
    {
     { 15.0, 115.0},
     { 20.0,  94.28571},
     { 24.0,  84.0},
     { 28.0,  76.36364},
     { 35.0,  63.0},
     { 50.0,  46.0},
     { 85.0,  28.0},
     {135.0,  18.0},
     {200.0,  12.0}
    };

  for (i = 0; i < 9; i++) if (l == lens_table[i].lens)
  {
   C->Fov = lens_table[i].fov;
   return;
  }
  C->Fov = 15.0 / l * 160;
 }

 // ---


void     Camera_Update(sCamera *C )
 {
  float   mm[16];
  sVector c;
  float   tanfov,focus,focus2;
  float   ax, ay, az;
  float   sinx, siny, sinz,cosx, cosy, cosz;

  Matrix_Identity(&C->Matrix);

  //Matrix_Translation(&(C->Matrix),-(C->Pos.x),-(C->Pos.y),-(C->Pos.z));
  C->Matrix[ 3] = -C->Pos.x;
  C->Matrix[ 7] = -C->Pos.y;
  C->Matrix[11] = -C->Pos.z;

  // Vector_Negate(&pivot,&(C->Pos));       // pivot is not used here.. ???

  //Vector_Sub(&c,&C->Target,&C->Pos);
  c.x = C->Target.x - C->Pos.x;
  c.y = C->Target.y - C->Pos.y;
  c.z = C->Target.z - C->Pos.z;

  //focus = Vector_Length(&c);
  focus = sqrt((c.x*c.x) + (c.y*c.y) + (c.z*c.z) );

  tanfov = tan ((C->Fov / 2) / 180 * PI);
  C->PerspX = C->SizeX / 2 / tanfov;
  C->PerspY = C->SizeY / 2 / C->AspectRatio / tanfov;
  ax = -atan2 (c.x, c.z);
  ay = asin (c.y / focus);
  az = -C->Roll * PI / 180;
  sinx = sin (ax);
  cosx = cos (ax);
  siny = sin (ay);
  cosy = cos (ay);
  sinz = sin (az);
  cosz = cos (az);
  mm[ 0] =  sinx * siny * sinz + cosx * cosz;
  mm[ 1] =  cosy * sinz;
  mm[ 2] =  sinx * cosz - cosx * siny * sinz;
  mm[ 3] = 0;

  mm[ 4] =  sinx * siny * cosz - cosx * sinz;
  mm[ 5] =  cosy * cosz;
  mm[ 6] = -cosx * siny * cosz - sinx * sinz;
  mm[ 7] = 0;

  mm[ 8] = -sinx * cosy;
  mm[ 9] =  siny;
  mm[10] =  cosx * cosy;
  mm[11] = 0;

  mm[12] = 0;
  mm[13] = 0;
  mm[14] = 0;
  mm[15] = 1;

  Matrix_Mul(&C->Matrix,&mm);
 }

//
// --- Scene ---
//

 // numpolys = number of polygons available in RenderList & Sorted
sScene    *Scene_New(int numpolys)
 {
  sScene *S;

  S = (sScene *)malloc(sizeof(sScene));

  //S->NumObjects = 0;
  //S->Objects      = NULL;
  //S->LastObject   = NULL;

  //S->NumLights = 0;
  //S->Lights       = NULL;
  //S->LastLight    = NULL;

  //S->NumCameras   = 0;
  //S->Cameras      = NULL;
  //S->LastCamera   = NULL;

  S->NumNodes     = 0;
  S->Nodes        = NULL;
  S->LastNode     = NULL;

  S->NumMaterials = 0;
  S->Materials    = NULL;
  S->LastMaterial = NULL;

  S->RenderList   = (sPolygon **)malloc(numpolys*sizeof(sPolygon **));
  S->Sorted       = (sPolygon **)malloc(numpolys*sizeof(sPolygon **));

  S->ActiveCamera = NULL;

  S->AmbientLightColor = 0;

  S->AmbientLightRGB.r = 0;
  S->AmbientLightRGB.g = 0;
  S->AmbientLightRGB.b = 0;

  S->UseFog            = NO;
  S->MasterScale       = 1.0;
  S->Frame             = 0;
  S->StartFrame        = 0;
  S->EndFrame          = 0;

  return S;
 }

 // ---

void       Scene_AddNode(sScene *S, sNode *N)
 {
  if (S->Nodes == NULL)
  {
   N->Prev     = NULL;
   S->Nodes    = N;
   S->LastNode = N;
  }
  else
  {
   N->Prev           = S->LastNode;
   S->LastNode->Next = N;
   S->LastNode       = N;
  }
  S->NumNodes++;
 }

 // ---

sNode     *Scene_NewNode(sScene *S)
 {
  sNode *N;

  N = Node_New();
  if (S->Nodes == NULL)
  {
   N->Prev     = NULL;
   S->Nodes    = N;
   S->LastNode = N;
  }
  else
  {
   N->Prev           = S->LastNode;
   S->LastNode->Next = N;
   S->LastNode       = N;
  }
  S->NumNodes++;
  return N;
 }

 // ---

sNode     *Scene_FindNodeByName(sScene *S, char *name)
 {
  sNode *N;

  N = S->Nodes;
  while (N != NULL)
  {
   if ( strcmp(N->Name,name ) == 0) return N;
   N = N->Next;
  }
  DEBUG(printf("ERROR! Node \"%s\" not found!!!\n",name);)
  return NULL;
 }

 // ---

sNode     *Scene_FindNodeById(sScene *S, int id)
 {
  sNode *N;

  N = S->Nodes;
  while (N != NULL)
  {
   if (id == N->Id) return N;
   N = N->Next;
  }
  DEBUG(printf("ERROR! Node %i not found!!!\n",id);)
  return NULL;
 }

 // ---

void       Scene_AddMaterial(sScene *S, sMaterial *M)
 {
  if (S->Materials == NULL)
  {
   M->Prev         = NULL;
   S->Materials    = M;
   S->LastMaterial = M;
  }
  else
  {
   M->Prev               = S->LastMaterial;
   S->LastMaterial->Next = M;
   S->LastMaterial       = M;
  }
  S->NumMaterials++;
 }

 // ---

sMaterial *Scene_FindMaterialByName(sScene *S, char *name)
 {
  sMaterial *M;

  M = S->Materials;
  while (M != NULL)
  {
   if ( strcmp(M->Name,name ) == 0) return M;
   M = M->Next;
  }
  DEBUG(printf("ERROR! Material %s not found!!!\n",name);)
  return NULL;
 }

 // SKift ut denne... Ha en double-linket liste med polygons.. Og en ptr. til
 // LastInserted... Sjekk ptr og g† riktig vei til riktig plass er funnet.
 // alle poly.Z er positiv, s† man kan sammenligne INT's fra minnet..
 // Vet at SWIV, og de fleste PlayStation spill bruker denne metoden.

 // ByteSort
void       Scene_SortPolys( sScene *Scene )
 {
  static int Numbers[256];

  int i;
  short int a,b,d;

  // clear numbers array
  memset( Numbers,0,sizeof (Numbers));
 // Count number of each byte-value
  for ( i=0;i<Scene->VisiblePolys;i++ )
  {
   Numbers[ Scene->RenderList[i]->Z & 255 ]++;
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
  for ( i=0;i<Scene->VisiblePolys;i++ )
  {
   a = Scene->RenderList[i]->Z & 255; //sorttable[i].sortvalue & 255;
   d = Numbers[a];
   Scene->Sorted[d] = Scene->RenderList[i];
   Numbers[a]++;
  }
 // -
 // Now, do the same, but with high byte...
 // -
  // clear numbers array
  memset( Numbers,0,sizeof (Numbers));
 // Count number of each byte-value
  for ( i=0;i<Scene->VisiblePolys;i++ )
  {
   Numbers[ (Scene->Sorted[i]->Z & 0xff00) >> 8 ]++;
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
  for ( i=0;i<Scene->VisiblePolys;i++ )
  {
   a = (Scene->Sorted[i]->Z & 0xff00) >> 8; //sorttable[i].sortvalue & 255;
   d = Numbers[a];
   Scene->RenderList[d] = Scene->Sorted[i];
   Numbers[a]++;
  }
 }

 // ---

void       Scene_Update(sScene *S)
 {
  sNode    *N,*Node,*Child;
  sCamera  *C;
  sLight   *L;
  sObject  *O, *CO;
  sPolygon *Poly;
  sVector CameraVector,LightVector;
  int i,test;

  S->VisiblePolys = 0;
  S->VisibleObjects = 0;

  // --- Update camera ---


  // --- Update nodes (update splines, create matrices) ---

  N = S->Nodes;
  while (N!=NULL)
  {
   switch(N->Type)
   {
    case NODEtype_object:

      O = (sObject *)N->Data;
      Spline_GetVector ( &(O->kf_Translate), O->kfPos.Keys,   S->Frame);
      Spline_GetVector ( &(O->kf_Scale),     O->kfScale.Keys, S->Frame);
      Spline_GetQuat   ( &(O->kf_Rotate),    O->kfRot.Keys,   S->Frame);
      Object_Update(O);
      break;

    case NODEtype_camera:

      C = (sCamera *)N->Data;
      Spline_GetVector ( &(C->Pos),    C->kfPos.Keys,    S->Frame);
      Spline_GetVector ( &(C->Target), C->kfTarget.Keys, S->Frame);
      Spline_GetFloat  ( &(C->Fov),    C->kfFov.Keys,    S->Frame);
      Spline_GetFloat  ( &(C->Roll),   C->kfRoll.Keys,   S->Frame);
      Camera_Update(C);
      break;

    case NODEtype_light:

     L = (sLight *)N->Data;
     Spline_GetVector ( &(L->Pos),    L->kfPos.Keys,    S->Frame);
     Spline_GetVector ( &(L->Target), L->kfTarget.Keys, S->Frame);
     // Light_Update(L);
     break;
   }
  }

  // --- update hierarchy ---

  for (Node=S->Nodes;Node;Node=Node->Next)
  {
   if (Node->Type == NODEtype_object)
   {
    for (Child = Node->Child;Child;Child=Child->Brother)
    {
     O  = (sObject *)Node->Data;
     CO = (sObject *)Child->Data;
     // mat_mul (obj->matrix, cobj->matrix, cobj->matrix);
     Matrix_Mul(&CO->kf_Matrix,&O->kf_Matrix,&CO->kf_Matrix);
    }
    Object_Handle( (sObject *)Node->Data, S );
   }
   //
  }

  // final transformation

 }

 /*

 1. Setup camera matrix
 2. Setup view frustum planes
 3. Calc Light->World matrix for all lights
 4. For all objects:
 4.1 Calc Object->World matrix
 4.2 Check Bounding Sphere - frustum... (distance point/plane). Use 1 bit in a
     flag for each frustm plane it intersects.
 4.3 If Bounding sphere is NOT totally outside frustum
 4.3.1 Clear all clipping and visible flags in object/vertices/faces/whatever
 4.3.2 Inverse transform Camera to Object Space (inverse Object->Camera matrix)
 4.3.3 For all polys in object:
 4.3.3.1 Check dot-prod plane/camera vector
 4.3.3.2 If visible (poly points towards camera)
 4.3.3.2.1 Project the poly's vertices to view/camera space (if not already
           projected, mark them as we go with a flag or something - clear them
           all before rendering, or..???) Also use the parent and camera
           matrix...
 4.3.3.2.2 if clipping needed (4.2): If all Z's is behind near or far z-plane,
           skip poly. If some, mark poly: Z-clip needed
 4.3.3.2.3 project vertices to 2d.
 4.3.3.2.4 Check if 2d poly is totally outside 2d screen boundaries, and if so,
           skip poly. if some outside, mark which sides needs to be clipped
 4.3.3.2.5 for all lights in scene:
 4.3.3.2.5.1 Inverse transform light to object space
 4.3.3.2.5.2 Calc vertex light intensity... And eventual face light if
             flat-shading
 4.3.3.2.5.4 Eventual Fog/Haze vertex calculations
 4.3.3.2.5.3 Insert poly in RenderList
 5. Clear buffer
 6. For all polys in Renderlist
 6.1 Calc things like constant gradients for texture mapping, etc (or: does it
     work if the poly is not a triangle?)
 6.2 Check Z-plane clipping flag (from 4.3.3.2.2). Clip if necessary.  May
     result in poly with more than 3 vertices
 6.3 Project all vertices in poly to 2d.
 6.4 2d clipping. Check which planes the bounding sphere intersects, and only
     clip against these sides.
 6.5 Triangularize poly (vertex 1-2-3, 1-3-4, 1-4-5, etc), and draw each poly as
     you create them
 7. Wait for retrace
 8. Copy buffer to visible screen

//
// ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

 /*

 // sets up the frustum
 // fovy       - field of view in vertical direction
 // aspect     - height to width aspect ratio (=width / height)
 // zNear/zFar - near and far clipping planes

 // Method 1 : Projection plane at a distance d from the origion, we get:
 //            sx = d*x / z  =  x / (z/d)
 //            sy = d*y / z  =  y / (z/d)
 // 1  0  0  0
 // 0  1  0  0
 // 0  0  1 z/d   =  [x, y, z, z/d]
 // 0  0  0  0
 //
 // Method 2 : Projection plane at z=0, center of projection at z = -d :
 //            sx = d*x / (z+d) = x / ((z+d) + 1)
 //            sy = d*y / (z+d) = y / ((z+d) + 1)
 // 1  0  0  0
 // 0  1  0  0
 // 0  0  0 1/d
 // 0  0  0  1

 // 1  0  0  0
 // 0  a  0  0
 // 0  0  b  c
 // 0  0  f  0
 //
 //    a = the aspect ratio (width / height of screen)
 //    b = f * (zFar / (zFar - zNear))
 //    c = -f * (zFar * zNear / (zFar - zNear))
 //    f = sin(fov / 2) / cos(fov / 2)
 //    fov = field of view
 //    zFar = distance to far clipping plane
 //    zNear = distance to near clipping plane

SCENE::SetPerspective(int fovy, float aspect, float zNear, float zFar)
 {
  // static float mat[4][4], S;
  static float b,c,f;
  f = sinT[fovy >> 1] / cosT[fovy >> 1];
  b = f * (zFar / (zFar - zNear));
  c = -f * (zFar * zNear / (zFar - zNear));
  // S = zNear/(
  mat[0][0] = 1.0; mat[0][1] = 0.0;    mat[0][2] = 0.0; mat[0][3] = 0.0;
  mat[1][0] = 0.0; mat[1][1] = aspect; mat[1][2] = 0.0; mat[1][3] = 0.0;
  mat[2][0] = 0.0; mat[2][1] = 0.0;    mat[2][2] = b;   mat[2][3] = c;
  mat[3][0] = 0.0; mat[3][1] = 0.0;    mat[3][2] = f;   mat[3][3] = 0.0;
  projmat.SetMatrix(mat);
 }

 // ---

SCENE::CalcLighting(void)
 {
  int i,j,k;
  float len, rec_len, dot;
  vec3 LN;
  OmniLight_t *pOmni;
  SpotLight_t *pSpot;
  Vertex_t    *pVert;
  Object_t    *pObj;

  if (numomnis > 0)
  {
   for (i=0; i < numomnis; i++)
   {
    pOmni = OmniLights[i];
    for (j=0; j < numobjects; j++)
    {
     pObj = Objects[j];
     if (pObj->dummy == true) continue;
     // pVert = Objects[j]->pVertices;
     for (k=0; k < Objects[j]->numvert; k++)
     {
      if (i == 0)
      {
       pObj->pVertices[k].L[0] = 0.0;
       pObj->pVertices[k].L[1] = 0.0;
       pObj->pVertices[k].L[2] = 0.0;
      }
      else
      {
       // Calculate inverse normal from vertex to omni light source
       // LN[0] = pOmni->Pos[0] - pObj->pVertices[k].C[0];
       // LN[1] = pOmni->Pos[1] - pObj->pVertices[k].C[1];
       // LN[2] = pOmni->Pos[2] - pObj->pVertices[k].C[2];
       LN[0] = pObj->pVertices[k].C[0] - pOmni->Pos[0];
       LN[1] = pObj->pVertices[k].C[1] - pOmni->Pos[1];
       LN[2] = pObj->pVertices[k].C[2] - pOmni->Pos[2];
       VectorNormalize(LN);
       // take dot product between light normal and vertex normal
       // DotProduct(LN,pObj->pVertices[k].RN,&dot);
       DotProduct(LN,pObj->pVertices[k].N,&dot);
       pObj->pVertices[k].L[0] += pOmni->Color.Red   * dot;
       pObj->pVertices[k].L[1] += pOmni->Color.Green * dot;
       pObj->pVertices[k].L[2] += pOmni->Color.Blue  * dot;
       if (pObj->pVertices[k].L[0] > 1.0) pObj->pVertices[k].L[0] = 1.0;
       if (pObj->pVertices[k].L[1] > 1.0) pObj->pVertices[k].L[1] = 1.0;
       if (pObj->pVertices[k].L[2] > 1.0) pObj->pVertices[k].L[2] = 1.0;
      }
     }
    }
   }
  }
  if (numspotlights > 0)
  {
   for (i=0; i < numspotlights; i++)
   {
    pSpot = SpotLights[i];
    LN[0] = pSpot->Target[0] - pSpot->Pos[0];
    LN[1] = pSpot->Target[1] - pSpot->Pos[1];
    LN[2] = pSpot->Target[2] - pSpot->Pos[2];
    VectorNormalize(LN);
    for (j=0; j < numobjects; j++)
    {
     pObj = Objects[j];
     if (pObj->dummy == true) continue;
     // pVert = Objects[j]->pVertices;
     for (k=0; k < Objects[j]->numvert; k++)
     {
      // Calculate inverse normal from vertex to omni light source
      // LN[0] = pOmni->Pos[0] - pObj->pVertices[k].C[0];
      // LN[1] = pOmni->Pos[1] - pObj->pVertices[k].C[1];
      // LN[2] = pOmni->Pos[2] - pObj->pVertices[k].C[2];
      // take dot product between spotlight normal and vertex normal
      DotProduct(LN,pObj->pVertices[k].N,&dot);
      pObj->pVertices[k].L[0] += pSpot->Color.Red   * dot;
      pObj->pVertices[k].L[1] += pSpot->Color.Green * dot;
      pObj->pVertices[k].L[2] += pSpot->Color.Blue  * dot;
      if (pObj->pVertices[k].L[0] > 1.0) pObj->pVertices[k].L[0] = 1.0;
      if (pObj->pVertices[k].L[1] > 1.0) pObj->pVertices[k].L[1] = 1.0;
      if (pObj->pVertices[k].L[2] > 1.0) pObj->pVertices[k].L[2] = 1.0;
     }
    }
   }
  }
 }


 */
