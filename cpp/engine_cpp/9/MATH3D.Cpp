//
// ÄÄÄÄÄ Vector ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

void  Vector_Print(sVector *v)
 {
  // printf ("x:%f y:%f z:%f\n", v->X, v->Y, v->Z);
 }

 // ÄÄÄ

void  Vector_Create(sVector *v, float x,float y,float z)
 {
  v->X = x;
  v->Y = y;
  v->Z = z;
 }

 // ÄÄÄ

void  Vector_Zero(sVector *v)
 {
  v->X = 0;
  v->Y = 0;
  v->Z = 0;
 }

 // ÄÄÄ

void  Vector_Copy(sVector *vd, sVector *vs)
 {
  vd->X = vs->X;
  vd->Y = vs->Y;
  vd->Z = vs->Z;
 }

 // ÄÄÄ

void  Vector_Add(sVector *vd, sVector *v1, sVector *v2)
 {
  vd->X = v1->X + v2->X;
  vd->Y = v1->Y + v2->Y;
  vd->Z = v1->Z + v2->Z;
 }

 // ÄÄÄ

void  Vector_Sub(sVector *vd, sVector *v1, sVector *v2)
 {
  vd->X = v1->X - v2->X;
  vd->Y = v1->Y - v2->Y;
  vd->Z = v1->Z - v2->Z;
 }

 // ÄÄÄ

void  Vector_Mul(sVector *vd, sVector *v1, sVector *v2)
 {
  vd->X = v1->X * v2->X;
  vd->Y = v1->Y * v2->Y;
  vd->Z = v1->Z * v2->Z;
 }

 // ÄÄÄ

void  Vector_Div(sVector *vd, sVector *v1, sVector *v2)
 {
  vd->X = v1->X / v2->X;
  vd->Y = v1->Y / v2->Y;
  vd->Z = v1->Z / v2->Z;
 }

void  Vector_Scale(sVector *vd, sVector *v, float scale)
 {
  vd->X = v->X * scale;
  vd->Y = v->Y * scale;
  vd->Z = v->Z * scale;
 }

 // ÄÄÄ

void  Vector_Negate(sVector *vd, sVector *vs)
 {
  vd->X = -vs->X;
  vd->Y = -vs->Y;
  vd->Z = -vs->Z;
 }

 // ÄÄÄ

int   Vector_Equal(sVector *v1, sVector *v2)
 {
  return (v1->X == v2->X && v1->Y == v2->Y && v1->Z == v2->Z);
 }

 // ÄÄÄ

float Vector_Length(sVector *v)
 {
  return (sqrt(v->X*v->X + v->Y*v->Y + v->Z*v->Z ));
 }

 // ÄÄÄ

float Vector_Distance(sVector *v1, sVector *v2)
 {
  static sVector temp;

  Vector_Sub(&temp,v1,v2);
  return Vector_Length(&temp);
 }

 // ÄÄÄ

float Vector_Dot(sVector *v1, sVector *v2)
 {
  return (v1->X*v2->X + v1->Y*v2->Y + v1->Z*v2->Z);
 }

 // ÄÄÄ

void  Vector_Cross(sVector *vd, sVector *v1, sVector *v2)
 {
  vd->X = v1->Y*v2->Z - v1->Z*v2->Y;
  vd->Y = v1->Z*v2->X - v1->X*v2->Z;
  vd->Z = v1->X*v2->Y - v1->Y*v2->X;
 }

 // ÄÄÄ

void  Vector_Normalize(sVector *vd, sVector *vs)
 {
  static sVector temp;
  static float len;

  len = Vector_Length(vs);
  if (len != 0)
  {
   vd->X = vs->X / len;
   vd->Y = vs->Y / len;
   vd->Z = vs->Z / len;
  }
  else
  {
   vd->X = 0;
   vd->Y = 0;
   vd->Z = 0;
  }
 }

 // ÄÄÄ

 // translate vector by matrix
void  Vector_Trans(sVector *dest, sVector v, float *m)
 {
  dest->X = v.X*m[ 0] + v.Y*m[ 1] + v.Z*m[ 2] + m[ 3];
  dest->Y = v.X*m[ 4] + v.Y*m[ 5] + v.Z*m[ 6] + m[ 7];
  dest->Z = v.X*m[ 8] + v.Y*m[ 9] + v.Z*m[10] + m[11];
 }

 // translate vector by matrix (rotation only)
