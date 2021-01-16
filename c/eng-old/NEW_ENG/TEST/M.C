#include "engine.h"

float m[16],m2[16];
sVector Test,T2;

void PrintMatrix( float *m )
{
 printf("\n%5.2f  %5.2f  %5.2f  %5.2f\n",m[ 0],m[ 1],m[ 2],m[ 3]);
 printf("%5.2f  %5.2f  %5.2f  %5.2f\n",m[ 4],m[ 5],m[ 6],m[ 7]);
 printf("%5.2f  %5.2f  %5.2f  %5.2f\n",m[ 8],m[ 9],m[10],m[11]);
 printf("%5.2f  %5.2f  %5.2f  %5.2f\n",m[12],m[13],m[14],m[15]);
}

void main(void)
{
 Test.x = 10;
 Test.y = 20;
 Test.z = 30;
 Matrix_Rotation(&m,1,2,3);
 PrintMatrix(&m);
 Matrix_Rotation(&m2,1.1,1.2,1.3);
 Matrix_Mult(&m,&m2);

 Matrix_Translation(&m2,100,200,300);
 Matrix_Mult(&m,&m2);

 Vector_Trans(&T2,Test,&m);
 printf("Vector: %f, %f, %f\n",T2.x,T2.y,T2.z);

 Matrix_Inverse_2(&m);
 PrintMatrix(&m);

 Vector_Trans(&Test,T2,&m);
 printf("Vector: %f, %f, %f\n",Test.x,Test.y,Test.z);

}
