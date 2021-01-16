x#include <conio.h>    // getch
#include <string.h>   // memset, memcpy
#include <stdio.h>    // file, sprintf

// #include "defines.h"
// #include "types.h"
// #include "vars.h"

// #include "gfx.h"
// #include "render.h"
// #include "engine.h"
#include "eng.h"

char palette[768];
char tekst[256];        // for FPS calc

#include "LandTex.i"
#include "LandPal.i"

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

 FILE *fp;
 int i,j;
 sObject *O;
 sCamera C;
 int quit;
 char c;


void Land_Init(void)
{
}

void Land_Start(void)
{
 Engine_Init(8192);         // max polys in RenderList
 O = Object_Load("crater2.o");
 World->Camera = &C;
 World->Objects = O;
 Object_SetPos(O,0,0,0);
 Object_SetRot(O,0,0,0);
 Camera_SetPos(&C,0,0,200);
 Camera_SetRot(&C,1.9,0,0);
 C.Flags = 0;
 _Buffer = (char *)malloc(64000);
 _Screen = (char *)0xA0000;
 _Texture = (char *)malloc(65536);
 memcpy(_Texture,LandTex,65536);
 memcpy(palette,LandPal,768);
 SetMode(0x13);
 for (i=0;i<256;i++) setrgb(i,palette[i*3],palette[i*3+1],palette[i*3+2]);
// setrgb(0,63,63,63); setrgb(255,63,63,63);
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
}

void Land_Update(int frames)
{
 Camera_AddRot(&C,0,0,0.01);
 memset(_Buffer,0,64000);
 Render();
 BlitBuffer = _Buffer;
 ReadyToBlit = 1;
}

void Land_Stop(void)
{
 free(_Texture);
 free(_Buffer);
 Engine_Exit();
}

void land_Exit(void)
{
}


