#ifndef eng_math_h_included
#define eng_math_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#include "eng_def.h"
#include "eng_type.h"
#include "eng_vars.h"

/*
long float2int( float d );
long float2fixed( float d );
int CEIL(float f);
*/

//#define S 65536.0
//#define MAGIC (((S*S*16)+(S*.5))*S)
//#define MAGICfixed (MAGIC/65536.0)
//#define MAGICfixed12_4 (MAGIC/16.0)

// 컴컴컴컴컴

#pragma inline float2int;
static long float2int( float d )
{
 double dtemp = MAGIC+d;
 return (*(long *)&dtemp)-0x80000000;
}

// 컴컴컴컴컴

#pragma inline float2fixed;
static long float2fixed( float d )
{
 double dtemp = MAGICfixed+d;
 return (*(long *)&dtemp)-0x80000000;
}

// 컴컴컴컴컴

#pragma inline float2fixed12_4;
static long float2fixed12_4( float d )
{
 double dtemp = MAGICfixed12_4+d;
 return (*(long *)&dtemp)-0x80000000;
}

// 컴컴컴컴컴

#pragma inline CEIL;
static int CEIL(float f)
{
 int i,j;

 i = float2fixed(f);
 j = i >> 16;
 if ((i&0xffff) != 0) j+=1;
 return j;
}

// ----- Matrix -------------------------------------------------------------
void Matrix_Identity(float *m);
void Matrix_Translation(float *m, float tx, float ty, float tz);
void Matrix_Rotation(float *m, float xang, float yang, float zang);
void Matrix_Mult(float *a, float *b);
// Copies the contents of m2 into m1
void Matrix_Copy(float *m1, float *m2);

// ----- Vector -------------------------------------------------------------

// Normalize vector
void Vector_Normalize(sVector *dest, sVector *v);
// dot product of two vectors
float Vector_Dot(sVector v1, sVector v2);
// cross product of two vectors
void Vector_Cross(sVector *dest, sVector v1, sVector v2);
// add two vectors together
void Vector_Add(sVector *dest, sVector v1, sVector v2);
// subtract vector v2 from vector v1
void Vector_Sub(sVector *dest, sVector v1, sVector v2);
// translate vector by matrix
void Vector_Trans(sVector *dest, sVector v, float *m);
// translate vector by matrix (rotation only)
void Vector_Trans3x3(sVector *dest, sVector v, float *m);
// translate vector by inverse matrix (rotation only)
void Vector_inv_Trans3x3(sVector *dest, sVector v, float *m);
// return distance between two vectors/points
float Vector_Distance(sVector *v1, sVector *v2);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif

