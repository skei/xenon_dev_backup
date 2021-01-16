 #include <math.h>
 #include <string.h>
 #include "engine.h"
 #include "file.h"

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void SetMode(unsigned short int);
 #pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

void setrgb(char color, char r,char g, char b);
 #pragma aux setrgb=\
 "mov dx,3c8h",\
 "out dx,al",\
 "inc dx",\
 "mov al,ah",\
 "out dx,al",\
 "mov al,bl",\
 "out dx,al",\
 "mov al,bh",\
 "out dx,al",\
 parm [al] [ah] [bl] [bh] modify [edx];

void waitvr (void);
 #pragma aux waitvr =\
 " mov dx,3dah",\
 "wait1:",\
 " in al,dx",\
 " test al,8",\
 " jnz wait1",\
 "wait2:",\
 " in al,dx",\
 " test al,8",\
 " jz wait2",\
 modify [al dx];

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void PrintMatrix( float *m )
{
 printf("%5.2f  %5.2f  %5.2f  %5.2f\n",m[ 0],m[ 1],m[ 2],m[ 3]);
 printf("%5.2f  %5.2f  %5.2f  %5.2f\n",m[ 4],m[ 5],m[ 6],m[ 7]);
 printf("%5.2f  %5.2f  %5.2f  %5.2f\n",m[ 8],m[ 9],m[10],m[11]);
 printf("%5.2f  %5.2f  %5.2f  %5.2f\n",m[12],m[13],m[14],m[15]);
}

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void _Camera_CreateMatrix_Aim(sCamera *c)
 {
  float TempFloat;
  float mm[16];
  sVector Vtemp,Up,U,V,N;

  TempFloat = Spline_GetValue( &c->PosKeyFrame, &c->Pos);
  TempFloat = Spline_GetValue( &c->TarKeyFrame, &c->Target);

  Spline_Update( &c->PosKeyFrame );
  Spline_Update( &c->TarKeyFrame );

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

void _Object_CreateMatrix( sObject *o, sWorld *World )
 {
  // sQuaternion q;
  float m1[16];
  sVector temp1,temp2;
  float TempFloat;

  Matrix_Identity(o->o2w);

  // --- rotation ---
  if (o->RotKeyFrame.KEY_defined == 1)
  {
   TempFloat = Spline_GetValue( &o->RotKeyFrame, &temp1);
   Spline_Update( &o->RotKeyFrame );
   o->q[qW] = TempFloat;
   o->q[qX] = temp1.x;
   o->q[qY] = temp1.y;
   o->q[qZ] = temp1.z;
   Quat_Normalize(o->q);
   Matrix_Identity(&m1);
   Quat_ToMatrix( o->q, &m1 );        // ???
   Matrix_Mult(o->o2w,&m1);
  }

  // --- translation ---
  if (o->PosKeyFrame.KEY_defined == 1)
  {
   TempFloat = Spline_GetValue( &o->PosKeyFrame, &temp1);
   Spline_Update( &o->PosKeyFrame );
   Matrix_Translation(&m1,temp1.x,temp1.y,temp1.z);
   Matrix_Mult(o->o2w,&m1);
  }

  // --- parent ---
  if (o->Parent != NULL) Matrix_Mult(o->o2w,o->Parent->o2w);

  Matrix_Copy(o->m,o->o2w);
  Matrix_Mult(o->m,World->Camera->m);
 }

 // --------------------------------------------------------------------------

void _Handle_Object( sObject *O, sWorld *World )
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
      O->Vertices[i].PosR.x = ((tempvector.x * World->viewdistance) * inv_z) + 160;
      O->Vertices[i].PosR.y = ((tempvector.y * World->viewdistance) * inv_z) + 100;
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
         - ((v3->PosR.x - v1->PosR.x) * (v2->PosR.y - v1->PosR.y)) <= 0))
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
  _Handle_Object( O->Next, World );
 }

 // --------------------------------------------------------------------------

void _Render_Setup( sWorld *World)
 {
  sObject *O;
  sCamera *C;
  float TempFloat;


  C = World->Camera;
  World->VisiblePolys = 0;
  World->VisibleObjects = 0;

  _Camera_CreateMatrix_Aim( C );

  World->viewdistance = 160 / ( sin(  ((2400/C->FOV)/360)*PI  )
                            /   cos(  ((2400/C->FOV)/360)*PI) );
  O = World->Objects;
  while (O!=NULL)
  {
   _Object_CreateMatrix(O,World);
   O = O->Next;
  }
  _Handle_Object( World->Objects, World );
 }

 // -------------------------------------------------------------------------

void _Render_Draw( sWorld *World)
 {
  int i;

  for (i=World->VisiblePolys-1 ; i>=0 ; i-- )
  {
   YuckuPolyWireFrame( World->RenderList[i] );
  }
 }

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void main(int argc, char *argv[])
 {
  sWorld *W;
  sCamera *C;
  sObject *O;
  sVector TempVector;
  float TempFloat;
  float m[16];

  if (argc!=2)
  {
   printf("\n%s <filename.3ds>\n",argv[0]);
   exit(1);
  }
  printf("\nLoading 3DS\n");
  W = Load3DS(argv[1]);
  if (W == NULL) { printf("Invalid 3DS file!\n"); exit(2); }

  if (W->NumCameras == 0) { printf("No camera in 3DS!\n"); exit(2); }

  O = W->Objects;
  while (O!=NULL)
  {
   O->PosKeyFrame.Speed = 1;
   O->RotKeyFrame.Speed = 1;
   // ---
   // ---
   O = O->Next;
  }

  _Buffer = (char *)malloc(64000);
  _Screen = (char *)0xA0000;
  MinX = 0;  MaxX = 319;
  MinY = 0;  MaxY = 199;
  ScreenWidth = 320;
  ScreenHeight = 200;

  #define maxpolys 10000
  W->RenderList = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
  W->Sorted = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
  W->Camera = W->Cameras;

  C = W->Camera;
  C->UpVector.x = 0;
  C->UpVector.y = -1;
  C->UpVector.z = 0;
  C->PosKeyFrame.Speed = 1;
  C->TarKeyFrame.Speed = 1;
  // C->FOVKeyFrame.Speed = 1;

  // printf("Camera Pos:  %5.2f %5.2f %5.2f\n",C->Pos.x,C->Pos.y,C->Pos.z);
  // printf("Camera Target:  %5.2f %5.2f %5.2f\n",C->Target.x,C->Target.y,C->Target.z);
  // printf("Camera Bank:  %5.2f\n",C->Bank);

//  C->Target.x = 0;
//  C->Target.y = 0;
//  C->Target.z = 0;

  SetMode(0x13);

  while (!kbhit())
  {
   _Render_Setup( W );
   memset(_Buffer,0,64000);
   _Render_Draw( W );
   waitvr();
   memcpy(_Screen,_Buffer,64000);
  }
  getch();
  SetMode(3);

 }
