#include <assert.h>
#include <stdio.h>    // file stuf
#include <string.h>   // memcpy
#include <time.h>     // clock


#include "defines.h"
#include "types.h"
#include "vars.h"

#include "gfx.h"
#include "math3d.h"
#include "engine.h"
#include "render.h"

int timestart;
float fps;
int frames_rendered;

sWorld W;
sCamera C;
sLight L[2];
sObject *O1;
sObject *oO1;
sObject *oO2;
sObject *oO3;
sObject *oO4;
sObject *oO5;
sObject *oO6;

char tekst[256];
char palette[768];
char *Tex;

// --------------------------------------------------------------------------

void main(void)
{
 FILE *fp;
 int quit;
 char c;

// ----- Init -----
 ScreenWidth = 320;
 ScreenHeight = 200;
 MinX = 0; MaxX = 319;
 MinY = 0; MaxY = 199;
#define viewdistance 256;
 halfscreenwidth_vd = (ScreenWidth/2)/viewdistance;
 halfscreenheight_vd = (ScreenHeight/2)/viewdistance;
 min_clip_z = 1;
 max_clip_z = 4000;
#define maxpolys 2500
 W.RenderList = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
 assert(W.RenderList!=0);
 W.Sorted = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
 assert(W.Sorted!=0);
 _Buffer = (char *)malloc(ScreenWidth*ScreenHeight);
 assert(_Buffer!=0);
 memset(_Buffer,0,64000);
 _Screen = (char *)0xA0000;

 Tex = (char *)malloc(65536);
 assert(Tex!=0);

 _ShadeTable = (char *)malloc(65536);
 assert(_ShadeTable!=0);
 memset(_ShadeTable,127,65536);

 _PhongMap = (char *)malloc(65536);
 assert(_PhongMap!=0);
 memset(_PhongMap,127,65536);


// ----------------

 fp = fopen("tex.pix","rb");
 fread(Tex,1,65536,fp);
 fclose(fp);
 fp = fopen("tex.pal","rb");
 fread(palette,1,768,fp);
 fclose(fp);

 Calc_ShadeHighLight(palette,_ShadeTable,50,0,0,0);
 Calc_PhongMap();

 O1 = Object_LoadFile("f:\\engine\\meshes\\bogga3_2.o");
 oO1 = Object_LoadFile("f:\\engine\\meshes\\blob1.o");

 Object_SetHierarchy( O1 ,NULL,oO1 ,FALSE);
 Object_SetPos(O1,0,0,0);
 O1->Texture1 = Tex;

 Object_SetHierarchy( oO1,O1  ,NULL ,TRUE);
 Object_SetPos(oO1, 200, 0  , 0);
 oO1->Texture1 = Tex;

 L[0].Pos.x = 0;
 L[0].Pos.y = 100;
 L[0].Pos.z = 0;
 L[0].Flags = LIGHT_point;

 L[1].Rot.x = 0;
 L[1].Rot.y = 0;
 L[1].Rot.z = -1;
 L[1].Flags = LIGHT_directional;

 W.Flags = 0;
 W.Objects = O1;
 W.Camera = &C;
 W.NumLights = 1;
 W.Lights = &L;

 SetMode(0x13);
 SetPalette(&palette);

 frames_rendered = 0;
 timestart = clock();

 C.Flags = CAMERA_aim;
 Camera_SetPos(&C,0,0,-800);
 Camera_SetRot(&C,0,0,0);
 Camera_SetTarget(&C,0,0,0);
 C.Roll = 0;

 quit = 0;
 while (quit==0)
 {
  memset(_Buffer,0,64000);
  Render( &W );

  frames_rendered++;
  fps = (float)(frames_rendered * CLOCKS_PER_SEC) / (clock() - timestart);

  sprintf(tekst,"VISIBLE POLYS %i",W.VisiblePolys);
  drawstring(0,0,tekst,127,_Buffer);
  sprintf(tekst,"FPS %5.1f",fps);
  drawstring(0,8,tekst,127,_Buffer);

//  waitvr();

  memcpy (_Screen,_Buffer,64000);
//  Object_AddRot(O1, 0.03,    0.02, 0.01);
  Object_AddRot(oO1,0.05,    0.01, 0.01);
//  Camera_AddRot(&C,0.05,    0.01, 0.01);

  if (kbhit())
  {
   c = getch();
   switch(c)
   {
    case 27 : quit = 1;
              break;
    case 'q' : Camera_AddTarget(&C,-10,  0,  0);
               break;
    case 'w' : Camera_AddTarget(&C, 10,  0,  0);
               break;
    case 'a' : Camera_AddTarget(&C,  0,-10,  0);
               break;
    case 'z' : Camera_AddTarget(&C,  0, 10,  0);
               break;
    case 'Q' : Camera_AddPos(&C,-10,  0,  0);
               break;
    case 'W' : Camera_AddPos(&C, 10,  0,  0);
               break;
    case 'A' : Camera_AddPos(&C,  0,-10,  0);
               break;
    case 'Z' : Camera_AddPos(&C,  0, 10,  0);
               break;
    case 'S' : Camera_AddPos(&C,  0,  0, 10);
               break;
    case 'X' : Camera_AddPos(&C,  0,  0,-10);
               break;
   }
   c = 0;
  }

 }
// getch();
 SetMode(3);

 Object_Free(O1);

 free(W.RenderList);
 free(W.Sorted);
 free(_Buffer);
 free(Tex);
}
