
//
// 컴컴� Vector 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
//

 // create vector (out = [x,y,z]).
void vec_make (float x, float y, float z, sVector *out)
 {
  out->x = x;
  out->y = y;
  out->z = z;
 }

 // zero vector (out = [0,0,0]).
void vec_zero (sVector *out)
 {
  out->x = 0.0;
  out->y = 0.0;
  out->z = 0.0;
 }

 // vector copy (out = a).
void vec_copy (sVector *a, sVector *out)
 {
  out->x = a->x;
  out->y = a->y;
  out->z = a->z;
 }

 // print vector on stdout.
void vec_print (sVector *a)
 {
  printf ("x:%f y:%f z:%f\n", a->x, a->y, a->z);
 }

 // vector addition (out = a+b).
void vec_add (sVector *a, sVector *b, sVector *out)
 {
  sVector temp;

  temp.x = a->x + b->x;
  temp.y = a->y + b->y;
  temp.z = a->z + b->z;
  vec_copy (&temp, out);
 }

 // vector substraction (out = a-b).
void vec_sub (sVector *a, sVector *b, sVector *out)
 {
  sVector temp;

  temp.x = a->x - b->x;
  temp.y = a->y - b->y;
  temp.z = a->z - b->z;
  vec_copy (&temp, out);
 }

 // vector multiplication (out = a*b).
void vec_mul (sVector *a, sVector *b, sVector *out)
 {
  sVector temp;

  temp.x = a->x * b->x;
  temp.y = a->y * b->y;
  temp.z = a->z * b->z;
  vec_copy (&temp, out);
 }

 // vector division (out = a / b).
void vec_div (sVector *a, sVector *b, sVector *out)
 {
  sVector temp;

  temp.x = a->x / b->x;
  temp.y = a->y / b->y;
  temp.z = a->z / b->z;
  vec_copy (&temp, out);
 }

 // vector multiplication by a scalar.
void vec_scale (sVector *a, float s, sVector *out)
 {
  float    len;
  sVector temp;

  temp.x = a->x * s;
  temp.y = a->y * s;
  temp.z = a->z * s;
  vec_copy (&temp, out);
 }

 // vector negation (out = -a).
void vec_negate (sVector *a, sVector *out)
 {
  sVector temp;

  temp.x = -a->x;
  temp.y = -a->y;
  temp.z = -a->z;
  vec_copy (&temp, out);
 }

 // vector compare.
int vec_equal (sVector *a, sVector *b)
 {
  return (a->x == b->x && a->y == b->y && a->z == b->z);
 }

 // computes vector magnitude.
float vec_length (sVector *a)
 {
  return (sqrt (a->x*a->x + a->y*a->y + a->z*a->z));
 }

 // computes distance between two vectors.
float vec_distance (sVector *a, sVector *b)
 {
  sVector v;

  vec_sub (a, b, &v);
  return (vec_length (&v));
 }

 // computes dot product of two vectors.
float vec_dot (sVector *a, sVector *b)
 {
  return (a->x * b->x + a->y * b->y + a->z * b->z);
 }

 // computes cross product of two vectors.
void vec_cross (sVector *a, sVector *b, sVector *out)
 {
  sVector temp;

  temp.x = a->y*b->z - a->z*b->y;
  temp.y = a->z*b->x - a->x*b->z;
  temp.z = a->x*b->y - a->y*b->x;
  vec_copy (&temp, out);
 }

 // linear interpolation of two vectors.
void vec_lerp (sVector *a, sVector *b, float alpha, sVector *out)
 {
  sVector temp;

 #define _LERP(a,l,h) ((l)+(((h)-(l))*(a)))
  temp.x = _LERP (alpha, a->x, b->x);
  temp.y = _LERP (alpha, a->y, b->y);
  temp.z = _LERP (alpha, a->z, b->z);
 #undef _LERP
  vec_copy (&temp, out);
 }

 // linear combination of two vectors.
void vec_combine (sVector *a, sVector *b, float as, float bs, sVector *out)
 {
  sVector temp;

  temp.x = (as * a->x) + (bs * b->x);
  temp.y = (as * a->y) + (bs * b->y);
  temp.z = (as * a->z) + (bs * b->z);
  vec_copy (&temp, out);
 }

 // vector normalization.
void vec_normalize (sVector *a, sVector *out)
 {
  float    len;
  sVector temp;

  len = vec_length (a);
  if (len == 0.0)
  {
   out->x = 0.0;
   out->y = 0.0;
   out->z = 0.0;
   return;
  }
  temp.x = a->x / len;
  temp.y = a->y / len;
  temp.z = a->z / len;
  vec_copy (&temp, out);
 }

//
// 컴컴� Matrix 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
//

 sMatrix Tidentity =
 {
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
 };

 // make identity matrix.
void mat_identity (sMatrix out)
 {
  int i, j;

  for (i = 0; i < 3; i++)
  {
   for (j = 0; j < 4; j++)
   {
    out[i][j] = Tidentity[i][j];
   }
  }
 }

 // clear matrix.
void mat_zero (sMatrix out)
 {
  int i, j;

  for (i = 0; i < 3; i++)
  {
   for (j = 0; j < 4; j++)
   {
    out[i][j] = 0.0;
   }
  }
 }

 // matrix copy.
