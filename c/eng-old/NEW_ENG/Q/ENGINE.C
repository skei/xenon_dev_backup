 #include <math.h>
 #include <stdlib.h>    // getmem
 #include "engine.h"

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

//
// ±±±±± Global variables :-/ ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

char *_Buffer;    // Background Buffer to draw to
char *_Screen;    // _the_ screen

int MinX,MaxX,MinY,MaxY;      // Clipping rectangle
int ScreenWidth,ScreenHeight;  // ViewPort size

//
// ±±±±± Misc / Help / Support ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void *GetMem( unsigned int Size )
{
 void *n;

 n = malloc(Size);
 memset(n,0,Size);
 return n;
}

 // -----

sVector *NewVector(int num)
 { return (sVector *)GetMem(num*sizeof(sVector)); }

sFloatKey *NewFloatKey(int num)
 { return (sFloatKey *)GetMem(num*sizeof(sFloatKey)); }

sVectorKey *NewVectorKey(int num)
 { return (sVectorKey *)GetMem(num*sizeof(sVectorKey)); }

sKeyFrame *NewKeyFrame(int num)
 { return (sKeyFrame *)GetMem(num*sizeof(sKeyFrame)); }

sMaterial *NewMaterial(int num)
 { return (sMaterial *)GetMem(num*sizeof(sMaterial)); }

sVertex *NewVertex(int num)
 { return (sVertex *)GetMem(num*sizeof(sVertex)); }

sPolygon *NewPolygon(int num)
 { return (sPolygon *)GetMem(num*sizeof(sPolygon)); }

sObject *NewObject(int num)
 { return (sObject *)GetMem(num*sizeof(sObject)); }

sLight *NewLight(int num)
 { return (sLight *)GetMem(num*sizeof(sLight)); }

sCamera *NewCamera(int num)
 { return (sCamera *)GetMem(num*sizeof(sCamera)); }

sWorld *NewWorld(int num)
 { return (sWorld *)GetMem(num*sizeof(sWorld)); }

int *NewInt(int num)
 { return (int *)GetMem(num*sizeof(int)); }

//
// ±±±±± Math ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

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

 // -----

void Matrix_RotationX(float *m, float xang)
 {
  float s1,s2,s3,c1,c2,c3;

  s1 = sin(xang);
  c1 = cos(xang);
 // Ú                 ¿
 // ³  1   0   0   0  ³
 // ³  0  c1  s1   0  ³
 // ³  0 -s1  c1   0  ³
 // ³  0   0   0   1  ³
 // À                 Ù
  Matrix_Identity(m);
  m[ 5] = c1;
  m[ 6] = s1;
  m[ 9] = -s1;
  m[10] = c1;
 }

 // -----

void Matrix_RotationY(float *m, float yang)
 {
  float s1,s2,s3,c1,c2,c3;

  s2 = sin(yang);
  c2 = cos(yang);
 // Ú                 ¿
 // ³ c2   0  s2   0  ³
 // ³  0   1   0   0  ³
 // ³-s2   0  c2   0  ³
 // ³  0   0   0   1  ³
 // À                 Ù
  Matrix_Identity(m);
  m[ 0] = c2;
  m[ 2] = s2;
  m[ 8] = -s2;
  m[10] = c2;
 }

 // -----

void Matrix_RotationZ(float *m, float zang)
 {
  float s1,s2,s3,c1,c2,c3;

  s3 = sin(zang);
  c3 = cos(zang);
 // Ú                 ¿
 // ³ c3  s3   0   0  ³
 // ³-s3  c3   0   0  ³
 // ³  0   0   1   0  ³
 // ³  0   0   0   1  ³
 // À                 Ù
  Matrix_Identity(m);
  m[ 0] = c3;
  m[ 1] = s3;
  m[ 4] = -s3;
  m[ 5] = c3;
 }

 // -----



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

 // -----

 // Creates inverse of matrix... stable???
