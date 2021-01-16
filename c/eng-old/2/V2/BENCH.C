#include <conio.h>    // getch
#include <string.h>   // memset, memcpy
#include <stdio.h>    // file, sprintf

#include "defines.h"
#include "types.h"
#include "vars.h"

#include "gfx.h"
#include "render.h"
#include "engine.h"
#include "poly.h"

char palette[768];
char tekst[256];        // for FPS calc

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void main(void)
{

 sVertex V1,V2,V3;
 sPolygon P;

 int i;

 _Buffer = (char *)malloc(64000);
 _Screen = (char *)0xA0000;
 _Texture = (char *)malloc(65536);

 ScreenWidth = 320;
 ScreenHeight = 200;
 MinX = 0; MaxX = 319;
 MinY = 0; MaxY = 199;

 V1.PosR.x = 10;
 V1.PosR.y = 10;
 V1.u = 0;
 V1.v = 0;

 V1.PosR.x = 15;
 V1.PosR.y = 10;
 V1.u = 1;
 V1.v = 0;

 V1.PosR.x = 10;
 V1.PosR.y = 15;
 V1.u = 0;
 V1.v = 1;

 P.v[0] = &V1;
 P.v[1] = &V2;
 P.v[2] = &V3;

 for (i=0;i<1000000;i++)
 {
  TexPoly(&P);
 }

 free(_Texture);
 free(_Buffer);
 Engine_Exit();
}

