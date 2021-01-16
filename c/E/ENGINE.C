 #include <math.h>
 #include "engine.h"

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

//
// ÄÄÄ variables ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

char *_Buffer;    // Background Buffer to draw to
char *_Screen;    // _the_ screen
int   MinX,MaxX,MinY,MaxY;      // Clipping rectangle
int   ScreenWidth,ScreenHeight;  // ViewPort size
int   TopClip;
int   BotClip;
int   LeftClip;
int   RightClip;

//
// ÄÄÄ misc ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

int *Int_New( int num )
 {
  int *I;

  I = (int *)malloc(num*sizeof(int));
  memset(I,0,num*sizeof(int));
  return I;
 }

//
// ÄÄÄ key ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

sKey *Key_New( int num )
 {
  sKey *K;

  K = (sKey *)malloc(num*sizeof(sKey));
  memset(K,0,num*sizeof(sKey));
  return K;
 }

//
// ÄÄÄ normal ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

sNormal *Normal_New( int num )
 {
  sNormal *N;

  N = (sNormal *)malloc(num*sizeof(sNormal));
  memset(N,0,num*sizeof(sNormal));
  return N;
 }


//
// ÄÄÄ vertex ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

sVertex *Vertex_New( int num )
 {
  sVertex *V;

  V = (sVertex *)malloc(num*sizeof(sVertex));
  memset(V,0,num*sizeof(sVertex));
  return V;
 }

//
// ÄÄÄ polygon ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

sPolygon *Polygon_New( int num )
 {
  sPolygon *P;

  P = (sPolygon *)malloc(num*sizeof(sPolygon));
  memset(P,0,num*sizeof(sPolygon));
  return P;
 }

//
// ÄÄÄ material ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

sMaterial *Material_New(void)
 {
  sMaterial *M;

  M = (sMaterial *)malloc(sizeof(sMaterial));
  memset(M,0,sizeof(sMaterial));
  return M;
 }

 // ÄÄÄ

sMaterial *Material_FindByName( sScene *Scene, char *name )
 {
  sMaterial *sM;
  int found;

  sM = Scene->Materials;
  for (;;)
  {
   if (sM==NULL)
   {
    return NULL;
    //printf("harra-baillj! Didn't find material \"%s\" in MaterialList\n",name);
    //exit(1);
   }
   if (strncmp ( sM->Name , name , 20 ) == 0) goto exitloop;
   sM=sM->Next;
  }
 exitloop:
  return sM;
 }

//
// ÄÄÄ light ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

sLight *Light_New(void)
 {
  sLight *L;

  L = (sLight *)malloc(sizeof(sLight));
  memset(L,0,sizeof(sLight));
  return L;
 }

 // ÄÄÄ

sLight *Light_FindByName( sScene *Scene, char *name )
 {
  sLight *sL;
  int found;

  sL = Scene->Lights;
  for (;;)
  {
   if (sL==NULL)
   {
    return NULL;
    //printf("harra-baillj! Didn't find light \"%s\" in LightList\n",name);
    //exit(1);
   }
   if (strncmp ( sL->Name , name , 20 ) == 0) goto exitloop;
   sL=sL->Next;
  }
 exitloop:
  return sL;
 }

//
// ÄÄÄ Object ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

sObject *Object_New(void)
 {
  sObject *O;

  O = (sObject *)malloc(sizeof(sObject));
  memset(O,0,sizeof(sObject));
  return O;
 }

 // ÄÄÄ

void     Object_SetPos( sObject *o, float x, float y, float z)
 {
  o->Pos.x = x;
  o->Pos.y = y;
  o->Pos.z = z;
 }

 // ÄÄÄ

void     Object_AddPos( sObject *o, float x, float y, float z)
 {
  o->Pos.x += x;
  o->Pos.y += y;
  o->Pos.z += z;
 }

 // ÄÄÄ

void     Object_SetRot( sObject *o, float x, float y, float z)
 {
  o->Rot.x = x;
  o->Rot.y = y;
  o->Rot.z = z;
 }

 // ÄÄÄ

void     Object_AddRot( sObject *o, float x, float y, float z)
 {
  o->Rot.x += x;
  o->Rot.y += y;
  o->Rot.z += z;
 }

 // ÄÄÄ

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
   a = O->Polygons[i].v[0];
   b = O->Polygons[i].v[1];
   c = O->Polygons[i].v[2];

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
    O->Polygons[i].Normal->Pos.x = (kx/leng);
    O->Polygons[i].Normal->Pos.y = (ky/leng);
    O->Polygons[i].Normal->Pos.z = (kz/leng);
   }
   else
   {
    O->Polygons[i].Normal->Pos.x = 0;
    O->Polygons[i].Normal->Pos.y = 0;
    O->Polygons[i].Normal->Pos.z = 0;
   }
  }
 }

 // ÄÄÄ

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
    if (    (O->Polygons[a].v[0] == vi)
         || (O->Polygons[a].v[1] == vi)
         || (O->Polygons[a].v[2] == vi) )
    {
     ax += O->Polygons[a].Normal->Pos.x;
     ay += O->Polygons[a].Normal->Pos.y;
     az += O->Polygons[a].Normal->Pos.z;
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
    O->Vertices[i].Normal->Pos.x = (ax/lengd);
    O->Vertices[i].Normal->Pos.y = (ay/lengd);
    O->Vertices[i].Normal->Pos.z = (az/lengd);
   }
   else
   {
    O->Vertices[i].Normal->Pos.x = 0;
    O->Vertices[i].Normal->Pos.y = 0;
    O->Vertices[i].Normal->Pos.z = 0;
   }
  }
 }

 // ÄÄÄ

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

 // ÄÄÄ

