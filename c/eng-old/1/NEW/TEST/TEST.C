#include <assert.h>
#include <stdio.h>    // file stuf
#include <string.h>   // memcpy
#include <time.h>     // clock

#include "engine.h"

float m1[16],m2[16],m3[16];
sVector v1,v2;

// --------------------------------------------------------------------------

void PrintMatrix( float *m)
{
 int i;

 for (i=0;i<4;i++)
 {
  printf("%5.2f : %5.2f : %5.2f : %5.2f\n",m[i*4+0],m[i*4+1],m[i*4+2],m[i*4+3]);
 }
 printf("\n");
}

void PrintVector( sVector *v)
{
 printf("%5.2f : %5.2f : %5.2f\n",v->x,v->y,v->z);
 printf("\n");
}

// --------------------------------------------------------------------------

void main(void)
{
 printf("\n");

 v1.x = 110;
 v1.y = 120;
 v1.z = 130;
 PrintVector(&v1);

 Matrix_Rotation(&m1,1,2,3);              // m1 = R
 Matrix_Translation(&m2,100,200,300);     // m2 = T
 Matrix_Mult(&m1,&m2);                    // m1 = R*T

 Matrix_Rotation(&m2,0,1,0.5);            // m2 = R
 Matrix_Translation(&m3,1000,2000,3000);  // m3 = T
 Matrix_Mult(&m2,&m3);                    // m2 = R*T

 Matrix_Mult(&m1,&m2);                    // m1 = R1*T1 * R2*T2

// -----

 Vector_Trans(&v2,v1,&m1);
 PrintVector(&v2);

// -----

// Create inverse matrix

// -----

 Matrix_Translation(&m3,-m1[3],-m1[7],-m1[11]);   // m3 = -T
 Matrix_Identity(&m2);
 m2[ 0] = m1[ 0];                                 // m2 = -R
 m2[ 1] = m1[ 4];
 m2[ 2] = m1[ 8];
 m2[ 4] = m1[ 1];
 m2[ 5] = m1[ 5];
 m2[ 6] = m1[ 9];
 m2[ 8] = m1[ 2];
 m2[ 9] = m1[ 6];
 m2[10] = m1[10];
 Matrix_Mult(&m3,&m2);                            // m3 = -T*-R

// -----

 Vector_Trans(&v1,v2,&m3);
 PrintVector(&v1);

}

