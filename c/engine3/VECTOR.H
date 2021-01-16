 #ifndef vector_h_included
 #define vector_h_included
 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

 // create vector (out = [x,y,z]).
void vec_make (float x, float y, float z, sVector *out)
 // zero vector (out = [0,0,0]).
void vec_zero (sVector *out)
 // vector copy (out = a).
void vec_copy (sVector *a, sVector *out)
 // print vector on stdout.
void vec_print (sVector *a)
 // vector addition (out = a+b).
void vec_add (sVector *a, sVector *b, sVector *out)
 // vector substraction (out = a-b).
void vec_sub (sVector *a, sVector *b, sVector *out)
 // vector multiplication (out = a*b).
void vec_mul (sVector *a, sVector *b, sVector *out)
 // vector division (out = a / b).
void vec_div (sVector *a, sVector *b, sVector *out)
 // vector multiplication by a scalar.
void vec_scale (sVector *a, float s, sVector *out)
 // vector negation (out = -a).
void vec_negate (sVector *a, sVector *out)
 // vector compare.
int vec_equal (sVector *a, sVector *b)
 // computes vector magnitude.
float vec_length (sVector *a)
 // computes distance between two vectors.
float vec_distance (sVector *a, sVector *b)
 // computes dot product of two vectors.
float vec_dot (sVector *a, sVector *b)
 // computes cross product of two vectors.
void vec_cross (sVector *a, sVector *b, sVector *out)
 // linear interpolation of two vectors.
void vec_lerp (sVector *a, sVector *b, float alpha, sVector *out)
 // linear combination of two vectors.
void vec_combine (sVector *a, sVector *b, float as, float bs, sVector *out)
 // vector normalization.
void vec_normalize (sVector *a, sVector *out)

