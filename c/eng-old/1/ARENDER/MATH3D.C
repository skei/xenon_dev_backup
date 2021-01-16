

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

  Matrix_Translation(d,-s[3],-s[7],-s[11]);
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

