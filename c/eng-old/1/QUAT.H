#ifndef quat_h_included
#define quat_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

 // normalize a quaternion
void Quat_Normalize(float q[4], float qq[4]);
 // Compute quaternion product qq = qL * qR
void Quat_Mult(float qL[4], float qR[4], float qq[4]);
 // Form multiplicative inverse of q
void Quat_Inverse(float q[4], float qq[4]);
 // Convert quaternion to 3x3 rotation matrix
 // Quaternion need not be unit magnitude.  When it always is,
 // this routine can be simplified
void Quat_ToMatrix(float q[4], float mat[3][3]);
 //  Convert 3x3 rotation matrix to unit quaternion
void Quat_FromMatrix(float mat[3][3], float q[4]);
 // Compute equivalent quaternion from [angle,axis] representation
void Quat_FromAngAxis(float ang, float axis[3], float q[4]);
 // Convert quaternion to equivalent [angle,axis] representation
void Quat_ToAngAxis(float q[4], float *ang, float axis[3]);
 // Compute [angle,axis] corresponding to the rotation from
 // p to q.  Function returns angle, sets axis.
float Quat_RelAngAxis(float p[4], float q[4], float axis[3]);


// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