void  Vector_Trans3x3(sVector *dest, sVector v, float *m)
 {
  dest->X = v.X*m[ 0] + v.Y*m[ 1] + v.Z*m[ 2];
  dest->Y = v.X*m[ 4] + v.Y*m[ 5] + v.Z*m[ 6];
  dest->Z = v.X*m[ 8] + v.Y*m[ 9] + v.Z*m[10];
 }

 // -----

 // translate vector by inverse matrix (rotation only)
void  Vector_Inv_Trans3x3(sVector *dest, sVector v, float *m)
 {
  dest->X = v.X*m[ 0] + v.Y*m[ 4] + v.Z*m[ 8];
  dest->Y = v.X*m[ 1] + v.Y*m[ 5] + v.Z*m[ 9];
  dest->Z = v.X*m[ 2] + v.Y*m[ 6] + v.Z*m[10];
 }


//
// ÄÄÄÄÄ Matrix ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

 //   1   0   0   0
 //   0   1   0   0
 //   0   0   1   0
 //   0   0   0   1
void Matrix_Identity(float *m)
 {
  m[ 0] = 1;  m[ 1] = 0;  m[ 2] = 0;  m[ 3] = 0;
  m[ 4] = 0;  m[ 5] = 1;  m[ 6] = 0;  m[ 7] = 0;
  m[ 8] = 0;  m[ 9] = 0;  m[10] = 1;  m[11] = 0;
  m[12] = 0;  m[13] = 0;  m[14] = 0;  m[15] = 1;
 }

 // ÄÄÄ

 //   1   0   0   tx
 //   0   1   0   ty
 //   0   0   1   tz
 //   0   0   0   1
void Matrix_Translation(float *m, float tx, float ty, float tz)
 {
  m[ 0] = 1;  m[ 1] = 0;  m[ 2] = 0;  m[ 3] = tx;
  m[ 4] = 0;  m[ 5] = 1;  m[ 6] = 0;  m[ 7] = ty;
  m[ 8] = 0;  m[ 9] = 0;  m[10] = 1;  m[11] = tz;
  m[12] = 0;  m[13] = 0;  m[14] = 0;  m[15] = 1;
 }

 // ÄÄÄ

void Matrix_RotationXYZ(float *m, float xang, float yang, float zang)
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

 // ÄÄÄ

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

 // ÄÄÄ

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

 // ÄÄÄ

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

 // ÄÄÄ

 // c[i][j] = ä 1ókóq ( a[i][k] * b[k][j] )
 // c[i][j] = sum of a[i][k] * b[k][j] for k varying from 1 to q

 // ABC   123   A1+B4+C7  A2+B5+C8  A3+B6+C9
 // DEF * 456 = D1+E4+F7  D2+E5+F8  D3+E6+C9
 // GHI   789   G1+H4+I7  G2+H5+I8  G3+H6+C9

void Matrix_Mul(float *a, float *b)
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

 // ÄÄÄ

 // Copies the contents of m2 into m1
void Matrix_Copy(float *m1, float *m2)
 {
  int i;

  for (i=0;i<16;i++)
  {
   m1[i] = m2[i];
  }
 }

 // ÄÄÄ

 // Creates inverse of matrix... stable???
void Matrix_Inverse( float *d, float *s )
 {
  float t[16];

  Matrix_Translation(d,-(s[3]),-(s[7]),-(s[11]));
  Matrix_Identity(t);
  t[ 0] = s[ 0];
  t[ 1] = s[ 4];
  t[ 2] = s[ 8];
  t[ 4] = s[ 1];
  t[ 5] = s[ 5];
  t[ 6] = s[ 9];
  t[ 8] = s[ 2];
  t[ 9] = s[ 6];
  t[10] = s[10];
  Matrix_Mul(d,t);
 }

 // ÄÄÄ

 // Creates inverse of matrix... returns 1 if success/posible, 0 otherwise
int  Matrix_Inverse2( float *matrix )
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
    return (0);
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
  return(1);
 }

 // ÄÄÄ

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
  VxxCos = (v->X * v->X) * OneMinCos;
  VyyCos = (v->Y * v->Y) * OneMinCos;
  VzzCos = (v->Z * v->Z) * OneMinCos;
  VxyCos = (v->X * v->Y) * OneMinCos;
  VxzCos = (v->X * v->Z) * OneMinCos;
  VyzCos = (v->Y * v->Z) * OneMinCos;
  VxSin = v->X * SIN;
  VySin = v->Y * SIN;
  VzSin = v->Z * SIN;
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

//
// ÄÄÄÄÄ Quaternion ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

void  Quat_Copy( float *out, float *a )
 {
  out[qW] = a[qW];
  out[qX] = a[qX];
  out[qY] = a[qY];
  out[qZ] = a[qZ];
 }

 // normalize a quaternion
