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

#include "S_pix1.i"
#include "S_pix2.i"
#include "S_pal.i"

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void main(void)
{
 FILE *fp;
 int i,j;
 sObject *O1,*O2;
 sCamera C;
 int quit;
 char c;
 unsigned char blah;
// int align;

 Engine_Init(8192);         // max polys in RenderList

 O1 = Object_Load("inv_sph.o");
 O1->Groups->Texture = S_pix1;
 O2 = Object_Load("blob74.o");
 O2->Groups->Texture = S_pix2;
 O2->Flags = O2->Flags | 0x10000;

 O1->Branch = O2;

 World->Camera = &C;
 World->Objects = O1;

 Object_SetPos(O1,0,0,0);
 Object_SetRot(O1,0,0,0);

 Object_SetPos(O2,0,0,300);
 Object_SetRot(O2,0,0,0);


 Camera_SetPos(&C,0,0,0);
 Camera_SetRot(&C,0,0,0);

 C.Flags = 0;

 _Buffer = (char *)malloc(64000);
 _Screen = (char *)0xA0000;

// _Texture = S_pix1;
// align = (int)_Texture & 0xffff;
// _Texture += (65536-align);

 SetMode(0x13);
 for (i=0;i<256;i++) setrgb(i,S_pal[i*3],S_pal[i*3+1],S_pal[i*3+2]);

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
  Object_AddRot(O1,0.05,0.01,0.03);
  Object_AddRot(O2,-0.05,-0.01,-0.03);
//  Camera_AddRot(&C,0.005,0.002,0.003);
  memset(_Buffer,0,64000);
  Render();
//  sprintf(tekst,"VISIBLE POLYS %i",World->VisiblePolys);
//  DrawString(0,0,tekst,255);
  waitvr();
  memcpy(_Screen,_Buffer,64000);
 }
 getch();
 SetMode(3);

// Object_Free(O);
 free(_Buffer);
 Engine_Exit();
}