void     Object_CheckBoundingSphere(sObject *O, sCamera *C)
 {
 }
 // ÄÄÄ

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

 // ÄÄÄ

sObject *Object_FindByName( sScene *Scene, char *name )
 {
  sObject *sO;
  int found;

  sO = Scene->Objects;
  for (;;)
  {
   if (sO==NULL)
   {
    return NULL;
    //printf("harra-baillj! Didn't find object \"%s\" in ObjectList\n",name);
    //exit(1);
   }
   if (strncmp ( sO->Name , name , 20 ) == 0) goto exitloop;
   sO=sO->Next;
  }
 exitloop:
  return sO;
 }

 // ÄÄÄ

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
// ÄÄÄ Camera ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

sCamera *Camera_New(void)
 {
  sCamera *C;

  C = (sCamera *)malloc(sizeof(sCamera));
  memset(C,0,sizeof(sCamera));
  return C;
 }

 // ÄÄÄ

sCamera *Camera_FindByName( sScene *Scene, char *name )
 {
  sCamera *sC;
  int found;

  sC = Scene->Cameras;
  for (;;)
  {
   if (sC==NULL)
   {
    return NULL;
    //printf("harra-baillj! Didn't find camera \"%s\" in CameraList\n",name);
    //exit(1);
   }
   if (strncmp ( sC->Name , name , 20 ) == 0) goto exitloop;
   sC=sC->Next;
  }
 exitloop:
  return sC;
 }

 // ÄÄÄ

void     Camera_SetPos( sCamera *c, float x, float y, float z)
 {
  c->Pos.x = x;
  c->Pos.y = y;
  c->Pos.z = z;
 }

 // ÄÄÄ

void     Camera_SetRot( sCamera *c, float x, float y, float z)
 {
  c->Rot.x = x;
  c->Rot.y = y;
  c->Rot.z = z;
 }

 // ÄÄÄ

void     Camera_SetTarget( sCamera *c, float x, float y, float z)
 {
  c->Target.x = x;
  c->Target.y = y;
  c->Target.z = z;
 }

 // ÄÄÄ

void     Camera_AddTarget( sCamera *c, float x, float y, float z)
 {
  c->Target.x += x;
  c->Target.y += y;
  c->Target.z += z;
 }

 // ÄÄÄ

void     Camera_AddRot( sCamera *c, float x, float y, float z)
 {
  c->Rot.x += x;
  c->Rot.y += y;
  c->Rot.z += z;
 }

 // ÄÄÄ

void     Camera_AddPos( sCamera *c, float x, float y, float z)
 {
  c->Pos.x += x;
  c->Pos.y += y;
  c->Pos.z += z;
 }

 // ÄÄÄ

void     Camera_CreateMatrix_Free(sCamera *c)
 {
  float mm[16];
  Matrix_Translation(c->m,-(c->Pos.x),-(c->Pos.y),-(c->Pos.z));
  Matrix_RotationXYZ(&mm,-(c->Rot.x),-(c->Rot.y),-(c->Rot.z));
  Matrix_Mul(c->m,&mm);
 }

 // ÄÄÄ

void     Camera_CreateMatrix_UVN(sCamera *c)
 {
  float TempFloat;
  float mm[16];
  sVector Vtemp,Up,U,V,N,tv;

  Matrix_Translation(c->m,-(c->Pos.x),-(c->Pos.y),-(c->Pos.z));
  Up = c->UpVector;
  Vector_Sub(&Vtemp,&(c->Target),&(c->Pos));
  Vector_Normalize(&N,&Vtemp);
  Vector_Cross(&U,&N,&Up);
  Vector_Cross(&V,&N,&U);
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
  Matrix_Mul(c->m,&mm);
 }

 // ÄÄÄ

