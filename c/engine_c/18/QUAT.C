#pragma warning (disable: 4035)	// no return value from __asm
#pragma warning (disable: 4244)	// conversion from 'double' to 'int'. Possible loss of data

// quat.c - Quaternions
#include "defines.h"

#define X 0
#define Y 1
#define Z 2
#define W 3

// -----

// Because of the following ASM block.... DON'T USE FASTCALL!!!
int fmul(int a, int b)
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

void Quat_multiply(int *q, int *q1, int *q2)
{
 
 q[0] = ( q1[W]*q2[W] - q1[X]*q2[X] - q1[Y]*q2[Y] - q1[Z]*q2[Z] ) >> DIVD; // w
 q[1] = ( q1[W]*q2[X] + q1[X]*q2[W] + q1[Y]*q2[Z] - q1[Z]*q2[Y] ) >> DIVD; // x
 q[2] = ( q1[W]*q2[Y] + q1[Y]*q2[W] + q1[Z]*q2[X] - q1[X]*q2[Z] ) >> DIVD; // y
 q[3] = ( q1[W]*q2[Z] + q1[Z]*q2[W] + q1[X]*q2[Y] - q1[Y]*q2[X] ) >> DIVD; // z
}


// -----

// quat_to_mat: Convert quaternion to 3x3 rotation matrix;
// Quaternion need not be unit magnitude.  When it always is,
// this routine can be simplified

void Quat_to_matrix(int *q, int *mat)
{
 int s,xs,ys,zs,wx,wy,wz,xx,xy,xz,yy,yz,zz,den;
// For unit q, just set s = 2.0; or or set xs = q[X] + q[X], etc
 den =  (q[X]*q[X] + q[Y]*q[Y] + q[Z]*q[Z] + q[W]*q[W])>>DIVD;
 if (den==0.0)
 {
	s = DIVD_NUM;
 }
 else s = DIVD_NUM/den;
 xs = q[X] * s;   ys = q[Y] * s;   zs = q[Z] * s;

 wx = q[W] * xs;  wy = q[W] * ys; wz = q[W] * zs;
 xx = q[X] * xs;  xy = q[X] * ys; xz = q[X] * zs;
 yy = q[Y] * ys;  yz = q[Y] * zs; zz = q[Z] * zs;

 mat[X][X] = DIVD_NUM - (yy +zz);
 mat[X][Y] = xy - wz;
 mat[X][Z] = xz + wy;

 mat[Y][X] = xy + wz;
 mat[Y][Y] = DIVD_NUM - (xx +zz);
 mat[Y][Z] = yz - wx;

 mat[Z][X] = xz - wy;
 mat[Z][Y] = yz + wx;
 mat[Z][Z] = DIVD_NUM - (xx + yy);
}

// -----

static int nxt[3] = {Y,Z,X};

// Convert 3x3 rotation matrix to unit quaternion
void Matrix_to_quat(float *mat, float *q)
{
 float tr,s;
 int i,j,k;
 tr = mat[X][X] + mat[Y][Y] + mat[Z][Z];
 if (tr > 0.0)
 {
  s = sqrt(tr + 1.0);
	q[W] = s * 0.5;
	s = 0.5 / s;
	q[X] = (mat[Z][Y] - mat[Y][Z]) * s;
	q[Y] = (mat[X][Z] - mat[Z][X]) * s;
	q[Z] = (mat[Y][X] - mat[X][Y]) * s;
 }
 else
 {
	i = X;
	if (mat[Y][Y] > mat[X][X]) i = Y;
	if (mat[Z][Z] > mat[i][i]) i = Z;
	j = nxt[i];
	k = nxt[j];
	s = sqrt( (mat[i][i] - (mat[j][j]+mat[k][k])) + 1.0 );
	q[i] = s * 0.5;
	if (s!=0.0)	s = 0.5 /s;
	q[W] = (mat[k][j] - mat[j][k]) * s;
	q[j] = (mat[j][i] + mat[i][j]) * s;
	q[k] = (mat[k][i] + mat[i][k]) * s;
 }
}

