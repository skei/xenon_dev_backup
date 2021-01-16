#include <stdio.h>    // file stuf
#include <stdlib.h>   // exit
#include <string.h>   // memcpy
#include <time.h>     // clock

#include "engine.h"
#include "vbe2.h"

int timestart;
float fps;
int frames_rendered;

sLight L[2];

char tekst[256];
char palette[768];
char *Tex1,*Tex2;

sKeySegment Seg[5];
sKeyFrame KeyFrame;

sObject *O;
sObject *O1;
sObject *Blob;

// --------------------------------------------------------------------------

void main(void)
{
 sVector dv;
 FILE *fp;
 int quit;
 char c;
 int i;

// ------------------------------
// Init engine + vesa
// ------------------------------

 if (Engine_Init(320,200,15,TRUE,10000) == FALSE)
 {
  printf("Where's your Vesa 2.0 driver???\n");
  exit(1);
 }

 printf("Vesa 2.0 found\n");
 printf("Mode %X: %i,%i,%i\n",VesaMode,320,200,15);
 printf("RedMaskSize  : %i   RedFieldPosition  : %i\n",RedMaskSize,RedFieldPosition);
 printf("GreenMaskSize: %i   GreenFieldPosition: %i\n",GreenMaskSize,GreenFieldPosition);
 printf("BlueMaskSize : %i   BlueFieldPosition : %i\n",BlueMaskSize,BlueFieldPosition);
 printf("Bytes per scanline: %i\n",BytesPerScanLine);
 printf("Address: %X\n",VesaPtr);

 printf("ESC to quit, any other key to continue\n");
 c = getch();
 if (c==27)
 {
  Engine_UnInit(TRUE);
  VesaUnInit();
  exit(1);
 }

// ------------------------------
// Allocate additional memory
// ------------------------------

 Tex1 = (char *)malloc(65536);
 Tex2 = (char *)malloc(65536);

// ------------------------------
// read the texture
// ------------------------------

 fp = fopen("a9.pix","rb");
 fread(Tex1,1,65536,fp);
 fclose(fp);
 fp = fopen("a.pix","rb");
 fread(Tex2,1,65536,fp);
 fclose(fp);

 fp = fopen("a9.pal","rb");
 fread(palette,1,768,fp);
 fclose(fp);

// ------------------------------
// Calculate shading table(s)
// ------------------------------

// VesaCalc_ShadeHighLight(palette,_VesaShadeTable,50,0,0,0);
 VesaCalc_ShadeTable(palette, _VesaShadeTable);
 Setup_Intensity(10);

// ------------------------------
// Load the objects
// ------------------------------

  O1 = Object_LoadFile("bogga3_2.o");

// ------------------------------
// Setup the object hierarchy
// ------------------------------

// Blob = Load3DS("thinblob.3ds",100,250);

 // namve, vertex mult, texture mult, default texture
 Blob = Load3DS("cubtorus.3ds",1,250);
 printf("Object: %s\n",Blob->ObjectName);
 getch();
 // "recurse" down to set some default variables
 O = Blob;
 while ( O!= NULL)
 {
  Object_SetPos(O,0,0,0);
  O->Texture1 = Tex1;
  O->KeyFramerPos = NULL;
  O->KeyFramerRot = NULL;
//  O->Flags |= OBJECT_envmap;
  O = O->Child;
 }

 // object, parent, child, inherit
 Object_SetHierarchy(   O1, NULL, Blob, FALSE );
// Object_SetHierarchy( Blob, NULL, NULL, FALSE );

 Object_SetPos(  O1, 0, 0, 0 );
 Object_SetPos(Blob, 0, 0, 0 );

   O1->Texture1 = Tex1;
 Blob->Texture1 = Tex1;

   O1->Flags |= OBJECT_envmap;
 Blob->Flags |= OBJECT_envmap;

// ------------------------------
// Setup the key-framer
// ------------------------------

 Seg[0].TimeStamp = 0;
 Seg[0].Pos.x = 0;
 Seg[0].Pos.y = 0;
 Seg[0].Pos.z = 0;

 Seg[1].TimeStamp = 30;
 Seg[1].Pos.x = 0;
 Seg[1].Pos.y = 0;
 Seg[1].Pos.z = -300;

 Seg[2].TimeStamp = 125;
 Seg[2].Pos.x = 0;
 Seg[2].Pos.y = 300;
 Seg[2].Pos.z = 0;

 Seg[3].TimeStamp = 250;
 Seg[3].Pos.x = -300;
 Seg[3].Pos.y = 100;
 Seg[3].Pos.z = 0;

 Seg[4].TimeStamp = 400;
 Seg[4].Pos.x = 100;
 Seg[4].Pos.y = -200;
 Seg[4].Pos.z = 300;

 KeyFrame.Flags = 0;      // ignored ???
 KeyFrame.CurrentPos = 0;
 KeyFrame.Speed = 2;
 KeyFrame.CurrentSegment = 0;
 KeyFrame.NumSegments = 5;
 KeyFrame.BendSize = 1.0;
 KeyFrame.SplineSegments = &Seg;

 Spline_Init( &KeyFrame );

 O1->KeyFramerPos = &KeyFrame;

// ------------------------------
// Setup lights
// ------------------------------

 L[0].Pos.x =    0.0;
 L[0].Pos.y =    0.0;
 L[0].Pos.z =    0.0;
 L[0].Direction.x = 0.0;
 L[0].Direction.y = -1.0;
 L[0].Direction.z = 0.0;
 L[0].Flags = LIGHT_point_dist;
 L[0].Strength = 2.0;
 L[0].MaxRange = 512;

 L[1].Pos.x =  100.0;
 L[1].Pos.y =    0.0;
 L[1].Pos.z = -100.0;
 L[1].Direction.x = 0.0;
 L[1].Direction.y = 0.0;
 L[1].Direction.z = -1.0;
 L[1].Flags = LIGHT_directional;
 L[1].Strength = 1.0;
 L[1].MaxRange = 300;

// ------------------------------
// Setup some other values
// ------------------------------

 World->Flags = 0;
 World->Objects = O1;
 World->Camera = Camera;
 World->NumLights = 0;
 World->Lights = &L;
 World->AmbientLight = 20.0/63.0;

 frames_rendered = 0;
 timestart = clock();

 Camera->Flags = CAMERA_aim;
// Camera->Flags = CAMERA_free;

 Camera_SetPos(Camera,0,0,-400);
 Camera_SetRot(Camera,0,0,0);
 Camera_SetTarget(Camera,0,0,0);
 Camera->Roll = 0;
 Camera->UpVector.x = 0;
 Camera->UpVector.y = -1;
 Camera->UpVector.z = 0;

// ------------------------------
// Gfx on!
// ------------------------------

// SetMode(0x13);
// SetPalette(&palette);

 Set_Vesa(VesaMode);

// ------------------------------
// Start the rendering loop
// ------------------------------

 quit = 0;
 while (quit==0)
 {
//  memset(_Buffer,0,64000);
  memset(_VesaBuffer,0,64000*2);

  Spline_GetValues( &O1->Pos, O1->KeyFramerPos );
  Spline_Add( O1->KeyFramerPos );

  Render_Setup( World );
  Render_Draw( World );

  frames_rendered++;
  fps = (float)(frames_rendered * CLOCKS_PER_SEC) / (clock() - timestart);

  sprintf(tekst,"VISIBLE POLYS %i",World->VisiblePolys);
  drawstring(0, 0,tekst,0x1f,_VesaBuffer);
  sprintf(tekst,"FPS %5.1f",fps);
  drawstring(0, 8,tekst,0x1f,_VesaBuffer);
  sprintf(tekst,"OBJ1 POS %5.1f %5.1f %5.1f",O1->PosR.x,O1->PosR.y,O1->PosR.z);
  drawstring(0,16,tekst,0x1f,_VesaBuffer);
  sprintf(tekst,"SPHERE RADIUS %5.1f",(O1->BSphereRadius * viewdistance)/O1->PosR.z);
  drawstring(0,24,tekst,0x1f,_VesaBuffer);

  sprintf(tekst,"LIGHT POS %5.1f %5.1f %5.1f",L[0].PosR.x,L[0].PosR.y,L[0].PosR.z);
  drawstring(0,32,tekst,0x1f,_VesaBuffer);

  sprintf(tekst,"SPLINE: %i %5.1f",KeyFrame.CurrentSegment,KeyFrame.CurrentPos);
  drawstring(0,40,tekst,0x1f,_VesaBuffer);

  memset(_VesaBuffer+(320*198),0xff,World->VisibleObjects*2);

//  waitvr();

  memcpy (_VesaScreen,_VesaBuffer,64000*2);

  Object_AddRot(  O1, 0.01, 0.03, 0.02 );
  Object_AddRot(Blob, 0.03, 0.04, 0.05 );


//  Camera_AddRot(&Camera, 0, 0.05, 0);

  if (kbhit())
  {
   c = getch();
   switch(c)
   {
    case 27 : quit = 1;
              break;
    case 'q' : L[0].Pos.x -= 5;
               break;
    case 'w' : L[0].Pos.x += 5;
               break;
    case 'a' : L[0].Pos.y -= 5;
               break;
    case 'z' : L[0].Pos.y += 5;
               break;
    case 'x' : L[0].Pos.z -= 5;
               break;
    case 's' : L[0].Pos.z += 5;
               break;
    case 'Q' : Camera_AddPos(Camera,-5, 0, 0);
               break;
    case 'W' : Camera_AddPos(Camera,+5, 0, 0);
               break;
    case 'A' : Camera_AddPos(Camera, 0,-5, 0);
               break;
    case 'Z' : Camera_AddPos(Camera, 0,+5, 0);
               break;
    case 'X' : Camera_AddPos(Camera, 0, 0,-5);
               break;
    case 'S' : Camera_AddPos(Camera, 0, 0,+5);
               break;
   }
   c = 0;
  }
 }

// ------------------------------
// Free objects
// ------------------------------

 Object_Free(O1);

// De nedenfor blir vel tatt h†nd om av automatisk, siden Object_Free
// rekurserer.. ???
// Engine_UnInit -> Object_Free(World->Objects) ?????

 Object_Free(Blob);

 free(Tex1);
 free(Tex2);

 Set_Vesa(3);
 SetMode(3);

 Engine_UnInit(TRUE);
}
