#include <math.h>

typedef struct
{
 float angle;
 float axis[3];
} Quat;

#ifndef M_PI
 #define M_PI 3.14159265358979323846
#endif


// Convert euler[psi, theta, phi] to quaternion q.
// Angles are defined in radians.
void Quat_FromEuler(float euler[3], Quat *q)
{
 float coshpsi, coshtheta, coshphi, sinhpsi, sinhtheta, sinhphi;
 float halfpsi, halftheta, halfphi;

 halfpsi   = 0.5 * euler[0];
 halftheta = 0.5 * euler[1];
 halfphi   = 0.5 * euler[2];
 coshpsi   = cos(halfpsi);
 coshtheta = cos(halftheta);
 coshphi   = cos(halfphi);
 sinhpsi   = sin(halfpsi);
 sinhtheta = sin(halftheta);
 sinhphi   = sin(halfphi);
 q->angle = coshpsi*coshtheta*coshphi + sinhpsi*sinhtheta*sinhphi;
 q->axis[0] = coshpsi*coshtheta*sinhphi - sinhpsi*sinhtheta*coshphi;
 q->axis[1] = coshpsi*sinhtheta*coshphi + sinhpsi*coshtheta*sinhphi;
 q->axis[2] = sinhpsi*coshtheta*coshphi - coshpsi*sinhtheta*sinhphi;
}


// Convert the quaternion q to euler[psi, theta, phi].
// Angles are defined in radians.
#define EPSILON (1.0e-05)
void Quat_ToEuler(Quat *q, float euler[3])
{
 float sin_theta;      /* sin of pitch */
 float R00, R01, R02;  /* rotation matrix elements */
 float      R11, R12;
 float      R21, R22;

 R02 = 2.0*(q->axis[0]*q->axis[2] - q->angle*q->axis[1]);
 sin_theta = -R02;
 if (fabs(sin_theta) > (1.0 - EPSILON))
 {
  // very close to +- 90 degrees pitch, set heading to zero
  // and compute the roll angle
  euler[0] = 0.0;
  if (sin_theta > 0) euler[1] = M_PI/2.0; else euler[1] = -M_PI/2.0;
  R11 = 2.0*(q->angle*q->angle + q->axis[1]*q->axis[1]) - 1.0;
  R21 = 2.0*(q->axis[1]*q->axis[2] - q->angle*q->axis[0]);
  euler[2] = atan2(-R21, R11);
 }
 else
 {
  R00 = 2.0*(q->angle*q->angle + q->axis[0]*q->axis[0]) - 1.0;
  R01 = 2.0*(q->axis[0]*q->axis[1] + q->angle*q->axis[2]);
  R12 = 2.0*(q->axis[1]*q->axis[2] + q->angle*q->axis[0]);
  R22 = 2.0*(q->angle*q->angle + q->axis[2]*q->axis[2]) - 1.0;
  /* heading */
  euler[0] = atan2(R01, R00);
  /* pitch */
  euler[1] = asin(sin_theta);
  /* roll */
  euler[2] = atan2(R12, R22);
 }
}


/*
 *  q u a t e r n i o n M u l t i p l y
 *
 *  Multiply the quaternions q1 and q2 and return the result in qr.
 */

static void Quat_Mult(Quat *q1, Quat *q2, Quat *qr)
{
 qr->angle    =   q1->angle*q2->angle - q1->axis[0]*q2->axis[0]
                 - q1->axis[1]*q2->axis[1] - q1->axis[2]*q2->axis[2];
 qr->axis[0] =   q1->axis[0]*q2->angle + q1->angle*q2->axis[0]
                 - q1->axis[2]*q2->axis[1] + q1->axis[1]*q2->axis[2];

 qr->axis[1] =   q1->axis[1]*q2->angle + q1->axis[2]*q2->axis[0]
                 + q1->angle*q2->axis[1] - q1->axis[0]*q2->axis[2];

 qr->axis[2] =   q1->axis[2]*q2->angle - q1->axis[1]*q2->axis[0]
                 + q1->axis[0]*q2->axis[1] + q1->angle*q2->axis[2];
}

#define D2R(x) ((x)*M_PI/180.0)
#define R2D(x) ((x)*180.0/M_PI)

// --- example: interpolates from qBegin to qEnd ---

