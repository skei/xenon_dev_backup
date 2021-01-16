#include <stdio.h>    // file stuf
#include <stdlib.h>   // exit
#include <string.h>   // memcpy
#include <time.h>     // clock

#include "engine.h"
#include "vbe2.h"

sObject *oo;
sLight L[2];

char tekst[256];

char palette[768];
char *Tex1,*Tex2;

int CurrentFrame;
int quit;
char c;

void main(void)
{
 FILE *fp;

 int timestart;
 float fps;
 int frames_rendered;


 if (Engine_Init(320,200,15,TRUE,5000,905) == FALSE)
 {
  printf("Error initializing VBE2 - 320x200x15\n");
  exit(1);
 }
 printf("Loading 3DS\n");
 // World->Objects = Load3DS("birdshow.3ds",1,250,FALSE);
 // World->Objects = Load3DS("inf1.3ds",1,250,FALSE);
 World->Objects = Load3DS("f:\\rip3ds\\vek2.3ds",1,250,FALSE);

 printf("Loading VUE\n");
 // LoadVUE("birdshow.vue");
 // LoadVUE("inf1.vue");
 LoadVUE("f:\\rip3ds\\vek2.vue");

 Camera->Flags = CAMERA_vue;
// Camera_SetPos(Camera,0,0,-1000);
// Camera_SetRot(Camera,0,0,0);
// Camera_SetTarget(Camera,0,0,0);
 Camera->Roll = 0;
 Camera->UpVector.x = 0;
 Camera->UpVector.y = 0;
 Camera->UpVector.z = 1;

 World->Camera = Camera;

 Tex1 = (char *)malloc(65536);

 // "recurse" down to set some default variables
 oo = World->Objects;
 while ( oo!= NULL)
 {
//  printf("%s\n",oo->ObjectName);
//  Object_SetPos(oo,0,0,0);
  oo->Texture1 = Tex1;
//  oo->Flags |= OBJECT_envmap;
  oo->Flags |= OBJECT_VUE;
  oo = oo->Child;
 }


 World->Flags = 0;
 World->NumLights = 0;
 World->Lights = &L;
 World->AmbientLight = 20.0/63.0;

 fp = fopen("a9.pix","rb");
 fread(Tex1,1,65536,fp);
 fclose(fp);
 fp = fopen("a9.pal","rb");
 fread(palette,1,768,fp);
 fclose(fp);

 VesaCalc_ShadeTable(palette, _VesaShadeTable);
 Setup_Intensity(10);


 World->FrameNum = 0;

 Set_Vesa(VesaMode);


// ------------------------------
// Start the rendering loop
// ------------------------------

 frames_rendered = 0;
 timestart = clock();

 CurrentFrame = 0;

 while (!kbhit())
 {
//  memset(_Buffer,0,64000);
  memset(_VesaBuffer,0,64000*2);

  Render_Setup( World );
//  Apply_VUEmatrix( CurrentFrame, O );
  Render_Draw( World );

  sprintf(tekst,"FRAME %i",World->FrameNum);
  drawstring(0, 0,tekst,0x3fff,_VesaBuffer);

  frames_rendered++;
  fps = (float)(frames_rendered * CLOCKS_PER_SEC) / (clock() - timestart);
  sprintf(tekst,"FPS %5.1f",fps);
  drawstring(0, 8,tekst,0x1f,_VesaBuffer);

  memset(_VesaBuffer+(320*198),0xff,World->VisibleObjects*2);

  World->FrameNum++;
  if (World->FrameNum > MaxVUEFrames) World->FrameNum = 0;

  waitvr();
  memcpy (_VesaScreen,_VesaBuffer,64000*2);
 }
 Set_Vesa(3);
 SetMode(3);

 Engine_UnInit(TRUE);
}






