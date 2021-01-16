#include <math.h>     // sqrt
#include "engine.h"


// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
// ฑ Vector routines ฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑ
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ

// Normalize vector
sVector Vector_Norm(sVector v)
{
 float len;
 sVector r;
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

// dot product of two vectors
int Vector_Dot(sVector v1, sVector v2)
{
 int r;
 r = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
 return r;
}

// -----

// cross product of two vectors
sVector Vector_Cross(sVector v1, sVector v2)
{
 sVector r;
 r.x = v1.y*v2.z - v1.z*v2.y;
 r.y = v1.z*v2.x - v1.x*v2.z;
 r.z = v1.x*v2.y - v1.y*v2.x;
 return r;
}

// -----

// add two vectors together
sVector Vector_Add(sVector v1, sVector v2)
{
 sVector r;
 r.x = v1.x + v2.x;
 r.y = v1.y + v2.y;
 r.z = v1.z + v2.z;
 return r;
}

// -----

// subtract vector v2 from vector v1
sVector Vector_Sub(sVector v1, sVector v2)
{
 sVector r;
 r.x = v1.x - v2.x;
 r.y = v1.y - v2.y;
 r.z = v1.z - v2.z;
 return r;
}

// -----

// translate vector by (rotation) matrix
sVector Vector_Trans(sVector v, int *m)
{
 sVector r;
 r.x = ( v.x*m[0*4+0] + v.y*m[1*4+0] + v.z*m[2*4+0] ) >> DIVD;
 r.y = ( v.x*m[0*4+1] + v.y*m[1*4+1] + v.z*m[2*4+1] ) >> DIVD;
 r.z = ( v.x*m[0*4+2] + v.y*m[1*4+2] + v.z*m[2*4+2] ) >> DIVD;
 return r;
}

