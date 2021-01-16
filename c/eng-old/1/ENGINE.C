 #include <string.h>       // strchr
 #include <math.h>
 #include <stdio.h>

 #include "engine.h"
 #include "vbe2.h"

 #include "poly.h"
 #include "poly2.h"

 #include "quat.h"

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ²²²²² Vars ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

char *_Screen;
unsigned short int *_VesaScreen;
char *_Buffer;
unsigned short int *_VesaBuffer;
char *_ShadeTable;
unsigned short int *_VesaShadeTable;
char *_PhongMap;
unsigned short int *_VesaPhongMap;

char *_Texture1;
char *_Texture2;

float *_DivTable;

int MinX;
int MaxX;
int MinY;
int MaxY;

int ScreenWidth;
int ScreenHeight;

float viewdistance;

float min_clip_z;
float max_clip_z;

float halfscreenwidth_vd;
float halfscreenheight_vd;

char PhongIntensity[129]={
 0x09,0x09,0x0A,0x0A,0x0A,0x0A,0x0B,0x0B,0x0B,0x0C,0x0C,0x0C,0x0C,0x0D,0x0D,
 0x0D,0x0D,0x0E,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x10,0x10,0x10,0x10,0x11,0x11,
 0x11,0x11,0x12,0x12,0x12,0x12,0x13,0x13,0x13,0x13,0x13,0x14,0x14,0x14,0x14,
 0x15,0x15,0x15,0x15,0x16,0x16,0x16,0x16,0x16,0x17,0x17,0x17,0x17,0x18,0x18,
 0x18,0x18,0x18,0x18,0x19,0x19,0x19,0x19,0x19,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,
 0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1D,0x1D,
 0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1E,0x1E,0x1E,0x1E,0x1E,0x1F,0x1F,0x1F,0x20,
 0x20,0x21,0x21,0x22,0x23,0x24,0x25,0x27,0x28,0x2A,0x2C,0x2E,0x2F,0x31,0x33,
 0x35,0x37,0x39,0x3B,0x3C,0x3D,0x3E,0x3E,0x3E };

char Intensity[128];

sWorld *World;
sCamera *Camera;

int MaxVUEFrames;

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ²²²²² Math ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

// ----- Matrix -------------------------------------------------------------

 //   1   0   0   0
 //   0   1   0   0
 //   0   0   1   0
 //   0   0   0   1
void Matrix_Identity(float *m)
 {
  m[ 0] = 1; m[ 1] = 0; m[ 2] = 0; m[ 3] = 0;
  m[ 4] = 0; m[ 5] = 1; m[ 6] = 0; m[ 7] = 0;
  m[ 8] = 0; m[ 9] = 0; m[10] = 1; m[11] = 0;
  m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
 }

 // -----

 //   1   0   0   tx
 //   0   1   0   ty
 //   0   0   1   tz
 //   0   0   0   1
void Matrix_Translation(float *m, float tx, float ty, float tz)
 {
  m[ 0] = 1; m[ 1] = 0; m[ 2] = 0; m[ 3] = tx;
  m[ 4] = 0; m[ 5] = 1; m[ 6] = 0; m[ 7] = ty;
  m[ 8] = 0; m[ 9] = 0; m[10] = 1; m[11] = tz;
  m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
 }

 // -----

void Matrix_Rotation(float *m, float xang, float yang, float zang)
 {
  float s1,s2,s3,c1,c2,c3;

  s1 = sin(xang);
  s2 = sin(yang);
  s3 = sin(zang);
  c1 = cos(xang);
  c2 = cos(yang);
  c3 = cos(zang);
 // Ú                                                 ¿
 // ³      c2*c3             c2*s3          s2      tx³
 // ³ -s1*s2*c3-c1*s3   -s1*s2*s3+c1*c3    s1*c2    ty³
 // ³ -c1*s2*c3+s1*s3   -c1*s2*s3-s1*c3    c1*c2    tz³
 // ³        0                 0            0        1³
 // À                                                 Ù
  m[ 0] = c2*c3;
  m[ 1] = c2*s3;
  m[ 2] = s2;
  m[ 3] = 0;

  m[ 4] = -s1*s2*c3 - c1*s3;
  m[ 5] = -s1*s2*s3 + c1*c3;
  m[ 6] = s1*c2;
  m[ 7] = 0;

  m[ 8] = -c1*s2*c3 + s1*s3;
  m[ 9] = -c1*s2*s3 - s1*c3;
  m[10] = c1*c2;
  m[11] = 0;

  m[12] = 0;
  m[13] = 0;
  m[14] = 0;
  m[15] = 1;
 }

 // -----

void Matrix_RotationX(float *m, float xang)
 {
  float s1,s2,s3,c1,c2,c3;

  s1 = sin(xang);
  c1 = cos(xang);
 // Ú                 ¿
 // ³  1   0   0   0  ³
 // ³  0  c1  s1   0  ³
 // ³  0 -s1  c1   0  ³
 // ³  0   0   0   1  ³
 // À                 Ù
  Matrix_Identity(m);
  m[ 5] = c1;
  m[ 6] = s1;
  m[ 9] = -s1;
  m[10] = c1;
 }

 // -----

void Matrix_RotationY(float *m, float yang)
 {
  float s1,s2,s3,c1,c2,c3;

  s2 = sin(yang);
  c2 = cos(yang);
 // Ú                 ¿
 // ³ c2   0  s2   0  ³
 // ³  0   1   0   0  ³
 // ³-s2   0  c2   0  ³
 // ³  0   0   0   1  ³
 // À                 Ù
  Matrix_Identity(m);
  m[ 0] = c2;
  m[ 2] = s2;
  m[ 8] = -s2;
  m[10] = c2;
 }

 // -----

void Matrix_RotationZ(float *m, float zang)
 {
  float s1,s2,s3,c1,c2,c3;

  s3 = sin(zang);
  c3 = cos(zang);
 // Ú                 ¿
 // ³ c3  s3   0   0  ³
 // ³-s3  c3   0   0  ³
 // ³  0   0   1   0  ³
 // ³  0   0   0   1  ³
 // À                 Ù
  Matrix_Identity(m);
  m[ 0] = c3;
  m[ 1] = s3;
  m[ 4] = -s3;
  m[ 5] = c3;
 }

 // -----



 // c[i][j] = ä 1ókóq ( a[i][k] * b[k][j] )
 // c[i][j] = sum of a[i][k] * b[k][j] for k varying from 1 to q

 // ABC   123   A1+B4+C7  A2+B5+C8  A3+B6+C9
 // DEF * 456 = D1+E4+F7  D2+E5+F8  D3+E6+C9
 // GHI   789   G1+H4+I7  G2+H5+I8  G3+H6+C9

void Matrix_Mult(float *a, float *b)
 {
  float ab[16];
  int i,j,k;

  for (i=0;i<4;i++)
  {
   for (j=0;j<4;j++)
   {
    ab[j*4+i] = 0.0;
    for (k=0;k<4;k++)
    {
     ab[j*4+i] += a[k*4+i] * b[j*4+k];
    }
   }
  }
  for (i=0;i<16;i++)
  {
   a[i] = ab[i];
  }
 }

 // -----

 // Copies the contents of m2 into m1
void Matrix_Copy(float *m1, float *m2)
 {
  int i;

  for (i=0;i<16;i++)
  {
   m1[i] = m2[i];
  }
 }

 // -----

 // Creates inverse of matrix... stable???
void Matrix_Inverse( float *d, float *s )
 {
  float t[16];

  Matrix_Translation(d,-s[3],-s[7],-s[11]);
  Matrix_Identity(&t);
  t[ 0] = s[ 0];
  t[ 1] = s[ 4];
  t[ 2] = s[ 8];
  t[ 4] = s[ 1];
  t[ 5] = s[ 5];
  t[ 6] = s[ 9];
  t[ 8] = s[ 2];
  t[ 9] = s[ 6];
  t[10] = s[10];
  Matrix_Mult(d,&t);
 }

 // -----

 // Create rotation matrix. angle/axis
void Matrix_AngleAxis ( float *m, float angle, sVector *v )
 {
  float COS,SIN;
  float VxxCos,VyyCos, VzzCos, VxyCos, VxzCos, VyzCos;
  float OneMinCos;
  float VxSin, VySin, VzSin;

  // Get the sine & cosine of the rotation value
  COS = cos(angle);
  SIN = sin(angle);
  OneMinCos = 1-COS;
  // Do the fixed multiplies ahead of time since some are reused.
  // There's definitely room for optimization here when any of the components
  // of V are 0
  VxxCos = (v->x * v->x) * OneMinCos;
  VyyCos = (v->y * v->y) * OneMinCos;
  VzzCos = (v->z * v->z) * OneMinCos;
  VxyCos = (v->x * v->y) * OneMinCos;
  VxzCos = (v->x * v->z) * OneMinCos;
  VyzCos = (v->y * v->z) * OneMinCos;
  VxSin = v->x * SIN;
  VySin = v->y * SIN;
  VzSin = v->z * SIN;
  // Now stuff the matrix

  m[ 0] = COS + VxxCos;
  m[ 1] = VxyCos + VzSin;
  m[ 2] = VxzCos - VySin;
  m[ 3] = 0;

  m[ 4] = VxyCos - VzSin;
  m[ 5] = COS + VyyCos;
  m[ 6] = VyzCos + VxSin;
  m[ 7] = 0;

  m[ 8] = VxzCos + VySin;
  m[ 9] = VyzCos - VxSin;
  m[10] = COS + VzzCos;
  m[11] = 0;

  m[12] = 0;
  m[13] = 0;
  m[14] = 0;
  m[15] = 1;
 }

// ----- Vector -------------------------------------------------------------

 // Normalize vector
void Vector_Normalize(sVector *dest, sVector *v)
 {
  float len,invlen;
  len = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
  if (len!=0)
  {
   invlen = 1/len;
   dest->x = (v->x*invlen);
   dest->y = (v->y*invlen);
   dest->z = (v->z*invlen);
  }
  else
  {
   dest->x = 0; dest->y = 0; dest->z = 0;
  }
 }

 // -----

 // dot product of two vectors
float Vector_Dot(sVector v1, sVector v2)
 {
  return ( v1.x*v2.x + v1.y*v2.y + v1.z*v2.z );
 }

 // -----

 // cross product of two vectors
void Vector_Cross(sVector *dest, sVector v1, sVector v2)
 {
  dest->x = v1.y*v2.z - v1.z*v2.y;
  dest->y = v1.z*v2.x - v1.x*v2.z;
  dest->z = v1.x*v2.y - v1.y*v2.x;
 }

 // -----

 // add two vectors together
void Vector_Add(sVector *dest, sVector v1, sVector v2)
 {
  dest->x = v1.x + v2.x;
  dest->y = v1.y + v2.y;
  dest->z = v1.z + v2.z;
 }

 // -----

 // subtract vector v2 from vector v1
void Vector_Sub(sVector *dest, sVector v1, sVector v2)
 {
  dest->x = v1.x - v2.x;
  dest->y = v1.y - v2.y;
  dest->z = v1.z - v2.z;
 }

 // -----

 // translate vector by matrix
void Vector_Trans(sVector *dest, sVector v, float *m)
 {
  dest->x = v.x*m[ 0] + v.y*m[ 1] + v.z*m[ 2] + m[ 3];
  dest->y = v.x*m[ 4] + v.y*m[ 5] + v.z*m[ 6] + m[ 7];
  dest->z = v.x*m[ 8] + v.y*m[ 9] + v.z*m[10] + m[11];
 }

 // translate vector by matrix (rotation only)
void Vector_Trans3x3(sVector *dest, sVector v, float *m)
 {
  dest->x = v.x*m[ 0] + v.y*m[ 1] + v.z*m[ 2];
  dest->y = v.x*m[ 4] + v.y*m[ 5] + v.z*m[ 6];
  dest->z = v.x*m[ 8] + v.y*m[ 9] + v.z*m[10];
 }

 // -----

 // translate vector by inverse matrix (rotation only)
void Vector_inv_Trans3x3(sVector *dest, sVector v, float *m)
 {
  dest->x = v.x*m[ 0] + v.y*m[ 4] + v.z*m[ 8];
  dest->y = v.x*m[ 1] + v.y*m[ 5] + v.z*m[ 9];
  dest->z = v.x*m[ 2] + v.y*m[ 6] + v.z*m[10];
 }

 // -----

 // return distance between two vectors/points
float Vector_Distance(sVector *v1, sVector *v2)
 {
  float xd,yd,zd;
  float dist;

  xd = (v2->x - v1->x);
  yd = (v2->y - v1->y);
  zd = (v2->z - v1->z);

  dist = xd*xd + yd*yd + zd*zd;
  if (dist!=0) return sqrt(dist); else return 0;
 }

 // -----

 // return length of vector
