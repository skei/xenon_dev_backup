 #include <string.h>       // strchr
 #include <math.h>
 #include <stdio.h>

 #include "engine.h"

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ²²²²² Vars ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

char *_Screen;
char *_Buffer;
char *_Texture1;
char *_Texture2;
char *_ShadeTable;
char *_PhongMap;

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

 // aeim
 // bfjn
 // cgko
 // dhlp

void Matrix_Inverse( float *newmatrix, float *matrix )
 {
  float temp[16];
  float det;
  float invdet;

  #define a matrix[ 0]
  #define b matrix[ 4]
  #define c matrix[ 8]
  #define d matrix[12]
  #define e matrix[ 1]
  #define f matrix[ 5]
  #define g matrix[ 9]
  #define h matrix[13]
  #define i matrix[ 2]
  #define j matrix[ 6]
  #define k matrix[10]
  #define l matrix[14]
  #define m matrix[ 3]
  #define n matrix[ 7]
  #define o matrix[11]
  #define p matrix[15]

  det = a*f*k*p - a*f*l*o - a*j*g*p + a*j*h*o
      + a*n*g*l - a*n*k*h - e*b*k*p + e*b*l*o
      + e*j*c*p - e*j*d*o - e*n*c*l + e*n*d*k
      + i*b*g*p - i*b*h*o - i*f*c*p + i*f*d*o
      + i*n*c*h - i*n*d*g - m*b*g*l + m*b*h*k
      + m*f*c*l - m*f*d*k - m*j*c*h + m*j*d*g;
  invdet = 1/det;

  temp[ 0] = (  f*k*p - f*l*o - j*g*p + j*h*o + n*g*l - n*h*k) * invdet;
  temp[ 1] = (- e*k*p + e*l*o + i*g*p - i*h*o - m*g*l + m*h*k) * invdet;
  temp[ 2] = (  e*j*p - e*l*n - i*f*p + i*h*n + m*f*l - m*h*j) * invdet;
  temp[ 3] = (- e*j*o + e*k*n + i*f*o - i*g*n - m*f*k + m*g*j) * invdet;

  temp[ 4] = (- b*k*p + b*l*o + j*c*p - j*d*o - n*c*l + n*d*k) * invdet;
  temp[ 5] = (  a*k*p - a*l*o - i*c*p + i*d*o + m*c*l - m*d*k) * invdet;
  temp[ 6] = (- a*j*p + a*l*n + i*b*p - i*d*n - m*b*l + m*d*j) * invdet;
  temp[ 7] = (  a*j*o - a*k*n - i*b*o + i*c*n + m*b*k - m*c*j) * invdet;

  temp[ 8] = (  b*g*p - b*h*o - f*c*p + f*d*o + n*c*h - n*d*g) * invdet;
  temp[ 9] = (- a*g*p + a*h*o + e*c*p - e*d*o - m*c*h + m*d*g) * invdet;
  temp[10] = (  a*f*p - a*h*n - e*b*p + e*d*n + m*b*h - m*d*f) * invdet;
  temp[11] = (- a*f*o + a*g*n + e*b*o - e*c*n - m*b*g + m*c*f) * invdet;

  temp[12] = (- b*g*l + b*h*k + f*c*l - f*d*k - j*c*h + j*d*g) * invdet;
  temp[13] = (  a*g*l - a*h*k - e*c*l + e*d*k + i*c*h - i*d*g) * invdet;
  temp[14] = (- a*f*l + a*h*j + e*b*l - e*d*j - i*b*h + i*d*f) * invdet;
  temp[15] = (  a*f*k - a*g*j - e*b*k + e*c*j + i*b*g - i*c*f) * invdet;

  #undef a
  #undef b
  #undef c
  #undef d
  #undef e
  #undef f
  #undef g
  #undef h
  #undef i
  #undef j
  #undef k
  #undef l
  #undef m
  #undef n
  #undef o
  #undef p

  memcpy(newmatrix,temp,sizeof(float)*16);
 }

 // -----

 // [x+4*y]