void Matrix_Inverse( float *d, float *s )
 {
  float t[16];

  Matrix_Translation(d,-(s[3]),-(s[7]),-(s[11]));
  Matrix_Identity(&t);
  t[ 0] = s[ 0];
  t[ 1] = s[ 4];
  t[ 2] = s[ 8];
  t[ 4] = s[ 1];
  t[ 5] = s[ 5];
  t[ 6] = s[ 9];
  t[ 8] = s[ 2];
  t[ 9] = s[ 6];
  t[10] = s[10];
  Matrix_Mult(d,&t);
 }

 // -----

 // Creates inverse of matrix...
bool Matrix_Inverse_2( float *matrix )
 {
  int i,j,k;

  float pvt_i[4];
  float pvt_j[4];

  float pvt_val;
  float hold;
  float determ;

  determ = 1.0;
  for(k=0; k<4; k++)
  {
   pvt_val = matrix[k+4*k];                       // ???
   pvt_i[k] = k;
   pvt_j[k] = k;
   for(i=k; i<4; i++)
   {
    for(j=0; j<4; j++)
    {
     if(fabs(matrix[i+4*j]) > fabs(pvt_val))      // ???
     {
      pvt_i[k] = i;
      pvt_j[k] = j;
      pvt_val = matrix[i+4*j];                    // ???
     }
    }
   }
   // Check for singularity
   determ *= pvt_val;
   if(determ == 0.0)
   {
    // Matrix is singular
    return (FALSE);
   }
   /* "Interchange" rows */
   i = pvt_i[k];
   if(i!=k)
   {
    for(j=0; j<4; j++)
    {
     hold = -matrix[k+4*j];                                   // ???
     matrix[k+4*j] = matrix[i+4*j];                           // ???
     matrix[i+4*j] = hold;                                    // ???
    }
   }
   /* "Interchange" columns */
   j = pvt_j[k];
   if(j!=k)
   {
    for(i=0; i<4; i++)
    {
     hold = -matrix[i+4*k];                                   // ???
     matrix[i+4*k] = matrix[i+4*j];                           // ???
     matrix[i+4*j] = hold;                                    // ???
    }
   }
   /* Divide column by minus pivot value */
   for(i=0; i<4; i++)
   {
    if(i!=k) matrix[i+4*k] /= -pvt_val;                       // ???
   }
   /* Reduce the matrix */
   for(i=0; i<4; i++)
   {
    hold = matrix[i+4*k];                                     // ???
    for(j=0; j<4; j++)
    {
     if(i!=k && j!=k) matrix[i+4*j] += hold * matrix[k+4*j];  // ???
    }
   }
   /* Divide row by pivot */
   for(j=0; j<4; j++)
   {
    if(j!=k) matrix[k+4*j] /= pvt_val;                        // ???
   }
   /* Replace pivot by reciprocal */
   matrix[k+4*k] = 1.0/pvt_val;                               // ???
  }
  /* Final pass of row/column interchange */
  for(k=2; k>=0; k--)
  {
   i = pvt_j[k];
   if(i!=k)
   {
    for(j=0; j<4; j++)
    {
     hold = matrix[k*4+j];                                    // ???
     matrix[k+4*j] = -matrix[i+4*j];                          // ???
     matrix[i+4*j] = hold;                                    // ???
    }
   }
   j = pvt_i[k];
   if(j!=k)
   {
    for(i=0; i<4; i++)
    {
     hold = matrix[i+4*k];                                    // ???
     matrix[i+4*k] = -matrix[i+4*j];                          // ???
     matrix[i+4*j] = hold;                                    // ???
    }
   }
  }
  return(TRUE);
 }

 // -----

 // Create rotation matrix. angle/axis
