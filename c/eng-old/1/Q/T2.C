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

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void main(void)
{
 FILE *fp;

 int timestart;
 float fps;
 int frames_rendered;


 if (Engine_Init(320,200,15,TRUE,7000,300) == FALSE)
 {
  printf("Error initializing VBE2 - 320x200x15\n");
  exit(1);
 }
 printf("Loading 3DS\n");
 World->Objects = Load3DS("birdshow.3ds",1,250,FALSE);

 printf("Loading VUE\n");
 LoadVUE("birdshow.vue");

 Camera->Flags = CAMERA_vue;
 Camera->UpVector.x = 0;
 Camera->UpVector.y = 0;
 Camera->UpVector.z = 1;

 World->Flags = 0;
 World->Camera = Camera;

 // Load texture
 Tex1 = (char *)malloc(65536);
 fp = fopen("a9.pix","rb");
 fread(Tex1,1,65536,fp);
 fclose(fp);
 fp = fopen("a9.pal","rb");
 fread(palette,1,768,fp);
 fclose(fp);

 // "recurse" down to set some default variables
 oo = World->Objects;
 while ( oo!= NULL)
 {
  oo->Texture1 = Tex1;
  oo->Flags |= OBJECT_envmap;
  oo->Flags |= OBJECT_VUE;
  oo = oo->Child;
 }

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
  memset(_VesaBuffer,0,64000*2);
  Render_Setup( World );
  Render_Draw( World );

  // Draw some info text on the screen
  sprintf(tekst,"FRAME %i",World->FrameNum);
  drawstring(0, 0,tekst,0x3fff,_VesaBuffer);
  frames_rendered++;
  fps = (float)(frames_rendered * CLOCKS_PER_SEC) / (clock() - timestart);
  sprintf(tekst,"FPS %5.1f",fps);
  drawstring(0, 8,tekst,0x1f,_VesaBuffer);

  // Draw a line on the botom of the screen, to show number of visible obj's
  memset(_VesaBuffer+(320*198),0xff,World->VisibleObjects*2);

  // Next VUE frame
  World->FrameNum++;
  if (World->FrameNum > MaxVUEFrames) World->FrameNum = 0;

  waitvr();
  memcpy (_VesaScreen,_VesaBuffer,64000*2);
 }

 Set_Vesa(3);
 SetMode(3);
 Engine_UnInit(TRUE);
}






