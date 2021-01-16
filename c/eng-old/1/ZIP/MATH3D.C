#include <assert.h>
#include <math.h>

#include "defines.h"
#include "types.h"
#include "vars.h"


/*

#define S 65536.0
#define MAGIC (((S*S*16)+(S*.5))*S)
#define MAGICfixed (MAGIC/65536.0)

// ÄÄÄÄÄÄÄÄÄÄ

#pragma inline float2int;
static long float2int( float d )
{
 double dtemp = MAGIC+d;
 return (*(long *)&dtemp)-0x80000000;
}

// ÄÄÄÄÄÄÄÄÄÄ

#pragma inline float2fixed;
static long float2fixed( float d )
{
 double dtemp = MAGICfixed+d;
 return (*(long *)&dtemp)-0x80000000;
}

// ÄÄÄÄÄÄÄÄÄÄ

#pragma inline CEIL;
static int CEIL(float f)
{
 int i,j;

 i = float2fixed(f);
 j = i >> 16;
 if ((i&0xffff) != 0) j+=1;
 return j;
}

*/

// ----- Matrix -------------------------------------------------------------

//   1   0   0   0
//   0   1   0   0
//   0   0   1   0
//   0   0   0   1
void Matrix_Identity(float *m)
{
 m[ 0] = 1; m[ 1] = 0; m[ 2] = 0; m[ 3] = 0;
 m[ 4] = 0; m[ 5] = 1; m[ 6] = 0; m[ 7] = 0;
 m[ 8] = 0; m[ 9] = 0; m[10] = 1; m[11] = 0;
 m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

// -----

//   1   0   0   tx
//   0   1   0   ty
//   0   0   1   tz
//   0   0   0   1
void Matrix_Translation(float *m, float tx, float ty, float tz)
{
 m[ 0] = 1; m[ 1] = 0; m[ 2] = 0; m[ 3] = tx;
 m[ 4] = 0; m[ 5] = 1; m[ 6] = 0; m[ 7] = ty;
 m[ 8] = 0; m[ 9] = 0; m[10] = 1; m[11] = tz;
 m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

// -----

void Matrix_Rotation(float *m, float xang, float yang, float zang)
{
 float s1,s2,s3,c1,c2,c3;

 s1 = sin(xang);
 s2 = sin(yang);
 s3 = sin(zang);
 c1 = cos(xang);
 c2 = cos(yang);
 c3 = cos(zang);
// Ú                                                 ¿
// ³      c2*c3             c2*s3          s2      tx³
// ³ -s1*s2*c3-c1*s3   -s1*s2*s3+c1*c3    s1*c2    ty³
// ³ -c1*s2*c3+s1*s3   -c1*s2*s3-s1*c3    c1*c2    tz³
// ³        0                 0            0        1³
// À                                                 Ù
 m[ 0] = c2*c3;
 m[ 1] = c2*s3;
 m[ 2] = s2;
 m[ 3] = 0;

 m[ 4] = -s1*s2*c3 - c1*s3;
 m[ 5] = -s1*s2*s3 + c1*c3;
 m[ 6] = s1*c2;
 m[ 7] = 0;

 m[ 8] = -c1*s2*c3 + s1*s3;
 m[ 9] = -c1*s2*s3 - s1*c3;
 m[10] = c1*c2;
 m[11] = 0;

 m[12] = 0;
 m[13] = 0;
 m[14] = 0;
 m[15] = 1;
}


// c[i][j] = ä 1ókóq ( a[i][k] * b[k][j] )
// c[i][j] = sum of a[i][k] * b[k][j] for k varying from 1 to q

// ABC   123   A1+B4+C7  A2+B5+C8  A3+B6+C9
// DEF * 456 = D1+E4+F7  D2+E5+F8  D3+E6+C9
// GHI   789   G1+H4+I7  G2+H5+I8  G3+H6+C9

void Matrix_Mult(float *a, float *b)
{
 float ab[16];
 int i,j,k;

 for (i=0;i<4;i++)
 {
  for (j=0;j<4;j++)
  {
   ab[j*4+i] = 0.0;
   for (k=0;k<4;k++)
   {
    ab[j*4+i] += a[k*4+i] * b[j*4+k];
   }
  }
 }
 for (i=0;i<16;i++)
 {
  a[i] = ab[i];
 }

}

// -----

// Copies the contents of m2 into m1
void Matrix_Copy(float *m1, float *m2)
{
 int i;

 for (i=0;i<16;i++)
 {
  m1[i] = m2[i];
 }
}

// ----- Vector -------------------------------------------------------------

// Normalize vector
void Vector_Normalize(sVector *dest, sVector *v)
{
 float len;
 len = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
 if (len!=0)
 {
  dest->x = (v->x/len);
  dest->y = (v->y/len);
  dest->z = (v->z/len);
 }
 else
 {
  dest->x = 0; dest->y = 0; dest->z = 0;
 }
}

// -----

// dot product of two vectors
float Vector_Dot(sVector v1, sVector v2)
{
 return ( v1.x*v2.x + v1.y*v2.y + v1.z*v2.z );
}

// -----

// cross product of two vectors
void Vector_Cross(sVector *dest, sVector v1, sVector v2)
{
 dest->x = v1.y*v2.z - v1.z*v2.y;
 dest->y = v1.z*v2.x - v1.x*v2.z;
 dest->z = v1.x*v2.y - v1.y*v2.x;
}

// -----

// add two vectors together
void Vector_Add(sVector *dest, sVector v1, sVector v2)
{
 dest->x = v1.x + v2.x;
 dest->y = v1.y + v2.y;
 dest->z = v1.z + v2.z;
}

// -----

// subtract vector v2 from vector v1
void Vector_Sub(sVector *dest, sVector v1, sVector v2)
{
 dest->x = v1.x - v2.x;
 dest->y = v1.y - v2.y;
 dest->z = v1.z - v2.z;
}

// -----

// translate vector by matrix
void Vector_Trans(sVector *dest, sVector v, float *m)
{
 dest->x = v.x*m[ 0] + v.y*m[ 1] + v.z*m[ 2] + m[ 3];
 dest->y = v.x*m[ 4] + v.y*m[ 5] + v.z*m[ 6] + m[ 7];
 dest->z = v.x*m[ 8] + v.y*m[ 9] + v.z*m[10] + m[11];
}

// translate vector by matrix (rotation only)
void Vector_Trans3x3(sVector *dest, sVector v, float *m)
{
 dest->x = v.x*m[ 0] + v.y*m[ 1] + v.z*m[ 2];
 dest->y = v.x*m[ 4] + v.y*m[ 5] + v.z*m[ 6];
 dest->z = v.x*m[ 8] + v.y*m[ 9] + v.z*m[10];
}

// -----

// translate vector by inverse matrix (rotation only)
void Vector_inv_Trans3x3(sVector *dest, sVector v, float *m)
{
 dest->x = v.x*m[ 0] + v.y*m[ 4] + v.z*m[ 8];
 dest->y = v.x*m[ 1] + v.y*m[ 5] + v.z*m[ 9];
 dest->z = v.x*m[ 2] + v.y*m[ 6] + v.z*m[10];
}


