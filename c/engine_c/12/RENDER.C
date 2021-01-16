#include "engine.h"

// -----

void insert_object(sObject *o)
{
 int i;
 int n;

 n = world.renderpos;
 if (o->active==1)
 {
  if (object_visible(o))
  {
   for (i=0;i<o->numpolys;i++)
   {
    insert_poly(..
    n++;
   }
  }
 }
}


