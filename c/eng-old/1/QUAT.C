 #include <math.h>

 #define X 0
 #define Y 1
 #define Z 2
 #define W 3

 // -------------------------------------------------------------------------

 // normalize a quaternion
void Quat_Normalize(float q[4], float qq[4])
 {
  float l,c;
  l =  sqrt(q[X]*q[X] + q[Y]*q[Y] + q[Z]*q[Z] + q[W]*q[W]);
  if (l==0)
  {
   qq[W] = 1.0;
   qq[Y] = qq[Z] = qq[X] = 0.0;
  }
  else
  {
   c = 1.0/l;
   qq[X] = q[X]*c;
   qq[Y] = q[Y]*c;
   qq[Z] = q[Z]*c;
   qq[W] = q[W]*c;
  }
 }

 // -------------------------------------------------------------------------

 // Compute quaternion product qq = qL * qR
void Quat_Mult(float qL[4], float qR[4], float qq[4])
 {
	float qp[4];
	qp[W] = qL[W]*qR[W] - qL[X]*qR[X] - qL[Y]*qR[Y] - qL[Z]*qR[Z];
	qp[X] = qL[W]*qR[X] + qL[X]*qR[W] + qL[Y]*qR[Z] - qL[Z]*qR[Y];
	qp[Y] = qL[W]*qR[Y] + qL[Y]*qR[W] + qL[Z]*qR[X] - qL[X]*qR[Z];
	qp[Z] = qL[W]*qR[Z] + qL[Z]*qR[W] + qL[X]*qR[Y] - qL[Y]*qR[X];
  qq[0] = qp[0];
  qq[1] = qp[1];
  qq[2] = qp[2];
  qq[3] = qp[3];
  // memcpy(qq,qp,4*sizeof(float));
 }

 // -------------------------------------------------------------------------

 // Form multiplicative inverse of q
void Quat_Inverse(float q[4], float qq[4])
 {
	float l,norminv;
	l =	 (q[X]*q[X] + q[Y]*q[Y] + q[Z]*q[Z] + q[W]*q[W]);
	if (l==0.0) l = 1.0;
	norminv = 1.0/l;
	qq[X] = -q[X] * norminv;
	qq[Y] = -q[Y] * norminv;
	qq[Z] = -q[Z] * norminv;
	qq[W] = q[W] * norminv;
 }

 // -------------------------------------------------------------------------

 // Convert quaternion to 3x3 rotation matrix
 // Quaternion need not be unit magnitude.  When it always is,
 // this routine can be simplified
void Quat_ToMatrix(float q[4], float mat[3][3])
 {
	float s,xs,ys,zs,wx,wy,wz,xx,xy,xz,yy,yz,zz,den;

  // For unit q, just set s = 2.0; or or set xs = q[X] + q[X], etc
	den =  (q[X]*q[X] + q[Y]*q[Y] + q[Z]*q[Z] + q[W]*q[W]);
  if (den==0.0) { s = 1.0; }
	else s = 2.0/den;
  xs = q[X] * s;   ys = q[Y] * s;   zs = q[Z] * s;
	wx = q[W] * xs;  wy = q[W] * ys; wz = q[W] * zs;
	xx = q[X] * xs;  xy = q[X] * ys; xz = q[X] * zs;
	yy = q[Y] * ys;  yz = q[Y] * zs; zz = q[Z] * zs;
  mat[X][X] = 1.0 - (yy +zz);
	mat[X][Y] = xy - wz;
	mat[X][Z] = xz + wy;
  mat[Y][X] = xy + wz;
	mat[Y][Y] = 1.0 - (xx +zz);
	mat[Y][Z] = yz - wx;
  mat[Z][X] = xz - wy;
	mat[Z][Y] = yz + wx;
	mat[Z][Z] = 1.0 - (xx + yy);
 }

 // -------------------------------------------------------------------------

 static int nxt[3] = {Y,Z,X};

 //  Convert 3x3 rotation matrix to unit quaternion
void Quat_FromMatrix(float mat[3][3], float q[4])
 {
	float tr,s;
	int i,j,k;
	tr = mat[X][X] + mat[Y][Y] + mat[Z][Z];
  if (tr > 0.0)
  {
   s = sqrt(tr + 1.0);
   q[W] = s * 0.5;
		s = 0.5 / s;
   q[X] = (mat[Z][Y] - mat[Y][Z]) * s;
   q[Y] = (mat[X][Z] - mat[Z][X]) * s;
   q[Z] = (mat[Y][X] - mat[X][Y]) * s;
  }
  else
  {
   i = X;
   if (mat[Y][Y] > mat[X][X]) i = Y;
   if (mat[Z][Z] > mat[i][i]) i = Z;
   j = nxt[i];
   k = nxt[j];
   s = sqrt( (mat[i][i] - (mat[j][j]+mat[k][k])) + 1.0 );
   q[i] = s * 0.5;
   if (s!=0.0) s = 0.5 /s;
   q[W] = (mat[k][j] - mat[j][k]) * s;
   q[j] = (mat[j][i] + mat[i][j]) * s;
   q[k] = (mat[k][i] + mat[i][k]) * s;
  }
 }

 // -------------------------------------------------------------------------

 // Compute equivalent quaternion from [angle,axis] representation
void Quat_FromAngAxis(float ang, float axis[3], float q[4])
 {
	int i;
	float omega,s;
	omega = ang*0.5;
	s = sin(omega);
	for (i=0; i<3; i++) q[i] = s*axis[i];
	q[3] = cos(omega);
 }

 // -------------------------------------------------------------------------

 // Convert quaternion to equivalent [angle,axis] representation
void Quat_ToAngAxis(float q[4], float *ang, float axis[3])
 {
	int i;
	float omega,s,qn[4];
  Quat_Normalize(q,qn);
	omega = acos(qn[3]);
	*ang = 2.0*omega;
	s = sin(omega);
	if (fabs(s)>.000001) for (i=0; i<3; i++) axis[i]  = qn[i]/s;
 }

 // -------------------------------------------------------------------------


 // Compute [angle,axis] corresponding to the rotation from
 // p to q.  Function returns angle, sets axis.
float Quat_RelAngAxis(float p[4], float q[4], float axis[3])
 {
	float pinv[4],r[4],ang;
  Quat_Inverse(p,pinv);
  Quat_Mult(pinv,q,r);
  Quat_ToAngAxis(r,&ang,axis);
	return(ang);
 }