void mat_copy (sMatrix a, sMatrix out)
 {
  int i, j;

  for (i = 0; i < 3; i++)
  {
   for (j = 0; j < 4; j++)
   {
    out[i][j] = a[i][j];
   }
  }
 }

 // print matrix on stdout.
void mat_print (sMatrix a)
 {
  printf ("xx:%f xy:%f xz:%f xw:%f\n", a[qX][qX], a[qX][qY], a[qX][qZ], a[qX][qW]);
  printf ("yx:%f yy:%f yz:%f yw:%f\n", a[qY][qX], a[qY][qY], a[qY][qZ], a[qY][qW]);
  printf ("zx:%f zy:%f zz:%f zw:%f\n", a[qZ][qX], a[qZ][qY], a[qZ][qZ], a[qZ][qW]);
 }

 // matrix addition.
void mat_add (sMatrix a, sMatrix b, sMatrix out)
 {
  int      i, j;
  sMatrix temp;

  for (i = 0; i < 3; i++)
  {
   for (j = 0; j < 4; j++)
   {
    temp[i][j] = a[i][j]+b[i][j];
   }
  }
  mat_copy (temp, out);
 }

 // matrix substraction.
void mat_sub (sMatrix a, sMatrix b, sMatrix out)
 {
  int      i, j;
  sMatrix temp;

  for (i = 0; i < 3; i++)
  {
   for (j = 0; j < 4; j++)
   {
    temp[i][j] = a[i][j] - b[i][j];
   }
  }
  mat_copy (temp, out);
 }

 // matrix multiplication.
void mat_mul (sMatrix a, sMatrix b, sMatrix out)
 {
  sMatrix temp;

  temp[qX][qX]=a[qX][qX]*b[qX][qX]+a[qX][qY]*b[qY][qX]+a[qX][qZ]*b[qZ][qX];
  temp[qX][qY]=a[qX][qX]*b[qX][qY]+a[qX][qY]*b[qY][qY]+a[qX][qZ]*b[qZ][qY];
  temp[qX][qZ]=a[qX][qX]*b[qX][qZ]+a[qX][qY]*b[qY][qZ]+a[qX][qZ]*b[qZ][qZ];
  temp[qX][qW]=a[qX][qX]*b[qX][qW]+a[qX][qY]*b[qY][qW]+a[qX][qZ]*b[qZ][qW]+a[qX][qW];
  temp[qY][qX]=a[qY][qX]*b[qX][qX]+a[qY][qY]*b[qY][qX]+a[qY][qZ]*b[qZ][qX];
  temp[qY][qY]=a[qY][qX]*b[qX][qY]+a[qY][qY]*b[qY][qY]+a[qY][qZ]*b[qZ][qY];
  temp[qY][qZ]=a[qY][qX]*b[qX][qZ]+a[qY][qY]*b[qY][qZ]+a[qY][qZ]*b[qZ][qZ];
  temp[qY][qW]=a[qY][qX]*b[qX][qW]+a[qY][qY]*b[qY][qW]+a[qY][qZ]*b[qZ][qW]+a[qY][qW];
  temp[qZ][qX]=a[qZ][qX]*b[qX][qX]+a[qZ][qY]*b[qY][qX]+a[qZ][qZ]*b[qZ][qX];
  temp[qZ][qY]=a[qZ][qX]*b[qX][qY]+a[qZ][qY]*b[qY][qY]+a[qZ][qZ]*b[qZ][qY];
  temp[qZ][qZ]=a[qZ][qX]*b[qX][qZ]+a[qZ][qY]*b[qY][qZ]+a[qZ][qZ]*b[qZ][qZ];
  temp[qZ][qW]=a[qZ][qX]*b[qX][qW]+a[qZ][qY]*b[qY][qW]+a[qZ][qZ]*b[qZ][qW]+a[qZ][qW];
  mat_copy (temp, out);
 }

 // inverse matrix calculation (non-singular).
int mat_inverse (sMatrix a, sMatrix out)
 {
  float    scale;
  sMatrix temp;

  scale = a[qX][qX]*a[qX][qX] + a[qX][qY]*a[qX][qY] + a[qX][qZ]*a[qX][qZ];
  if (scale == 0.0)
  {
   mat_identity (out);
   return ERROR;
  }
  scale = 1.0 / scale;
  temp[qX][qX] = scale * a[qX][qX];
  temp[qY][qX] = scale * a[qX][qY];
  temp[qZ][qX] = scale * a[qX][qZ];
  temp[qX][qY] = scale * a[qY][qX];
  temp[qY][qY] = scale * a[qY][qY];
  temp[qZ][qY] = scale * a[qY][qZ];
  temp[qX][qZ] = scale * a[qZ][qX];
  temp[qY][qZ] = scale * a[qZ][qY];
  temp[qZ][qZ] = scale * a[qZ][qZ];
  temp[qX][qW] = -(temp[qX][qX]*a[qX][qW] + temp[qX][qY]*a[qY][qW] + temp[qX][qZ]*a[qZ][qW]);
  temp[qY][qW] = -(temp[qY][qX]*a[qX][qW] + temp[qY][qY]*a[qY][qW] + temp[qY][qZ]*a[qZ][qW]);
  temp[qZ][qW] = -(temp[qZ][qX]*a[qX][qW] + temp[qZ][qY]*a[qY][qW] + temp[qZ][qZ]*a[qZ][qW]);
  mat_copy (temp, out);
  return OK;
 }

 // create a translation matrix.
