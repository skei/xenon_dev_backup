 #include <math.h>
 #include <string.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <conio.h>
 #include "engine.h"
 #include "file.h"
 #include "poly2.h"

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

void main(int argc, char *argv[])
 {
  FILE *fp;
  char Pal[768];
  int i;
  sWorld *W;
  sCamera C;
  sObject *O;
  sMaterial M1;
  sVector TempVector;
  float TempFloat;
  float muller;
  char *dummy;
  float m[16];

  if (argc!=3)
  {
   printf("\n%s <filename.3ds> <mul>\n",argv[0]);
   exit(1);
  }

  muller = strtod(argv[2],&dummy);

  printf("\nLoading 3DS\n");
  W = NewWorld(1);
  Load3DS(argv[1],W);
  if (W == NULL) { printf("Invalid 3DS file!\n"); exit(2); }

  O = W->Objects;

  for (i=0;i<O->NumVertices;i++)
  {
   O->Vertices[i].Pos.x *= muller;
   O->Vertices[i].Pos.y *= muller;
   O->Vertices[i].Pos.z *= muller;
  }

  Object_Center(O);             // center object around its average pos
  Object_SetPos(O,0,0,0);
  Object_CalcFaceNormals(O);
  Object_CalcVertexNormals(O);
  O->Parent = NULL;
  O->Next = NULL;

  // ---

  #define UVmult 256

  // if object not mapped, apply fake spherical mapping :-/
  if (O->OBJECT_mapped != 1)
  {
   for (i=0;i<O->NumVertices;i++)
   {
    O->Vertices[i].u = O->Vertices[i].Normal.x * (UVmult/2) + (UVmult/2);
    O->Vertices[i].v = O->Vertices[i].Normal.y * (UVmult/2) + (UVmult/2);
   }
   O->OBJECT_mapped = 1;
  }
  else
  {
   for (i=0;i<O->NumVertices;i++)
   {
    O->Vertices[i].u *= 256;
    O->Vertices[i].v *= 256;
   }
  }

  // ---
  // load a default texture for all materials
  // ---

  printf("NumVertices: %i\n",O->NumVertices);
  printf("NumPolys: %i\n",O->NumPolygons);

  for (i=0;i<O->NumPolygons;i++)
  {
   O->Polygons[i].Material = &M1;
  }

  printf("Loading texture\n");
  M1.Tex1 = (char *)malloc(65536);
  fp = fopen("texture.pix","rb");
  fread(M1.Tex1,1,65536,fp);
  fclose(fp);

  fp = fopen("texture.pal","rb");
  fread(Pal,1,768,fp);
  fclose(fp);

  printf("Init some engine speciic stuff...\n");
  _Buffer = (char *)malloc(64000);
  _Screen = (char *)0xA0000;
  MinX = 0;  MaxX = 319;
  MinY = 0;  MaxY = 199;
  ScreenWidth = 320;
  ScreenHeight = 200;
  TopClip   = 0;
  BotClip   = 199;
  LeftClip  = 0;
  RightClip = 319;
  #define maxpolys 10000
  W->RenderList = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
  W->Sorted = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));

  printf("Seting up camera...\n");
  W->Camera = W->Cameras;
  W->Camera->UpVector.x = 0;
  W->Camera->UpVector.y = 1;
  W->Camera->UpVector.z = 0;
  Camera_SetTarget(W->Camera,0,0,0);
  Camera_SetPos(W->Camera,0,0,-500);
  printf("OK\n");

  printf("OK.. Start rendering... \n");
  getch();
  SetMode(0x13);
  for (i=0;i<256;i++) setrgb(i,Pal[i*3],Pal[i*3+1],Pal[i*3+2]);

  while (!kbhit())
  {
   Render_Setup( W );
   memset(_Buffer,0,64000);
   Render_Draw( W );
   waitvr();
   memcpy(_Screen,_Buffer,64000);
   Object_AddRot(W->Objects,0.006,0.004,0.003);
  }
  getch();
  SetMode(3);
 }

