/*
Currently I just interpolate the 4 values in the quaternion as any
other position key using Hermite curves. Then normalize it and convert
to a rotation matrix. It works pretty ok, in fact, it looks pretty ok,
but I know it's not correct.
*/

// --------------------------------------------------------------------------


/*
This code needs one cosine and one sine (plus a number of multiplications
and adds) per interplation. It is derived from algorithms used to do
orientation deadreckoning in the DIS protocol.


1) Convert from (qBegin, qEnd) to (qBegin, qRot) where qRot is the
quaternion
	that describes the rotation from qBegin to qEnd. This only needs to be
done
	once (i.e. when qBegin or qEnd is changed).

	qRot.scalar = the angle to rotate in radians;
	qRot.vector[] = the axis to rotate around;

	Note that only the vector should be normalized, not the complete
quaternion.
	(qRot maybe should not be called a quaternion at all?)

2) Given an interpolation constant, compute qMove that rotates qBegin to
	qInterpolated. qInterpolated = qBegin * qMove.

	halfAngle = 0.5 * interpolationConstant * qRot.scalar;
	qMove.scalar = cos(halfAngle);
	sinHalfAngle = sin(halfAngle);
	qMove.vector[0] = sinHalfAngle * qRot.vector[0];
	qMove.vector[1] = sinHalfAngle * qRot.vector[1];
	qMove.vector[2] = sinHalfAngle * qRot.vector[2];

	quaternionMultiply(qBegin, qMove, qInterpolated);


3) Convert from the interpolated quaternion directly to a rotation matrix
	instead of doing quaternion -> axis, rotation -> rot matrix (the last
	step is done by OpenGL when you use glRotate{d,f}).

	(^2 means square to save some typing,
	 q[0] = q.scalar, q[1] = q.vector[0] etc)


	rotmat[0][0] = qI[0]^2 + qI[1]^2 - qI[2]^2 - qI[3]^2;
	rotmat[0][1] = 2*(qI[1]*qI[2] + qI[0]*qI[3]);
	rotmat[0][2] = 2*(qI[1]*qI[3] - qI[0]*qI[2]);

	rotmat[1][0] = 2*(qI[1]*qI[2] - qI[0]*qI[3]);
	rotmat[1][1] = qI[0]^2 - qI[1]^2 + qI[2]^2 - qI[3]^2;
	rotmat[1][2] = 2*(qI[2]*qI[3] + qI[0]*qI[1]);

	rotmat[2][0] = 2*(qI[1]*qI[3] + qI[0]*qI[2]);
	rotmat[2][1] = 2*(qI[2]*qI[3] - qI[0]*qI[1]);
	rotmat[2][2] = qI[0]^2 - qI[1]^2 - qI[2]^2 + qI[3]^2;



More complete test code for step 1 and 2 is appended below.

		_
Mats Lofkvist
mal@algonet.se


---------------------test-code----------------------
*/
#include <math.h>

typedef struct {
  double scalar;
  double vector[3];
} Quaternion;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


/*
 *  e u l e r T o Q u a t e r n i o n
 *
 *  Convert euler[psi, theta, phi] to quaternion q.
 *  Angles are defined in radians.
 */

void eulerToQuaternion(double euler[3], Quaternion *q)
{
  double coshpsi, coshtheta, coshphi, sinhpsi, sinhtheta, sinhphi;
  double halfpsi, halftheta, halfphi;

  halfpsi   = 0.5 * euler[0];
  halftheta = 0.5 * euler[1];
  halfphi   = 0.5 * euler[2];

  coshpsi   = cos(halfpsi);
  coshtheta = cos(halftheta);
  coshphi   = cos(halfphi);
  sinhpsi   = sin(halfpsi);
  sinhtheta = sin(halftheta);
  sinhphi   = sin(halfphi);

  q->scalar = coshpsi*coshtheta*coshphi + sinhpsi*sinhtheta*sinhphi;

  q->vector[0] = coshpsi*coshtheta*sinhphi - sinhpsi*sinhtheta*coshphi;
  q->vector[1] = coshpsi*sinhtheta*coshphi + sinhpsi*coshtheta*sinhphi;
  q->vector[2] = sinhpsi*coshtheta*coshphi - coshpsi*sinhtheta*sinhphi;
}