void main(void)
{
 /* start with zero heading, 80 deg pitch and zero roll */
 float eBegin[3];// = { 0, D2R(0.0), 0 };
 float eEnd[3];
 Quat qBegin, qEnd;
 float t;

// eBegin[0] = 0;
// eBegin[1] = D2R(45.0);
// eBegin[2] = 0;
// Quat_FromEuler(eBegin, &qBegin);   // starting

 // rotate 20 degrees in pitch

// eEnd[0] = 0;
// eEnd[1] = D2R(20.0);
// eEnd[2] = 0;
// Quat_FromEuler(eEnd, &qEnd);

// qEnd.angle   = D2R(20.0);
// qEnd.axis[0] = 0.0;
// qEnd.axis[1] = 1.0;
// qEnd.axis[2] = 0.0;

 qBegin.angle = D2R(20.0);
 qBegin.axis[0] = 0.0;
 qBegin.axis[1] = 1.0;
 qBegin.axis[2] = 0.0;

 qEnd.angle = D2R(50.0);
 qEnd.axis[0] = 0.0;
 qEnd.axis[1] = 1.0;
 qEnd.axis[2] = 0.0;


 for (t = 0.0; t <= 1.0; t += 0.1)
 {
  float halfAngle, sinHalfAngle;
  Quat qMove;
  Quat qInterpolated;
  float eMove[3];
  float eInterpolated[3];

  halfAngle = 0.5 * t * qEnd.angle;
  sinHalfAngle = sin(halfAngle);

  qMove.angle = cos(halfAngle);
  qMove.axis[0] = sinHalfAngle * qEnd.axis[0];
  qMove.axis[1] = sinHalfAngle * qEnd.axis[1];
  qMove.axis[2] = sinHalfAngle * qEnd.axis[2];

  Quat_Mult(&qBegin, &qMove, &qInterpolated);
//  Quat_ToEuler(&qMove, eMove);
  Quat_ToEuler(&qInterpolated, eInterpolated);

  printf("Euler: %5.2f %5.2f %5.2f\n",R2D(eInterpolated[0]),
                                      R2D(eInterpolated[1]),
                                      R2D(eInterpolated[2]));
  printf("Quat : %5.2f %5.2f %5.2f, %5.2f\n",qInterpolated.axis[0],
                                             qInterpolated.axis[1],
                                             qInterpolated.axis[2],
                                             R2D(qInterpolated.angle));

 }
}

/*

   This code needs one cosine and one sine (plus a number of multiplications
   and adds) per interplation. It is derived from algorithms used to do
   orientation deadreckoning in the DIS protocol.


1) Convert from (qBegin, qEnd) to (qBegin, qRot) where qRot is the quaternion
   that describes the rotation from qBegin to qEnd. This only needs to be
   done once (i.e. when qBegin or qEnd is changed).

   qRot.angle = the angle to rotate in radians;
   qRot.axis[] = the axis to rotate around;

   Note that only the axis should be normalized, not the complete
   quaternion. (qRot maybe should not be called a quaternion at all?)

2) Given an interpolation constant, compute qMove that rotates qBegin to
   qInterpolated. qInterpolated = qBegin * qMove.

   halfAngle = 0.5 * interpolationConstant * qRot.angle;
   qMove.angle = cos(halfAngle);
   sinHalfAngle = sin(halfAngle);
   qMove.axis[0] = sinHalfAngle * qRot.axis[0];
   qMove.axis[1] = sinHalfAngle * qRot.axis[1];
   qMove.axis[2] = sinHalfAngle * qRot.axis[2];

   quaternionMultiply(qBegin, qMove, qInterpolated);

3) Convert from the interpolated quaternion directly to a rotation matrix
   instead of doing quaternion -> axis, rotation -> rot matrix (the last step is
   done by OpenGL when you use glRotate{d,f}).

   (^2 means square to save some typing,
   q[0] = q.angle, q[1] = q.axis[0] etc)


   rotmat[0][0] = qI[0]^2 + qI[1]^2 - qI[2]^2 - qI[3]^2;
   rotmat[0][1] = 2*(qI[1]*qI[2] + qI[0]*qI[3]);
   rotmat[0][2] = 2*(qI[1]*qI[3] - qI[0]*qI[2]);

   rotmat[1][0] = 2*(qI[1]*qI[2] - qI[0]*qI[3]);
   rotmat[1][1] = qI[0]^2 - qI[1]^2 + qI[2]^2 - qI[3]^2;
   rotmat[1][2] = 2*(qI[2]*qI[3] + qI[0]*qI[1]);

   rotmat[2][0] = 2*(qI[1]*qI[3] + qI[0]*qI[2]);
   rotmat[2][1] = 2*(qI[2]*qI[3] - qI[0]*qI[1]);
   rotmat[2][2] = qI[0]^2 - qI[1]^2 - qI[2]^2 + qI[3]^2;

*/