void Matrix_AngleAxis ( float *m, float angle, sVector *v )
 {
  float COS,SIN;
  float VxxCos,VyyCos, VzzCos, VxyCos, VxzCos, VyzCos;
  float OneMinCos;
  float VxSin, VySin, VzSin;

  // Get the sine & cosine of the rotation value
  COS = cos(angle);
  SIN = sin(angle);
  OneMinCos = 1-COS;
  // Do the fixed multiplies ahead of time since some are reused.
  // There's definitely room for optimization here when any of the components
  // of V are 0
  VxxCos = (v->x * v->x) * OneMinCos;
  VyyCos = (v->y * v->y) * OneMinCos;
  VzzCos = (v->z * v->z) * OneMinCos;
  VxyCos = (v->x * v->y) * OneMinCos;
  VxzCos = (v->x * v->z) * OneMinCos;
  VyzCos = (v->y * v->z) * OneMinCos;
  VxSin = v->x * SIN;
  VySin = v->y * SIN;
  VzSin = v->z * SIN;
  // Now stuff the matrix

  m[ 0] = COS + VxxCos;
  m[ 1] = VxyCos + VzSin;
  m[ 2] = VxzCos - VySin;
  m[ 3] = 0;

  m[ 4] = VxyCos - VzSin;
  m[ 5] = COS + VyyCos;
  m[ 6] = VyzCos + VxSin;
  m[ 7] = 0;

  m[ 8] = VxzCos + VySin;
  m[ 9] = VyzCos - VxSin;
  m[10] = COS + VzzCos;
  m[11] = 0;

  m[12] = 0;
  m[13] = 0;
  m[14] = 0;
  m[15] = 1;
 }

// ----- Vector -------------------------------------------------------------

 // Normalize vector
