// math.c
// all math functions

// --------------------------------------------------------------------------

#include <math.h>
#include "defines.h"
#include "math3d.h"

#ifdef VisCVersion
 #pragma warning (once:4035)
 #pragma warning (once:4244)
#endif

// Sine/Cosine tables
int sine[TABLESIZE];
int cosine[TABLESIZE];

// -----
// Inline assembler fixed point multiplication routines
// Both Watcom and Visual C versions
// -----

//::::::::::::::::
#ifdef WatcomVersion
//::::::::::::::::

 // fixed point multiplication...
 int fixedmul2(int, int);
 #pragma aux fixedmul2=\
  "imul ebx",\
  "shrd eax,edx,14",\
  parm [eax] [ebx] value [eax] modify [edx];
 
 // -----

 int fixedmul3(int, int, int);
 #pragma aux fixedmul3=\
  "imul ebx",\
  "shrd eax,edx,14",\
  "imul ecx",\
  "shrd eax,edx,14",\
  parm [eax] [ebx] [ecx] value [eax] modify [edx];

//:::::::::::::::
#endif
#ifdef VisCVersion
//:::::::::::::::

 int fixedmul2(int a, int b)
 {
	__asm
  {
   mov eax,a 
   mov ebx,b
   imul ebx
   shrd eax,edx,14
  }
 }

// -----

 int fixedmul3(int a, int b, int c)
 {
  __asm
  {
   mov eax,a 
   mov ebx,b
	 mov ecx,c
   imul ebx
   shrd eax,edx,14
	 imul ecx
	 shrd eax,edx,14
  }
 }

//::::
#endif
//::::

// ----------------------------------------------
 
// Calculate sin/cos tables.
void calc_sincos(void)
{
 int i;
 float realangle = -PI;

 for (i = 0; i < TABLESIZE; i++)
 {
  sine[i] =  DIVD_NUM * sin(realangle);
  cosine[i] = DIVD_NUM * cos(realangle);
  realangle = realangle + ((PI*2)/TABLESIZE);
 }
}

// --------------------------------------------------------------------------
// : Matrix routines ::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// --------------------------------------------------------------------------

//   1   0   0   0
//   0   1   0   0
//   0   0   1   0
//   0   0   0   1
void Matrix_Identity(int *m)
{
 #define I DIVD_NUM
 m[0*4+0] = I; m[0*4+1] = 0; m[0*4+2] = 0; m[0*4+3] = 0;
 m[1*4+0] = 0; m[1*4+1] = I; m[1*4+2] = 0; m[1*4+3] = 0;
 m[2*4+0] = 0; m[2*4+1] = 0; m[2*4+2] = I; m[2*4+3] = 0;
 m[3*4+0] = 0; m[3*4+1] = 0; m[3*4+2] = 0; m[3*4+3] = I;
}

// -----

//   1   0   0   0
//   0   1   0   0
//   0   0   1   0
//  tx  ty  tz   1
void Matrix_Translation(int *m, int tx, int ty, int tz)
{
 #define I DIVD_NUM
 tx<<=DIVD;
 ty<<=DIVD;
 tz<<=DIVD;
 m[0*4+0] = I; m[0*4+1] = 0; m[0*4+2] = 0; m[0*4+3] = 0;
 m[1*4+0] = 0; m[1*4+1] = I; m[1*4+2] = 0; m[1*4+3] = 0;
 m[2*4+0] = 0; m[2*4+1] = 0; m[2*4+2] = I; m[2*4+3] = 0;
 m[3*4+0] =tx; m[3*4+1] =ty; m[3*4+2] =tz; m[3*4+3] = I;
}

// -----

void Matrix_Rotation(int *m,int xang, int yang, int zang)
{
 #define I DIVD_NUM
 int s1,s2,s3,c1,c2,c3;

 xang &=4095;
 yang &=4095;
 zang &=4095;

 s1 = sine[xang];
 s2 = sine[yang];
 s3 = sine[zang];
 c1 = cosine[xang];
 c2 = cosine[yang];
 c3 = cosine[zang];
// Ú                                                ¿
// ³      c2*c3             c2*s3          s2      0³
// ³ -s1*s2*c3-c1*s3   -s1*s2*s3+c1*c3    s1*c2    0³
// ³ -c1*s2*c3+s1*s3   -c1*s2*s3-s1*c3    c1*c2    0³
// ³        tx                ty           tz      1³
// À                                                Ù
 m[0*4+0] = fixedmul2(c2,c3);
 m[0*4+1] = fixedmul2(c2,s3);
 m[0*4+2] = s2;
 m[0*4+3] = 0;

 m[1*4+0] = fixedmul3(-s1,s2,c3) - fixedmul2(c1,s3);
 m[1*4+1] = fixedmul3(-s1,s2,s3) + fixedmul2(c1,c3);
 m[1*4+2] = fixedmul2(s1,c2);
 m[1*4+3] = 0;

 m[2*4+0] = fixedmul3(-c1,s2,c3) + fixedmul2(s1,s3);
 m[2*4+1] = fixedmul3(-c1,s2,s3) - fixedmul2(s1,c3);
 m[2*4+2] = fixedmul2(c1,c2);
 m[2*4+3] = 0;

 m[3*4+0] = 0;
 m[3*4+1] = 0;
 m[3*4+2] = 0;
 m[3*4+3] = I;
}


