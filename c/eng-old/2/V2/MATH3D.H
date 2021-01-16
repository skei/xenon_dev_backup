#ifndef math3d_h_included
#define math3d_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

// (16.16 * 16.16) >> 16
// (16.16 *  8.8 ) >> 16
int fixedmul(int, int);
#pragma aux fixedmul=\
 "imul ebx",\
 "shrd eax,edx,16",\
 parm [eax] [ebx] value [eax] modify [edx];

// (16.16 << 16) / 16.16
// ( 8.8  << 16) / 16.16
int fixeddiv(int,int);
#pragma aux fixeddiv=\
 "cdq",\
 "shld edx,eax,16",\
 "idiv ebx",\
 parm [eax][ebx] value [eax] modify [edx];

void Init_SinCos(void);

void Matrix_Identity(float *m);
void Matrix_Translation(float *m, float tx, float ty, float tz);
void Matrix_Rotation(float *m, float xang, float yang, float zang);
void Matrix_Mult(float *a, float *b);
void Matrix_Copy(float *m1, float *m2);

sVector Vector_Normalize(sVector *v);
float Vector_Dot(sVector *v1, sVector *v2);
sVector Vector_Cross(sVector *v1, sVector *v2);
sVector Vector_Add(sVector *v1, sVector *v2);
sVector Vector_Sub(sVector *v1, sVector *v2);
sVector Vector_Trans(sVector *v, float *m);
sVector Vector_Trans3x3(sVector *v, float *m);

float Vector_Angle( sVector *v1, sVector *v2);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
