// math3d.c

#include "math3d.h"
#include <math.h>

#pragma warning (once: 4244)

// -----

vector Normalize(vector v)
{
 float len;
 vector r;

 len = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
 if (len!=0)
 {
  r.x = v.x/len;
	r.y = v.y/len;
	r.z = v.z/len;
 }
 else
 {
	r.x = 0; r.y = 0; r.z = 0;
 }

 return r;
}

// -----

float DotProduct(vector v1, vector v2)
{
 float r;

 r = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;

 return r;
}

// -----

vector CrossProduct(vector v1, vector v2)
{
 vector r;

 r.x = v1.y*v2.z - v1.z*v2.y;
 r.y = v1.z*v2.x - v1.x*v2.z;
 r.z = v1.x*v2.y - v1.y*v2.x;

 return r;
}