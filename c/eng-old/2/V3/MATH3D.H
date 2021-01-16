#ifndef math3d_h_included
#define math3d_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#define S 65536.0
#define MAGIC (((S*S*16)+(S*.5))*S)
#define MAGICfixed (MAGIC/65536.0)

// --------------------------------------------------------------------------

#pragma inline float2int;
static long float2int( float d )
{
 double dtemp = MAGIC+d;
 return (*(long *)&dtemp)-0x80000000;
}

#pragma inline float2fixed;
static long float2fixed( float d )
{
 double dtemp = MAGICfixed+d;
 return (*(long *)&dtemp)-0x80000000;
}

#pragma inline CEIL;
static int CEIL(float f)
{
 int i,j;

 i = float2fixed(f);
 j = i >> 16;
 if ((i&0xffff) != 0) j+=1;
 return j;
}

// --------------------------------------------------------------------------

// (16.16 * 16.16) >> 16
// (16.16 *  8.8 ) >> 16
int fixedmul(int, int);
#pragma aux fixedmul=\
 "imul ebx",\
 "shrd eax,edx,16",\
 parm [eax] [ebx] value [eax] modify [edx];

// (16.16 << 16) / 16.16
// ( 8.8  << 16) / 16.16
int fixeddiv(int,int);
#pragma aux fixeddiv=\
 "cdq",\
 "shld edx,eax,16",\
 "idiv ebx",\
 parm [eax][ebx] value [eax] modify [edx];

void Init_SinCos(void);

void Matrix_Identity(float *m);
void Matrix_Translation(float *m, float tx, float ty, float tz);
void Matrix_Rotation(float *m, float xang, float yang, float zang);
void Matrix_Mult(float *a, float *b);
void Matrix_Copy(float *m1, float *m2);

sVector Vector_Normalize(sVector *v);
float Vector_Dot(sVector *v1, sVector *v2);
sVector Vector_Cross(sVector *v1, sVector *v2);
sVector Vector_Add(sVector *v1, sVector *v2);
sVector Vector_Sub(sVector *v1, sVector *v2);
sVector Vector_Trans(sVector *v, float *m);
sVector Vector_Trans3x3(sVector *v, float *m);

float Vector_Angle( sVector *v1, sVector *v2);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