// c[i][j] = ä 1ókóq ( a[i][k] * b[k][j] )
// c[i][j] = sum of a[i][k] * b[k][j] for k varying from 1 to q

// ABC   123   A1+B4+C7  A2+B5+C8  A3+B6+C9
// DEF * 456 = D1+E4+F7  D2+E5+F8  D3+E6+C9
// GHI   789   G1+H4+I7  G2+H5+I8  G3+H6+C9

void Matrix_Mult(int *a, int *b)
{
 int ab[16];
 int i,j,k;

 for (i=0;i<4;i++)
 {
  for (j=0;j<4;j++)
  {
   ab[i*4+j] = 0;
   for (k=0;k<4;k++)
   {
    ab[i*4+j] += fixedmul2( a[i*4+k],b[k*4+j] );
   }
  }
 }
 for (i=0;i<16;i++)
 {
  a[i] = ab[i];
 }

}

// -----

// X = x*m[0][0] + y*m[1][0] + z*m[2][0] + m[3][0]
// Y = x*m[0][1] + y*m[1][1] + z*m[2][1] + m[3][1]
// Z = x*m[0][2] + y*m[1][2] + z*m[2][2] + m[3][2]

void Matrix_Process(int *m, int *src, int *dst, int num)
{
 int i;
 int x,y,z;
 int *from = src;
 int *to = dst;

 for (i=0;i<num;i++)
 {
  x = *from++;
  y = *from++;
  z = *from++;
  *to++ = ( x*m[0*4+0] + y*m[1*4+0] + z*m[2*4+0] + m[3*4+0] ) >> DIVD;
  *to++ = ( x*m[0*4+1] + y*m[1*4+1] + z*m[2*4+1] + m[3*4+1] ) >> DIVD;
  *to++ = ( x*m[0*4+2] + y*m[1*4+2] + z*m[2*4+2] + m[3*4+2] ) >> DIVD;
 }
}

// -----

// Copies the contents of m2 into m1
void Matrix_Copy(int *m1, int *m2)
{
 int i;

 for (i=0;i<16;i++)
 {
  m1[i] = m2[i];
 }
}

// --------------------------------------------------------------------------
// : Vector routines ::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// --------------------------------------------------------------------------

// Normalize vector
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

// dot product of two vectors
int Vector_Dot(vector v1, vector v2)
{
 int r;

 r = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;

 return r;
}

// -----

// cross product of two vectors
vector Vector_Cross(vector v1, vector v2)
{
 vector r;

 r.x = v1.y*v2.z - v1.z*v2.y;
 r.y = v1.z*v2.x - v1.x*v2.z;
 r.z = v1.x*v2.y - v1.y*v2.x;

 return r;
}

// -----

// add two vectors together
vector Vector_Add(vector v1, vector v2)
{
 vector r;

 r.x = v1.x + v2.x;
 r.y = v1.y + v2.y;
 r.z = v1.z + v2.z;

 return r;
}

// -----

// subtract vector v2 from vector v1
vector Vector_Sub(vector v1, vector v2)
{
 vector r;

 r.x = v1.x - v2.x;
 r.y = v1.y - v2.y;
 r.z = v1.z - v2.z;

 return r;
}

// -----

// translate vector by (rotation) matrix
vector Vector_Trans(vector v, int *m)
{
 vector r;

 r.x = ( v.x*m[0*4+0] + v.y*m[1*4+0] + v.z*m[2*4+0] ) >> DIVD;
 r.y = ( v.x*m[0*4+1] + v.y*m[1*4+1] + v.z*m[2*4+1] ) >> DIVD;
 r.z = ( v.x*m[0*4+2] + v.y*m[1*4+2] + v.z*m[2*4+2] ) >> DIVD;

 return r;
}