void     Camera_CreateMatrix_3DS(sCamera *C)
 {
  float   mm[16];
  sVector c,pivot;
  float   tanfov,focus;
  float   ax, ay, az;
  float   sinx, siny, sinz,cosx, cosy, cosz;

  Matrix_Translation(C->m,-(C->Pos.x),-(C->Pos.y),-(C->Pos.z));
  Matrix_Identity(&mm);
  Vector_Negate(&pivot,&(C->Pos));
  Vector_Sub(&c,&(C->Target),&(C->Pos));
  focus = Vector_Length(&c);
  tanfov = tan ((C->Fov / 2) / 180 * PI);
  C->PerspX = C->SizeX / 2 / tanfov;
  C->PerspY = C->SizeY / 2 / C->AspectRatio / tanfov;
  ax = -atan2 (c.x, c.z);
  ay = asin (c.y / focus);
  az = -C->Roll * PI / 180;
  sinx = sin (ax); cosx = cos (ax); siny = sin (ay);
  cosy = cos (ay); sinz = sin (az); cosz = cos (az);
  mm[ 0] =  sinx * siny * sinz + cosx * cosz;
  mm[ 1] =  cosy * sinz;
  mm[ 2] =  sinx * cosz - cosx * siny * sinz;
  mm[ 4] =  sinx * siny * cosz - cosx * sinz;
  mm[ 5] =  cosy * cosz;
  mm[ 6] = -cosx * siny * cosz - sinx * sinz;
  mm[ 8] = -sinx * cosy;
  mm[ 9] =  siny;
  mm[10] =  cosx * cosy;
  Matrix_Mul(C->m,&mm);
 }

void     Camera_SetupFrustum(sCamera *C)
 {
 }


//
// ÄÄÄ yucku ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

void Yucku_Line(int x0, int y0, int x1, int y1, char color, char *_B)
 {
  int dx,ix,cx,dy,iy,cy,m,i,pos;

  if (x0<x1) { dx=x1-x0; ix=1; } else { dx=x0-x1; ix=-1; }
  if (y0<y1) { dy=y1-y0; iy=1; } else { dy=y0-y1; iy=-1; }
  if (dx>dy) m=dx; else m=dy;
  cx=cy=(m>>1);
  for (i=0;i<m;i++)
  {
   if ((x0>=MinX)&&(x0<=MaxX)&&(y0>=MinY)&&(y0<=MaxY))
   {
    _B[y0*ScreenWidth+x0] = color;
   }
   if ((cx+=dx)>=m) { cx-=m; x0+=ix; }
   if ((cy+=dy)>=m) { cy-=m; y0+=iy; }
  }
 }

 // ÄÄÄ

void Yucku_PolyWireFrame( sPolygon *p )
 {
  sVertex *v[3];

  v[0] = p->v[0];
  v[1] = p->v[1];
  v[2] = p->v[2];

  Yucku_Line( (int)(v[0]->PosR.x),
             (int)(v[0]->PosR.y),
             (int)(v[1]->PosR.x),
             (int)(v[1]->PosR.y),15,_Buffer);
  Yucku_Line( (int)(v[0]->PosR.x),
             (int)(v[0]->PosR.y),
             (int)(v[2]->PosR.x),
             (int)(v[2]->PosR.y),15,_Buffer);
  Yucku_Line( (int)(v[1]->PosR.x),
             (int)(v[1]->PosR.y),
             (int)(v[2]->PosR.x),
             (int)(v[2]->PosR.y),15,_Buffer);
 }


//
// ÄÄÄ Scene ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

sScene *Scene_New( int maxpolys )
 {
  sScene *Scene;

  Scene = (sScene *)malloc(sizeof(sScene));
  memset(Scene,0,sizeof(sScene));
  Scene->RenderList = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
  Scene->Sorted = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
  _Buffer = (char *)malloc(64000);
  _Screen = (char *)0xA0000;
  MinX = 0;
  MaxX = 319;
  MinY = 0;
  MaxY = 199;
  ScreenWidth = 320;
  ScreenHeight = 200;
  TopClip   = 0;
  BotClip   = 199;
  LeftClip  = 0;
  RightClip = 319;
  return Scene;
 }

 // ÄÄÄ

 // ByteSort
void    Scene_SortPolys( sScene *Scene )
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

 // ÄÄÄ

void    Scene_SetupForRendering(sScene *S)
 {
  sObject *O;
  sCamera *C;
  float TempFloat;

  C = S->Camera;
  S->VisiblePolys = 0;
  S->VisibleObjects = 0;
  switch(C->Type)
  {
   case CAMERAtype_free : Camera_CreateMatrix_Free( C ); break;
   case CAMERAtype_uvn  : Camera_CreateMatrix_UVN( C ); break;
   case CAMERAtype_3ds  : Camera_CreateMatrix_3DS( C ); break;
  }
  S->ViewDistanceX = FOV2FOC( S->ScreenWidth, DEG2RAD(C->Fov));
  S->ViewDistanceY = FOV2FOC( S->ScreenWidth, DEG2RAD(C->Fov));
  // Create all matrices
  O = S->Objects;
  while (O!=NULL)
  {
   switch(O->Type)
   {
    case OBJECTtype_free : Object_CreateMatrix_Free( O,S ); break;
    // case OBJECTtype_3ds  : Object_CreateMatrix_3ds ( O,S ); break;
   }
   O = O->Next;
  }
  // Handle all objects
  Object_Handle( S->Objects, S );
  // Sort polys
  Scene_SortPolys( S );
 }

 // ÄÄÄ

void    Scene_DrawAllPolys(sScene *S)
 {
  int i;

  for (i=S->VisiblePolys-1 ; i>=0 ; i-- )
  {
   Yucku_PolyWireFrame( S->RenderList[i] );
  }
 }

