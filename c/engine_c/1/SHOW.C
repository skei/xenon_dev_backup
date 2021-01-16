 #include <math.h>
 #include "engine.h"
 #include "math3d.h"

 #define MaxPolys 10000

//
// --- variables ---
//

// char   *Buffer;
sScene *Scene;
sCamera *C;
sNode *N;

//
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

void mode(short int mode);
 #pragma aux mode = \
  "int 10h" \
 parm [ax];

 // ---


//
// ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void main(int argc, char **argv)
 {
  // check if any arguments...
  if (argc != 3)
  {
   printf("usage:   SHOW <3DS-file> <camera-name>\n");
   exit(1);
  }
  // Initialize & Load scene
  Scene = Scene_New(MaxPolys);
  if (Scene_Load3DS(Scene,argv[1]) != Error3DS_NoError)
  {
   printf("Error loading %s\n",argv[1]);
   exit(1);
  }
  // setup other Engine related stuff
  Scene->Buffer = (char *)malloc(64000);
  // Do some stuff with the camera

  // Find camera?
  N = Scene_FindNodeByName(Scene,argv[2]);
  if (N==NULL)
  {
   printf("Camera %s not found!\n",argv[2]);
   exit(1);
  }
  if (N->Type == NODEtype_camera) C = (sCamera *)N->Data;
  else
  {
   printf("%s not a camera!\n",argv[2]);
   exit(1);
  }

  Scene->ActiveCamera = C;

  Scene->ActiveCamera->AspectRatio = 1;
  Scene->ActiveCamera->SizeX = 320;
  Scene->ActiveCamera->SizeY = 200;

  printf("ş Ready for rendering... Press any key...\n");
  getch();
  mode(0x13);
  // render loop
  while (!kbhit())
  {
   // ÄÄÄÄÄÄÄÄÄÄ engine running ÄÄÄÄÄÄÄÄÄÄ
   memset(Scene->Buffer,0,64000);
   Scene_Update(Scene);
   memcpy((char *)0xA0000,Scene->Buffer,64000);
   Scene->Frame += 0.5;
   // printf("Frame: %f\n",Scene->Frame);
   // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  }
  mode(3);
  printf("Frame: %f\n",Scene->Frame);
 }

//
// ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//


