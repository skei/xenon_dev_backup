#include "engine.h"

sScene *Scene;

void main(void)
{
 printf("Creating scene\n");
 Scene = Scene_New(10000);
 printf("Loading 3DS\n");
 Load3DS("test.3ds",Scene);
}