void mat_settrans (sVector *v, sMatrix out)
 {
  mat_identity (out);
  out[qX][qW] = v->x;
  out[qY][qW] = v->y;
  out[qZ][qW] = v->z;
 }

 // create a scale matrix.
void mat_setscale (sVector *v, sMatrix out)
 {
  mat_identity (out);
  out[qX][qX] = v->x;
  out[qY][qY] = v->y;
  out[qZ][qZ] = v->z;
 }

 // create rotation matrix around X axis.
void mat_rotateX (float ang, sMatrix out)
 {
  float sinx, cosx;

  mat_identity (out);
  sinx = sin (ang);
  cosx = cos (ang);
  out[qY][qY] =  cosx;
  out[qY][qZ] =  sinx;
  out[qZ][qY] = -sinx;
  out[qZ][qZ] =  cosx;
 }

 // create rotation matrix around Y axis.
void mat_rotateY (float ang, sMatrix out)
 {
  float siny, cosy;

  mat_identity (out);
  siny = sin (ang);
  cosy = cos (ang);
  out[qX][qX] =  cosy;
  out[qX][qZ] = -siny;
  out[qZ][qX] =  siny;
  out[qZ][qZ] =  cosy;
 }

 // // create rotation matrix around Z axis.
void mat_rotateZ (float ang, sMatrix out)
 {
  float sinz, cosz;

  mat_identity (out);
  sinz = sin (ang);
  cosz = cos (ang);
  out[qX][qX] =  cosz;
  out[qX][qY] =  sinz;
  out[qY][qX] = -sinz;
  out[qY][qY] =  cosz;
 }

 // multiply a vector by matrix (out = [a]*b)
void mat_mulvec (sMatrix a, sVector *b, sVector *out)
 {
  sVector temp;

  temp.x = b->x*a[qX][qX] + b->y*a[qX][qY] + b->z*a[qX][qZ] + a[qX][qW];
  temp.y = b->x*a[qY][qX] + b->y*a[qY][qY] + b->z*a[qY][qZ] + a[qY][qW];
  temp.z = b->x*a[qZ][qX] + b->y*a[qZ][qY] + b->z*a[qZ][qZ] + a[qZ][qW];
  vec_copy (&temp, out);
 }

 // multiply a vector by upper 3x3 matrix (out = [a]*b)
void mat_mulvec3 (sMatrix a, sVector *b, sVector *out)
 {
  sVector temp;

  temp.x = b->x*a[qX][qX] + b->y*a[qX][qY] + b->z*a[qX][qZ];
  temp.y = b->x*a[qY][qX] + b->y*a[qY][qY] + b->z*a[qY][qZ];
  temp.z = b->x*a[qZ][qX] + b->y*a[qZ][qY] + b->z*a[qZ][qZ];
  vec_copy (&temp, out);
 }


//
// 컴컴� Quaternion 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
//

 // compute quaterion from [angle,axis] representation.
void qt_fromang (float ang, float x, float y, float z, sQuat *out)
 {
  float s, omega;

  omega = ang / 2.0;
  s = sin (omega);
  out->w = cos (omega);
  out->x = x * s;
  out->y = y * s;
  out->z = z * s;
 }

 // convert quaterion to [angle,axis] representation.
void qt_toang (sQuat *a, float *ang, float *x, float *y, float *z)
 {
  sQuat q;
  float  s, omega;

  qt_normalize (a, &q);
  omega = acos (q.w);
  *ang = omega * 2.0;
  s = sin (omega);
  if (fabs (s) > EPSILON)
  {
   *x = q.x / s;
   *y = q.y / s;
   *z = q.z / s;
  }
  else
  {
   *x = 0.0;
   *y = 0.0;
   *z = 0.0;
  }
 }

 // create quaternion (out = [w,[x,y,z]])
void qt_make (float w, float x, float y, float z, sQuat *out)
 {
  out->w = w;
  out->x = x;
  out->y = y;
  out->z = z;
 }

 // make identity quaternion (out = [1,0,0,0]).
void qt_identity (sQuat *out)
 {
  out->w = 1.0;
  out->x = 0.0;
  out->y = 0.0;
  out->z = 0.0;
 }

 // zero quaternion (out = [0,0,0,0]).
void qt_zero (sQuat *out)
 {
  out->w = 0.0;
  out->x = 0.0;
  out->y = 0.0;
  out->z = 0.0;
 }

 // quaternion copy (out = a).
void qt_copy (sQuat *a, sQuat *out)
 {
  out->w = a->w;
  out->x = a->x;
  out->y = a->y;
  out->z = a->z;
 }

 // print quaternion on stdout.
void qt_print (sQuat *a)
 {
  printf ("w:%f x:%f y:%f z:%f\n", a->w, a->x, a->y, a->z);
 }

 // quaternion addition (out = a+b).