void Matrix_Inverse2 (float *in, float *out)
 {
  double  scale;

  // Calculate the square of the isotropic scale factor
  scale = in[0+4*0] * in[0+4*0] +
          in[0+4*1] * in[0+4*1] +
          in[0+4*2] * in[0+4*2];
  // Is the submatrix A singular?
  if (scale == 0.0)
  {
   // Matrix M has no inverse
 //  fprintf (stderr, "angle_preserving_matrix4_inverse: singular matrix\n");
   return /*FALSE*/;
  }

  // Calculate the inverse of the square of the isotropic scale factor
  scale = 1.0 / scale;
  // Transpose and scale the 3 by 3 upper-left submatrix
  out[0+4*0] = scale * in[0+4*0];
  out[1+4*0] = scale * in[0+4*1];
  out[2+4*0] = scale * in[0+4*2];
  out[0+4*1] = scale * in[1+4*0];
  out[1+4*1] = scale * in[1+4*1];
  out[2+4*1] = scale * in[1+4*2];
  out[0+4*2] = scale * in[2+4*0];
  out[1+4*2] = scale * in[2+4*1];
  out[2+4*2] = scale * in[2+4*2];

  // Calculate -(transpose(A) / s*s) C
  out[0+4*3] = - ( out[0+4*0] * in[0+4*3] +
                   out[0+4*1] * in[1+4*3] +
                   out[0+4*2] * in[2+4*3] );
  out[1+4*3] = - ( out[1+4*0] * in[0+4*3] +
                   out[1+4*1] * in[1+4*3] +
                   out[1+4*2] * in[2+4*3] );
  out[2+4*3] = - ( out[2+4*0] * in[0+4*3] +
                   out[2+4*1] * in[1+4*3] +
                   out[2+4*2] * in[2+4*3] );

  // Fill in last row
  out[3+4*0] = out[3+4*1] = out[3+4*2] = 0.0;
  out[3+4*3] = 1.0;
  // return TRUE;
 }

// ----- Vector -------------------------------------------------------------

 // Normalize vector
void Vector_Normalize(sVector *dest, sVector *v)
 {
  float len;
  len = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
  if (len!=0)
  {
   dest->x = (v->x/len);
   dest->y = (v->y/len);
   dest->z = (v->z/len);
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

void DrawChar( int x, int y, unsigned char c, short int color,char *_B)
 {
  int i,j;
  unsigned char n;
  unsigned char cc;
  unsigned char *pscr;

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

void DrawString(int x1, int y1, char *s, short int color,char *_B)
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

  GTexFillPoly(_y1,_y2,float2fixed(uadd),float2fixed(vadd), float2fixed(cadd));
 }

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ²²²²² Render ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 /*
void Light_CreateMatrix(sLight *l, sCamera *c)
 {
 // Create Light -> Camera matrix
  Matrix_Rotation(l->m,l->Direction.x,l->Direction.y,l->Direction.z);
  l->m[ 3] += l->Pos.x;
  l->m[ 7] += l->Pos.y;
  l->m[11] += l->Pos.z;
  Matrix_Mult(l->m,c->m);
  Vector_Trans(l->DirectionR,l->Direction
 }
 */

 // -------------------------------------------------------------------------

void Light_Calc( sObject *o, sWorld *w )
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

  for (i=0;i<w->NumLights;i++)
  {
   L = &w->Lights[i];
    // Back-rotate light to object space...
   Matrix_Inverse2( o->O2C, &tm );
   Vector_Trans( &L->PosRobj, L->PosR, &tm );
   Vector_Trans( &L->DirectionRobj, L->DirectionR, &tm );
  }

  // change this to only process _visible_ vertices (marked in backface
  // culling.. The vertices of a visible poly is marked _visible_
  for (i=0;i<o->NumVertices;i++)
  {
   vertexnormal = o->Vertices[i].Normal;
   c = 0;     // resulting color (0..1)
   for (j=0;j<w->NumLights;j++)
   {
    L = &w->Lights[j];
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
       Vector_Sub(&tempvector, o->Vertices[i].Pos, L->PosRobj);
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
       dist = (Vector_Distance(&L->PosRobj,&o->Vertices[i].Pos));
       dist = (L->MaxRange-dist);
       if (dist > 0)
       {
        cc *= (dist/L->MaxRange);
       }
       c+=cc;
       break;
    }
   }
   c = c * 127;
   if (c>127) c = 127;
   if (c<0) c = 0;
   o->Vertices[i].c = PhongIntensity[float2int(c)];
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
     Vector_inv_Trans3x3( &l->DirectionR, l->Direction, c->m );
     break;
  }
 }

 // --------------------------------------------------------------------------