void  Quat_Normalize( float *qd,float *q )
 {
  static float l,c;
  static float n;

  l =  sqrt( q[qX]*q[qX] + q[qY]*q[qY] + q[qZ]*q[qZ] + q[qW]*q[qW] );
  if (l==0)
  {
   qd[qW] = 1.0;
   qd[qX] = 0.0;
   qd[qY] = 0.0;
   qd[qZ] = 0.0;
  }
  else
  {
   c = 1.0/l;
   qd[qX] = q[qX]*c;
   qd[qY] = q[qY]*c;
   qd[qZ] = q[qZ]*c;
   qd[qW] = q[qW]*c;
  }
 }

 // ÄÄÄ

 // Note: Order is important! To combine two rotations, use
 // the product Mul(qSecond, qFirst), which gives the effect
 // of rotation by qFirst, then qSecond.

 // Compute quaternion product qL = qL * qR
void  Quat_Mul(float *qd,float *qL, float *qR)
 {
  qd[qW] = qL[qW]*qR[qW] - qL[qX]*qR[qX] - qL[qY]*qR[qY] - qL[qZ]*qR[qZ];
  qd[qX] = qL[qW]*qR[qX] + qL[qX]*qR[qW] + qL[qY]*qR[qZ] - qL[qZ]*qR[qY];
  qd[qY] = qL[qW]*qR[qY] + qL[qY]*qR[qW] + qL[qZ]*qR[qX] - qL[qX]*qR[qZ];
  qd[qZ] = qL[qW]*qR[qZ] + qL[qZ]*qR[qW] + qL[qX]*qR[qY] - qL[qY]*qR[qX];
 }

 // ÄÄÄ

 // Form multiplicative inverse of q
void  Quat_Inverse(float *qd,float *q)
 {
  static float l,norminv;

  l =  (q[qX]*q[qX] + q[qY]*q[qY] + q[qZ]*q[qZ] + q[qW]*q[qW]);
	if (l==0.0) l = 1.0;
	norminv = 1.0/l;

  qd[qX] = -q[qX] * norminv;
  qd[qY] = -q[qY] * norminv;
  qd[qZ] = -q[qZ] * norminv;
  qd[qW] =  q[qW] * norminv;
 }

 // ÄÄÄ

 // Convert quaternion to 3x3 rotation matrix
 // Quaternion need not be unit magnitude.  When it always is,
 // this routine can be simplified