void qt_add (sQuat *a, sQuat *b, sQuat *out)
 {
  sQuat temp;

  temp.w = a->w + b->w;
  temp.x = a->x + b->x;
  temp.y = a->y + b->y;
  temp.z = a->z + b->z;
  qt_copy (&temp, out);
 }

 // quaterion substraction (out = a-b).
void qt_sub (sQuat *a, sQuat *b, sQuat *out)
 {
  sQuat temp;

  temp.w = a->w - b->w;
  temp.x = a->x - b->x;
  temp.y = a->y - b->y;
  temp.z = a->z - b->z;
  qt_copy (&temp, out);
 }

 // quaternion multiplication (out = a*b).
void qt_mul (sQuat *a, sQuat *b, sQuat *out)
 {
  sQuat temp;

  temp.w = a->w*b->w - a->x*b->x - a->y*b->y - a->z*b->z;
  temp.x = a->w*b->x + a->x*b->w + a->y*b->z - a->z*b->y;
  temp.y = a->w*b->y + a->y*b->w + a->z*b->x - a->x*b->z;
  temp.z = a->w*b->z + a->z*b->w + a->x*b->y - a->y*b->x;
  qt_copy (&temp, out);
 }

 // scale quaternion to a new length.
void qt_scale (sQuat *a, float s, sQuat *out)
 {
  sQuat temp;
  float  len;

  len = (a->x*a->x + a->y*a->y + a->z*a->z + a->w*a->w);
  if (len == 0.0)
  {
   out->w = 1.0;
   out->x = 0.0;
   out->y = 0.0;
   out->z = 0.0;
   return;
  }
  temp.w = a->w * s / len;
  temp.x = a->x * s / len;
  temp.y = a->y * s / len;
  temp.z = a->y * s / len;
  qt_copy (&temp, out);
 }

 // quaterion division (out = a/b).
void qt_div (sQuat *a, sQuat *b, sQuat *out)
 {
  sQuat q, t, s, temp;
  float  p;

  qt_copy (b, &q);
  q.x = -q.x;
  q.y = -q.y;
  q.z = -q.z;
  qt_mul (a, &q, &t);
  qt_mul (&q, &q, &s);
  temp.w = t.w / s.w;
  temp.x = t.x / s.w;
  temp.y = t.y / s.w;
  temp.z = t.z / s.w;
  qt_copy (&temp, out);
 }

 // quaternion square (out = a^2).
void qt_square (sQuat *a, sQuat *out)
 {
  float  s;
  sQuat temp;

  s = 2 * a->w;
  temp.w = (a->w*a->w - a->x*a->x - a->y*a->y - a->z*a->z);
  temp.x = s * a->x;
  temp.y = s * a->y;
  temp.z = s * a->z;
  qt_copy (&temp, out);
 }

 // quaternion square root (out = sqrt(a)).
void qt_sqrt (sQuat *a, sQuat *out)
 {
  float  len, m;
  float  A, B;
  sQuat r, temp;

  len = sqrt (a->w*a->w + a->x*a->x + a->y*a->y);
  r.w = a->w / len;
  r.x = a->x / len;
  r.y = a->y / len;
  r.z = 0.0;
  m = 1 / sqrt (r.w*r.w + r.x*r.x);
  A = sqrt ((1 + r.y) / 2);
  B = sqrt ((1 - r.y) / 2);
  temp.w = sqrt (len) * B * r.w * m;
  temp.x = sqrt (len) * B * r.x * m;
  temp.y = sqrt (len) * A;
  temp.z = a->z;
  qt_copy (&temp, out);
 }

 // quaternion normalization.
void qt_normalize (sQuat *a, sQuat *out)
 {
  float  len;
  sQuat temp;

  len = sqrt (a->x*a->x + a->y*a->y + a->z*a->z + a->w*a->w);
  if (len == 0.0)
  {
   out->w = 1.0;
   out->x = 0.0;
   out->y = 0.0;
   out->z = 0.0;
   return;
  }
  temp.w = a->w / len;
  temp.x = a->x / len;
  temp.y = a->y / len;
  temp.z = a->z / len;
  qt_copy (&temp, out);
 }

 // form multiplicative inverse of quaternion.
void qt_inverse (sQuat *a, sQuat *out)
 {
  float  len;
  sQuat temp;

  len = sqrt (a->x*a->x + a->y*a->y + a->z*a->z + a->w*a->w);
  if (len == 0.0)
  {
   out->w = 1.0;
   out->x = 0.0;
   out->y = 0.0;
   out->z = 0.0;
   return;
  }
  temp.w =  a->w / len;
  temp.x = -a->x / len;
  temp.y = -a->y / len;
  temp.z = -a->z / len;
  qt_copy (&temp, out);
 }

 // spherical linear interpolation of quaternions.