float Vector_Length(sVector *v1)
 {
  float leng;

  leng = (v1->x*v1->x) + (v1->y*v1->y) + (v1->z*v1->z);

  if (leng!=0.0) return sqrt(leng); else return 0;
 }


// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ²²²²² GFX ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 #include "font.inc"

 #define fontwidth  1
 #define fontheight 8
 unsigned char s[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!.'\"?:-";

 // --------------------------------------------------------------------------

void Line(int x0, int y0, int x1, int y1, char color, char *_B)
 {
  int dx,ix,cx,dy,iy,cy,m,i,pos;

  if (x0<x1) { dx=x1-x0; ix=1; } else { dx=x0-x1; ix=-1; }
  if (y0<y1) { dy=y1-y0; iy=1; } else { dy=y0-y1; iy=-1; }
  if (dx>dy) m=dx; else m=dy;
  cx=cy=(m>>1);
  for (i=0;i<m;i++)
  {
   if ((x0>=MinX)&&(x0<=MaxX)&&(y0>=MinY)&&(y0<=MaxY))
   {
    _B[y0*ScreenWidth+x0] = color;
   }
   if ((cx+=dx)>=m) { cx-=m; x0+=ix; }
   if ((cy+=dy)>=m) { cy-=m; y0+=iy; }
  }
 }

 // --------------------------------------------------------------------------

void DrawChar( int x, int y, unsigned char c, short int color,unsigned short int *_B)
 {
  int i,j;
  unsigned char n;
  unsigned char cc;
  unsigned short int *pscr;

  if (c==' ') return;
  cc = (int)strchr(s,c) - (int)s;
  pscr = &(_B[(y*ScreenWidth)+x]);
 // (short int *)( (int)VesaPtr + ( y * 640 ) * page + x*2 );
  for (i=0;i<fontheight;i++)
  {
   n = FontData[(cc*8)+i];
   if ((n & 128) != 0) pscr[0] = color;
   if ((n &  64) != 0) pscr[1] = color;
   if ((n &  32) != 0) pscr[2] = color;
   if ((n &  16) != 0) pscr[3] = color;
   if ((n &   8) != 0) pscr[4] = color;
   if ((n &   4) != 0) pscr[5] = color;
   if ((n &   2) != 0) pscr[6] = color;
   if ((n &   1) != 0) pscr[7] = color;
   pscr+=ScreenWidth;
  }
 }

 // --------------------------------------------------------------------------

void DrawString(int x1, int y1, char *s, short int color,unsigned short int *_B)
 {
  int i;

  i=0;
  while (s[i] != 0)
  {
   DrawChar(x1,y1,s[i],color,_B);
   x1+=fontwidth*8;
   i++;
  }
 }

 // --------------------------------------------------------------------------

void SetPalette( char *p )
 {
  int i;

  for (i=0;i<256;i++) setrgb(i,p[i*3],p[i*3+1],p[i*3+2]);
 }


void PolyWireFrame( sPolygon *p )
 {
  sVertex *v[3];

  v[0] = p->v[0];
  v[1] = p->v[1];
  v[2] = p->v[2];

  Line(
       float2int(v[0]->PosR.x),
       float2int(v[0]->PosR.y),
       float2int(v[1]->PosR.x),
       float2int(v[1]->PosR.y),15,_Buffer);
  Line(float2int(v[0]->PosR.x),
       float2int(v[0]->PosR.y),
       float2int(v[2]->PosR.x),
       float2int(v[2]->PosR.y),15,_Buffer);
  Line(float2int(v[1]->PosR.x),
       float2int(v[1]->PosR.y),
       float2int(v[2]->PosR.x),
       float2int(v[2]->PosR.y),15,_Buffer);
 }

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ²²²²² Poly ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 int _y1,_y2;
 volatile int _inneru,_innerv,_inneruadd,_innervadd;

typedef struct sEdge
 {
  int x1,x2;
  int u,v;
  int c;
 } sEdge;

 sEdge Edge[200];

// --------------------------------------------------------------------------

void ScanRight( sVertex *v1, sVertex *v2 )
 {
  int x,y1,y2,j;
  int skip;
  float height,width;
  float xslope,prestep;
  int   ixslope;
  sEdge *EdgePtr;

  y1 = CEIL( v1->PosR.y );
  y2 = CEIL( v2->PosR.y );
  if ( y1 == y2 ) return;

  xslope   = (v2->PosR.x - v1->PosR.x) / (v2->PosR.y - v1->PosR.y);
 // xslope   = (v2->PosR.x - v1->PosR.x) * _DivTable[2048.0+float2fixed12_4(v2->PosR.y-v1->PosR.y)];

  ixslope  = float2fixed(xslope);
  prestep = y1 - v1->PosR.y;
  x = float2fixed(v1->PosR.x + (xslope*prestep)) + (1<<15);

  if (y1 > 199) return;
  if (y2 < 0) return;
  if (y1 < 0)
  {
   skip = (0-y1);
   x  += (ixslope*skip);
   y1 = 0;
  }
  if (y2 > 199) y2 = 199;

  if (y1 < _y1) _y1 = y1;
  if (y2 > _y2) _y2 = y2;

  EdgePtr = &(Edge[y1]);
  for (j=y1 ; j<y2 ; j++)
  {
   EdgePtr->x2 = x >> 16;
   x += ixslope;
   EdgePtr++;
  }
 }

 // --------------------------------------------------------------------------

void TexScanLeft( sVertex *v1, sVertex *v2 )
 {
  int x,u,v;
  int skip;
  int j,y1,y2;
  float height, invheight;
  float  xslope, uslope, vslope;
  int   ixslope,iuslope,ivslope;
  float prestep;
  sEdge *EdgePtr;

  y1 = CEIL( v1->PosR.y );
  y2 = CEIL( v2->PosR.y );
  if ( y1 == y2 ) return;
  prestep = y1 - v1->PosR.y;

  height = (v2->PosR.y - v1->PosR.y);
  invheight = _DivTable[float2fixed12_4(height)];
 // invheight = 1/height;

  xslope   = (v2->PosR.x - v1->PosR.x) * invheight;//(v2->PosR.y - v1->PosR.y);

  ixslope  = float2fixed(xslope);
  x = float2fixed(v1->PosR.x + (xslope*prestep)) + (1<<15);

  uslope   = (v2->u - v1->u) * invheight;//(v2->PosR.y - v1->PosR.y);
  iuslope  = float2fixed(uslope);
  u = float2fixed(v1->u + (uslope*prestep)) + (1<<15);

  vslope   = (v2->v - v1->v) * invheight;//(v2->PosR.y - v1->PosR.y);
  ivslope  = float2fixed(vslope);
  v = float2fixed(v1->v + (vslope*prestep)) + (1<<15);
 // ----- clipping -----
  if (y1 > 199) return;
  if (y2 < 0) return;
  if (y1 < 0)
  {
   skip = (0-y1);
   x  += (ixslope*skip);
   u  += (iuslope*skip);
   v  += (ivslope*skip);
   y1 = 0;
  }
  if (y2 > 199) y2 = 199;
 // --------------------
  EdgePtr = &(Edge[y1]);
  for (j=y1 ; j<y2 ; j++)
  {
   EdgePtr->x1 = x >> 16;
   EdgePtr->u  = u;
   EdgePtr->v  = v;
   x += ixslope;
   u += iuslope;
   v += ivslope;
   EdgePtr++;
  }
 }

 // --------------------------------------------------------------------------

void GTexScanLeft( sVertex *v1, sVertex *v2 )
 {
  int x,u,v,c;
  float height,invheight;
  int skip;
  int j,y1,y2;
  float  xslope, uslope, vslope,cslope;
  int   ixslope,iuslope,ivslope,icslope;
  float prestep;
  sEdge *EdgePtr;

  y1 = CEIL( v1->PosR.y );
  y2 = CEIL( v2->PosR.y );
  if ( y1 == y2 ) return;
  prestep = y1 - v1->PosR.y;

  height = (v2->PosR.y - v1->PosR.y);
  invheight = 1/height;
 // invheight = _DivTable[2048.0+float2fixed12_4(height)];

  xslope   = (v2->PosR.x - v1->PosR.x) * invheight;//(v2->PosR.y - v1->PosR.y);
  ixslope  = float2fixed(xslope);
  x = float2fixed(v1->PosR.x + (xslope * prestep)) + (1<<15);

  uslope   = (v2->u - v1->u) * invheight;//(v2->PosR.y - v1->PosR.y);
  iuslope  = float2fixed(uslope);
  u = float2fixed(v1->u + (uslope * prestep)) + (1<<15);

  vslope   = (v2->v - v1->v) * invheight;//(v2->PosR.y - v1->PosR.y);
  ivslope  = float2fixed(vslope);
  v = float2fixed(v1->v + (vslope * prestep)) + (1<<15);

  cslope   = (v2->c - v1->c) * invheight;//(v2->PosR.y - v1->PosR.y);
  icslope  = float2fixed(cslope);
  c = float2fixed(v1->c + (cslope * prestep)) + (1<<15);

  if (y1 > 199) return;
  if (y2 < 0) return;
  if (y1 < 0)
  {
   skip = (0-y1);
   x  += (ixslope*skip);
   u  += (iuslope*skip);
   v  += (ivslope*skip);
   c  += (icslope*skip);
   y1 = 0;
  }
  if (y2 > 199) y2 = 199;

  EdgePtr = &(Edge[y1]);
  for (j=y1 ; j<y2 ; j++)
  {
   EdgePtr->x1 = x >> 16;
   EdgePtr->u  = u;
   EdgePtr->v  = v;
   EdgePtr->c  = c;
   x += ixslope;
   u += iuslope;
   v += ivslope;
   c += icslope;
   EdgePtr++;
  }
 }

 // --------------------------------------------------------------------------

 // eax   vfracADD  temp
 // ebx   ufracADD  vadd:uadd
 // ecx * ufrac
 // edx * TEXTURE    v:u
 // esi * vfrac
 // edi   screen/buffer
 // ebp   length/counter
 void TexInner(int length, char *scrpos, char *texture);
#pragma aux TexInner=\
 " push ebp",\
 " mov ebp,eax",\
 " mov edx,[_Texture1]",\
 " mov eax,[_inneru]",\
 " rol eax,16",\
 " mov ecx,eax",\
 " mov dl,al",\
 " mov eax,[_innerv]",\
 " rol eax,16",\
 " mov esi,eax",\
 " mov dh,al",\
 " mov ebx,[_inneruadd]",\
 " rol ebx,16",\
 " mov eax,[_innervadd]",\
 " rol eax,16",\
 " mov bh,al",\
 "L1:",\
 " mov al,[edx]",\
 " add ecx,ebx",\
 " adc dl,bl",\
 " mov [edi],al",\
 " add esi,eax",\
 " adc dh,bh",\
 " inc edi",\
 " dec ebp",\
 " jnz L1",\
 " pop ebp",\
 parm [eax][edi][esi] modify [eax ebx ecx edx esi edi];

 /*
 ....................
 Gouraud texture

 eax   0        temp
 ebx            temp
 ecx ufrac   cint:cfrac
 edx   0        v:u
 esi vfrac
 edi screen/buffer
 ebp length/counter

 L1:
  mov al,[0x12345678h+edx]  ; texture
  mov ah,ch
  mov bl,[0x12345678h+eax]  ; shadetable
  mov [edi],bl

  add ecx,0x11112222h
  adc dl,0x11h
  add esi,0x11110000h
  adc dh,0x11h

  dec ebp
  jnz L1
 */

 // --------------------------------------------------------------------------

void TexFillPoly( int y1, int y2, int uadd, int vadd )
 {
  int i,j;
  int length,skip;
  int x1,x2;
  int u,v;
  char *ptr;
  char *test;

  if (y1==y2) return;
  if (y1 > 199) return;
  if (y2 < 0) return;
  for ( i=y1;i<y2;i++)
  {
   x1 = Edge[i].x1;
   x2 = Edge[i].x2;
 //  x = 0;
   u = Edge[i].u;
   v = Edge[i].v;

   if (x1 > 319) goto skipthisline;
   if (x2 < 0) goto skipthisline;
   if (x2 > 319) x2 = 319;
   if (x1 < 0)
   {
    skip = (0-x1);
    u += (uadd*skip);
    v += (vadd*skip);
    x1 = 0;
   }
   length = x2 - x1;
   if (length > 0)
   {
 //   _inneru = u;
 //   _innerv = v;
 //   _inneruadd = uadd;
 //   _innervadd = vadd;
    ptr = &(_Buffer[i*320+x1]);
 //   TexInner(length,ptr,_Texture1);
    for (j=0;j<length;j++)
    {
     *ptr++ = _Texture1[((v>>8)&0xff00)+((u>>16)&0xff)];
     u+=uadd;
     v+=vadd;
    }
   }
   skipthisline:;
  }
 }

 // --------------------------------------------------------------------------

void GTexFillPoly( int y1, int y2, int uadd, int vadd, int cadd )
 {
  int i,j;
  int length,skip;
  int x1,x2;
  int u,v,c;
  char *ptr;
  char *test;

  if (y1==y2) return;
  if (y1 > 199) return;
  if (y2 < 0) return;
  for ( i=y1;i<y2;i++)
  {
   x1 = Edge[i].x1;
   x2 = Edge[i].x2;
 //  x = 0;
   u = Edge[i].u;
   v = Edge[i].v;
   c = Edge[i].c;

   if (x1 > 319) goto skipthisline;
   if (x2 < 0) goto skipthisline;
   if (x2 > 319) x2 = 319;
   if (x1 < 0)
   {
    skip = (0-x1);
    u += (uadd*skip);
    v += (vadd*skip);
    c += (cadd*skip);
    x1 = 0;
   }
   length = x2 - x1;
   if (length > 0)
   {
 //   _inneru = u;
 //   _innerv = v;
 //   _inneruadd = uadd;
 //   _innervadd = vadd;
    ptr = &(_Buffer[i*320+x1]);
 //   TexInner(length,ptr,_Texture1);
    for (j=0;j<length;j++)
    {
     *ptr++ = _ShadeTable[((c>>8)&0xff00) + _Texture1[((v>>8)&0xff00)+((u>>16)&0xff)] ];
     u+=uadd;
     v+=vadd;
     c+=cadd;
    }
   }
   skipthisline:;
  }
 }

 // --------------------------------------------------------------------------

void VesaGTexFillPoly( int y1, int y2, int uadd, int vadd, int cadd )
 {
  int i,j;
  int length,skip;
  int x1,x2;
  int u,v,c;
  unsigned short int *ptr;
  char *test;

  if (y1==y2) return;
  if (y1 > 199) return;
  if (y2 < 0) return;
  for ( i=y1;i<y2;i++)
  {
   x1 = Edge[i].x1;
   x2 = Edge[i].x2;
 //  x = 0;
   u = Edge[i].u;
   v = Edge[i].v;
   c = Edge[i].c;

   if (x1 > 319) goto skipthisline;
   if (x2 < 0) goto skipthisline;
   if (x2 > 319) x2 = 319;
   if (x1 < 0)
   {
    skip = (0-x1);
    u += (uadd*skip);
    v += (vadd*skip);
    c += (cadd*skip);
    x1 = 0;
   }
   length = x2 - x1;
   if (length > 0)
   {
 //   _inneru = u;
 //   _innerv = v;
 //   _inneruadd = uadd;
 //   _innervadd = vadd;
    ptr = &(_VesaBuffer[i*320+x1]);
 //   TexInner(length,ptr,_Texture1);
    for (j=0;j<length;j++)
    {
     *ptr++ = _VesaShadeTable[((c>>8)&0xff00) + _Texture1[((v>>8)&0xff00)+((u>>16)&0xff)] ];
     u+=uadd;
     v+=vadd;
     c+=cadd;
    }
   }
   skipthisline:;
  }
 }


 // --------------------------------------------------------------------------

void TexPoly( sPolygon *p )
 {
  int a,b,c;
  sVertex *v[3];
  float delta,divdelta,uadd,vadd;
  float y2_y0,y1_y0;

  float minu,maxu,minv,maxv,d;

  _y1 =  9999;
  _y2 = -9999;

  v[2] = p->v[0];
  v[1] = p->v[1];
  v[0] = p->v[2];

  a = 0;
  if (v[1]->PosR.y < v[a]->PosR.y) a = 1;
  if (v[2]->PosR.y < v[a]->PosR.y) a = 2;
  b = a+1; if (b>2) b=0;
  c = a-1; if (c<0) c=2;
 //  a     a   a b  c a
 //   b   c     c    b    a
 // c       b            c b
  TexScanLeft( v[a],v[c] );
  ScanRight  ( v[a],v[b] );
  if (v[b]->PosR.y < v[c]->PosR.y)
  {
   ScanRight(v[b],v[c]);
  }
  else
  {
   TexScanleft(v[c],v[b]);
  }

  _Texture1 = p->Object->Texture1;

  y1_y0 = (v[1]->PosR.y - v[0]->PosR.y);
  y2_y0 = (v[2]->PosR.y - v[0]->PosR.y);

  delta =   (v[1]->PosR.x - v[0]->PosR.x) * y2_y0  // (v[2]->PosR.y - v[0]->PosR.y)
          - (v[2]->PosR.x - v[0]->PosR.x) * y1_y0; // (v[1]->PosR.y - v[0]->PosR.y);
  divdelta = 1/delta;
  uadd =  ( (v[1]->u - v[0]->u) * y2_y0             // (v[2]->PosR.y - v[0]->PosR.y)
           -(v[2]->u - v[0]->u) * y1_y0 )*divdelta; // (v[1]->PosR.y - v[0]->PosR.y) )*divdelta;
  vadd =  ( (v[1]->v - v[0]->v) * y2_y0             // (v[2]->PosR.y - v[0]->PosR.y)
           -(v[2]->v - v[0]->v) * y1_y0 )*divdelta; // (v[1]->PosR.y - v[0]->PosR.y) )*divdelta;

  TexFillPoly(_y1,_y2,float2fixed(uadd),float2fixed(vadd));
 }

 // --------------------------------------------------------------------------

void GTexPoly( sPolygon *p )
 {
  int a,b,c;
  sVertex *v[3];
  float delta,divdelta,uadd,vadd,cadd ;
  float y2_y0,y1_y0;

  _y1 =  9999;
  _y2 = -9999;

  v[2] = p->v[0];
  v[1] = p->v[1];
  v[0] = p->v[2];

  if ((v[0]->PosR.y>199)&&(v[1]->PosR.y>199)&&(v[2]->PosR.y>199)) return;
  if ((v[0]->PosR.x>319)&&(v[1]->PosR.x>319)&&(v[2]->PosR.x>319)) return;

  a = 0;
  if (v[1]->PosR.y < v[a]->PosR.y) a = 1;
  if (v[2]->PosR.y < v[a]->PosR.y) a = 2;
  b = a+1; if (b>2) b=0;
  c = a-1; if (c<0) c=2;
 //  a     a   a b  c a
 //   b   c     c    b    a
 // c       b            c b
  GTexScanLeft( v[a],v[c] );
  ScanRight  ( v[a],v[b] );
  if (v[b]->PosR.y < v[c]->PosR.y)
  {
   ScanRight(v[b],v[c]);
  }
  else
  {
   GTexScanleft(v[c],v[b]);
  }

  _Texture1 = p->Object->Texture1;

  y1_y0 = (v[1]->PosR.y - v[0]->PosR.y);
  y2_y0 = (v[2]->PosR.y - v[0]->PosR.y);

  delta =   (v[1]->PosR.x - v[0]->PosR.x) * y2_y0  // (v[2]->PosR.y - v[0]->PosR.y)
          - (v[2]->PosR.x - v[0]->PosR.x) * y1_y0; // (v[1]->PosR.y - v[0]->PosR.y);
  divdelta = 1/delta;
  uadd =  ( (v[1]->u - v[0]->u) * y2_y0             // (v[2]->PosR.y - v[0]->PosR.y)
           -(v[2]->u - v[0]->u) * y1_y0 )*divdelta; // (v[1]->PosR.y - v[0]->PosR.y) )*divdelta;
  vadd =  ( (v[1]->v - v[0]->v) * y2_y0             // (v[2]->PosR.y - v[0]->PosR.y)
           -(v[2]->v - v[0]->v) * y1_y0 )*divdelta; // (v[1]->PosR.y - v[0]->PosR.y) )*divdelta;
  cadd =  ( (v[1]->c - v[0]->c) * y2_y0             // (v[2]->PosR.y - v[0]->PosR.y)
           -(v[2]->c - v[0]->c) * y1_y0 )*divdelta; // (v[1]->PosR.y - v[0]->PosR.y) )*divdelta;

// **********
  VesaGTexFillPoly(_y1,_y2,float2fixed(uadd),float2fixed(vadd), float2fixed(cadd));
 }

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ²²²²² Spline ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void Spline_CalcV( sKeySegment *K, float T, float D, sVector *ret)
 {
  float T2,T3;

  T2 = T * T;       // Square of t
  T3 = T2 * T;      // Cube of t
  ret->x = ((K->ApV.x*T3) + (K->BpV.x*T2) + (K->CpV.x*T) + K->DpV.x)/D; // Calc x value
  ret->y = ((K->ApV.y*T3) + (K->BpV.y*T2) + (K->CpV.y*T) + K->DpV.y)/D; // Calc y value
  ret->z = ((K->ApV.z*T3) + (K->BpV.z*T2) + (K->CpV.z*T) + K->DpV.z)/D; // Calc y value
 }

 // -------------------------------------------------------------------------

void Spline_CoeffsV(sKeyFrame *K,int N)
 {
  int a,b,c,d;

  a = N-1;
  b = N;
  c = N+1;
  d = N+2;
  if (a<0) a += K->NumSegments;
  if (c>=K->NumSegments) c -= K->NumSegments;
  if (d>=K->NumSegments) d -= K->NumSegments;

  K->SplineSegments[N].ApV.x =    -(K->SplineSegments[a].PosV.x)
                               + 3*(K->SplineSegments[b].PosV.x)
                               - 3*(K->SplineSegments[c].PosV.x)
                               +   (K->SplineSegments[d].PosV.x);
  K->SplineSegments[N].BpV.x =   2*(K->SplineSegments[a].PosV.x)
                               - 5*(K->SplineSegments[b].PosV.x)
                               + 4*(K->SplineSegments[c].PosV.x)
                               -   (K->SplineSegments[d].PosV.x);
  K->SplineSegments[N].CpV.x = -   (K->SplineSegments[a].PosV.x)
                               +   (K->SplineSegments[c].PosV.x);
  K->SplineSegments[N].DpV.x =   2*(K->SplineSegments[b].PosV.x);

  K->SplineSegments[N].ApV.y =    -(K->SplineSegments[a].PosV.y)
                               + 3*(K->SplineSegments[b].PosV.y)
                               - 3*(K->SplineSegments[c].PosV.y)
                               +   (K->SplineSegments[d].PosV.y);
  K->SplineSegments[N].BpV.y =   2*(K->SplineSegments[a].PosV.y)
                               - 5*(K->SplineSegments[b].PosV.y)
                               + 4*(K->SplineSegments[c].PosV.y)
                               -   (K->SplineSegments[d].PosV.y);
  K->SplineSegments[N].CpV.y = -   (K->SplineSegments[a].PosV.y)
                               +   (K->SplineSegments[c].PosV.y);
  K->SplineSegments[N].DpV.y =   2*(K->SplineSegments[b].PosV.y);

  K->SplineSegments[N].ApV.z =    -(K->SplineSegments[a].PosV.z)
                               + 3*(K->SplineSegments[b].PosV.z)
                               - 3*(K->SplineSegments[c].PosV.z)
                               +   (K->SplineSegments[d].PosV.z);
  K->SplineSegments[N].BpV.z =   2*(K->SplineSegments[a].PosV.z)
                               - 5*(K->SplineSegments[b].PosV.z)
                               + 4*(K->SplineSegments[c].PosV.z)
                               -   (K->SplineSegments[d].PosV.z);
  K->SplineSegments[N].CpV.z = -   (K->SplineSegments[a].PosV.z)
                               +   (K->SplineSegments[c].PosV.z);
  K->SplineSegments[N].DpV.z =   2*(K->SplineSegments[b].PosV.z);

 }

 // -------------------------------------------------------------------------

void Spline_InitV( sKeyFrame *K )
 {
  int i;

  for (i=0;i<K->NumSegments;i++) Spline_CoeffsV(K,i);
 }

 // -------------------------------------------------------------------------

void Spline_GetValuesV( sVector *V, sKeyFrame *K)
 {
  int x1,x2;
  float J;

  x1 = K->CurrentSegment;
  x2 = K->CurrentSegment+1;
  if (x2 >= K->NumSegments) x2-= K->NumSegments;

  J = (K->CurrentPos - (float)K->SplineSegments[x1].TimeStamp)
      / (float)(K->SplineSegments[x2].TimeStamp - K->SplineSegments[x1].TimeStamp);

  Spline_CalcV(&K->SplineSegments[K->CurrentSegment],J,2,V);
 }

 // -------------------------------------------------------------------------
 // -------------------------------------------------------------------------
 // -------------------------------------------------------------------------

void Spline_CalcQ( sKeySegment *K, float T, float D, sQuat *ret)
 {
  float T2,T3;

  T2 = T * T;       // Square of t
  T3 = T2 * T;      // Cube of t
  ret->x = ((K->ApQ.x*T3) + (K->BpQ.x*T2) + (K->CpQ.x*T) + K->DpQ.x)/D; // Calc x value
  ret->y = ((K->ApQ.y*T3) + (K->BpQ.y*T2) + (K->CpQ.y*T) + K->DpQ.y)/D; // Calc y value
  ret->z = ((K->ApQ.z*T3) + (K->BpQ.z*T2) + (K->CpQ.z*T) + K->DpQ.z)/D; // Calc z value
  ret->w = ((K->ApQ.w*T3) + (K->BpQ.w*T2) + (K->CpQ.w*T) + K->DpQ.w)/D; // Calc w value
 }

 // -------------------------------------------------------------------------

void Spline_CoeffsQ(sKeyFrame *K,int N)
 {
  int a,b,c,d;

  a = N-1;
  b = N;
  c = N+1;
  d = N+2;
  if (a<0) a += K->NumSegments;
  if (c>=K->NumSegments) c -= K->NumSegments;
  if (d>=K->NumSegments) d -= K->NumSegments;

  K->SplineSegments[N].ApQ.x =    -(K->SplineSegments[a].PosQ.x)
                               + 3*(K->SplineSegments[b].PosQ.x)
                               - 3*(K->SplineSegments[c].PosQ.x)
                               +   (K->SplineSegments[d].PosQ.x);
  K->SplineSegments[N].BpQ.x =   2*(K->SplineSegments[a].PosQ.x)
                               - 5*(K->SplineSegments[b].PosQ.x)
                               + 4*(K->SplineSegments[c].PosQ.x)
                               -   (K->SplineSegments[d].PosQ.x);
  K->SplineSegments[N].CpQ.x = -   (K->SplineSegments[a].PosQ.x)
                               +   (K->SplineSegments[c].PosQ.x);
  K->SplineSegments[N].DpQ.x =   2*(K->SplineSegments[b].PosQ.x);

  K->SplineSegments[N].ApQ.y =    -(K->SplineSegments[a].PosQ.y)
                               + 3*(K->SplineSegments[b].PosQ.y)
                               - 3*(K->SplineSegments[c].PosQ.y)
                               +   (K->SplineSegments[d].PosQ.y);
  K->SplineSegments[N].BpQ.y =   2*(K->SplineSegments[a].PosQ.y)
                               - 5*(K->SplineSegments[b].PosQ.y)
                               + 4*(K->SplineSegments[c].PosQ.y)
                               -   (K->SplineSegments[d].PosQ.y);
  K->SplineSegments[N].CpQ.y = -   (K->SplineSegments[a].PosQ.y)
                               +   (K->SplineSegments[c].PosQ.y);
  K->SplineSegments[N].DpQ.y =   2*(K->SplineSegments[b].PosQ.y);

  K->SplineSegments[N].ApQ.z =    -(K->SplineSegments[a].PosQ.z)
                               + 3*(K->SplineSegments[b].PosQ.z)
                               - 3*(K->SplineSegments[c].PosQ.z)
                               +   (K->SplineSegments[d].PosQ.z);
  K->SplineSegments[N].BpQ.z =   2*(K->SplineSegments[a].PosQ.z)
                               - 5*(K->SplineSegments[b].PosQ.z)
                               + 4*(K->SplineSegments[c].PosQ.z)
                               -   (K->SplineSegments[d].PosQ.z);
  K->SplineSegments[N].CpQ.z = -   (K->SplineSegments[a].PosQ.z)
                               +   (K->SplineSegments[c].PosQ.z);
  K->SplineSegments[N].DpQ.z =   2*(K->SplineSegments[b].PosQ.z);

  K->SplineSegments[N].ApQ.w =    -(K->SplineSegments[a].PosQ.w)
                               + 3*(K->SplineSegments[b].PosQ.w)
                               - 3*(K->SplineSegments[c].PosQ.w)
                               +   (K->SplineSegments[d].PosQ.w);
  K->SplineSegments[N].BpQ.w =   2*(K->SplineSegments[a].PosQ.w)
                               - 5*(K->SplineSegments[b].PosQ.w)
                               + 4*(K->SplineSegments[c].PosQ.w)
                               -   (K->SplineSegments[d].PosQ.w);
  K->SplineSegments[N].CpQ.w = -   (K->SplineSegments[a].PosQ.w)
                               +   (K->SplineSegments[c].PosQ.w);
  K->SplineSegments[N].DpQ.w =   2*(K->SplineSegments[b].PosQ.w);

 }

 // -------------------------------------------------------------------------

void Spline_InitQ( sKeyFrame *K )
 {
  int i;

  for (i=0;i<K->NumSegments;i++) Spline_CoeffsQ(K,i);
 }

 // -------------------------------------------------------------------------

void Spline_GetValuesQ( sQuat *Q, sKeyFrame *K)
 {
  int x1,x2;
  float J;

  x1 = K->CurrentSegment;
  x2 = K->CurrentSegment+1;
  if (x2 >= K->NumSegments) x2-= K->NumSegments;

  J = (K->CurrentPos - (float)K->SplineSegments[x1].TimeStamp)
      / (float)(K->SplineSegments[x2].TimeStamp - K->SplineSegments[x1].TimeStamp);

  Spline_CalcQ(&K->SplineSegments[K->CurrentSegment],J,2,Q);
 }



 // -------------------------------------------------------------------------

void Spline_Add( sKeyFrame *K )
 {
  int next;

  K->CurrentPos += K->Speed;

  next = K->CurrentSegment+1;
  if (next >= K->NumSegments) next -= K->NumSegments;

  if (K->CurrentPos > (float)K->SplineSegments[next].TimeStamp)
  {
   K->CurrentSegment++;
  }
  // Wrap if reached the end...
  if (K->CurrentSegment == K->NumSegments)
  {
   K->CurrentSegment = 0;
   K->CurrentPos = 0;
  }
 }

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ²²²²² Render ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ


void Light_Calc( sObject *o/*, sWorld *w*/ )
 {
  float tm[16];
  int i,j;
  int flag;
  float c,cc;
  float dist;
  sVector vertexnormal;
  sVector tempvector,lightvector;
  sVertex *V;
  sLight *L;
  int xx,yy;

  for (i=0;i<World->NumLights;i++)
  {
   L = &World->Lights[i];
    // Back-rotate light to object space...
   Matrix_Inverse( &tm,o->m );
   Vector_Trans( &L->PosRobj, L->PosR, &tm );
   Vector_Trans( &L->DirectionRobj, L->DirectionR, &tm );
  }

  // change this to only process _visible_ vertices (marked in backface
  // culling.. The vertices of a visible poly is marked _visible_
  for (i=0;i<o->NumVertices;i++)
  {
   if ((o->Vertices[i].Flags & VERTEX_visible) != 0)
   {

    vertexnormal = o->Vertices[i].Normal;
    c = 0;     // resulting color (0..1)
    for (j=0;j<World->NumLights;j++)
    {
     L = &World->Lights[j];
     flag = L->Flags;
     cc = 0;     // current color intensity

     switch(flag)
     {
      case LIGHT_directional : // ----- Directional light -----
        cc = Vector_Dot(vertexnormal,L->DirectionRobj) * L->Strength;
        if (cc<0) cc = 0;
        c+=cc;
        break;

      case LIGHT_point : // ----- point light source -----
        // tempvector = Light-Vertex
        Vector_Sub(&tempvector, L->PosRobj, o->Vertices[i].Pos);
        // normalize tempvector to get Unit Light normal
        Vector_Normalize(&lightvector,&tempvector);
        // dot product metween Light/Vertex
        cc = Vector_Dot(vertexnormal,lightvector) * L->Strength;
        if (cc<0) cc = 0;
        c+=cc;
        break;

      case LIGHT_point_dist : // ----- point light source w/distance factor -----
        // tempvector = Light-Vertex
        Vector_Sub(&tempvector, L->PosRobj, o->Vertices[i].Pos);
        // normalize tempvector to get Unit Light normal
        Vector_Normalize(&lightvector,&tempvector);
        // dot product metween Light/Vertex
        cc = Vector_Dot(vertexnormal,lightvector) * L->Strength;
        if (cc<0) cc = 0;
        dist = (Vector_Distance(&o->Vertices[i].PosR,&L->PosR));
        dist = (L->MaxRange-dist);
        if (dist > 0)
        {
         cc *= (dist/L->MaxRange);
        }
        c+=cc;
        break;
     }
    }

   }
   c += World->AmbientLight;
   c = c * 127;
   if (c>127) c = 127;
   if (c<0) c = 0;
   o->Vertices[i].c = Intensity[float2int(c)];
  }
 }

 // -------------------------------------------------------------------------

void Handle_Light(sLight *l, sCamera *c)
 {
  switch(l->Flags)
  {
   case LIGHT_point : // ----- Point light source light -----
     Matrix_Identity(l->m);
     l->m[ 3] += l->Pos.x;
     l->m[ 7] += l->Pos.y;
     l->m[11] += l->Pos.z;
     Matrix_Mult(l->m,c->m);
     Vector_Trans( &l->PosR, l->Pos, l->m );
     break;
   case LIGHT_point_dist : // ----- Point light source light w/distance factor -----
     Matrix_Identity(l->m);
     l->m[ 3] += l->Pos.x;
     l->m[ 7] += l->Pos.y;
     l->m[11] += l->Pos.z;
     Matrix_Mult(l->m,c->m);
     Vector_Trans( &l->PosR, l->Pos, l->m );
     break;
   case LIGHT_directional : // ----- Directional light source -----
     Vector_inv_Trans3x3( &l->DirectionR, l->Direction, l->m );

     break;
  }
 }

 // --------------------------------------------------------------------------

void Camera_CreateMatrix_Free(sCamera *c)
 {
  float mm[16];
  Matrix_Translation(c->m,-(c->Pos.x),-(c->Pos.y),-(c->Pos.z));
  Matrix_Rotation(&mm,-(c->Rot.x),-(c->Rot.y),-(c->Rot.z));

  Matrix_Mult(Camera->m,&mm);

 }

 // --------------------------------------------------------------------------

void Camera_CreateMatrix_Aim(sCamera *c)
 {
  float mm[16];
  sVector Vtemp,Up,U,V,N;

  Matrix_Translation(c->m,-(c->Pos.x),-(c->Pos.y),-(c->Pos.z));
  Up = c->UpVector;
 // Up.x = c->Upvector.x
 // Up.x = 0;
 // Up.y = -1;
 // Up.z = 0;
  Vector_Sub(&Vtemp,c->Target,c->Pos);

  Vector_Normalize(&N,&Vtemp);
  Vector_Cross(&U,N,Up);
  Vector_Cross(&V,N,U);

  Matrix_Identity(&mm);
  mm[ 0] = U.x;
  mm[ 1] = U.y;
  mm[ 2] = U.z;

  mm[ 4] = V.x;
  mm[ 5] = V.y;
  mm[ 6] = V.z;

  mm[ 8] = N.x;
  mm[ 9] = N.y;
  mm[10] = N.z;

  Matrix_Mult(c->m,&mm);
 }

 // --------------------------------------------------------------------------

void Camera_CreateMatrix_Roll( sCamera *C )
 {
/*
  sVector view,temp;
  float m1[16];
  float ca,sa;
  float a,b,c,cc,n;

  Matrix_Translation(C->m,-(C->Pos.x),-(C->Pos.y),-(C->Pos.z));

  Vector_Sub(&temp,C->Target,C->Pos);
  Vector_Normalize(&view,&temp);
  a = view.x;
  b = view.y;
  c = view.z;
  cc = c*c;
  if (cc == 1) n = 1; else n = sqrt(1-cc);
  Matrix_Identity(&m1);
  m1[0] =      b/n; m1[4] =     -a/n; m1[ 8] = a;
  m1[1] = -(a*c)/n; m1[5] = -(b*c)/n; m1[ 9] = b;
                    m1[6] =        n; m1[10] = c;
  Matrix_Mult(C->m, &m1);

  ca = (cos(2*PI*C->Roll));
  sa = (sin(2*PI*C->Roll));
  Matrix_Identity(&m1);
  m1[0] =  ca; m1[4] =  sa;
  m1[1] = -sa; m1[5] =  ca;
  Matrix_Mult(C->m, &m1);
*/

 float m1[16];
 sVector focus;
 float d1,d2;
 float ax,ay,az;

 Vector_Sub(&focus,C->Target,C->Pos);

 d1 = sqrt((focus.x * focus.x) + (focus.y * focus.y) + (focus.z * focus.z));
 d2 = sqrt((focus.x * focus.x) + (focus.z * focus.z));

 ax = (asin(focus.y / d1) * 180 / PI );
 ay = (acos(focus.z / d2) * 180 / PI );
 az = (C->Roll);

 Matrix_Translation(&m1,-(C->Pos.x),-(C->Pos.y),-(C->Pos.z));
 Matrix_Rotation(C->m,-ax,-ay,-az);
 Matrix_Mult(C->m,&m1);

}

 // -------------------------------------------------------------------------

void Camera_CreateMatrix_Vue(sCamera *c, int frame)
 {
  float mm[16];
  sVector Vtemp,Up,U,V,N;

  c->Pos.x = c->VUEpos[frame].x;
  c->Pos.y = c->VUEpos[frame].y;
  c->Pos.z = c->VUEpos[frame].z;

  c->Target.x = c->VUEtarget[frame].x;
  c->Target.y = c->VUEtarget[frame].y;
  c->Target.z = c->VUEtarget[frame].z;

  Matrix_Translation(c->m,-(c->Pos.x),-(c->Pos.y),-(c->Pos.z));
  Up = c->UpVector;
 // Up.x = c->Upvector.x
 // Up.x = 0;
 // Up.y = -1;
 // Up.z = 0;
  Vector_Sub(&Vtemp,c->Target,c->Pos);

  Vector_Normalize(&N,&Vtemp);
  Vector_Cross(&U,N,Up);
  Vector_Cross(&V,N,U);

  Matrix_Identity(&mm);
  mm[ 0] = U.x;
  mm[ 1] = U.y;
  mm[ 2] = U.z;

  mm[ 4] = V.x;
  mm[ 5] = V.y;
  mm[ 6] = V.z;

  mm[ 8] = N.x;
  mm[ 9] = N.y;
  mm[10] = N.z;

  Matrix_Mult(c->m,&mm);
 }

 // --------------------------------------------------------------------------

void Object_CreateMatrix(sObject *o, sCamera *c)
 {
  Matrix_Rotation(o->m,o->Rot.x,o->Rot.y,o->Rot.z);
  o->m[ 3] += o->Pos.x;
  o->m[ 7] += o->Pos.y;
  o->m[11] += o->Pos.z;
  Matrix_Mult(o->m,c->m);

  // Transform object center (bounding sphere) to Camera-space
  Vector_Trans(&o->PosR, o->Pos, &c->m);
 }

 // --------------------------------------------------------------------------

void Object_CreateMatrix_Inherit(sObject *o, sCamera *c, sObject *ParentO)
 {
  float mm[16];
  sVector temp;

  Matrix_Rotation(o->m,o->Rot.x,o->Rot.y,o->Rot.z);
  o->m[ 3] += o->Pos.x;
  o->m[ 7] += o->Pos.y;
  o->m[11] += o->Pos.z;
  Matrix_Mult(o->m,ParentO->m);

  // Transform object center (bounding sphere) to Camera-space
  Vector_Trans(&temp, o->Pos, ParentO->m);
  Vector_Trans(&o->PosR, temp, &c->m);
 }

 // --------------------------------------------------------------------------

void Object_CreateMatrix_VUE(sObject *o, sCamera *c, int frame)
 {
  Matrix_Copy(o->m,o->VUElist[frame].m);
  Matrix_Mult(o->m,c->m);
  // Transform object center (bounding sphere) to Camera-space
  Vector_Trans(&o->PosR, o->Pos, &c->m);
 }

 // --------------------------------------------------------------------------

 // Returns 1 if the object's bounding-box is visible in ViewFrustum, else 0
int Object_Visible(sObject *o, sCamera *c)
 {
  sVector vec;
  int xcompare,ycompare;

  if (o->PosR.z + o->BSphereRadius < min_clip_z) return FALSE;
  if (o->PosR.z - o->BSphereRadius > max_clip_z) return FALSE;

  xcompare = (halfscreenwidth_vd * o->PosR.z);
  if (o->PosR.x + o->BSphereRadius < -xcompare) return FALSE;
  if (o->PosR.x - o->BSphereRadius >  xcompare) return FALSE;

  ycompare = (halfscreenheight_vd * o->PosR.z);
  if (o->PosR.y + o->BSphereRadius < -ycompare) return FALSE;
  if (o->PosR.y - o->BSphereRadius >  ycompare) return FALSE;
  return TRUE;
 }


 /*

 // Returns 1 if the object's bounding-box is visible in ViewFrustum, else 0
int Object_Visible(sObject *o, sCamera *c)
 {
  sVector t1,t2;
  float Radius;
  float xcompare,ycompare;

  Radius = (o->BSphereRadius * viewdistance)/o->PosR.z;
  // Radius = o->BSphereRadius;

  if (o->PosR.z + Radius < min_clip_z) return FALSE;
  if (o->PosR.z - Radius > max_clip_z) return FALSE;

  if (o->PosR.x + Radius < -o->PosR.z) return FALSE;
  if (o->PosR.x - Radius >  o->PosR.z) return FALSE;

  if (o->PosR.y + Radius < -o->PosR.z) return FALSE;
  if (o->PosR.y - Radius >  o->PosR.z) return FALSE;

  return TRUE;
 }

 */

 // --------------------------------------------------------------------------
 // ByteSort the "world"-object
 // --------------------------------------------------------------------------
void SortWorldObject( sWorld *World )
 {
  static int Numbers[256];

  int i;
  short int a,b,d;

  // clear numbers array
  memset( Numbers,0,sizeof (Numbers));
 // Count number of each byte-value
  for ( i=0;i<World->VisiblePolys;i++ )
  {
   Numbers[ World->RenderList[i]->z & 255 ]++;
  }
 // Convert from numbers to offset in list to sort
  a = 0;
  for ( i=0;i<256;i++ )
  {
   b = Numbers[i];
   Numbers[i] = a;
   a+=b;
  }
 // Sort according to LOW byte
  for ( i=0;i<World->VisiblePolys;i++ )
  {
   a = World->RenderList[i]->z & 255; //sorttable[i].sortvalue & 255;
   d = Numbers[a];
   World->Sorted[d] = World->RenderList[i];
   Numbers[a]++;
  }
 // -
 // Now, do the same, but with high byte...
 // -
  // clear numbers array
  memset( Numbers,0,sizeof (Numbers));
 // Count number of each byte-value
  for ( i=0;i<World->VisiblePolys;i++ )
  {
   Numbers[ (World->Sorted[i]->z & 0xff00) >> 8 ]++;
  }
 // Convert from numbers to offset in list to sort
  a = 0;
  for ( i=0;i<256;i++ )
  {
   b = Numbers[i];
   Numbers[i] = a;
   a+=b;
  }
 // Sort according to HIGH byte
  for ( i=0;i<World->VisiblePolys;i++ )
  {
   a = (World->Sorted[i]->z & 0xff00) >> 8; //sorttable[i].sortvalue & 255;
   d = Numbers[a];
   World->RenderList[d] = World->Sorted[i];
   Numbers[a]++;
  }
 }

 // --------------------------------------------------------------------------
 // Recursively handle an object and all child- and branch-objects
 // to render "world", start with:
 //   HandleObject(World->Object);
 // --------------------------------------------------------------------------

void Handle_Object( sObject *O, sCamera *C/*, sWorld *W */)
 {
  int i;
  sVector t1,t2;
  sVector tempvector;
  sPolygon *poly;
  sVertex *v1,*v2,*v3;
  float inv_z;

  if ( O == NULL) return;
  if ((O->Flags & OBJECT_active)!=0)
  {
//   if (Object_Visible(O,C) == TRUE)
   {
    World->VisibleObjects++;
    // ------------------------------
    // rotate all vertices of object
    // ------------------------------

    for (i=0;i<O->NumVertices;i++)
    {
     O->Vertices[i].Flags |= -VERTEX_visible;
     Vector_Trans(&tempvector, (O->Vertices[i].Pos), &O->m);
     if (tempvector.z <= 1)// vector.z = 1;
     {
      O->Vertices[i].PosR.z = -1;
     }
     else
     {
      O->Vertices[i].PosR.z = tempvector.z;
      inv_z = 1/tempvector.z;

 //     O->Vertices[i].PosR.x = (tempvector.x * inv_z) + 160;
 //     O->Vertices[i].PosR.y = (tempvector.y * inv_z) + 100;

      O->Vertices[i].PosR.x = ((tempvector.x * C->viewdistance) * inv_z) + 160;
      O->Vertices[i].PosR.y = ((tempvector.y * C->viewdistance) * inv_z) + 100;

      O->Vertices[i].ix = float2int(O->Vertices[i].PosR.x);
      O->Vertices[i].iy = float2int(O->Vertices[i].PosR.y);
      O->Vertices[i].iu = float2int(O->Vertices[i].u) << 10;
      O->Vertices[i].iv = float2int(O->Vertices[i].v) << 10;
      O->Vertices[i].ic = 63;//float2int(O->Vertices[i].c);

     }
    }

    // ------------------------------
    // EnvMap
    // ------------------------------
    if ((O->Flags & OBJECT_envmap)!=0)
    {
     for (i=0;i<O->NumVertices;i++)
     {
      if ((O->Vertices[i].Flags & VERTEX_visible)!=0)
      {

       Vector_Trans3x3(&tempvector, (O->Vertices[i].Normal), &O->m);

       O->Vertices[i].u = tempvector.x * 127 + 128;
       O->Vertices[i].v = tempvector.y * 127 + 128;

       O->Vertices[i].iu = float2int(tempvector.x * 127 + 128);
       O->Vertices[i].iv = float2int(tempvector.y * 127 + 128);
       O->Vertices[i].ic = 63;
      }
     }
    }

//    Light_Calc( O, W );


    // ------------------------------
    // Go through all objects, check if visible, and add to world.renderlist
    // ------------------------------
    poly = O->Polygons;
    for (i=0 ; i<O->NumPolys ; i++)
    {
     v1 = poly->v[0];
     v2 = poly->v[1];
     v3 = poly->v[2];
     // if any of the Z-coordinates are zero or less, skip the entire poly
     // CHANGE THIS!!!! Clip poly if any of the vertices crosses the
     // camera/view plane!!!
     if (    (v1->PosR.z != -1)
          && (v2->PosR.z != -1)
          && (v3->PosR.z != -1))
     {
      // check z-component of face-normal
      if ( (O->Flags & OBJECT_twosided) ||
           (((v2->PosR.x - v1->PosR.x) * (v3->PosR.y - v1->PosR.y))
         - ((v3->PosR.x - v1->PosR.x) * (v2->PosR.y - v1->PosR.y)) <= 0))
      {
       v1->Flags |= VERTEX_visible;
       v2->Flags |= VERTEX_visible;
       v3->Flags |= VERTEX_visible;
       poly->z = (v1->PosR.z + v2->PosR.z + v3->PosR.z);
       World->RenderList[ World->VisiblePolys ] = poly;
       World->VisiblePolys++;
       poly->Object = O;
      }
     }
     poly++;
    }
   }
  }
  Handle_Object( O->Next, C/*, W */);
 }

 // --------------------------------------------------------------------------

 // Renders the entire scene
void Render_Setup( /*sWorld *World*/)
 {
  int i;
  int frame;
  float mm[16];
 // int cc;
  sObject *O;
  sCamera *C;

  C = World->Camera;
  frame = World->FrameNum;

  World->VisiblePolys = 0;
  World->VisibleObjects = 0;

  switch(C->Flags)
  {
   case CAMERA_free : Camera_CreateMatrix_Free( C );
                      break;
   case CAMERA_aim  : Camera_CreateMatrix_Aim( C );
                      break;
   case CAMERA_vue  : C->Pos.x = C->VUEpos[frame].x;
                      C->Pos.y = C->VUEpos[frame].y;
                      C->Pos.z = C->VUEpos[frame].z;
                      C->Target.x = C->VUEtarget[frame].x;
                      C->Target.y = C->VUEtarget[frame].y;
                      C->Target.z = C->VUEtarget[frame].z;
                      C->Roll = ((C->VUEroll[frame])/180)*PI;
                      C->FOV = C->VUEfov[frame];
                      Camera_CreateMatrix_Aim( C );
                      Matrix_AngleAxis(&mm,C->Roll,&C->UpVector);
                      Matrix_Mult(C->m,&mm);
                      break;

  }

  #define aFov (2400/C->FOV)
  C->viewdistance = 160 / ( sin((aFov/360)*PI) / cos((aFov/360)*PI));

/*
  for (i=0;i<World->NumLights;i++)
  {
   Handle_Light( &World->Lights[i], World->Camera );
  }
*/

  O = World->Objects;
  while (O!=NULL)
  {
   // Create object matrix
/*
   if ((O->Flags & OBJECT_VUE)!=0)
   {
    Object_CreateMatrix_VUE(O,World->Camera,World->FrameNum);
   }
   else
   {

    if ((O->Flags&OBJECT_inherit)!=0)
    {
     Object_CreateMatrix_Inherit(O,World->Camera,O->Parent);
    }
    else
    {
     Object_CreateMatrix(O,World->Camera);
    }

   }
*/
   if (O->Parent != NULL)
   {
    Object_CreateMatrix_Inherit(O,World->Camera,O->Parent);
   }
   else
   {
    Object_CreateMatrix(O,World->Camera);
   }
   O = O->Next;
  }

  Handle_Object( World->Objects, World->Camera/*, World*/ );
  SortWorldObject( World );
 }

 // -------------------------------------------------------------------------

void Render_Draw( /*sWorld *World*/ )
 {
  int i;

  for (i=World->VisiblePolys-1 ; i>=0 ; i-- )
  {
 //  GTexPoly(World->RenderList[i]);
 //  WobblePolyGouraudTexture( World->RenderList[i] );
   DrawTextureTriangle( World->RenderList[i] );
  }
 }

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ²²²²² Engine ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

/*

 char *fp;

void f_read(void *buffer,int size)
 {
  memcpy(buffer,fp,size);
  fp+=size;
 }

int FileSize( FILE *fp )
 {
  int pos;
  int size;

  pos = ftell(fp);
  fseek(fp,0,SEEK_END);
  size = ftell(fp);
  fseek(fp,0,SEEK_SET);
  return size;
 }

*/

// ----- Object -------------------------------------------------------------

 struct
 {
  unsigned short chunk_id;
  unsigned int next_chunk;
 } chunk;

 // --------------------

sObject *Load3DS( char *filename, float VertexMult, float UVmult, int center_obj )
 {
  FILE *fp;
  long filelen;
  unsigned int nextobj;
  unsigned int unknown;

  char objname[20];
  unsigned short vertices,polys,num;
  float x,y,z;
  unsigned short int facea,faceb,facec,faceflags;
  float ma,mb,mc,md,me,mf,mg,mh,mi,mj,mk,ml;
  float m1[16],m2[16];
  sVector tempvector;
  // light
  float lightx,lighty,lightz;
  // camera
  float camx,camy,camz;
  float tarx,tary,tarz;
  float bank,lens;

  float u,v;

  char c;
  int i,j;

  unsigned short int nf,flags;
  float temp;

  sObject *returnO;
  sObject *O;
  sKeyFrame *KF;

 // --------------------

  returnO = NULL;
  fp = fopen(filename,"rb");
  filelen = filelength(fileno(fp));
  fread (&chunk,6,1,fp);
  if (chunk.chunk_id != 0x4d4d)
  {
   // printf("Invalid 3DS file!!!\n");
   return NULL;
  }
  else
  {
   while (!feof(fp) && (ftell(fp) < filelen))
   {
    fread (&chunk,6,1,fp);

    switch(chunk.chunk_id)
    {
 // --------------------
     case 0x3d3d : // mesh;
                   break;
     case 0x4000 : // Found object
                   nextobj = ftell(fp) + chunk.next_chunk - 6;
                   // allocate memory for object
                   // O = (sObject *)malloc(sizeof(sObject));
                   if (returnO == NULL)
                   {
                    O = (sObject *)malloc(sizeof(sObject));
                    returnO = O;
                   }
                   else
                   {
                    O->Next = (sObject *)malloc(sizeof(sObject));
                    O->Next->Prev = O;
                    O = O->Next;
                   }
                   // set some default values
                   O->Flags = OBJECT_active;
                   Object_SetPos(O,0.0001,0.0001,0.0001);
                   Object_SetRot(O,0.0001,0.0001,0.0001);
                   O->Parent = NULL;
                   O->Next = NULL;
                   O->NumVertices = 0;
                   O->NumPolys = 0;
 //                  printf("Creating VUE list. %i frames\n",MaxVUEFrames);
                   O->VUElist = (sMatrix *)malloc(MaxVUEFrames*sizeof(sMatrix));
                   // read object name

                   c = 0;
                   do
                   {
                    objname[c] = fgetc(fp);
                    c++;
                   } while (objname[c-1] != 0);
                   strcpy (O->ObjectName,objname);
                   j=0;
                   for (i=0;i<strlen(objname);i++)
                   {
                    if (objname[i] != ' ') O->ObjectName[j++] = objname[i];
                   }
                   O->ObjectName[j] = '\0';

//                   printf("Object found: '%s'\n",objname,O->ObjectName);
                   break;
     case 0x4100 : // Triangular polygon object
                   break;
     case 0x4110 : // vertices
                   fread(&vertices,2,1,fp);
//                   printf("%i vertices...\n",vertices);
                   O->NumVertices = vertices;
                   O->Vertices = (sVertex *)malloc(vertices*sizeof(sVertex));
                   for (i=0;i<vertices;i++)
                   {
                    fread(&x,4,1,fp);
                    fread(&y,4,1,fp);
                    fread(&z,4,1,fp);
                    O->Vertices[i].Pos.x = x * VertexMult;
                    O->Vertices[i].Pos.y = y * VertexMult;
                    O->Vertices[i].Pos.z = z * VertexMult;
                    O->Vertices[i].u = 0;
                    O->Vertices[i].v = 0;
                    O->Vertices[i].c = 63;
                   }
                   break;
     case 0x4120 : // polygons
//                   printf("polys\n");
                   fread(&polys,2,1,fp);
//                   printf("%i faces...\n",polys);
                   O->NumPolys = polys;
                   O->Polygons = (sPolygon *)malloc(polys*sizeof(sPolygon));
                   for (i=0;i<polys;i++)
                   {
                    fread(&facea,2,1,fp);
                    fread(&faceb,2,1,fp);
                    fread(&facec,2,1,fp);
                    fread(&faceflags,2,1,fp);
                    // faceflags: 04,02,01 = ab,bc,ca
                    //            08,10 = uwrap,vwrap
                    O->Polygons[i].v[0] = &O->Vertices[facea];
                    O->Polygons[i].v[1] = &O->Vertices[faceb];
                    O->Polygons[i].v[2] = &O->Vertices[facec];
                    O->Polygons[i].Object = O;
                   }
 // Insert this one, and "increase" pointer to child object
 //                  fseek(fp,nextobj,SEEK_SET);       // ?????
                   break;
     case 0x4140 : // U/V mapping
                   O->Flags |= OBJECT_mapped;
                   fread(&num,2,1,fp);
//                   printf("%i U/V mappings...\n",num);
                   for (i=0;i<num;i++)
                   {
                    fread(&u,4,1,fp);
                    fread(&v,4,1,fp);
                    O->Vertices[i].u = u * UVmult;
                    O->Vertices[i].v = v * UVmult;
                   }
                   break;

     case 0x4160 : // matrix
                   fread(&ma,4,1,fp);
                   fread(&mb,4,1,fp);
                   fread(&mc,4,1,fp);
                   fread(&md,4,1,fp);
                   fread(&me,4,1,fp);
                   fread(&mf,4,1,fp);
                   fread(&mg,4,1,fp);
                   fread(&mh,4,1,fp);
                   fread(&mi,4,1,fp);
                   fread(&mj,4,1,fp);
                   fread(&mk,4,1,fp);
                   fread(&ml,4,1,fp);

/*
                   m1[ 0] =  ma;
                   m1[ 1] =  mg;
                   m1[ 2] =  md;
                   m1[ 3] =  mj;

                   m1[ 4] =  mc;
                   m1[ 5] =  mi;
                   m1[ 6] =  mf;
                   m1[ 7] =  ml;

                   m1[ 8] =  mb;
                   m1[ 9] =  mh;
                   m1[10] =  me;
                   m1[11] =  mk;

                   m1[12] =  0;
                   m1[13] =  0;
                   m1[14] =  0;
                   m1[15] =  1;
*/
                   m1[ 0] =  ma;
                   m1[ 1] =  mb;
                   m1[ 2] =  mg;
                   m1[ 3] =  mj;

                   m1[ 4] =  mb;
                   m1[ 5] =  me;
                   m1[ 6] =  mh;
                   m1[ 7] =  mk;

                   m1[ 8] =  mc;
                   m1[ 9] =  mf;
                   m1[10] =  mi;
                   m1[11] =  ml;

                   m1[12] =  0;
                   m1[13] =  0;
                   m1[14] =  0;
                   m1[15] =  1;

                   Matrix_Inverse(&m2,&m1);
                   for (i=0;i<O->NumVertices;i++)
                   {
                    Vector_Trans(&tempvector,O->Vertices[i].Pos, &m2);
                    O->Vertices[i].Pos = tempvector;
                   }
                   break;
     case 0x4600 : // Light
                   fread(&lightx,4,1,fp);
                   fread(&lighty,4,1,fp);
                   fread(&lightz,4,1,fp);
                   break;
     case 0x4700 : // Camera;
                   fread(&camx,4,1,fp);
                   fread(&camy,4,1,fp);
                   fread(&camz,4,1,fp);
                   fread(&tarx,4,1,fp);
                   fread(&tary,4,1,fp);
                   fread(&tarz,4,1,fp);
                   fread(&bank,4,1,fp);
                   fread(&lens,4,1,fp);
                   World->Camera->Pos.x = camx;
                   World->Camera->Pos.x = camy;
                   World->Camera->Pos.x = camz;
                   World->Camera->Target.x = tarx;
                   World->Camera->Target.x = tarx;
                   World->Camera->Target.x = tarx;
                   World->Camera->Roll = bank;
                   World->Camera->FOV = lens;

                   World->Camera->UpVector.x = 0;
                   World->Camera->UpVector.y = 0;
                   World->Camera->UpVector.z = 1;
                   break;
     case 0xb000 : // Keyframer data
                   break;
     case 0xb002 : // Object framedata
                   break;
     case 0xb003 : // Camera track (pos)
                   break;
     case 0xb004 : // Camera target
                   break;
     case 0xb010 : // object name
                   break;
     case 0xb020 : // translation key
/*
                   for(i=0; i<5; i++) fread(&unknown, 2, 1, fp);
                   fread(&num, 2, 1, fp);
                   // num = number of anim keys
                   KF = &Camera->KeyFramerPos;
                   KF->NumSegments = num;
                   KF->SplineSegments = (sKeySegment *)malloc(num*sizeof(sKeySegment));

                   fread(&unknown, 2, 1, fp);
                   for (i=0;i<num;i++)
                   {
                    fread(&nf, 2, 1, fp);    // frame number
                    fread(&unknown, 2, 1, fp);
                    fread(&flags, 2, 1, fp); // frame flags
                    // Will probably crash if Spline has any flags!!!
                    // any bits set in flags means additionally 1 float
                    for (j=0; j<16; j++)
                    {
                     if (flags & (1 << j))
                     {
                      fread(&temp, 4, 1, fp);
                     }
                    }

                    fread(&x, 4, 1, fp);
                    fread(&y, 4, 1, fp);
                    fread(&z, 4, 1, fp);
                    KF->SplineSegments[i].TimeStamp = nf;
                    KF->SplineSegments[i].Pos.x = x;
                    KF->SplineSegments[i].Pos.y = y;
                    KF->SplineSegments[i].Pos.z = z;
                   }
*/
                   break;
     case 0xb021 : // rotation key
                   break;
     case 0xb022 : // scale key
                   break;
     default : fseek(fp,chunk.next_chunk-6, SEEK_CUR);
 // --------------------
    }
    if (chunk.chunk_id == 0xb001) break;
   }
  }
  // recorse down from returnO...
  O = returnO;
  while (O!=NULL)
  {
   Object_CalcFaceNormals(O);
   Object_CalcVertexNormals(O);
   if (center_obj == TRUE) Object_Center(O);
   Object_CalcBoundingSphere(O);
   // if object not mapped, apply fake spherical mapping :-/
   if ((O->Flags & OBJECT_mapped) == 0)
   {
    for (i=0;i<O->NumVertices;i++)
    {
     O->Vertices[i].u = O->Vertices[i].Normal.x * (UVmult/2) + (UVmult/2);
     O->Vertices[i].v = O->Vertices[i].Normal.y * (UVmult/2) + (UVmult/2);
    }
   }
   O = O->Next;
  }
  return returnO;
 }

 // -------------------------------------------------------------------------

void Object_CalcFaceNormals( sObject *O )
 {
  sVertex *a,*b,*c;
  float ax,ay,az;
  float bx,by,bz;
  float kx,ky,kz;
  float leng;
  int i;

  for (i=0;i<O->NumPolys;i++)
  {
   a = O->Polygons[i].v[0];
   b = O->Polygons[i].v[1];
   c = O->Polygons[i].v[2];

   ax = b->Pos.x - a->Pos.x;
   ay = b->Pos.y - a->Pos.y;
   az = b->Pos.z - a->Pos.z;

   bx = c->Pos.x - a->Pos.x;
   by = c->Pos.y - a->Pos.y;
   bz = c->Pos.z - a->Pos.z;

   kx = (ay*bz)-(az*by);
   ky = (az*bx)-(ax*bz);
   kz = (ax*by)-(ay*bx);

   leng = (sqrt( kx*kx + ky*ky + kz*kz));
   if (leng!=0)
   {
    O->Polygons[i].Normal.x = (kx/leng);
    O->Polygons[i].Normal.y = (ky/leng);
    O->Polygons[i].Normal.z = (kz/leng);
   }
   else
   {
    O->Polygons[i].Normal.x = 0;
    O->Polygons[i].Normal.y = 0;
    O->Polygons[i].Normal.z = 0;
   }
  }
 }

 // -------------------------------------------------------------------------

void Object_CalcVertexNormals( sObject *O )
 {
  int i,a,num;
  sVertex *vi;
  float ax,ay,az;
  float lengd;

  for (i=0;i<O->NumVertices;i++)
  {
   vi = &O->Vertices[i];
   ax = 0;
   ay = 0;
   az = 0;
   num = 0;
   for (a=0;a<O->NumPolys;a++)
   {
    if (    (O->Polygons[a].v[0] == vi)
         || (O->Polygons[a].v[1] == vi)
         || (O->Polygons[a].v[2] == vi) )
    {
     ax += O->Polygons[a].Normal.x;
     ay += O->Polygons[a].Normal.y;
     az += O->Polygons[a].Normal.z;
     num++;
    }
   }
   if ( num != 0 )
   {
    ax = ax / num;
    ay = ay / num;
    az = az / num;
   }
   lengd = sqrt( ax*ax + ay*ay + az*az);
   if ( lengd != 0)
   {
    O->Vertices[i].Normal.x = (ax/lengd);
    O->Vertices[i].Normal.y = (ay/lengd);
    O->Vertices[i].Normal.z = (az/lengd);
   }
   else
   {
    O->Vertices[i].Normal.x = 0;
    O->Vertices[i].Normal.y = 0;
    O->Vertices[i].Normal.z = 0;
   }
  }
 }

 // -------------------------------------------------------------------------

 // Center object around its average X/Y/Z pos
void Object_Center( sObject *O )
 {
  float xmax,xmin,ymax,ymin,zmax,zmin;
  float xmove, ymove, zmove;
  int i;

  xmax = -32767; xmin = 32767;
  ymax = -32767; ymin = 32767;
  zmax = -32767; zmin = 32767;
  for (i=0;i<O->NumVertices;i++)
  {
   if ( O->Vertices[i].Pos.x > xmax ) xmax = O->Vertices[i].Pos.x;
   if ( O->Vertices[i].Pos.x < xmin ) xmin = O->Vertices[i].Pos.x;
   if ( O->Vertices[i].Pos.y > ymax ) ymax = O->Vertices[i].Pos.y;
   if ( O->Vertices[i].Pos.y < ymin ) ymin = O->Vertices[i].Pos.y;
   if ( O->Vertices[i].Pos.z > zmax ) zmax = O->Vertices[i].Pos.z;
   if ( O->Vertices[i].Pos.z < zmin ) zmin = O->Vertices[i].Pos.z;
  }
 // Sphere = xmax;
 // if (ymax > Sphere) Sphere = ymax;
 // if (zmax > Sphere) Sphere = zmax;
 // if (fabs(xmin) > Sphere) Sphere = fabs(xmin);
 // if (fabs(ymin) > Sphere) Sphere = fabs(ymin);
 // if (fabs(zmin) > Sphere) Sphere = fabs(zmin);

  xmove = xmin+((xmax-xmin) /2 );
  ymove = ymin+((ymax-ymin) /2 );
  zmove = zmin+((zmax-zmin) /2 );
  for (i=0;i<O->NumVertices;i++)
  {
   O->Vertices[i].Pos.x-=xmove;
   O->Vertices[i].Pos.y-=ymove;
   O->Vertices[i].Pos.z-=zmove;
  }
  O->Pos.x += xmove;
  O->Pos.y += ymove;
  O->Pos.z += zmove;
 }

 // -------------------------------------------------------------------------

void Object_CalcBoundingSphere( sObject *O )
 {
  int i;
  float sphere,leng;

  sphere = 0;
  for (i=0;i<O->NumVertices;i++)
  {
   leng = Vector_Length(&O->Vertices[i].Pos);
   if ( leng > sphere) sphere = leng;
  }
  O->BSphereRadius = sphere;
 }


 // -------------------------------------------------------------------------

sObject *Object_LoadFile( char *filename )
 {
  FILE *fp;
  sObject *O;
  int i;
  int a,b,c;
  int itemp;
  float ftemp;
  int NumVertices;
  int NumPolys;

  fp = fopen(filename,"rb");
  // check (and ignore) file header
  fread(&itemp,4,1,fp);
  // allocate memory for object structure
  O = (sObject *)malloc(sizeof(sObject));
  // Some default values:
  O->Flags = OBJECT_active;
  Object_SetPos(O,0.0001,0.0001,0.0001);
  Object_SetRot(O,0.0001,0.0001,0.0001);
  O->Parent = NULL;
  O->Next = NULL;
  // read bounding box radius
  fread(&O->BSphereRadius,4,1,fp);
  // read number of vertices
  fread(&NumVertices,4,1,fp);
  O->NumVertices = NumVertices;
  O->Vertices = (sVertex *)malloc(sizeof(sVertex)*NumVertices);
  // read all vertices
  for (i=0;i<NumVertices;i++)
  {
   // read vertex position
   fread(&(O->Vertices[i].Pos.x),4,1,fp);
   fread(&(O->Vertices[i].Pos.y),4,1,fp);
   fread(&(O->Vertices[i].Pos.z),4,1,fp);
   // read vertex U/V
   fread(&(O->Vertices[i].u),4,1,fp);
   fread(&(O->Vertices[i].v),4,1,fp);
   O->Vertices[i].c = 63;
   // read vertex normals
   fread(&(O->Vertices[i].Normal.x),4,1,fp);
   fread(&(O->Vertices[i].Normal.y),4,1,fp);
   fread(&(O->Vertices[i].Normal.z),4,1,fp);
  }
  // read number of polygons in object
  fread(&NumPolys,4,1,fp);
  O->NumPolys = NumPolys;
  O->Polygons = (sPolygon *)malloc(sizeof(sPolygon)*NumPolys);
  // read all polygons
  for (i=0;i<NumPolys;i++)
  {
   fread(&a,4,1,fp);
   fread(&b,4,1,fp);
   fread(&c,4,1,fp);
 // why was the abc order inverted in old version of engine??? Check 3DS!!!
   O->Polygons[i].v[0] = &(O->Vertices[a]);
   O->Polygons[i].v[1] = &(O->Vertices[b]);
   O->Polygons[i].v[2] = &(O->Vertices[c]);
   fread(&(O->Polygons[i].Normal.x),4,1,fp);
   fread(&(O->Polygons[i].Normal.y),4,1,fp);
   fread(&(O->Polygons[i].Normal.z),4,1,fp);
   O->Polygons[i].Object = O;
  }
  fclose(fp);
  return O;
 }

 // --------------------------------------------------------------------------

void Object_Free(sObject *O)
 {
  int i;

  if (O->Next != NULL)  Object_Free(O->Next);
 // if (O->Parent != NULL) Object_Free(O->Parent);
  if (O->Vertices != NULL) free(O->Vertices);
  if (O->Polygons != NULL) free(O->Polygons);
//  if (O->KeyFramerPos != NULL) free(O->KeyFramerPos);
//  if (O->KeyFramerRot != NULL) free(O->KeyFramerPos);
  free(O);
 }

 // --------------------------------------------------------------------------

void Object_SetHierarchy( sObject *O, sObject *parentO, sObject *nextO, int inherit )
 {
  O->Parent = parentO;
  O->Next = nextO;
  if (inherit == TRUE) O->Flags |= OBJECT_inherit;
 }

void Object_SetPos( sObject *o, float x, float y, float z)
 {
  o->Pos.x = x;
  o->Pos.y = y;
  o->Pos.z = z;
 }

void Object_AddPos( sObject *o, float x, float y, float z)
 {
  o->Pos.x += x;
  o->Pos.y += y;
  o->Pos.z += z;
 }

void Object_SetRot( sObject *o, float x, float y, float z)
 {
  o->Rot.x = x;
  o->Rot.y = y;
  o->Rot.z = z;
 }

void Object_AddRot( sObject *o, float x, float y, float z)
 {
  o->Rot.x += x;
  o->Rot.y += y;
  o->Rot.z += z;
 }

sObject *Object_FindByName( sObject *startO, char *name )
 {
  sObject *sO;
  int found;

  sO = startO;
  for (;;)
  {
   if (sO==NULL)
   {
    printf("harra-baillj! Didn't find object \"%s\" in ObjectList\n",name);
    exit(1);
   }
   if (strncmp ( sO->ObjectName , name , 20 ) == 0) goto exitloop;
   sO=sO->Next;
  }
exitloop:
  return sO;

 }

// ----- Camera -------------------------------------------------------------

void Camera_SetPos( sCamera *c, float x, float y, float z)
 {
  c->Pos.x = x;
  c->Pos.y = y;
  c->Pos.z = z;
 }

void Camera_SetRot( sCamera *c, float x, float y, float z)
 {
  c->Rot.x = x;
  c->Rot.y = y;
  c->Rot.z = z;
 }

void Camera_SetTarget( sCamera *c, float x, float y, float z)
 {
  c->Target.x = x;
  c->Target.y = y;
  c->Target.z = z;
 }

void Camera_AddTarget( sCamera *c, float x, float y, float z)
 {
  c->Target.x += x;
  c->Target.y += y;
  c->Target.z += z;
 }

void Camera_AddRot( sCamera *c, float x, float y, float z)
 {
  c->Rot.x += x;
  c->Rot.y += y;
  c->Rot.z += z;
 }

void Camera_AddPos( sCamera *c, float x, float y, float z)
 {
  c->Pos.x += x;
  c->Pos.y += y;
  c->Pos.z += z;
 }

// ----- Init ---------------------------------------------------------------

char ClosestColor(char *palette,char r, char g, char b)
 {
  int i;
  char closest;
  char r1,g1,b1;
  int tc;
  int t;

  closest = 0;
  tc = 0x7FFFFFFF;
  for (i=0;i<256;i++)
  {
   r1 = palette[i*3];
   g1 = palette[i*3+1];
   b1 = palette[i*3+2];
   t = ((r-r1)*(r-r1) + (g-g1)*(g-g1) + (b-b1)*(b-b1));
   if (t < tc)
   {
    closest = i;
    tc = t;
   }
  }
  return closest;
 }

 // --------------------------------------------------------------------------

void Calc_ShadeTable(char *palette, char *ShadeTable)
 {
  int x,y;

  for (x=0;x<64;x++)
  {
   for (y=0;y<256;y++)
   {
    ShadeTable[x*256+y] = ClosestColor(palette,
                                      (palette[y*3]   * x) / 64,
                                      (palette[y*3+1] * x) / 64,
                                      (palette[y*3+2] * x) / 64);
   }
  }
 }

 // --------------------------------------------------------------------------

void Calc_ShadeHighLight(char *palette, char *ShadeTable,int diffuse,
                         float rr, float gg, float bb)
 {
  int x,y;
  float rv,gv,bv;
  float radd,gadd,badd;

  for (x=0;x<256;x++)
  {
   rv = rr;
   gv = gg;
   bv = bb;
   radd = ((float)palette[x*3]   - rr)/ diffuse;
   gadd = ((float)palette[x*3+1] - gg)/ diffuse;
   badd = ((float)palette[x*3+2] - bb)/ diffuse;
   for (y=0;y<diffuse;y++)
   {
    ShadeTable[y*256+x] = ClosestColor(palette,(char)rv,(char)gv,(char)bv);
    rv += radd;
    gv += gadd;
    bv += badd;
   }
  }
  for (x=0;x<256;x++)
  {
   rv = (float)palette[x*3];
   gv = (float)palette[x*3+1];
   bv = (float)palette[x*3+2];
   radd = (63-(float)palette[x*3]+1)   / (63-diffuse+1);
   gadd = (63-(float)palette[x*3+1]+1) / (63-diffuse+1);
   badd = (63-(float)palette[x*3+2]+1) / (63-diffuse+1);
   for (y=diffuse;y<64;y++)
   {
    ShadeTable[y*256+x] = ClosestColor(palette,(char)rv,(char)gv,(char)bv);
    rv += radd;
    gv += gadd;
    bv += badd;
   }
  }
 }

 // --------------------------------------------------------------------------

void Setup_Intensity(int ambient)
 {
  int i;
  float adder,c;

  adder = (float)(128-ambient)/128;
  c = (float)ambient;
  for (i=0;i<128;i++)
  {
   Intensity[i] = (int)(c/2);
   c+=adder;
  }
 }

 // --------------------------------------------------------------------------

void Calc_PhongIntensity(float _L, float _F, float _D, float _S, float _G)
 {
 // L = Intensity of light that sihines on surface
 // F = Intensity of surface itself
 // D = Diffuse reflection level (0-1)
 // S = Specular reflection level (0-1)
 // G = Glossiness factor (1-400)
  int i;
  float realangle = -PI;
  float c;

  for (i = 0; i < 128; i++)
  {
   c = ( _D*cos(realangle)*_L*_F + _S*pow(cos(realangle),_G)*_L ) * 63;
   if (c > 63) c = 63;
   if (c < 0) c = 0;
   Intensity[i] = (unsigned char)c;
   realangle = realangle + ((PI*2)/128);
  }
 //  PhongIntensity[128] = PhongIntensity[127];
 }

 // --------------------------------------------------------------------------

void Calc_PhongMap(void)
 {
  int x,y;
  long dist;
  char *p;

  p = _PhongMap;
  for (y=0;y<256;y++)
  {
   for (x=0;x<256;x++)
   {
    long c;
    int dist;
    // get the distance from the center (128,128) of the map
    dist=sqrt((long)(y-128)*(long)(y-128)+(long)(x-128)*(long)(x-128));
    if (dist>127) dist=127;
    // use the intensity table for the color at a given distance
    *p++=PhongIntensity[127-dist];
   }
  }
 }

 // --------------------------------------------------------------------------

void Calc_GouraudMap(void)
 {
  int x,y;
  char c;

  memset(_PhongMap,0,65536);
  for (y=0;y<256/*127*/;y++)
  {
   for (x=0;x<256/*127*/;x++)
   {
    c = 127 - sqrt((x-128)*(x-128) + (y-128)*(y-128));
    if (c >= 127) c = 127;
    if (c <= 14) c = 14;
    _PhongMap[y*256+x] = c >> 1;
   }
  }
 }

 // -------------------------------------------------------------------------

void VesaCalc_ShadeTable(char *palette, unsigned short int *VesaShadeTable)
 {
  int x,y;
  int r,g,b;

  for (x=0;x<64;x++)
  {
   for (y=0;y<256;y++)
   {
    r = (palette[y*3  ] * x) / 64;
    g = (palette[y*3+1] * x) / 64;
    b = (palette[y*3+2] * x) / 64;
    VesaShadeTable[x*256+y] = ((r >> 1)<<10)
                            + ((g >> 1)<< 5)
                            + ((b >> 1)<< 0);
   }
  }
 }

 // -------------------------------------------------------------------------

void VesaCalc_ShadeHighLight(char *palette, unsigned short int *VesaShadeTable,
                             int diffuse, float rr, float gg, float bb)
 {
  int x,y;
  float rv,gv,bv;
  float radd,gadd,badd;

  for (x=0;x<256;x++)
  {
   rv = rr;
   gv = gg;
   bv = bb;
   radd = ((float)palette[x*3]   - rr)/ diffuse;
   gadd = ((float)palette[x*3+1] - gg)/ diffuse;
   badd = ((float)palette[x*3+2] - bb)/ diffuse;
   for (y=0;y<diffuse;y++)
   {
    VesaShadeTable[y*256+x] = (((int)rv >> 1)<<10)
                            + (((int)gv >> 1)<< 5)
                            + (((int)bv >> 1)<< 0);
    rv += radd;
    gv += gadd;
    bv += badd;
   }
  }
  for (x=0;x<256;x++)
  {
   rv = (float)palette[x*3];
   gv = (float)palette[x*3+1];
   bv = (float)palette[x*3+2];
   radd = (63-(float)palette[x*3]+1)   / (63-diffuse+1);
   gadd = (63-(float)palette[x*3+1]+1) / (63-diffuse+1);
   badd = (63-(float)palette[x*3+2]+1) / (63-diffuse+1);
   for (y=diffuse;y<64;y++)
   {
    VesaShadeTable[y*256+x] = (((int)rv >> 1)<<10)
                            + (((int)gv >> 1)<< 5)
                            + (((int)bv >> 1)<< 0);
    rv += radd;
    gv += gadd;
    bv += badd;
   }
  }
 }

 // -------------------------------------------------------------------------

int Engine_Init(int w, int h, int c, int vesa, int maxpolys, int maxframes)
 {
  MaxVUEFrames = maxframes;

  ScreenWidth = w;
  ScreenHeight = h;
  MinX = 0; MaxX = w-1;
  MinY = 0; MaxY = h-1;

  viewdistance = 280;
  halfscreenwidth_vd =  ((float)(ScreenWidth /2))/viewdistance;
  halfscreenheight_vd = ((float)(ScreenHeight/2))/viewdistance;
  min_clip_z = 10;//viewdistance;
  max_clip_z = 4000;
  // #define maxpolys 3000

  if (vesa==TRUE)
  {
   if (VesaInit(w,h,c) == 0) return FALSE;
   _VesaBuffer = (unsigned short int *)malloc(ScreenWidth*ScreenHeight*2);
   memset(_VesaBuffer,0,64000*2);
   _VesaScreen = (unsigned short int *)VesaPtr;
   _VesaShadeTable = (unsigned short int *)malloc(65536*2);
   memset(_VesaShadeTable,127,65536*2);
  }
  else
  {
   _Buffer = (char *)malloc(ScreenWidth*ScreenHeight);
   memset(_Buffer,0,64000);
   _Screen = (char *)0xA0000;
   _ShadeTable = (char *)malloc(65536);
   memset(_ShadeTable,127,65536);
  }

   World = ( sWorld *)malloc(sizeof( sWorld));
  Camera = (sCamera *)malloc(sizeof(sCamera));

  Camera->VUEpos = (sVector *)malloc(MaxVUEFrames*sizeof(sVector));
  Camera->VUEtarget = (sVector *)malloc(MaxVUEFrames*sizeof(sVector));
  Camera->VUEroll = (float *)malloc(MaxVUEFrames*sizeof(float));
  Camera->VUEfov = (float *)malloc(MaxVUEFrames*sizeof(float));

  World->RenderList = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
  World->Sorted = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));

  return TRUE;
 }

 // -------------------------------------------------------------------------

