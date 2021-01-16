// render.c

#include "types.h"
#include "vars.h"

// -----

int object_visible(sObject *o)
{
 return 1;
}

void insert_object(sObject *o)
{
 int i;
 int n;
 int c1,c2,c3;

 if (o=0) return;             // return if null pointer
 if (o->status == 0) return;  // return if object is (temporary) in-active
// calc_bounding_box
 if (object_visible(o)==1)           // check if bounding box visible
 {
// project_object
  n = world.totalpolys;
  for (i=0;i<o->numpolys;i++)
  {
   c1 = o->polys[i*3];     // Coordinate set #1
   c2 = o->polys[i*3+1];   // Coordinate set #2
   c3 = o->polys[i*3+2];   // Coordinate set #3
// If any of the Z-coordinates are zero or less, skip the entire poly
   if (   ( z[c1] != -1 )
       && ( z[c2] != -1 )
       && ( z[c3] != -1 ))
   {
    x1 = t[c1*2];
    y1 = t[c1*2+1];
    x2 = t[c2*2];
    y2 = t[c2*2+1];
    x3 = t[c3*2];
    y3 = t[c3*2+1];
    if ( (x2-x1)*(y3-y1)-(x3-x1)*(y2-y1) < 0 )
    {
     renderlist[n].x1 = x1;
     renderlist[n].y1 = y1;
     renderlist[n].x2 = x2;
     renderlist[n].y2 = y2;
     renderlist[n].x3 = x3;
     renderlist[n].y3 = y3;
// prepare U/V
     renderlist[n].texture = o->texture;
// calculate light & hazing (C)
     sorttable[n] = ((unsigned short int)n<<16) + (short int)( z[c1]+z[c2]+z[c3] );
     n++;
    }
   }
  }
  world.totalpolys = n;
 }
}

// -----

void render_scene(void)
{
 sObject *o;

// do_camera(world.cam);
// do_light(world.light);
 world.totalpolys = 0;
 insert_object(world.objects);
}

// -----

