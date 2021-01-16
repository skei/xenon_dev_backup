 #ifndef math3d_h_included
 #define math3d_h_included
 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

// --- Vector ---

 // create vector (out = [x,y,z]).
void vec_make (float x, float y, float z, sVector *out);
 // zero vector (out = [0,0,0]).
void vec_zero (sVector *out);
 // vector copy (out = a).
void vec_copy (sVector *a, sVector *out);
 // print vector on stdout.
void vec_print (sVector *a);
 // vector addition (out = a+b).
void vec_add (sVector *a, sVector *b, sVector *out);
 // vector substraction (out = a-b).
void vec_sub (sVector *a, sVector *b, sVector *out);
 // vector multiplication (out = a*b).
void vec_mul (sVector *a, sVector *b, sVector *out);
 // vector division (out = a / b).
void vec_div (sVector *a, sVector *b, sVector *out);
 // vector multiplication by a scalar.
void vec_scale (sVector *a, float s, sVector *out);
 // vector negation (out = -a).
void vec_negate (sVector *a, sVector *out);
 // vector compare.
int vec_equal (sVector *a, sVector *b);
 // computes vector magnitude.
float vec_length (sVector *a);
 // computes distance between two vectors.
float vec_distance (sVector *a, sVector *b);
 // computes dot product of two vectors.
float vec_dot (sVector *a, sVector *b);
 // computes cross product of two vectors.
void vec_cross (sVector *a, sVector *b, sVector *out);
 // linear interpolation of two vectors.
void vec_lerp (sVector *a, sVector *b, float alpha, sVector *out);
 // linear combination of two vectors.
void vec_combine (sVector *a, sVector *b, float as, float bs, sVector *out);
 // vector normalization.
void vec_normalize (sVector *a, sVector *out);

// --- Matrix ---

 // make identity matrix.
void mat_identity (sMatrix out);
 // clear matrix.
void mat_zero (sMatrix out);
 // matrix copy.
void mat_copy (sMatrix a, sMatrix out);
 // print matrix on stdout.
void mat_print (sMatrix a);
 // matrix addition.
void mat_add (sMatrix a, sMatrix b, sMatrix out);
 // matrix substraction.
void mat_sub (sMatrix a, sMatrix b, sMatrix out);
 // matrix multiplication.
void mat_mul (sMatrix a, sMatrix b, sMatrix out);
 // inverse matrix calculation (non-singular).
int mat_inverse (sMatrix a, sMatrix out);
 // create a translation matrix.
void mat_settrans (sVector *v, sMatrix out);
 // create a scale matrix.
void mat_setscale (sVector *v, sMatrix out);
 // create rotation matrix around X axis.
void mat_rotateX (float ang, sMatrix out);
 // create rotation matrix around Y axis.
void mat_rotateY (float ang, sMatrix out);
 // // create rotation matrix around Z axis.
void mat_rotateZ (float ang, sMatrix out);
 // multiply a vector by matrix (out = [a]*b)
void mat_mulvec (sMatrix a, sVector *b, sVector *out);

// --- Quaternion ---

 // compute quaterion from [angle,axis] representation.
void qt_fromang (float ang, float x, float y, float z, sQuat *out);
 // convert quaterion to [angle,axis] representation.
void qt_toang (sQuat *a, float *ang, float *x, float *y, float *z);
 // create quaternion (out = [w,[x,y,z]])
void qt_make (float w, float x, float y, float z, sQuat *out);
 // make identity quaternion (out = [1,0,0,0]).
void qt_identity (sQuat *out);
 // zero quaternion (out = [0,0,0,0]).
void qt_zero (sQuat *out);
 // quaternion copy (out = a).
void qt_copy (sQuat *a, sQuat *out);
 // print quaternion on stdout.
void qt_print (sQuat *a);
 // quaternion addition (out = a+b).
void qt_add (sQuat *a, sQuat *b, sQuat *out);
 // quaterion substraction (out = a-b).
void qt_sub (sQuat *a, sQuat *b, sQuat *out);
 // quaternion multiplication (out = a*b).
void qt_mul (sQuat *a, sQuat *b, sQuat *out);
 // scale quaternion to a new length.
void qt_scale (sQuat *a, float s, sQuat *out);
 // quaterion division (out = a/b).
void qt_div (sQuat *a, sQuat *b, sQuat *out);
 // quaternion square (out = a^2).
void qt_square (sQuat *a, sQuat *out);
 // quaternion square root (out = sqrt(a)).
void qt_sqrt (sQuat *a, sQuat *out);
 // quaternion normalization.
void qt_normalize (sQuat *a, sQuat *out);
 // form multiplicative inverse of quaternion.
void qt_inverse (sQuat *a, sQuat *out);
 // spherical linear interpolation of quaternions.
void qt_slerp (sQuat *a, sQuat *b, float alpha, sQuat *out);
 // spherical linear interpolation of quaternions with extra spins.
void qt_slerpspin (sQuat *a, sQuat *b, float alpha, int spin, sQuat *out);
 // convert quaternion to rotation matrix.
 //      ( 1-yy-zz , xy+wz   , xz-wy   )
 //  T = ( xy-wz   , 1-xx-zz , yz+wx   )
 //      ( xz+wy   , yz-wx   , 1-xx-yy )
void qt_matrix (sQuat *a, sMatrix mat);
 // convert quaternion to inversed rotation matrix.
 //      ( 1-yy-zz , xy-wz   , xz+wy   )
 //  T = ( xy+wz   , 1-xx-zz , yz-wx   )
 //      ( xz-wy   , yz+wx   , 1-xx-yy )
void qt_invmatrix (sQuat *a, sMatrix mat);
 // convert rotation matrix to quaternion.
void qt_frommat (sMatrix mat, sQuat *out);

// --- Spline ---

 // initialize spline key list.
int Spline_Init (kfKey *keys);
 // get spline interpolated float for frame "frame".
int Spline_GetKey_Float (kfKey *keys, float frame, float *out);
 // get spline interpolated vector for frame "frame".
int Spline_GetKey_Vect (kfKey *keys, float frame, sVector *out);
 // get spline interpolated quaternion for frame "frame".
int Spline_GetKey_Quat (kfKey *keys, float frame, sQuat *out);

// --- Camera ---

 // converts 3dstudio lens to fov.
void cam_lens_fov (float lens, float *fov);
 // creates a camera matrix.
void Camera_Update (sCamera *cam);

 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
 #endif