// -----

// Compute equivalent quaternion from [angle,axis] representation
void AngAxis_to_quat(float ang, float *axis, float *q)
{
 int i;
 float omega,s;
 omega = ang*0.5;
 s = sin(omega);
 for (i=0; i<3; i++) q[i] = s*axis[i];
 q[3] = cos(omega);
}

// -----

// Convert quaternion to equivalent [angle,axis] representation

void Quat_to_angaxis(float *q, float *ang, float *axis)
{
 int i;
 float omega,s,qn[4];
 Quat_norl(q,qn);
 omega = acos(qn[3]);
 *ang = 2.0*omega;
 s = sin(omega);
 if (fabs(s)>.000001) for (i=0; i<3; i++) axis[i]  = qn[i]/s;
}



// -----


void Quat_to_matrix(int *q, int *m)
{
 #define I DIVD_NUM
 
 int w,x,y,z;

// Ú                                            ¿
// ³ 1-2(yy+zz)     2(xy-wz)     2(xz+wy)     0 ³
// ³   2(xy+wz)   1-2(xx+zz)     2(yz-wx)     0 ³
// ³   2(xz-wy)     2(yz+wx)   1-2(xx+yy)     0 ³
// ³      0            0            0         1 ³
// À                                            Ù
 
 w = q[0]; x = q[1]; y = q[2]; z = q[3];

 m[0*4+0] = (I - (2*( fmul(y,y) + fmul(z,z) )));
 m[0*4+1] =      (2*( fmul(x,y) + fmul(w,z) ));
 m[0*4+2] =      (2*( fmul(x,z) + fmul(w,y) ));
 m[0*4+3] = 0;

 m[1*4+0] =      (2*( fmul(x,y) + fmul(w,z) ));
 m[1*4+1] = (I - (2*( fmul(x,x) + fmul(z,z) )));
 m[1*4+2] =      (2*( fmul(y,z) + fmul(w,x) ));
 m[1*4+3] = 0;

 m[2*4+0] =      (2*( fmul(x,z) + fmul(w,y) ));
 m[2*4+1] =      (2*( fmul(y,z) + fmul(w,x) ));
 m[2*4+2] = (I - (2*( fmul(x,x) + fmul(y,y) )));
 m[2*4+3] = 0;

 m[3*4+0] = 0;
 m[3*4+1] = 0;
 m[3*4+2] = 0;
 m[3*4+3] = I;
}

// -----


// -----

void Quat_add(int *q, int *q1, int *q2)
{
 q[0] = q1[0]+q2[0];
 q[1] = q1[1]+q2[1];
 q[2] = q1[2]+q2[2];
 q[3] = q1[3]+q2[3];
}

int Qt_Norm2(int *q)
{
 int qww = fmul( q[0],q[0] );
 int qxx = fmul( q[1],q[1] );
 int qyy = fmul( q[2],q[2] );
 int qzz = fmul( q[3],q[3] );
 return qxx + qyy + qzz + qww;
}

int Qt_Norm(int *q)
{
 int len = Qt_Norm2(q);
 len = sqrt(len >> DIVD) * DIVD_NUM;
 return len;
}

void Quat_normalize(int *qq, int *q)
{
 int len = Qt_Norm(q);
 int inv;
 
 if (len < DIVD_NUM)
 {
  q[0] = 1;
	q[1] = 0;
	q[2] = 0;
	q[3] = 0;
 }
 if (len != DIVD_NUM)
 {
  inv = DIVD_NUM / len;
  qq[0] = fmul(q[0],inv);
  qq[1] = fmul(q[1],inv);
  qq[2] = fmul(q[2],inv);
  qq[3] = fmul(q[3],inv);
 }
 else
 {
	qq[0] = q[0];
	qq[1] = q[1];
	qq[2] = q[2];
	qq[3] = q[3];
 }
}