void Engine_UnInit(int vesa)
 {
  free(World->RenderList);
  free(World->Sorted);

  free(World);
  free(Camera);

  if (vesa==TRUE)
  {
   free(_VesaBuffer);
   free(_VesaShadeTable);

   VesaUnInit();
   SetMode(3);
  }
  else
  {
   free(_Buffer);
   free(_ShadeTable);

   SetMode(3);
  }
 }

 // -------------------------------------------------------------------------

void LoadVUE(char *filename )
 {
	FILE *fp;
  char temp[180];
	char temp2[180];
	char done;
  char ObjectName[20],tempName[20];
	char LightName[20];
  float tempreal;
  unsigned short int rr;
  unsigned short int objnum;
  unsigned short int i;
  unsigned short int j;
  unsigned short int j2;
  unsigned short int r;
	char name[20];
  float x,y,z;
  char c;
  float ma,mb,mc,md,me,mf,mg,mh,mi,mj,mk,ml;

  int StartFrame,EndFrame,FrameNum;
  sObject *O;
  sCamera *C;

  C = Camera;
  if ((fp = fopen(filename,"rt")) == NULL)
  {
   printf("ş Error loading VUE file!\n");
   exit(1);
	}
	StartFrame=0xffff;
	EndFrame=0;
	done=0;
	while (done==0)
	{
   if (fscanf(fp,"%s",name) == EOF)
   {
    done=1;
   }
   else
   {
    if (strcmp(name,"frame")==0)      // is this the frame number?
    {
     fscanf(fp,"%d",&FrameNum);
 //    printf("*** Frame number %i\n",FrameNum);

     if (FrameNum > EndFrame)
     {
      EndFrame=FrameNum;
     }
     if (FrameNum < StartFrame)
     {
      StartFrame=FrameNum;
     }
    }

    if (strcmp(name,"transform")==0)  // is this the transformation matrix?
    {
 //    printf("    transform\n");
     c = 0;
     while (c!='\"') fread(&c,1,1,fp);
     c = 0; j=0;
     while (c!='\"')
     {
      fread(&c,1,1,fp);
      tempName[j++] = c;
     }
     tempName[j] = '\0';

//     fscanf(fp,"%s",tempName);
//     printf("tempName=%s\n",tempName);
     // Remove the yucku "'s
     ObjectName[0] = '\0';
     j=0;
     for (i=0;i<strlen(tempName);i++)
     {
      if ((tempName[i] != '\"')&&(tempName[i] != ' ')) ObjectName[j++] = tempName[i];
     }
     ObjectName[j] = '\0';
//    printf("*** Transform: %s\n",ObjectName);

//     printf("%i: Searching for : %s\n",FrameNum, ObjectName);
//     getch();
     O = Object_FindByName( World->Objects, ObjectName );
//     printf("%s ",O->ObjectName);
//     printf("\n");

     Matrix_Identity( &(O->VUElist[FrameNum].m) );

     fscanf(fp,"%e",&ma);
     fscanf(fp,"%e",&mb);
     fscanf(fp,"%e",&mc);
     fscanf(fp,"%e",&md);
     fscanf(fp,"%e",&me);
     fscanf(fp,"%e",&mf);
     fscanf(fp,"%e",&mg);
     fscanf(fp,"%e",&mh);
     fscanf(fp,"%e",&mi);
     fscanf(fp,"%e",&mj);
     fscanf(fp,"%e",&mk);
     fscanf(fp,"%e",&ml);


     O->VUElist[FrameNum].m[ 0] =  ma;
     O->VUElist[FrameNum].m[ 1] =  md;
     O->VUElist[FrameNum].m[ 2] =  mg;
     O->VUElist[FrameNum].m[ 3] =  mj;

     O->VUElist[FrameNum].m[ 4] =  mb;
     O->VUElist[FrameNum].m[ 5] =  me;
     O->VUElist[FrameNum].m[ 6] =  mh;
     O->VUElist[FrameNum].m[ 7] =  mk;

     O->VUElist[FrameNum].m[ 8] =  mc;
     O->VUElist[FrameNum].m[ 9] =  mf;
     O->VUElist[FrameNum].m[10] =  mi;
     O->VUElist[FrameNum].m[11] =  ml;

     O->VUElist[FrameNum].m[12] =  0;
     O->VUElist[FrameNum].m[13] =  0;
     O->VUElist[FrameNum].m[14] =  0;
     O->VUElist[FrameNum].m[15] =  1;

//     for (i=0;i<4;i++)
//     {
//      for (j=0;j<3;j++)
//      {
//     fscanf(fp,"%e",&tempreal);
//     O->VUElist[FrameNum].m[i+4*j] = tempreal;
//      }
//     }
    }

    if (strcmp(name,"light")==0)      // is this the ligth values?
    {
     fscanf(fp,"%s",LightName);
     for (i=0;i<3;i++)
     {
      fscanf(fp,"%e",&tempreal);
//      LightFrames[FrameNum].Pos[i]=tempreal*SCALER;
     }
     for (i=0;i<3;i++)
     {
      fscanf(fp,"%e",&tempreal);
//      LightFrames[FrameNum].RGBVal[i]=tempreal;
     }
    }

    if (strcmp(name,"camera")==0)     // is this the camera values?
    {
     // read camera's position
     fscanf(fp,"%e",&tempreal);
     C->VUEpos[FrameNum].x = tempreal;
     fscanf(fp,"%e",&tempreal);
     C->VUEpos[FrameNum].y = tempreal;
     fscanf(fp,"%e",&tempreal);
     C->VUEpos[FrameNum].z = tempreal;
     // read camera's target
     fscanf(fp,"%e",&tempreal);
     C->VUEtarget[FrameNum].x = tempreal;
     fscanf(fp,"%e",&tempreal);
     C->VUEtarget[FrameNum].y = tempreal;
     fscanf(fp,"%e",&tempreal);
     C->VUEtarget[FrameNum].z = tempreal;
     // read camera's roll
     fscanf(fp,"%e",&tempreal);
     C->VUEroll[FrameNum] = tempreal;
     // read camera's FOV
     fscanf(fp,"%e",&tempreal);
     C->VUEfov[FrameNum] = tempreal;
    }
   }
  }
  MaxVUEFrames = EndFrame;
  fclose(fp);
 }

 // -------------------------------------------------------------------------

void Apply_VUEmatrix( int frame, sObject *O )
 {
  while (O != NULL)
  {
   Matrix_Copy(O->m,O->VUElist[frame].m);
//   O->mw[ 3] = -O->VUElist[frame].m[ 3];
//   O->mw[ 7] = -O->VUElist[frame].m[11];
//   O->mw[11] = -O->VUElist[frame].m[ 7];

   O = O->Next;
  }
 }

 // --------------------------------------------------------------------------


