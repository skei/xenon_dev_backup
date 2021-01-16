#include <stdio.h>    // printf
#include <stdlib.h>   // exit

#include "defines.h"
#include "types.h"
#include "vars.h"

#include "engine.h"

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void main(void)
{
 sObject *O;

 O = Object_Load("torus.3do");
 if (O==NULL)
 {
  printf("Error loading TORUS.3DO!\n");
  exit(1);
 }
 else
 {
  printf("Bounding Sphere Radius: %f\n",O->BSphereRadius);
 }
 Object_Free(O);
}