/*
 *  q u a t e r n i o n T o E u l e r
 *
 *  Convert the quaternion q to euler[psi, theta, phi].
 *  Angles are defined in radians.
 */

#define EPSILON (1.0e-05)

void quaternionToEuler(Quaternion *q, double euler[3])
{
  double sin_theta;      /* sin of pitch */

  double R00, R01, R02;  /* rotation matrix elements */
  double      R11, R12;
  double      R21, R22;

  R02 = 2.0*(q->vector[0]*q->vector[2] - q->scalar*q->vector[1]);

  sin_theta = -R02;

  if (fabs(sin_theta) > (1.0 - EPSILON))
	 {
		/* very close to +- 90 degrees pitch, set heading to zero
	 and compute the roll angle */
		euler[0] = 0.0;

		if (sin_theta > 0)
	euler[1] = M_PI/2.0;
		else
	euler[1] = -M_PI/2.0;

		R11 = 2.0*(q->scalar*q->scalar + q->vector[1]*q->vector[1]) - 1.0;
		R21 = 2.0*(q->vector[1]*q->vector[2] - q->scalar*q->vector[0]);

		euler[2] = atan2(-R21, R11);
	 }
  else
	 {
		R00 = 2.0*(q->scalar*q->scalar + q->vector[0]*q->vector[0]) - 1.0;
		R01 = 2.0*(q->vector[0]*q->vector[1] + q->scalar*q->vector[2]);
		R12 = 2.0*(q->vector[1]*q->vector[2] + q->scalar*q->vector[0]);
		R22 = 2.0*(q->scalar*q->scalar + q->vector[2]*q->vector[2]) - 1.0;

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

static void quaternionMultiply(Quaternion *q1, Quaternion *q2, Quaternion
*qr)
{
  qr->scalar = q1->scalar*q2->scalar - q1->vector[0]*q2->vector[0]
	 - q1->vector[1]*q2->vector[1] - q1->vector[2]*q2->vector[2];

  qr->vector[0] = q1->vector[0]*q2->scalar + q1->scalar*q2->vector[0]
	 - q1->vector[2]*q2->vector[1] + q1->vector[1]*q2->vector[2];

  qr->vector[1] = q1->vector[1]*q2->scalar + q1->vector[2]*q2->vector[0]
	 + q1->scalar*q2->vector[1] - q1->vector[0]*q2->vector[2];

  qr->vector[2] = q1->vector[2]*q2->scalar - q1->vector[1]*q2->vector[0]
	 + q1->vector[0]*q2->vector[1] + q1->scalar*q2->vector[2];
}


#define D2R(x) ((x)*M_PI/180.0)
#define R2D(x) ((x)*180.0/M_PI)

main()
{
  /* start with zero heading, 80 deg pitch and zero roll */
  double eBegin[3] = { 0, D2R(80.0), 0 };

  Quaternion qBegin, qRot;
  double k;

  eulerToQuaternion(eBegin, &qBegin);

  /* rotate 20 degrees in pitch */
  qRot.scalar = D2R(20.0);
  qRot.vector[0] = 0.0;
  qRot.vector[1] = 1.0;
  qRot.vector[2] = 0.0;

  for (k = 0.0; k <= 1.0; k += 0.1)
	 {
		double halfAngle, sinHalfAngle;
		Quaternion qMove, qInterpolated;
		double eMove[3], eInterpolated[3];

		halfAngle = 0.5 * k * qRot.scalar;
		qMove.scalar = cos(halfAngle);
		sinHalfAngle = sin(halfAngle);
		qMove.vector[0] = sinHalfAngle * qRot.vector[0];
		qMove.vector[1] = sinHalfAngle * qRot.vector[1];
		qMove.vector[2] = sinHalfAngle * qRot.vector[2];

		quaternionMultiply(&qBegin, &qMove, &qInterpolated);

		quaternionToEuler(&qMove, eMove);
		quaternionToEuler(&qInterpolated, eInterpolated);

		printf("%7.2f %7.2f %7.2f\n",
		  R2D(eInterpolated[0]),
		  R2D(eInterpolated[1]),
		  R2D(eInterpolated[2]));
	 }
}

