 #ifndef math3d_h_included
 #define math3d_h_included
 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

 #include "engine.h"

void  Vector_Print( sVector *v);
void  Vector_Create(sVector *v, float x,float y,float z);
void  Vector_Zero(sVector *v);
void  Vector_Copy(sVector *vd, sVector *vs);
void  Vector_Add(sVector *vd, sVector *v1, sVector *v2);
void  Vector_Sub(sVector *vd, sVector *v1, sVector *v2);
void  Vector_Mul(sVector *vd, sVector *v1, sVector *v2);
void  Vector_Div(sVector *vd, sVector *v1, sVector *v2);
void  Vector_Scale(sVector *vd, sVector *v, float scale);
void  Vector_Negate(sVector *vd, sVector *vs);
int   Vector_Equal(sVector *v1, sVector *v2);
float Vector_Length(sVector *v);
float Vector_Distance(sVector *v1, sVector *v2);
float Vector_Dot(sVector *v1, sVector *v2);
void  Vector_Cross(sVector *vd, sVector *v1, sVector *v2);
void  Vector_Normalize(sVector *vd, sVector *vs);
void  Vector_Trans(sVector *dest, sVector *v, float *m);
void  Vector_Trans3x3(sVector *dest, sVector *v, float *m);
void  Vector_Inv_Trans3x3(sVector *dest, sVector *v, float *m);

void Matrix_Print( float *m);
void Matrix_Identity(float *m);
void Matrix_Translation(float *m, float tx, float ty, float tz);
void Matrix_RotationXYZ(float *m, float xang, float yang, float zang);
void Matrix_RotationX(float *m, float xang);
void Matrix_RotationY(float *m, float yang);
void Matrix_RotationZ(float *m, float zang);
void Matrix_Mul(float *a, float *b);
void Matrix_Copy(float *m1, float *m2);
void Matrix_Inverse( float *d, float *s );
int  Matrix_Inverse2( float *matrix );
int  Matrix_InvScale(float *out, float *a);
void Matrix_AngleAxis ( float *m, float angle, sVector *v );

void  Quat_Print( float *q);
void  Quat_Identity (float *out);
void  Quat_Copy( float *out, float *a );
void  Quat_Normalize( float *qd,float *q );
void  Quat_Mul(float *qd,float *qL, float *qR);
void  Quat_Inverse(float *qd,float *q);
void  Quat_ToMatrix(float *q, float *m);
void  Quat_ToInvMatrix( float *q, float *m);

void  Quat_FromAngAxis(float *q,float ang, sVector *axis);
void  Quat_ToAngAxis(float *q, float *ang, sVector *axis);
float Quat_RelAngAxis(float *p, float *q, sVector *axis);
float Quat_Length(float *a);
float Quat_Dot (float *a, float *b);
float Quat_DotUnit (float *a, float *b);
void  Quat_Slerp(float *out, float *a, float *b, float spin, float alpha);
void  Quat_Slerpl (float *out, float *a, float *b, float spin, float alpha);
void  Quat_Negate (float *out, float *a);
void  Quat_Exp(float *out,float *a);
void  Quat_Log(float *out,float *a);
void  Quat_Lndif (float *out, float *a, float *b);

 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
 #endif