void qt_slerp (sQuat *a, sQuat *b, float alpha, sQuat *out)
 {
  float  beta, theta;
  float  sin_t, cos_t;
  float  phi;
  int    bflip;
  sQuat temp;

  cos_t = a->x*b->x + a->y*b->y + a->z*b->z + a->w*b->w;
  if (cos_t < 0.0)
  {
   cos_t = -cos_t;
   bflip = 1;
  }
  else bflip = 0;
  if (1.0 - cos_t < EPSILON) beta = 1.0 - alpha; else
  {
   theta = acos (cos_t);
   phi = theta;
   sin_t = sin (theta);
   beta = sin (theta - alpha * phi) / sin_t;
   alpha = sin (alpha * phi) / sin_t;
  }
  if (bflip) alpha = -alpha;
  temp.x = beta*a->x + alpha*b->x;
  temp.y = beta*a->y + alpha*b->y;
  temp.z = beta*a->z + alpha*b->z;
  temp.w = beta*a->w + alpha*b->w;
  qt_copy (&temp, out);
 }

 // spherical linear interpolation of quaternions with extra spins.
void qt_slerpspin (sQuat *a, sQuat *b, float alpha, int spin, sQuat *out)
 {
  float  beta, theta;
  float  sin_t, cos_t;
  float  phi;
  int    bflip;
  sQuat temp;

  cos_t = a->x*b->x + a->y*b->y + a->z*b->z + a->w*b->w;
  if (cos_t < 0.0)
  {
   cos_t = -cos_t;
   bflip = 1;
  }
  else bflip = 0;
  if (1.0 - cos_t < EPSILON) beta = 1.0 - alpha; else
  {
   theta = acos (cos_t);
   phi = theta + spin * PI;
   sin_t = sin (theta);
   beta = sin (theta - alpha * phi) / sin_t;
   alpha = sin (alpha * phi) / sin_t;
  }
  if (bflip) alpha = -alpha;
  temp.x = beta*a->x + alpha*b->x;
  temp.y = beta*a->y + alpha*b->y;
  temp.z = beta*a->z + alpha*b->z;
  temp.w = beta*a->w + alpha*b->w;
  qt_copy (&temp, out);
 }

 // convert quaternion to rotation matrix.
 //      ( 1-yy-zz , xy+wz   , xz-wy   )
 //  T = ( xy-wz   , 1-xx-zz , yz+wx   )
 //      ( xz+wy   , yz-wx   , 1-xx-yy )
void qt_matrix (sQuat *a, sMatrix mat)
 {
  float x2, y2, z2, wx, wy, wz,
        xx, xy, xz, yy, yz, zz;

  x2 = a->x + a->x; y2 = a->y + a->y; z2 = a->z + a->z;
  wx = a->w * x2;   wy = a->w * y2;   wz = a->w * z2;
  xx = a->x * x2;   xy = a->x * y2;   xz = a->x * z2;
  yy = a->y * y2;   yz = a->y * z2;   zz = a->z * z2;

  mat[qX][qX] = 1.0 - (yy + zz);
  mat[qX][qY] = xy + wz;
  mat[qX][qZ] = xz - wy;
  mat[qX][qW] = 0.0;
  mat[qY][qX] = xy - wz;
  mat[qY][qY] = 1.0 - (xx + zz);
  mat[qY][qZ] = yz + wx;
  mat[qY][qW] = 0.0;
  mat[qZ][qX] = xz + wy;
  mat[qZ][qY] = yz - wx;
  mat[qZ][qZ] = 1.0 - (xx + yy);
  mat[qZ][qW] = 0.0;
 }

 // convert quaternion to inversed rotation matrix.
 //      ( 1-yy-zz , xy-wz   , xz+wy   )
 //  T = ( xy+wz   , 1-xx-zz , yz-wx   )
 //      ( xz-wy   , yz+wx   , 1-xx-yy )
void qt_invmatrix (sQuat *a, sMatrix mat)
 {
  float x2, y2, z2, wx, wy, wz,
        xx, xy, xz, yy, yz, zz;

  x2 = a->x + a->x; y2 = a->y + a->y; z2 = a->z + a->z;
  wx = a->w * x2;   wy = a->w * y2;   wz = a->w * z2;
  xx = a->x * x2;   xy = a->x * y2;   xz = a->x * z2;
  yy = a->y * y2;   yz = a->y * z2;   zz = a->z * z2;

  mat[qX][qX] = 1.0 - (yy + zz);
  mat[qX][qY] = xy - wz;
  mat[qX][qZ] = xz + wy;
  mat[qX][qW] = 0.0;
  mat[qY][qX] = xy + wz;
  mat[qY][qY] = 1.0 - (xx + zz);
  mat[qY][qZ] = yz - wx;
  mat[qY][qW] = 0.0;
  mat[qZ][qX] = xz - wy;
  mat[qZ][qY] = yz + wx;
  mat[qZ][qZ] = 1.0 - (xx + yy);
  mat[qZ][qW] = 0.0;
 }

 // convert rotation matrix to quaternion.
