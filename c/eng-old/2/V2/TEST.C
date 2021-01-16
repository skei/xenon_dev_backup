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

#include "LandTex.i"
#include "LandPal.i"

#include "Overlay1.i"
#include "OverlayP.i"

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void main(void)
{
 FILE *fp;
 int i,j;
 sObject *O;
 sCamera C;
 int quit;
 char c;
 unsigned char blah;
// int align;


 Engine_Init(8192);         // max polys in RenderList

 O = Object_Load("crater2.o");
 World->Camera = &C;
 World->Objects = O;

 Object_SetPos(O,0,0,0);
 Object_SetRot(O,0,0,0);

 Camera_SetPos(&C,0,0,100);
 Camera_SetRot(&C,1.7,0,0);

// C.Target.x = 1;
// C.Target.y = 2;
// C.Target.z = 3;
// C.Roll = 1;

 C.Flags = 0;

 _Buffer = (char *)malloc(64000);
 _Screen = (char *)0xA0000;

 _Texture = (char *)malloc(65536);
// align = (int)_Texture & 0xffff;
// _Texture += (65536-align);

 memcpy(_Texture,LandTex,65536);
 memcpy(palette,LandPal,768);

 SetMode(0x13);
 for (i=0;i<128;i++) setrgb(i,LandPal[i*3],LandPal[i*3+1],LandPal[i*3+2]);
 for (i=192;i<192+32;i++) setrgb(i,OverLayPal[i*3],OverLayPal[i*3+1],OverLayPal[i*3+2]);
 setrgb(0,63,63,63);

 ScreenWidth = 320;
 ScreenHeight = 200;
 MinX = 0; MaxX = 319;
 MinY = 0; MaxY = 199;
 viewdistance = PROJ_VALUE;
 halfscreenwidth_vd = (ScreenWidth/2)/viewdistance;
 halfscreenheight_vd = (ScreenHeight/2)/viewdistance;
 minclip_z = 10;
 maxclip_z = 4000;

 quit = 0;
 while (!kbhit())
 {
//  Object_AddRot(O,0.01,0.02,0);
  Camera_AddRot(&C,0,0,0.01);
  memset(_Buffer,0,64000);
  Render();

  for (i=0;i<64000;i++)
  {
   blah = OverLayPic[i];
   if (blah!=255) _Buffer[i] = blah;
  }

//  sprintf(tekst,"VISIBLE POLYS %i",World->VisiblePolys);
//  DrawString(0,0,tekst,255);
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
