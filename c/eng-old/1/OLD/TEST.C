#include <assert.h>
#include <stdio.h>    // file stuf
#include <string.h>   // memcpy
#include <time.h>     // clock


#include "eng_def.h"
#include "eng_type.h"
#include "eng_vars.h"
#include "eng_gfx.h"
#include "eng_math.h"
#include "eng_rend.h"

#include "engine.h"


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
// Det meste av dette skal flyttes til Engine_Init, Camera_New, etc...
// --------------------------------------------------------------------------

void Calc_DivTable(void)
{
 int i;
 float fff;
 float *ptr;

 ptr = _DivTable;
 *ptr++ = 0.0;
 fff = (1.0/16.0);
 for (i=1;i<65536;i++)
 {
  *ptr++ = (1.0/fff);//(1/f);
  fff = fff + (1.0/16.0);
 }
}

// -----

void main(void)
{
 sVector dv;
 FILE *fp;
 int quit;
 char c;
 int i;

// ----- Init -----
 ScreenWidth = 320;
 ScreenHeight = 200;
 MinX = 0; MaxX = 319;
 MinY = 0; MaxY = 199;

 viewdistance = 320;
 halfscreenwidth_vd = (float)(ScreenWidth/2)/viewdistance;
 halfscreenheight_vd = (float)(ScreenHeight/2)/viewdistance;
 min_clip_z = 1;
 max_clip_z = 4000;
#define maxpolys 3000
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

  O1 = Object_LoadFile("f:\\engine\\meshes\\sph256.o");
 oO1 = Object_LoadFile("f:\\engine\\meshes\\sph256.o");
 oO2 = Object_LoadFile("f:\\engine\\meshes\\sph256.o");
 oO3 = Object_LoadFile("f:\\engine\\meshes\\sph256.o");
 oO4 = Object_LoadFile("f:\\engine\\meshes\\sph256.o");
 oO5 = Object_LoadFile("f:\\engine\\meshes\\sph256.o");
 oO6 = Object_LoadFile("f:\\engine\\meshes\\sph256.o");

 Object_SetHierarchy( O1 ,NULL,oO1 ,FALSE);
 Object_SetPos(O1,200,0,0);
 O1->Texture1 = Tex;

 Object_SetHierarchy( oO1,O1  ,oO2 ,TRUE);
 Object_SetPos(oO1,  200,   0 ,    0);
 oO1->Texture1 = Tex;
 Object_SetHierarchy( oO2,O1  ,oO3 ,TRUE);
 Object_SetPos(oO2, -200,    0 ,    0);
 oO2->Texture1 = Tex;
 Object_SetHierarchy( oO3,O1  ,oO4 ,TRUE);
 Object_SetPos(oO3,    0,  200 ,    0);
 oO3->Texture1 = Tex;
 Object_SetHierarchy( oO4,O1  ,oO5 ,TRUE);
 Object_SetPos(oO4,    0, -200 ,    0);
 oO4->Texture1 = Tex;
 Object_SetHierarchy( oO5,O1  ,oO6 ,TRUE);
 Object_SetPos(oO5,    0,    0 ,  200);
 oO5->Texture1 = Tex;
 Object_SetHierarchy( oO6,O1  ,NULL ,TRUE);
 Object_SetPos(oO6,    0,    0 , -200);

 oO6->Texture1 = Tex;

 L[0].Pos.x = -100.0;
 L[0].Pos.y =    0.0;
 L[0].Pos.z = -100.0;
 L[0].Direction.x = 0.0;
 L[0].Direction.y = 0.0;
 L[0].Direction.z = -1.0;
 L[0].Flags = LIGHT_point_dist;
 L[0].Strength = 1.0;
 L[0].MaxRange = 256;

 L[1].Pos.x =  100.0;
 L[1].Pos.y =    0.0;
 L[1].Pos.z = -100.0;
 L[1].Direction.x = 0.0;
 L[1].Direction.y = 0.0;
 L[1].Direction.z = -1.0;
 L[1].Flags = LIGHT_point_dist;
 L[1].Strength = 1.0;
 L[1].MaxRange = 300;

 W.Flags = 0;
 W.Objects = O1;
 W.Camera = &C;
 W.NumLights = 2;
 W.Lights = &L;

 SetMode(0x13);
 SetPalette(&palette);

 frames_rendered = 0;
 timestart = clock();

 C.Flags = CAMERA_aim;
 Camera_SetPos(&C,0,0,-1000);
 Camera_SetRot(&C,0,0,0);
 Camera_SetTarget(&C,0,0,0);
 C.Roll = 0;
 C.UpVector.x = 0;
 C.UpVector.y = -1;
 C.UpVector.z = 0;

// _DivTable = (float *)malloc(65536*sizeof(float));
// Calc_DivTable();

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
  vector_Trans(&dv,O1->Pos,&O1->m);
  sprintf(tekst,"OBJ1 POS %5.1f %5.1f %5.1f",dv.x,dv.y,dv.z);
  drawstring(0,16,tekst,127,_Buffer);
  sprintf(tekst,"LIGHT POS %5.1f %5.1f %5.1f",L[0].Pos.x,L[0].Pos.y,L[0].Pos.z);
  drawstring(0,24,tekst,127,_Buffer);

//  waitvr();

  memcpy (_Screen,_Buffer,64000);
//  Object_AddRot(O1, 0.03,    0.02, 0.01);
//  Object_AddRot(oO1,0.05,    0.01, 0.07);
//  Object_AddRot(oO2,0.03,    0.03, 0.03);
//  Object_AddRot(oO3,0.02,    0.02, 0.01);
//  Object_AddRot(oO4,0.07,    0.05, 0.05);
//  Object_AddRot(oO5,0.01,    0.04, 0.04);
//  Object_AddRot(oO6,0.02,    0.06, 0.06);

//  Camera_AddRot(&C,0.05,    0.01, 0.01);

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
    case 'Q' : Object_AddPos(O1,-1, 0, 0);
               break;
    case 'W' : Object_AddPos(O1,+1, 0, 0);
               break;
    case 'A' : Object_AddPos(O1, 0,-1, 0);
               break;
    case 'Z' : Object_AddPos(O1, 0,+1, 0);
               break;
    case 'X' : Object_AddPos(O1, 0, 0,-1);
               break;
    case 'S' : Object_AddPos(O1, 0, 0,+1);
               break;

   }
   c = 0;
  }

 }
// getch();
 SetMode(3);

 Object_Free(O1);
 Object_Free(oO1);
 Object_Free(oO2);
 Object_Free(oO3);
 Object_Free(oO4);
 Object_Free(oO5);
 Object_Free(oO6);

 free(W.RenderList);
 free(W.Sorted);
 free(_Buffer);
 free(Tex);

// for (i=0;i<33;i++) printf("%i : %5.2f : ",i,_DivTable[i]);
// printf("\n");

// free(_DivTable);
}
