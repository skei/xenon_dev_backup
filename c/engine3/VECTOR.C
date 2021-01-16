 #include <math.h>
 #include <stdio.h>
 #include "engine.h"

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