void qt_frommat (sMatrix mat, sQuat *out)
 {
  float  tr, s;
  float  q[4];
  int    i, j, k;
  static int nxt[3] = {qY,qZ,qX};

  tr = mat[qX][qX] + mat[qY][qY] + mat[qZ][qZ];
  if (tr > 0.0)
  {
   s = sqrt (tr + 1.0);
   out->w = s / 2.0;
   s = 0.5 / s;
   out->x = (mat[qY][qZ] - mat[qZ][qY]) * s;
   out->y = (mat[qZ][qX] - mat[qX][qZ]) * s;
   out->z = (mat[qX][qY] - mat[qY][qX]) * s;
  }
  else
  {
   i = qX;
   if (mat[qY][qY] > mat[qX][qX]) i = qY;
   if (mat[qZ][qZ] > mat[ i][ i]) i = qZ;
   j = nxt[i];
   k = nxt[j];
   s = sqrt ((mat[i][i] - (mat[j][j]+mat[k][k])) + 1.0);
   q[i] = s / 2.0;
   if (s != 0.0) s = 0.5 / s;
   q[qW] = (mat[j][k] - mat[k][j]) * s;
   q[ j] = (mat[i][j] + mat[j][i]) * s;
   q[ k] = (mat[i][k] + mat[k][i]) * s;
   out->w = q[qW];
   out->x = q[qX];
   out->y = q[qY];
   out->z = q[qZ];
  }
 }

//
// 컴컴� Spline 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
//

float Ease (float t, float a, float b)
 {
  float k;
  float s = a+b;

  if (s == 0.0) return t;
  if (s > 1.0)
  {
   a = a/s;
   b = b/s;
  }
  k = 1.0/(2.0-a-b);
  if (t < a) return ((k/a)*t*t);
  else
  {
   if (t < 1.0-b) return (k*(2*t-a));
   else
   {
    t = 1.0-t;
    return (1.0-(k/b)*t*t);
   }
  }
 }

 // -------------------------------------------------------------------------

void CompElementDeriv (float pp,  float p,   float pn,
                       float *ds, float *dd, float ksm,
                       float ksp, float kdm, float kdp)
 {
  float delm, delp;

  delm = p - pp;
  delp = pn - p;
  *ds  = ksm*delm + ksp*delp;
  *dd  = kdm*delm + kdp*delp;
 }

 // -------------------------------------------------------------------------

void CompDeriv (kfKey *keyp, kfKey *key, kfKey *keyn)
 {
  float tm, cm, cp, bm, bp, tmcm, tmcp;
  float ksm, ksp, kdm, kdp, delm, delp;
  float dt, fp, fn, c;

  dt = 0.5 * (keyn->Frame - keyp->Frame);
  fp = (key->Frame - keyp->Frame) / dt;
  fn = (keyn->Frame - key->Frame) / dt;
  c  = fabs (key->Cont);
  fp = fp + c - c * fp;
  fn = fn + c - c * fn;
  cm = 1.0 - key->Cont;
  tm = 0.5 * (1.0 - key->Tens);
  cp = 2.0 - cm;
  bm = 1.0 - key->Bias;
  bp = 2.0 - bm;
  tmcm = tm * cm;
  tmcp = tm * cp;
  ksm = tmcm * bp * fp; ksp = tmcp * bm * fp;
  kdm = tmcp * bp * fn; kdp = tmcm * bm * fn;
  CompElementDeriv (keyp->Val._quat.w, key->Val._quat.w, keyn->Val._quat.w,
                    &key->dsa, &key->dda, ksm, ksp, kdm, kdp);
  CompElementDeriv (keyp->Val._quat.x, key->Val._quat.x, keyn->Val._quat.x,
                    &key->dsb, &key->ddb, ksm, ksp, kdm, kdp);
  CompElementDeriv (keyp->Val._quat.y, key->Val._quat.y, keyn->Val._quat.y,
                    &key->dsc, &key->ddc, ksm, ksp, kdm, kdp);
  CompElementDeriv (keyp->Val._quat.z, key->Val._quat.z, keyn->Val._quat.z,
                    &key->dsd, &key->ddd, ksm, ksp, kdm, kdp);
 }

 // -------------------------------------------------------------------------

void CompDerivFirst (kfKey *key, kfKey *keyn, kfKey *keynn)
 {
  float f20, f10, v20, v10;

  f20 = keynn->Frame - key->Frame;
  f10 = keyn->Frame - key->Frame;
  v20 = keynn->Val._quat.w - key->Val._quat.w;
  v10 = keyn->Val._quat.w - key->Val._quat.w;
  key->dda = (1-key->Tens)*(v20*(0.25-f10/(2*f20))+(v10-v20/2)*3/2+v20/2);
  v20 = keynn->Val._quat.x - key->Val._quat.x;
  v10 = keyn->Val._quat.x - key->Val._quat.x;
  key->ddb = (1-key->Tens)*(v20*(0.25-f10/(2*f20))+(v10-v20/2)*3/2+v20/2);
  v20 = keynn->Val._quat.y - key->Val._quat.y;
  v10 = keyn->Val._quat.y - key->Val._quat.y;
  key->ddc = (1-key->Tens)*(v20*(0.25-f10/(2*f20))+(v10-v20/2)*3/2+v20/2);
  v20 = keynn->Val._quat.z - key->Val._quat.z;
  v10 = keyn->Val._quat.z - key->Val._quat.z;
  key->ddd = (1-key->Tens)*(v20*(0.25-f10/(2*f20))+(v10-v20/2)*3/2+v20/2);
 }

 // -------------------------------------------------------------------------