void  Quat_ToMatrix(float *q, float *m)
 {
  static float s,xs,ys,zs,wx,wy,wz,xx,xy,xz,yy,yz,zz,den;

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

 // ÄÄÄ

 // Compute equivalent quaternion from [angle,axis] representation
void  Quat_FromAngAxis(float *q,float ang, sVector *axis)
 {
  static float omega,s;

	omega = ang*0.5;
	s = sin(omega);
  q[qX] = s*axis->X;
  q[qY] = s*axis->Y;
  q[qZ] = s*axis->Z;
  q[qW] = cos(omega);
 }

 // ÄÄÄ

 // Convert quaternion to equivalent [angle,axis] representation
void  Quat_ToAngAxis(float *q, float *ang, sVector *axis)
 {
  static int i;
  static float omega,s;
  static float qtemp[4];


  Quat_Normalize(qtemp,q);
  omega = acos(qtemp[3]);
	*ang = 2.0*omega;
	s = sin(omega);
  if (fabs(s)>.000001)
  axis->X  = qtemp[qX]/s;
  axis->Y  = qtemp[qY]/s;
  axis->Z  = qtemp[qZ]/s;
 }

 // ÄÄÄ

 // Compute [angle,axis] corresponding to the rotation from
 // p to q.  Function returns angle, sets axis.
float Quat_RelAngAxis(float *p, float *q, sVector *axis)
 {
  static float ang;
  static float q1[4],q2[4];

  Quat_Inverse(q1,p);
  Quat_Mul(q2,q1,q);
  Quat_ToAngAxis(q2,&ang,axis);
	return(ang);
 }

 // ÄÄÄ

 // computes quaternion magnitude.
float Quat_Length(float *a)
 {
  return sqrt (a[qW]*a[qW] + a[qX]*a[qX] + a[qY]*a[qY] + a[qZ]*a[qZ]);
 }

 // ÄÄÄ

 // computes dot product of a*b.
float Quat_Dot (float *a, float *b)
 {
  float len;

  len = 1.0 / (Quat_Length(a) * Quat_Length(b));
  return (a[qW]*b[qW] + a[qX]*b[qX] + a[qY]*b[qY] + a[qZ]*b[qZ]) * len;
 }

 // ÄÄÄ

 // computes dot product of a*b
float Quat_DotUnit (float *a, float *b)
 {
  return (a[qW]*b[qW] + a[qX]*b[qX] + a[qY]*b[qY] + a[qZ]*b[qZ]);
 }

 // ÄÄÄ

 // spherical interpolation of quaternions.
void  Quat_Slerp(float *out, float *a, float *b, float spin, float alpha)
 {
  float temp[4];
  float k1, k2;
  float angle, anglespin;
  float sina, cosa;
  int flip;

  cosa = Quat_DotUnit(a, b);
  if (cosa < 0.0) cosa = -cosa, flip = -1; else flip = 1;
  if ((1.0 - cosa) < EPSILON)
  {
   k1 = 1.0 - alpha;
   k2 = alpha;
  }
  else
  {
   angle = acos (cosa);
   sina = sin (angle);
   anglespin = angle + spin*PI;
   k1 = sin (angle - alpha*anglespin) / sina;
   k2 = sin (alpha*anglespin) / sina;
  }
  k2 *= flip;
  out[qX] = k1*a[qX] + k2*b[qX];
  out[qY] = k1*a[qY] + k2*b[qY];
  out[qZ] = k1*a[qZ] + k2*b[qZ];
  out[qW] = k1*a[qW] + k2*b[qW];
 }

 // ÄÄÄ

 // spherical interpolation of quaternions.
void  Quat_Slerpl (float *out, float *a, float *b, float spin, float alpha)
 {
  float  k1, k2;
  float  angle, anglespin;
  float  sina, cosa;
  int    flip;

  cosa = Quat_DotUnit(a, b);
  if (1.0 - fabs (cosa) < EPSILON)
  {
   k1 = 1.0 - alpha;
   k2 = alpha;
  }
  else
  {
   angle = acos (cosa);
   sina = sin (angle);
   anglespin = angle + spin*PI;
   k1 = sin (angle - alpha*anglespin) / sina;
   k2 = sin (alpha*anglespin) / sina;
  }
  out[qX] = k1*a[qX] + k2*b[qX];
  out[qY] = k1*a[qY] + k2*b[qY];
  out[qZ] = k1*a[qZ] + k2*b[qZ];
  out[qW] = k1*a[qW] + k2*b[qW];
 }

 // ÄÄÄ

 // negates quaternion.
void  Quat_Negate (float *out, float *a)
 {
  static float temp[4];

  Quat_Normalize(temp,a);
  out[qW]  = temp[qW];
  out[qX] = -temp[qX];
  out[qY] = -temp[qY];
  out[qZ] = -temp[qZ];
 }

 // ÄÄÄ

 // compute quaternion exponent.
void  Quat_Exp(float *out,float *a)
 {
  float  len, len1;

  len = sqrt ( a[qX]*a[qX] + a[qY]*a[qY] + a[qZ]*a[qZ] );
  if (len > 0.0) len1 = sin(len) / len; else len1 = 1.0;
  out[qW] = cos (len);
  out[qX] = a[qX] * len1;
  out[qY] = a[qY] * len1;
  out[qZ] = a[qZ] * len1;
 }

 // ÄÄÄ

 // compute quaternion logarithm.
void  Quat_Log(float *out,float *a)
 {
  float  len;

  len = sqrt ( a[qX]*a[qX] + a[qY]*a[qY] + a[qZ]*a[qZ] );
  if (a[qW] != 0.0) len = atan (len / a[qW]); else len = PI/2;
  out[qW] = 0.0;
  out[qX] = a[qX] * len;
  out[qY] = a[qY] * len;
  out[qZ] = a[qZ] * len;
 }

 // ÄÄÄ

 // calculate logarithm of relative rotation from a to b.
void  Quat_Lndif (float *out, float *a, float *b)
 {
  float inv[4], dif[4], temp[4];
  float len, len1, s;

  Quat_Inverse(inv,a);
  Quat_Mul(dif,inv,b);
  len = sqrt ( dif[qX]*dif[qX] + dif[qY]*dif[qY] + dif[qZ]*dif[qZ] );
  s = Quat_Dot(a,b);
  if (s != 0.0) len1 = atan (len / s); else len1 = PI/2;
  if (len != 0.0) len1 /= len;
  out[qW] = 0.0;
  out[qX] = dif[qX] * len1;
  out[qY] = dif[qY] * len1;
  out[qZ] = dif[qZ] * len1;
 }

