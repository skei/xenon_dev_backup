#include <conio.h>    // getch
#include <string.h>   // memset, memcpy
#include <stdio.h>    // file, sprintf

#include "defines.h"
#include "types.h"
#include "vars.h"

#include "gfx.h"
#include "render.h"
#include "engine.h"

char palette[768];
char tekst[256];        // for FPS calc

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void main(void)
{
 FILE *fp;
 int i;
 sObject *O;
 sCamera C;

 int align;

 Engine_Init(4096);         // max 1024 polys in RenderList
 O = Object_Load("inv_sph.o");
 World->Camera = &C;
 World->Objects = O;
 Object_SetPos(O,0,0,0);
 Camera_SetPos(&C,0,0,00);

 C.Target.x = 1;
 C.Target.y = 2;
 C.Target.z = 3;
 C.Roll = 1;

 C.Flags = 0;

 _Buffer = (char *)malloc(64000);
 _Screen = (char *)0xA0000;

 _Texture = (char *)malloc(65536);
// align = (int)_Texture & 0xffff;
// _Texture += (65536-align);

 fp = fopen("tex.pix","rb");
 fread(_Texture,1,65536,fp);
 fclose(fp);

 fp = fopen("tex.pal","rb");
 fread(palette,1,768,fp);
 fclose(fp);

 SetMode(0x13);
 for (i=0;i<256;i++) setrgb(i,palette[i*3],palette[i*3+1],palette[i*3+2]);

 ScreenWidth = 320;
 ScreenHeight = 200;
 MinX = 0; MaxX = 319;
 MinY = 0; MaxY = 199;

 viewdistance = PROJ_VALUE;
 halfscreenwidth_vd = (ScreenWidth/2)/viewdistance;
 halfscreenheight_vd = (ScreenHeight/2)/viewdistance;
 minclip_z = 10;
 maxclip_z = 4000;

 while (!kbhit())
 {
  Object_AddRot(O,0.01,0.02,0);
//  Camera_AddRot(&C,0.003,0.005,0.004);
  memset(_Buffer,0,64000);
  Render();
  sprintf(tekst,"VISIBLE POLYS %i",World->VisiblePolys);
  DrawString(0,0,tekst,255);

  waitvr();
  memcpy(_Screen,_Buffer,64000);
 }
 getch();
 SetMode(3);

// Object_Free(O);
 free(_Texture);
 free(_Buffer);
 Engine_Exit();
}