void CalcLight(sObject *o,sWorld *w)
 {
  int i,j;
  int flag;
  float c,cc;
  float dist;
  sVector vertexnormal;
  sVector lightvector;
  sVector tempvector;
  sLight *L;
  int xx,yy;

  for (i=0;i<o->NumVertices;i++)
  {
   Vector_Trans3x3(&vertexnormal,  o->Vertices[i].Normal, &o->O2C);

   c = 0;
   for (j=0;j<w->NumLights;j++)
   {
    L = &w->Lights[j];
    flag = L->Flags;
    cc = 0;

    switch(flag)
    {
     case LIGHT_directional : // ----- Directional light -----
       cc = Vector_Dot(vertexnormal,L->DirectionR) * L->Strength;
       if (cc<0) cc = 0;
       c+=cc;
       break;

     case LIGHT_point : // ----- point light source -----
       // tempvector = Light-Vertex
       Vector_Sub(&tempvector, L->PosR, o->Vertices[i].PosR3D);
       // normalize tempvector to get Unit Light normal
       Vector_Normalize(&lightvector,&tempvector);
       // dot product metween Light/Vertex
       cc = Vector_Dot(vertexnormal,lightvector) * L->Strength;
       if (cc<0) cc = 0;
       c+=cc;
       break;

     case LIGHT_point_dist : // ----- point light source w/distance factor -----
       // tempvector = Light-Vertex
       Vector_Sub(&tempvector, L->PosR, o->Vertices[i].PosR3D);
       // normalize tempvector to get Unit Light normal
       Vector_Normalize(&lightvector,&tempvector);
       // dot product metween Light/Vertex
       cc = Vector_Dot(vertexnormal,lightvector) * L->Strength;
       if (cc<0) cc = 0;
       dist = (Vector_Distance(&L->PosR,&o->Vertices[i].PosR3D));
       dist = (L->MaxRange-dist);
       if (dist > 0)
       {
        cc *= (dist/L->MaxRange);
       }
       c+=cc;
       break;
    }
   }
   c = c * 127;
   if (c>127) c = 127;
   if (c<0) c = 0;
   o->Vertices[i].c = PhongIntensity[float2int(c)];
  }
 }

 // --------------------------------------------------------------------------