void CompDerivLast (kfKey *keypp, kfKey *keyp, kfKey *key)
 {
  float f20, f10, v20, v10;

  f20 = key->Frame - keypp->Frame;
  f10 = key->Frame - keyp->Frame;
  v20 = key->Val._quat.w - keypp->Val._quat.w;
  v10 = key->Val._quat.w - keyp->Val._quat.w;
  key->dsa = (1-key->Tens)*(v20*(0.25-f10/(2*f20))+(v10-v20/2)*3/2+v20/2);
  v20 = key->Val._quat.x - keypp->Val._quat.x;
  v10 = key->Val._quat.x - keyp->Val._quat.x;
  key->dsb = (1-key->Tens)*(v20*(0.25-f10/(2*f20))+(v10-v20/2)*3/2+v20/2);
  v20 = key->Val._quat.y - keypp->Val._quat.y;
  v10 = key->Val._quat.y - keyp->Val._quat.y;
  key->dsc = (1-key->Tens)*(v20*(0.25-f10/(2*f20))+(v10-v20/2)*3/2+v20/2);
  v20 = key->Val._quat.z - keypp->Val._quat.z;
  v10 = key->Val._quat.z - keyp->Val._quat.z;
  key->dsd = (1-key->Tens)*(v20*(0.25-f10/(2*f20))+(v10-v20/2)*3/2+v20/2);
 }

 // -------------------------------------------------------------------------

void CompDerivTwo (kfKey *key)
 {
  kfKey *keyn;

  keyn = key->Next;
  key->dsa = 0.0;
  key->dsb = 0.0;
  key->dsc = 0.0;
  key->dsd = 0.0;
  key->dda = (keyn->Val._quat.w - key->Val._quat.w) * (1 - key->Tens);
  key->ddb = (keyn->Val._quat.x - key->Val._quat.x) * (1 - key->Tens);
  key->ddc = (keyn->Val._quat.y - key->Val._quat.y) * (1 - key->Tens);
  key->ddd = (keyn->Val._quat.z - key->Val._quat.z) * (1 - key->Tens);
  keyn->dda = 0.0;
  keyn->ddb = 0.0;
  keyn->ddc = 0.0;
  keyn->ddd = 0.0;
  keyn->dsa = (keyn->Val._quat.w - key->Val._quat.w) * (1 - keyn->Tens);
  keyn->dsb = (keyn->Val._quat.x - key->Val._quat.x) * (1 - keyn->Tens);
  keyn->dsc = (keyn->Val._quat.y - key->Val._quat.y) * (1 - keyn->Tens);
  keyn->dsd = (keyn->Val._quat.z - key->Val._quat.z) * (1 - keyn->Tens);
 }

 // -------------------------------------------------------------------------

 // initialize spline key list.
int Spline_Init (kfKey *keys)
 {
  int   count;
  kfKey *curr;

  if (!keys) return ERROR;
  if (!keys->Next) return ERROR;

  if (keys->Next->Next)
  {
   // 3 or more keys
   for (curr = keys->Next; curr->Next; curr = curr->Next)
      CompDeriv (curr->Prev, curr, curr->Next);
   CompDerivFirst (keys, keys->Next, keys->Next->Next);
   CompDerivLast (curr->Prev->Prev, curr->Prev, curr);
  }
  else CompDerivTwo (keys); // 2 keys
  return OK;
 }

 // -------------------------------------------------------------------------

 // get spline interpolated float for frame "frame".
int Spline_GetKey_Float (kfKey *keys, float frame, float *out)
 {
  float t, t2, t3;
  float h[4];

  if (frame < 0.0) return ERROR;
  if (!keys) return ERROR;

  while (frame > keys->Next->Frame && keys->Next) keys = keys->Next;
  if (!keys->Next || frame < keys->Frame)
  {
   // frame is above last key
   *out = keys->Val._float;
   return OK;
  }
  t = (frame - keys->Frame) / (keys->Next->Frame - keys->Frame);
  t = Ease(t, keys->EaseFrom, keys->Next->EaseTo);
  t2 = t * t;
  t3 = t2 * t;
  h[0] =  2 * t3 - 3 * t2 + 1;
  h[1] = -2 * t3 + 3 * t2;
  h[2] = t3 - 2 * t2 + t;
  h[3] = t3 - t2;
  *out = (h[0]*keys->Val._float) + (h[1]*keys->Next->Val._float) +
         (h[2]*keys->dda) +        (h[3]*keys->Next->dsa);
  return OK;
 }

 // -------------------------------------------------------------------------

 // get spline interpolated vector for frame "frame".
