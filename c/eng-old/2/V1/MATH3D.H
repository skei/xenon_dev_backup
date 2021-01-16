#ifndef math_h_included
#define math_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#include "types.h"

void Init_SinCos(void);

void Matrix_Identity(float *m);
void Matrix_Translation(float *m, float tx, float ty, float tz);
void Matrix_Rotation(float *m, int xang, int yang, int zang);
void Matrix_Mult(float *a, float *b);
void Matrix_Copy(float *m1, float *m2);

sVector Vector_Normalize(sVector v);
float Vector_Dot(sVector v1, sVector v2);
sVector Vector_Cross(sVector v1, sVector v2);
sVector Vector_Add(sVector v1, sVector v2);
sVector Vector_Sub(sVector v1, sVector v2);
sVector Vector_Trans(sVector v, float *m);
sVector Vector_Trans3x3(sVector v, float *m);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