void Camera_CreateMatrix_Free(sCamera *c)
 {
  float mm[16];
  Matrix_Translation(c->m,-(c->Pos.x),-(c->Pos.y),-(c->Pos.z));
  Matrix_Rotation(&mm,-(c->Rot.x),-(c->Rot.y),-(c->Rot.z));
  Matrix_Mult(c->m,&mm);
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

void Object_CreateMatrix(sObject *o, sCamera *c)
 {
 // Create Object -> Camera matrix
  Matrix_Rotation(o->O2W,o->Rot.x,o->Rot.y,o->Rot.z);
  o->O2W[ 3] += o->Pos.x;
  o->O2W[ 7] += o->Pos.y;
  o->O2W[11] += o->Pos.z;
  Matrix_Copy(o->O2C,o->O2W);
  Matrix_Mult(o->O2C,c->m);
 }

 // --------------------------------------------------------------------------

void Object_CreateMatrix_Inherit(sObject *o, sCamera *c, sObject *ParentO)
 {
 // Create Object -> Camera matrix
  Matrix_Rotation(o->O2W,o->Rot.x,o->Rot.y,o->Rot.z);
  o->O2W[ 3] += o->Pos.x;
  o->O2W[ 7] += o->Pos.y;
  o->O2W[11] += o->Pos.z;
  Matrix_Mult(o->O2W,ParentO->O2W);
  Matrix_Copy(o->O2C,o->O2W);
  Matrix_Mult(o->O2C,c->m);
 }

 // --------------------------------------------------------------------------

 // Returns 1 if the object's bounding-box is visible in ViewFrustum, else 0
int Object_Visible(sObject *o, sCamera *c)
 {
  sVector s1;
  float Radius;
  float xcompare,ycompare;

  Vector_Trans(&s1, o->Pos, &o->O2C);
  Radius = (o->BSphereRadius * viewdistance)/s1.z;
  Radius = o->BSphereRadius;

  if (s1.z + Radius < min_clip_z) return FALSE;
  if (s1.z - Radius > max_clip_z) return FALSE;

  if (s1.x + Radius < -s1.z) return FALSE;
  if (s1.x - Radius >  s1.z) return FALSE;

  if (s1.y + Radius < -s1.z) return FALSE;
  if (s1.y - Radius >  s1.z) return FALSE;

  return TRUE;
 }

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

void Handle_Object( sObject *O, sCamera *C, sWorld *W )
 {
  int i;
  sVector tempvector;
  sPolygon *poly;
  sVertex *v1,*v2,*v3;
  float inv_z;

  if ( O == NULL) return;
  if ((O->Flags & OBJECT_active)!=0)
  {
   // Create object matrix
   if ((O->Flags&OBJECT_inherit)!=0) Object_CreateMatrix_Inherit(O,C,O->Parent);
   else Object_CreateMatrix(O,C);

 //   if (Object_Visible(O,C) == TRUE)
   {
    W->VisibleObjects++;
    // ------------------------------
    // rotate all vertices of object
    // ------------------------------

    for (i=0;i<O->NumVertices;i++)
    {
     Vector_Trans(&tempvector, (O->Vertices[i].Pos), &O->O2C);
     if (tempvector.z <= 1)// vector.z = 1;
     {
      O->Vertices[i].PosR.z = -1;
     }
     else
     {
      O->Vertices[i].PosR3D.x = tempvector.x;
      O->Vertices[i].PosR3D.y = tempvector.y;
      O->Vertices[i].PosR3D.z = tempvector.z;
      O->Vertices[i].PosR.z = tempvector.z;
 //     tempvector.z += PROJ_VALUE;
      inv_z = 1/tempvector.z;

 //     O->Vertices[i].PosR.x = ((tempvector.x * PROJ_VALUE2) /*/ (tempvector.z) */ * inv_z) + 160;
 //     O->Vertices[i].PosR.y = ((tempvector.y * PROJ_VALUE2) /*/ (tempvector.z) */ * inv_z) + 100;

 //     O->Vertices[i].PosR.x = ((tempvector.x * viewdistance) /*/ (tempvector.z) */ * inv_z) + 160;
 //     O->Vertices[i].PosR.y = ((tempvector.y * viewdistance) /*/ (tempvector.z) */ * inv_z) + 100;

      O->Vertices[i].PosR.x = ((tempvector.x * viewdistance) * inv_z) + 160;
      O->Vertices[i].PosR.y = ((tempvector.y * viewdistance) * inv_z) + 100;
     }
    }


    // ------------------------------
    // EnvMap
    // ------------------------------
    if ((O->Flags & OBJECT_envmap)!=0)
    {
     for (i=0;i<O->NumVertices;i++)
     {
      Vector_Trans3x3(&tempvector, (O->Vertices[i].Normal), &O->O2C);
      O->Vertices[i].u = tempvector.x * 63 + 63;
      O->Vertices[i].v = tempvector.y * 63 + 63;
     }
    }

 //   CalcLight(O,W);
    Light_Calc( O, W );


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
       poly->z = (v1->PosR.z + v2->PosR.z + v3->PosR.z);
       W->RenderList[ W->VisiblePolys ] = poly;
       W->VisiblePolys++;
       poly->Object = O;
      }
     }
     poly++;
    }
   }
  }
 // Handle_Object( O->Child , C, W, O,  TRUE );
 // Handle_Object( O->Branch, C, W, O, FALSE );
  Handle_Object( O->Child, C, W );
 }

 // --------------------------------------------------------------------------

 // Renders the entire scene
void Render( sWorld *World)
 {
  int i;
  int cc;


  World->VisiblePolys = 0;
  World->VisibleObjects = 0;
  switch(World->Camera->Flags)
  {
   case CAMERA_free : Camera_CreateMatrix_Free( World->Camera );
                      break;
   case CAMERA_aim  : Camera_CreateMatrix_Aim( World->Camera );
                      break;
  }

  for (i=0;i<World->NumLights;i++)
  {
   Handle_Light( &World->Lights[i], World->Camera );
  }

  Handle_Object( World->Objects, World->Camera, World );
  SortWorldObject( World );
 // for (i=0 ; i<World.VisiblePolys ; i++ )
  for (i=World->VisiblePolys-1 ; i>=0 ; i-- )
  {
 //  PolyWireFrame( World->RenderList[i] );
   GTexPoly(World->RenderList[i]);
 //  switch(World->RenderList[i]->group->RenderMethod)
  }
  memset(_Buffer+(320*198),1,World->VisibleObjects);
 }

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ²²²²² Engine ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

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

// ----- Object -------------------------------------------------------------

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
  O->Child = NULL;
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

  if (O->Child != NULL)  Object_Free(O->Child);
 // if (O->Parent != NULL) Object_Free(O->Parent);
  if (O->Vertices != NULL) free(O->Vertices);
  if (O->Polygons != NULL) free(O->Polygons);
 }

 // --------------------------------------------------------------------------

void Object_SetHierarchy( sObject *O, sObject *parentO, sObject *childO, int inherit )
 {
  O->Parent = parentO;
  O->Child = childO;
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
   PhongIntensity[i] = (unsigned char)c;
   realangle = realangle + ((PI*2)/128);
  }
  PhongIntensity[128] = PhongIntensity[127];
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


