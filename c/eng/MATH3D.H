#ifndef math3d_h_included
#define math3d_h_included

#include "types.h"

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

// fixed point multiplication routines
int fixedmul2(int, int);
int fixedmul3(int, int, int);
// Calculate sin/cos tables.
void calc_sincos(void);

// --- Matrix ---------------------------------------------------------------

void Matrix_Identity(int *m);
void Matrix_Translation(int *m, int tx, int ty, int tz);
void Matrix_Rotation(int *m,int xang, int yang, int zang);
// a = a*b
void Matrix_Mult(int *a, int *b);
// m -> matrix
// src -> un-processed XYZ's
// dst -> saved XYZ
// num = number of XYS's
void Matrix_Process(int *m, int *src, int *dst, int num);
// Copies the contents of m2 into m1
void Matrix_Copy(int *m1, int *m2);

// --- Vector ---------------------------------------------------------------

// Normalize vector
sVector Vector_Norm(sVector v);
// dot product of two vectors
int Vector_Dot(sVector v1, sVector v2);
// cross product of two vectors
sVector Vector_Cross(sVector v1, sVector v2);
// add two vectors together
sVector Vector_Add(sVector v1, sVector v2);
// subtract vector v2 from vector v1
sVector Vector_Sub(sVector v1, sVector v2);
// translate vector by (rotation) matrix
sVector Vector_Trans(sVector v, int *m);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
