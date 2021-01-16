// vector.c

#include "vector.h"
#include <math.h>

#pragma warning (once: 4244)

// -----

vector Vector_Norm(vector v)
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

float Vector_Dot(vector v1, vector v2)
{
 float r;

 r = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;

 return r;
}

// -----

vector Vector_Cross(vector v1, vector v2)
{
 vector r;

 r.x = v1.y*v2.z - v1.z*v2.y;
 r.y = v1.z*v2.x - v1.x*v2.z;
 r.z = v1.x*v2.y - v1.y*v2.x;

 return r;
}

// -----

vector Vector_Add(vector v1, vector v2)
{
 vector r;

 r.x = v1.x + v2.x;
 r.y = v1.y + v2.y;
 r.z = v1.z + v2.z;

 return r;
}

// -----

vector Vector_Sub(vector v1, vector v2)
{
 vector r;

 r.x = v1.x - v2.x;
 r.y = v1.y - v2.y;
 r.z = v1.z - v2.z;

 return r;
}

// -----

vector Vector_Trans(vector v, float *m)
{
 vector r;
 
 r.x = v.x*m[0*4+0] + v.y*m[1*4+0] + v.z*m[2*4+0];
 r.y = v.x*m[0*4+1] + v.y*m[1*4+1] + v.z*m[2*4+1];
 r.z = v.x*m[0*4+2] + v.y*m[1*4+2] + v.z*m[2*4+2];

 return r;
}


