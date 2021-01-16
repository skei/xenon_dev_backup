#include <conio.h>    // getch

#include "defines.h"
#include "types.h"
#include "vars.h"

#include "engine.h"
#include "render.h"

// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

void main(void)
{
 sObject *O;
 sCamera C;

/* debug */ printf ("\nStart loading\n-------------------------------------------------------------------------------\n");
 O = Object_Load("torus.o");
/* debug */ printf ("\n-------------------------------------------------------------------------------\nfinished loading\n");

 if (O==NULL)
 {
  printf("Error loading TORUS.3DO!\n");
  exit(1);
 }
 printf("Object.Flags: %i\n",O->Flags);
 printf("Object.Pos: %f,%f,%f\n",O->Pos.x,O->Pos.y,O->Pos.z);
 printf("Object.Rot: %f,%f,%f\n",O->Rot.x,O->Rot.y,O->Rot.z);

 Engine_Init(0,1000,1,320,200,9,10,4000);
 O->Flags = OBJECT_Active;
 C.Pos.x = 0;
 C.Pos.y = 0;
 C.Pos.z = -500;
 C.Rot.x = 0;
 C.Rot.y = 0;
 C.Rot.z = 0;
 C.Flags = 0;

 World.Objects = O;
 World.Camera = &C;

 while (!kbhit())
 {
  C.Rot.y += 0.01;
  if (C.Rot.y > PI) C.Rot.y -= PI*2;
  Render();
 }
 Engine_UnInit();
 Object_Free(O);
}
