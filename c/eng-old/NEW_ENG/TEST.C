 #include <math.h>
 #include <string.h>
 #include <stdio.h>
 #include "engine.h"

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#define UVmult 256
#define GLOBAL_SPEED 0.1

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

void main(void)
 {
  FILE *fp;
  char Pal[768];
  int i;
  sCamera *C;
  sObject *O;
  sMaterial *M;
  sVector TempVector;
  float TempFloat;
  float m[16];

  // ---
  // Init some engine stuff
  // ---
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

  World = NewWorld(1);
  #define maxpolys 10000
  World->RenderList = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
  World->Sorted = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));

  // ---
  // Load 3DS file
  // ---
  printf("\nLoading 3DS\n");
  Load3DS("blob1.3ds",World);
  printf("...Loaded\n");

  // ---
  // Calc face/vertex normals, apply spherical mapping
  // ---
  O = World->Objects;
  while (O!=NULL)
  {
   Object_CalcFaceNormals(O);
   Object_CalcVertexNormals(O);
   // ---
   // if object not mapped, apply fake spherical mapping :-/
   if (O->OBJECT_mapped != 1)
   {
    printf("Applying fake spherical mapping to: %s\n",O->Name);
    for (i=0;i<O->NumVertices;i++)
    {
     O->Vertices[i].u = O->Vertices[i].Normal.x * (UVmult/2) + (UVmult/2);
     O->Vertices[i].v = O->Vertices[i].Normal.y * (UVmult/2) + (UVmult/2);
    }
    O->OBJECT_mapped = 1;
   }
   // ---
   O = O->Next;
  }

  // ---
  // load a default texture for all materials
  // ---
  M = World->Materials;
  while (M!=NULL)
  {
   M->Tex1 = (char *)malloc(65536);
   fp = fopen("texture.pix","rb");
   fread(M->Tex1,1,65536,fp);
   fclose(fp);
   M=M->Next;
  }
  fp = fopen("texture.pal","rb");
  fread(Pal,1,768,fp);
  fclose(fp);

  // ---
  // Camera setup
  // ---
  World->Camera = World->Cameras;
  World->Camera->UpVector.x = 0;
  World->Camera->UpVector.y = 1;
  World->Camera->UpVector.z = 0;

  getch();
  SetMode(0x13);
  for (i=0;i<256;i++) setrgb(i,Pal[i*3],Pal[i*3+1],Pal[i*3+2]);

  while (!kbhit())
  {
   Render_Setup( World );
   memset(_Buffer,0,64000);
   Render_Draw( World );
   waitvr();
   memcpy(_Screen,_Buffer,64000);
   World->CurrentPos += World->Speed;
   if (World->CurrentPos >= 60) setrgb(0,32,0,0);
   if (World->CurrentPos >= World->EndFrame) setrgb(0,32,32,32);
  }
  getch();
  SetMode(3);
 }

