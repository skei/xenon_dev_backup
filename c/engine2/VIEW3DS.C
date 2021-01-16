 #include "engine.h"

void main(int argc, char *argv[])
{
 sScene *Scene;

 Scene = Scene_New(1);
 Load3DS(argv[1],Scene);
}