void Vector_Normalize(sVector *dest, sVector *v)
 {
  float len,invlen;
  len = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
  if (len!=0)
  {
   invlen = 1/len;
   dest->x = (v->x*invlen);
   dest->y = (v->y*invlen);
   dest->z = (v->z*invlen);
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

 // -----

 // return distance between two vectors/points
float Vector_Distance(sVector *v1, sVector *v2)
 {
  float xd,yd,zd;
  float dist;

  xd = (v2->x - v1->x);
  yd = (v2->y - v1->y);
  zd = (v2->z - v1->z);

  dist = xd*xd + yd*yd + zd*zd;
  if (dist!=0) return sqrt(dist); else return 0;
 }

 // -----

 // return length of vector
float Vector_Length(sVector *v1)
 {
  float leng;

  leng = (v1->x*v1->x) + (v1->y*v1->y) + (v1->z*v1->z);

  if (leng!=0.0) return sqrt(leng); else return 0;
 }

// ----- Quaternion ---------------------------------------------------------

 // normalize a quaternion
void Quat_Normalize( float *q )
 {
  float l,c;
  float n;

  l =  sqrt( q[qX]*q[qX] + q[qY]*q[qY] + q[qZ]*q[qZ] + q[qW]*q[qW] );
  if (l==0)
  {
   q[qW] = 1.0;
   q[qX] = 0.0;
   q[qY] = 0.0;
   q[qZ] = 0.0;
  }
  else
  {
   c = 1.0/l;
   q[qX] *= c;
   q[qY] *= c;
   q[qZ] *= c;
   q[qW] *= c;
  }
 }

 // -------------------------------------------------------------------------

 // Note: Order is important! To combine two rotations, use
 // the product Mul(qSecond, qFirst), which gives the effect
 // of rotation by qFirst, then qSecond.

 // Compute quaternion product qL = qL * qR
void Quat_Mult(float *qL, float *qR)
 {
	float qp[4];
  qp[qW] = qL[qW]*qR[qW] - qL[qX]*qR[qX] - qL[qY]*qR[qY] - qL[qZ]*qR[qZ];
  qp[qX] = qL[qW]*qR[qX] + qL[qX]*qR[qW] + qL[qY]*qR[qZ] - qL[qZ]*qR[qY];
  qp[qY] = qL[qW]*qR[qY] + qL[qY]*qR[qW] + qL[qZ]*qR[qX] - qL[qX]*qR[qZ];
  qp[qZ] = qL[qW]*qR[qZ] + qL[qZ]*qR[qW] + qL[qX]*qR[qY] - qL[qY]*qR[qX];

  qL[qW] = qp[qW];
  qL[qX] = qp[qX];
  qL[qY] = qp[qY];
  qL[qZ] = qp[qZ];
 }

 // -------------------------------------------------------------------------

 // Form multiplicative inverse of q
void Quat_Inverse(float *q)
 {
  float l,norminv;
  l =  (q[qX]*q[qX] + q[qY]*q[qY] + q[qZ]*q[qZ] + q[qW]*q[qW]);
	if (l==0.0) l = 1.0;
	norminv = 1.0/l;

  q[qX] = -q[qX] * norminv;
  q[qY] = -q[qY] * norminv;
  q[qZ] = -q[qZ] * norminv;
  q[qW] =  q[qW] * norminv;
 }

 // -------------------------------------------------------------------------

 // Convert quaternion to 3x3 rotation matrix
 // Quaternion need not be unit magnitude.  When it always is,
 // this routine can be simplified
void Quat_ToMatrix(float *q, float *m)
 {
	float s,xs,ys,zs,wx,wy,wz,xx,xy,xz,yy,yz,zz,den;

  // For unit q, just set s = 2.0; or or set xs = q[X] + q[X], etc
  den =  (q[qX]*q[qX] + q[qY]*q[qY] + q[qZ]*q[qZ] + q[qW]*q[qW]);
  if (den==0.0) { s = 1.0; }
	else s = 2.0/den;
  xs = q[qX] * s;   ys = q[qY] * s;  zs = q[qZ] * s;
  wx = q[qW] * xs;  wy = q[qW] * ys; wz = q[qW] * zs;
  xx = q[qX] * xs;  xy = q[qX] * ys; xz = q[qX] * zs;
  yy = q[qY] * ys;  yz = q[qY] * zs; zz = q[qZ] * zs;

  m[ 0] = 1.0 - (yy +zz);
  m[ 1] = xy - wz;
  m[ 2] = xz + wy;

  m[ 4] = xy + wz;
  m[ 5] = 1.0 - (xx +zz);
  m[ 6] = yz - wx;

  m[ 8] = xz - wy;
  m[ 9] = yz + wx;
  m[10] = 1.0 - (xx + yy);
 }

 // -------------------------------------------------------------------------

 // Compute equivalent quaternion from [angle,axis] representation
void Quat_FromAngAxis(float ang, sVector *axis, float *q)
 {
  float omega,s;
	omega = ang*0.5;
	s = sin(omega);
  q[qX] = s*axis->x;
  q[qY] = s*axis->y;
  q[qZ] = s*axis->z;
  q[qW] = cos(omega);
 }

 // -------------------------------------------------------------------------

 // Convert quaternion to equivalent [angle,axis] representation
void Quat_ToAngAxis(float *q, float *ang, sVector *axis)
 {
	int i;
  float omega,s;

  Quat_Normalize(q);
  omega = acos(q[3]);
	*ang = 2.0*omega;
	s = sin(omega);
  if (fabs(s)>.000001)
  axis->x  = q[qX]/s;
  axis->y  = q[qY]/s;
  axis->z  = q[qZ]/s;
 }

 // -------------------------------------------------------------------------


 // Compute [angle,axis] corresponding to the rotation from
 // p to q.  Function returns angle, sets axis.
float Quat_RelAngAxis(float *p, float *q, sVector *axis)
 {
  float ang;

  Quat_Inverse(p);
  Quat_Mult(p,q);
  Quat_ToAngAxis(p,&ang,axis);
	return(ang);
 }

//
// ±±±±± Spline ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void Spline_Init( sKeyFrame *K )
 {
  int i;
  int a,b,c,d;

  if (K->NumKeys > 1)
  {
   for (i=0;i<K->NumKeys;i++)
   {
    a = i-1;
    b = i;
    c = i+1;
    d = i+2;
    if (a<0) a += K->NumKeys;
    if (c>=K->NumKeys) c -= K->NumKeys;
    if (d>=K->NumKeys) d -= K->NumKeys;

    if (K->KEY_float==1)
    {
     K->FloatKeys[i].Ap =    -(K->FloatKeys[a].Value)
                          + 3*(K->FloatKeys[b].Value)
                          - 3*(K->FloatKeys[c].Value)
                          +   (K->FloatKeys[d].Value);
     K->FloatKeys[i].Bp =   2*(K->FloatKeys[a].Value)
                          - 5*(K->FloatKeys[b].Value)
                          + 4*(K->FloatKeys[c].Value)
                          -   (K->FloatKeys[d].Value);
     K->FloatKeys[i].Cp = -   (K->FloatKeys[a].Value)
                          +   (K->FloatKeys[c].Value);
     K->FloatKeys[i].Dp =   2*(K->FloatKeys[b].Value);
    }

    if (K->KEY_vector==1)
    {
     K->VectorKeys[i].Ap.x =    -(K->VectorKeys[a].Pos.x)
                             + 3*(K->VectorKeys[b].Pos.x)
                             - 3*(K->VectorKeys[c].Pos.x)
                             +   (K->VectorKeys[d].Pos.x);
     K->VectorKeys[i].Bp.x =   2*(K->VectorKeys[a].Pos.x)
                             - 5*(K->VectorKeys[b].Pos.x)
                             + 4*(K->VectorKeys[c].Pos.x)
                             -   (K->VectorKeys[d].Pos.x);
     K->VectorKeys[i].Cp.x =  -  (K->VectorKeys[a].Pos.x)
                             +   (K->VectorKeys[c].Pos.x);
     K->VectorKeys[i].Dp.x =   2*(K->VectorKeys[b].Pos.x);

     K->VectorKeys[i].Ap.y =    -(K->VectorKeys[a].Pos.y)
                             + 3*(K->VectorKeys[b].Pos.y)
                             - 3*(K->VectorKeys[c].Pos.y)
                             +   (K->VectorKeys[d].Pos.y);
     K->VectorKeys[i].Bp.y =   2*(K->VectorKeys[a].Pos.y)
                             - 5*(K->VectorKeys[b].Pos.y)
                             + 4*(K->VectorKeys[c].Pos.y)
                             -   (K->VectorKeys[d].Pos.y);
     K->VectorKeys[i].Cp.y =  -  (K->VectorKeys[a].Pos.y)
                             +   (K->VectorKeys[c].Pos.y);
     K->VectorKeys[i].Dp.y =   2*(K->VectorKeys[b].Pos.y);

     K->VectorKeys[i].Ap.z =    -(K->VectorKeys[a].Pos.z)
                             + 3*(K->VectorKeys[b].Pos.z)
                             - 3*(K->VectorKeys[c].Pos.z)
                             +   (K->VectorKeys[d].Pos.z);
     K->VectorKeys[i].Bp.z =   2*(K->VectorKeys[a].Pos.z)
                             - 5*(K->VectorKeys[b].Pos.z)
                             + 4*(K->VectorKeys[c].Pos.z)
                             -   (K->VectorKeys[d].Pos.z);
     K->VectorKeys[i].Cp.z =  -  (K->VectorKeys[a].Pos.z)
                             +   (K->VectorKeys[c].Pos.z);
     K->VectorKeys[i].Dp.z =   2*(K->VectorKeys[b].Pos.z);
    }
   }
  }
 }

 // -------------------------------------------------------------------------

void Spline_Update( sKeyFrame *K )
 {
  int next;

  if (K->NumKeys > 1)
  {
   K->CurrentPos += K->Speed;
   next = K->CurrentKey+1;
   if (next >= K->NumKeys) next -= K->NumKeys;
   if (K->CurrentPos > (float)K->FrameNumber[next])
   {
    K->CurrentKey++;
   }
   // Wrap if reached the end...
   if (K->CurrentKey >= K->NumKeys)
   {
    K->CurrentKey = 0;
    K->CurrentPos = 0;
   }
  }
 }

 // -------------------------------------------------------------------------

float Spline_GetValue( sKeyFrame *K, sVector *Vector)
 {
  int x1,x2;
  float T,T2,T3;
  float Value;

  if (K->NumKeys == 1)
  {
   Value = K->FloatKeys[0].Value;
   Vector->x = K->VectorKeys[0].Pos.x;
   Vector->y = K->VectorKeys[0].Pos.y;
   Vector->z = K->VectorKeys[0].Pos.z;
  }
  else
  {
   Value = 0;
   x1 = K->CurrentKey;
   x2 = K->CurrentKey+1;
   if (x2 >= K->NumKeys) x2-= K->NumKeys;
   T = (K->CurrentPos - (float)K->FrameNumber[x1])
       / (float)(K->FrameNumber[x2] - K->FrameNumber[x1]);
   T2 = T * T;       // Square of t
   T3 = T2 * T;      // Cube of t
   if (K->KEY_float==1)
   {
    Value    = (  (K->FloatKeys[K->CurrentKey].Ap*T3)
                + (K->FloatKeys[K->CurrentKey].Bp*T2)
                + (K->FloatKeys[K->CurrentKey].Cp*T )
                +  K->FloatKeys[K->CurrentKey].Dp   )/2; // Calc x value /D
   }

   if (K->KEY_vector==1)
   {
    Vector->x = (  (K->VectorKeys[K->CurrentKey].Ap.x*T3)
                 + (K->VectorKeys[K->CurrentKey].Bp.x*T2)
                 + (K->VectorKeys[K->CurrentKey].Cp.x*T )
                 +  K->VectorKeys[K->CurrentKey].Dp.x   )/2; // Calc x value /D
    Vector->y = (  (K->VectorKeys[K->CurrentKey].Ap.y*T3)
                 + (K->VectorKeys[K->CurrentKey].Bp.y*T2)
                 + (K->VectorKeys[K->CurrentKey].Cp.y*T )
                 +  K->VectorKeys[K->CurrentKey].Dp.y   )/2; // Calc x value /D
    Vector->z = (  (K->VectorKeys[K->CurrentKey].Ap.z*T3)
                 + (K->VectorKeys[K->CurrentKey].Bp.z*T2)
                 + (K->VectorKeys[K->CurrentKey].Cp.z*T )
                 +  K->VectorKeys[K->CurrentKey].Dp.z   )/2; // Calc x value /D
   }
  }
  return Value;
 }

//
// ±±±±± Init ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void Engine_Init(void)
 {
 }

 // -------------------------------------------------------------------------

void Engine_Exit(void)
 {
 }

//
// ±±±±± Object ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void Object_SetPos( sObject *o, float x, float y, float z)
 {
  o->Pos.x = x;
  o->Pos.y = y;
  o->Pos.z = z;
 }

 // -------------------------------------------------------------------------


void Object_AddPos( sObject *o, float x, float y, float z)
 {
  o->Pos.x += x;
  o->Pos.y += y;
  o->Pos.z += z;
 }

 // -------------------------------------------------------------------------

void Object_SetRot( sObject *o, float x, float y, float z)
 {
  o->Rot.x = x;
  o->Rot.y = y;
  o->Rot.z = z;
 }

 // -------------------------------------------------------------------------

void Object_AddRot( sObject *o, float x, float y, float z)
 {
  o->Rot.x += x;
  o->Rot.y += y;
  o->Rot.z += z;
 }

 // -------------------------------------------------------------------------

void Object_CalcFaceNormals( sObject *O )
 {
  sVertex *a,*b,*c;
  float ax,ay,az;
  float bx,by,bz;
  float kx,ky,kz;
  float leng;
  int i;

  for (i=0;i<O->NumPolygons;i++)
  {
   a = O->Polygons[i].v[0];
   b = O->Polygons[i].v[1];
   c = O->Polygons[i].v[2];

   ax = b->Pos.x - a->Pos.x;
   ay = b->Pos.y - a->Pos.y;
   az = b->Pos.z - a->Pos.z;

   bx = c->Pos.x - a->Pos.x;
   by = c->Pos.y - a->Pos.y;
   bz = c->Pos.z - a->Pos.z;

   kx = (ay*bz)-(az*by);
   ky = (az*bx)-(ax*bz);
   kz = (ax*by)-(ay*bx);

   leng = (sqrt( kx*kx + ky*ky + kz*kz));
   if (leng!=0)
   {
    O->Polygons[i].Normal.x = (kx/leng);
    O->Polygons[i].Normal.y = (ky/leng);
    O->Polygons[i].Normal.z = (kz/leng);
   }
   else
   {
    O->Polygons[i].Normal.x = 0;
    O->Polygons[i].Normal.y = 0;
    O->Polygons[i].Normal.z = 0;
   }
  }
 }

 // -------------------------------------------------------------------------

void Object_CalcVertexNormals( sObject *O )
 {
  int i,a,num;
  sVertex *vi;
  float ax,ay,az;
  float lengd;

  for (i=0;i<O->NumVertices;i++)
  {
   vi = &O->Vertices[i];
   ax = 0;
   ay = 0;
   az = 0;
   num = 0;
   for (a=0;a<O->NumPolygons;a++)
   {
    if (    (O->Polygons[a].v[0] == vi)
         || (O->Polygons[a].v[1] == vi)
         || (O->Polygons[a].v[2] == vi) )
    {
     ax += O->Polygons[a].Normal.x;
     ay += O->Polygons[a].Normal.y;
     az += O->Polygons[a].Normal.z;
     num++;
    }
   }
   if ( num != 0 )
   {
    ax = ax / num;
    ay = ay / num;
    az = az / num;
   }
   lengd = sqrt( ax*ax + ay*ay + az*az);
   if ( lengd != 0)
   {
    O->Vertices[i].Normal.x = (ax/lengd);
    O->Vertices[i].Normal.y = (ay/lengd);
    O->Vertices[i].Normal.z = (az/lengd);
   }
   else
   {
    O->Vertices[i].Normal.x = 0;
    O->Vertices[i].Normal.y = 0;
    O->Vertices[i].Normal.z = 0;
   }
  }
 }

 // -------------------------------------------------------------------------

 // Center object around its average X/Y/Z pos
void Object_Center( sObject *O )
 {
  float xmax,xmin,ymax,ymin,zmax,zmin;
  float xmove, ymove, zmove;
  int i;

  xmax = -32767; xmin = 32767;
  ymax = -32767; ymin = 32767;
  zmax = -32767; zmin = 32767;
  for (i=0;i<O->NumVertices;i++)
  {
   if ( O->Vertices[i].Pos.x > xmax ) xmax = O->Vertices[i].Pos.x;
   if ( O->Vertices[i].Pos.x < xmin ) xmin = O->Vertices[i].Pos.x;
   if ( O->Vertices[i].Pos.y > ymax ) ymax = O->Vertices[i].Pos.y;
   if ( O->Vertices[i].Pos.y < ymin ) ymin = O->Vertices[i].Pos.y;
   if ( O->Vertices[i].Pos.z > zmax ) zmax = O->Vertices[i].Pos.z;
   if ( O->Vertices[i].Pos.z < zmin ) zmin = O->Vertices[i].Pos.z;
  }
 // Sphere = xmax;
 // if (ymax > Sphere) Sphere = ymax;
 // if (zmax > Sphere) Sphere = zmax;
 // if (fabs(xmin) > Sphere) Sphere = fabs(xmin);
 // if (fabs(ymin) > Sphere) Sphere = fabs(ymin);
 // if (fabs(zmin) > Sphere) Sphere = fabs(zmin);

  xmove = xmin+((xmax-xmin) /2 );
  ymove = ymin+((ymax-ymin) /2 );
  zmove = zmin+((zmax-zmin) /2 );
  for (i=0;i<O->NumVertices;i++)
  {
   O->Vertices[i].Pos.x-=xmove;
   O->Vertices[i].Pos.y-=ymove;
   O->Vertices[i].Pos.z-=zmove;
  }
  O->Pos.x += xmove;
  O->Pos.y += ymove;
  O->Pos.z += zmove;
 }

 // -------------------------------------------------------------------------

void Object_CalcBoundingSphere( sObject *O )
 {
  int i;
  float sphere,leng;

  sphere = 0;
  for (i=0;i<O->NumVertices;i++)
  {
   leng = Vector_Length(&O->Vertices[i].Pos);
   if ( leng > sphere) sphere = leng;
  }
  O->SphereRadius = sphere;
 }

 // -------------------------------------------------------------------------

sObject *Object_FindByName( sWorld *World, char *name )
 {
  sObject *sO;
  int found;

  sO = World->Objects;
  for (;;)
  {
   if (sO==NULL)
   {
    printf("harra-baillj! Didn't find object \"%s\" in ObjectList\n",name);
    exit(1);
   }
   if (strncmp ( sO->Name , name , 20 ) == 0) goto exitloop;
   sO=sO->Next;
  }
 exitloop:
  return sO;
 }

//
// ±±±±± Camera ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void Camera_SetPos( sCamera *c, float x, float y, float z)
 {
  c->Pos.x = x;
  c->Pos.y = y;
  c->Pos.z = z;
 }

void Camera_SetRot( sCamera *c, float x, float y, float z)
 {
  c->Rot.x = x;
  c->Rot.y = y;
  c->Rot.z = z;
 }

void Camera_SetTarget( sCamera *c, float x, float y, float z)
 {
  c->Target.x = x;
  c->Target.y = y;
  c->Target.z = z;
 }

void Camera_AddTarget( sCamera *c, float x, float y, float z)
 {
  c->Target.x += x;
  c->Target.y += y;
  c->Target.z += z;
 }

void Camera_AddRot( sCamera *c, float x, float y, float z)
 {
  c->Rot.x += x;
  c->Rot.y += y;
  c->Rot.z += z;
 }

void Camera_AddPos( sCamera *c, float x, float y, float z)
 {
  c->Pos.x += x;
  c->Pos.y += y;
  c->Pos.z += z;
 }

 // -------------------------------------------------------------------------

sCamera *Camera_FindByName( sWorld *World, char *name )
 {
  sCamera *sC;
  int found;

  sC = World->Cameras;
  for (;;)
  {
   if (sC==NULL)
   {
    printf("harra-baillj! Didn't find camera \"%s\" in CameraList\n",name);
    exit(1);
   }
   if (strncmp ( sC->Name , name , 20 ) == 0) goto exitloop;
   sC=sC->Next;
  }
 exitloop:
  return sC;
 }


//
// ±±±±± Material ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

sMaterial *Material_FindByName( sWorld *World, char *name )
 {
  sMaterial *sM;
  int found;

  sM = World->Materials;
  for (;;)
  {
   if (sM==NULL)
   {
    printf("harra-baillj! Didn't find material \"%s\" in MaterialList\n",name);
    exit(1);
   }
   if (strncmp ( sM->Name , name , 20 ) == 0) goto exitloop;
   sM=sM->Next;
  }
 exitloop:
  return sM;
 }

//
// ±±±±± Yucku GFX ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void YuckuLine(int x0, int y0, int x1, int y1, char color, char *_B)
 {
  int dx,ix,cx,dy,iy,cy,m,i,pos;

  if (x0<x1) { dx=x1-x0; ix=1; } else { dx=x0-x1; ix=-1; }
  if (y0<y1) { dy=y1-y0; iy=1; } else { dy=y0-y1; iy=-1; }
  if (dx>dy) m=dx; else m=dy;
  cx=cy=(m>>1);
  for (i=0;i<m;i++)
  {
   if ((x0>=MinX)&&(x0<=MaxX)&&(y0>=MinY)&&(y0<=MaxY))
   {
    _B[y0*ScreenWidth+x0] = color;
   }
   if ((cx+=dx)>=m) { cx-=m; x0+=ix; }
   if ((cy+=dy)>=m) { cy-=m; y0+=iy; }
  }
 }

 // -------------------------------------------------------------------------

void YuckuPolyWireFrame( sPolygon *p )
 {
  sVertex *v[3];

  v[0] = p->v[0];
  v[1] = p->v[1];
  v[2] = p->v[2];

  YuckuLine( (int)(v[0]->PosR.x),
             (int)(v[0]->PosR.y),
             (int)(v[1]->PosR.x),
             (int)(v[1]->PosR.y),15,_Buffer);
  YuckuLine( (int)(v[0]->PosR.x),
             (int)(v[0]->PosR.y),
             (int)(v[2]->PosR.x),
             (int)(v[2]->PosR.y),15,_Buffer);
  YuckuLine( (int)(v[1]->PosR.x),
             (int)(v[1]->PosR.y),
             (int)(v[2]->PosR.x),
             (int)(v[2]->PosR.y),15,_Buffer);
 }