int Spline_GetKey_Vect (kfKey *keys, float frame, sVector *out)
 {
  float t, t2, t3;
  float h[4];

  if (frame < 0.0) return ERROR;
  if (!keys) return ERROR;

  while (frame > keys->Next->Frame && keys->Next) keys = keys->Next;
  if (!keys->Next || frame < keys->Frame)
  {
   // frame is above last key
   vec_copy (&keys->Val._vector, out);
   return OK;
  }
  t = (frame - keys->Frame) / (keys->Next->Frame - keys->Frame);
  t = Ease(t, keys->EaseFrom, keys->Next->EaseTo);
  t2 = t * t;
  t3 = t2 * t;
  h[0] =  2 * t3 - 3 * t2 + 1;
  h[1] = -2 * t3 + 3 * t2;
  h[2] = t3 - 2 * t2 + t;
  h[3] = t3 - t2;
  out->x = (h[0]*keys->Val._vector.x) + (h[1]*keys->Next->Val._vector.x) +
           (h[2]*keys->dda) +         (h[3]*keys->Next->dsa);
  out->y = (h[0]*keys->Val._vector.y) + (h[1]*keys->Next->Val._vector.y) +
           (h[2]*keys->ddb) +         (h[3]*keys->Next->dsb);
  out->z = (h[0]*keys->Val._vector.z) + (h[1]*keys->Next->Val._vector.z) +
           (h[2]*keys->ddc) +         (h[3]*keys->Next->dsc);
  return OK;
 }

 // -------------------------------------------------------------------------

 // get spline interpolated quaternion for frame "frame".
int Spline_GetKey_Quat (kfKey *keys, float frame, sQuat *out)
 {
  float t, t2, t3;
  float h[4];

  if (frame < 0.0) return ERROR;
  if (!keys) return ERROR;

  while (frame > keys->Next->Frame && keys->Next) keys = keys->Next;
  if (!keys->Next || frame < keys->Frame) { /* frame is above last key */
    qt_copy (&keys->Val._quat, out);
    return OK;
  }
  t = (frame - keys->Frame) / (keys->Next->Frame - keys->Frame);
  t = Ease (t, keys->EaseFrom, keys->Next->EaseTo);
  t2 = t * t;
  t3 = t2 * t;
  h[0] =  2 * t3 - 3 * t2 + 1;
  h[1] = -2 * t3 + 3 * t2;
  h[2] = t3 - 2 * t2 + t;
  h[3] = t3 - t2;
  out->w = (h[0]*keys->Val._quat.w) + (h[1]*keys->Next->Val._quat.w) +
           (h[2]*keys->dda) +         (h[3]*keys->Next->dsa);
  out->x = (h[0]*keys->Val._quat.x) + (h[1]*keys->Next->Val._quat.x) +
           (h[2]*keys->ddb) +         (h[3]*keys->Next->dsb);
  out->y = (h[0]*keys->Val._quat.y) + (h[1]*keys->Next->Val._quat.y) +
           (h[2]*keys->ddc) +         (h[3]*keys->Next->dsc);
  out->z = (h[0]*keys->Val._quat.z) + (h[1]*keys->Next->Val._quat.z) +
           (h[2]*keys->ddd) +         (h[3]*keys->Next->dsd);
  return OK;
 }

//
// 컴컴� Camera 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
//

 // converts 3dstudio lens to fov.
void cam_lens_fov (float lens, float *fov)
 {
  int i;

  struct
  {
   float lens, fov;
  } lens_table[] = {
    {15.0,  115.0}, {20.0, 94.28571}, {24.0, 84.0}, {28.0,  76.36364},
    {35.0,  63.0},  {50.0, 46.0},     {85.0, 28.0}, {135.0, 18.0},
    {200.0, 12.0}
  };

  for (i = 0; i < 9; i++) if (lens == lens_table[i].lens)
  {
   *fov = lens_table[i].fov;
   return;
  }
  *fov = 15.0 / lens * 160;
 }

 // -------------------------------------------------------------------------

 // creates a camera matrix.
void Camera_Update (sCamera *cam)
 {
  sVector c;
  float    tanfov, focus;
  float    ax, ay, az;
  float    sinx, siny, sinz,
           cosx, cosy, cosz;

  vec_sub (&cam->Target, &cam->Pos, &c);
  focus = vec_length (&c);
  tanfov = tan ((cam->Fov / 2) / 180 * PI);
  cam->PerspX = cam->SizeX / 2 / tanfov;
  cam->PerspY = cam->SizeY / 2 / cam->Aspectratio / tanfov;

  ax = -atan2 (c.x, c.z);
  ay = asin (c.y / focus);
  az = -cam->Roll * PI / 180;

  sinx = sin (ax); cosx = cos (ax); siny = sin (ay);
  cosy = cos (ay); sinz = sin (az); cosz = cos (az);

  mat_identity (cam->CameraMat);
  cam->CameraMat[qX][qX] =  sinx * siny * sinz + cosx * cosz;
  cam->CameraMat[qX][qY] =  cosy * sinz;
  cam->CameraMat[qX][qZ] =  sinx * cosz - cosx * siny * sinz;
  cam->CameraMat[qY][qX] =  sinx * siny * cosz - cosx * sinz;
  cam->CameraMat[qY][qY] =  cosy * cosz;
  cam->CameraMat[qY][qZ] = -cosx * siny * cosz - sinx * sinz;
  cam->CameraMat[qZ][qX] = -sinx * cosy;
  cam->CameraMat[qZ][qY] =  siny;
  cam->CameraMat[qZ][qZ] =  cosx * cosy;

  vec_negate(&cam->Pos, &c);
  mat_settrans(&c,cam->M);
  mat_mul(cam->CameraMat,cam->M,cam->M);
 }

