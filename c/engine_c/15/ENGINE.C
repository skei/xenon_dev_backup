// Engine.c - New, more compact version... Everything in one file ...
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

#include <math.h>
#include <stdio.h>    // file functions
#include <string.h>   // memset

#include "engine.h"

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

// ±±±±± Variables ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

int Numbers[256];
int mul960[600];
sWorld World;           // world
sCamera Camera;         // camera
char *_Buffer;
char *_Texture;         // int polyfiller
char *_ShadeTable;
char *_PhongMap;
unsigned char LightAdd;            // center of PhongMap as adder
float *sine;
float *cosine;
// Poly-filler variables
int _innerTexAdder;
int _innerColorAdder;
int _outerTexAdder;
int _outerColorAdder;
int _outerLeftAdder;
int _outerRightAdder;
int _savedLeft;
int _savedRight;
int _savedTex;
int _savedC;

// ±±±±± Graphics ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void SetPalette(char *palette)
{
 int i;

 for (i=0;i<256;i++) setrgb(i,palette[i*3],palette[i*3+1],palette[i*3+2]);
}

// ±±±±± Poly fillers ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

// ÄÄÄÄÄ The GouraudTexture polyfiller innerloop ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

// eax = temp
// ebx = temp
// ecx = linelength
// edx = TXint:TXfrac - TYint:TYfrac
// esi =           ?  - Cint:Cfrac
// edi = screenpos
// ebp =
void GTinnerloop(char *screenpos, int texpos, int color, int linelength);
#pragma aux GTinnerloop=\
 " cmp ecx,0",\
 " jl skipline",\
 "inner:",\
 " xor eax,eax",\
 " shld eax,edx,8",\
 " mov ah,dh",\
 " add eax,[_Texture]",\
 " movzx ebx,si",\
 " mov bl,[eax]",\
 " add edx,[_innerTexAdder]",\
 " add ebx,[_ShadeTable]",\
 " add esi,[_innerColorAdder]",\
 " mov al,[ebx]",\
 " mov [edi],al",\
 " inc edi",\
 " dec ecx",\
 " jns inner",\
 "skipline:",\
 parm [edi][edx][esi][ecx] modify [eax ebx];

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void GTfiller1(int y1, int y2, int left, int right, int tex, int c)
{
 int i;

 for (i=y1;i<y2;i++)
 {
  GTinnerloop(
               (char *)((int)_Buffer+mul960[i+200]+(left>>16)),
               tex,
               c,
               ((right - left) >> 16)
              );
  tex   += _outerTexAdder;
  c     += _outerColorAdder;
  left  += _outerLeftAdder;
  right += _outerRightAdder;
 }
 _savedLeft = left;
 _savedRight = right;
 _savedTex = tex;
 _savedC = c;
}

// -----

void GTfiller2(int y1, int y2)
{
 int left,right,tex,c;
 int i;

 left  = _savedLeft;
 right = _savedRight;
 tex   = _savedTex;
 c     = _savedC;

 for (i=y1;i<y2;i++)
 {
  GTinnerloop(
               (char *)((int)_Buffer+mul960[i+200]+(left>>16)),
               tex,
               c,
               ((right - left) >> 16)

              );
  tex   += _outerTexAdder;
  c     += _outerColorAdder;
  left  += _outerLeftAdder;
  right += _outerRightAdder;
 }
}


// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void GTpoly(sPolygonR *p)
{
 sVertex *v1,*v2,*v3,*temp;
// sGroup *g;
 int y2_y1, y3_y1, y3_y2;
 int x2_x1, x3_x1, x3_x2;
 int x4_x3, x3_x4;
 int slope12, slope13;
 int X4,U4,V4,C4;

 #define SWAP(a,b) { temp=a; a=b; b=temp; }

 #define X1 (int)v1->Pos.x
 #define Y1 (int)v1->Pos.y
 #define U1 (int)v1->tu
 #define V1 (int)v1->tv
 #define C1 (int)v1->c

 #define X2 (int)v2->Pos.x
 #define Y2 (int)v2->Pos.y
 #define U2 (int)v2->tu
 #define V2 (int)v2->tv
 #define C2 (int)v2->c

 #define X3 (int)v3->Pos.x
 #define Y3 (int)v3->Pos.y
 #define U3 (int)v3->tu
 #define V3 (int)v3->tv
 #define C3 (int)v3->c

 v1 = p->v1;
 v2 = p->v2;
 v3 = p->v3;

 if ( ((unsigned)Y1>199) && ((unsigned)Y2>199) && ((unsigned)Y3>199) ) return;
 if ( ((unsigned)X1>319) && ((unsigned)X2>319) && ((unsigned)X3>319) ) return;

 if ( Y1 > Y2 ) SWAP(v1,v2);
 if ( Y1 > Y3 ) SWAP(v1,v3);
 if ( Y2 > Y3 ) SWAP(v2,v3);

 y3_y1 = (Y3 - Y1);
 if (y3_y1 == 0) return;
 if ((Y1 == Y2) && (X1 > X2)) SWAP(v1,v2);
 if ((Y2 == Y3) && (X2 > X3)) SWAP(v2,v3);

 _Texture = p->group->texture;

// poly is one of the following:
// FlatLeft  FlatRight  FlatTop  FlatBottom
// ----------------------------------------
//  1         1
//   2       2          1 2       1
// 3           3         3       2 3

 y2_y1 = (Y2 - Y1);
 if (y2_y1 == 0) goto FlatTop;
 if ((Y3 - Y2) == 0) goto FlatBottom;
 slope12 = ((X2 - X1)<<16) / y2_y1;
 slope13 = ((X3 - X1)<<16) / y3_y1;
 if (slope12 == slope13) return;
 if ( slope12 > slope13 ) goto FlatLeft;
 goto FlatRight;

// ..........................................................................
 FlatTop:
 // 1 2
 //  3
 {
  x2_x1 = ( X2 - X1 );
  if (x2_x1 == 0) return;
  _innerColorAdder = ((C2-C1)<<8) / x2_x1;
  _innerTexAdder   = ((short int)(((U2-U1)<<8)/x2_x1) << 16)
                    + (short int)(((V2-V1)<<8)/x2_x1);
  _outerTexAdder   = ((short int)(((U3-U1)<<8)/y3_y1) << 16)
                    + (short int)(((V3-V1)<<8)/y3_y1);
  _outerColorAdder = ((C3-C1)<<8) / y3_y1;
  _outerLeftAdder  = ((X3-X1)<<16) / y3_y1;
  _outerRightAdder = ((X3-X2)<<16) / y3_y1;
  GTfiller1(Y1,Y3,(X1<<16),
                  (X2<<16),
                  ((U1<<24)+(V1<<8)),
                  (C1<<8));
  goto FinitoGT;
 }
// ..........................................................................
 FlatBottom:
 //  1
 // 2 3
 {
  x3_x2 = ( X3 - X2 );
  if (x3_x2 == 0) return;
  _innerColorAdder = ((C3-C2)<<8) / x3_x2;
  _innerTexAdder   = ((short int)(((U3-U2)<<8)/x3_x2) << 16)
                    + (short int)(((V3-V2)<<8)/x3_x2);
  _outerTexAdder   = ((short int)(((U2-U1)<<8)/y3_y1) << 16)
                    + (short int)(((V2-V1)<<8)/y3_y1);
  _outerColorAdder = ((C2-C1)<<8) / y3_y1;
  _outerLeftAdder  = ((X2-X1)<<16) / y2_y1;
  _outerRightAdder = ((X3-X1)<<16) / y3_y1;
  GTfiller1(Y1,Y3,(X1<<16)+0x8000,
                  (X1<<16)+0x8000,
                  ((U1<<24)+(V1<<8)),
                  (C1<<8));

  goto FinitoGT;
 }
// ..........................................................................
 FlatLeft:
 //  1            1
 //   2             2
 // 3 (4)          3
 {
  X4 = X1 + (( slope12 * y3_y1) >> 16);
  C4 = C1 + (((C2-C1) * y3_y1) / y2_y1);
  U4 = U1 + (((U2-U1) * y3_y1) / y2_y1);
  V4 = V1 + (((V2-V1) * y3_y1) / y2_y1);

  x4_x3 = ( X4 - X3 );
  if (x4_x3 == 0) return;
  _innerColorAdder = ((C4-C3)<<8) / x4_x3;
  _innerTexAdder   = ((short int)(((U4-U3)<<8)/x4_x3) << 16)
                    + (short int)(((V4-V3)<<8)/x4_x3);
  _outerTexAdder   = ((short int)(((U3-U1)<<8)/y3_y1) << 16)
                    + (short int)(((V3-V1)<<8)/y3_y1);
  _outerColorAdder = ((C3-C1)<<8) / y3_y1;
  _outerLeftAdder  = slope13;
  _outerRightAdder = slope12;
  GTfiller1(Y1,Y2,(X1<<16)+0x8000,
                  (X1<<16)+0x8000,
                  ((U1<<24)+(V1<<8)),
                  (C1<<8));
  _outerRightAdder = ((X3-X2)<<16) / ( Y3 - Y2 );
  GTfiller2(Y2,Y3);
  goto FinitoGT;
 }
// ..........................................................................
 FlatRight:
 //   1
 //  2
 //(4) 3
 {
  X4 = X1 + (( slope12 * y3_y1) >> 16);
  C4 = C1 + (((C2-C1) * y3_y1) / y2_y1);
  U4 = U1 + (((U2-U1) * y3_y1) / y2_y1);
  V4 = V1 + (((V2-V1) * y3_y1) / y2_y1);

  y3_y2 = ( Y3 - Y2 );
  x3_x4 = ( X3 - X4 );
  if (x3_x4 == 0) return;

  _innerColorAdder = ((C3-C4)<<8) / x3_x4;
  _innerTexAdder   = ((short int)(((U3-U4)<<8)/x3_x4) << 16)
                    + (short int)(((V3-V4)<<8)/x3_x4);
  _outerTexAdder   = ((short int)(((U2-U1)<<8)/y2_y1) << 16)
                    + (short int)(((V2-V1)<<8)/y2_y1);
  _outerColorAdder = ((C2-C1)<<8) / y2_y1;
  _outerLeftAdder  = slope12;
  _outerRightAdder = slope13;
  GTfiller1(Y1,Y2,(X1<<16)+0x8000,
                  (X1<<16)+0x8000,
                  ((U1<<24)+(V1<<8)),
                  (C1<<8));
  _outerTexAdder   = ((short int)(((U3-U2)<<8)/y3_y2) << 16)
                    + (short int)(((V3-V2)<<8)/y3_y2);
  _outerColorAdder = ((C3-C2)<<8)  / y3_y2;
  _outerLeftAdder  = ((X3-X2)<<16) / y3_y2;
  GTfiller2(Y2,Y3);
  goto FinitoGT;
 }
// ..........................................................................
 FinitoGT:
 ;
}

// ÄÄÄÄÄ The Texture polyfiller innerloop ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

// eax = temp
// ebx = temp
// ecx = linelength
// edx = TXint:TXfrac - TYint:TYfrac
// esi =
// edi = screenpos
// ebp =
void Tinnerloop(char *screenpos, int texpos, int linelength);
#pragma aux Tinnerloop=\
 " cmp ecx,0",\
 " jl skipline",\
 "inner:",\
 " xor eax,eax",\
 " shld eax,edx,8",\
 " mov ah,dh",\
 " add eax,[_Texture]",\
 " mov bl,[eax]",\
 " mov [edi],bl",\
 " add edx,[_innerTexAdder]",\
 " inc edi",\
 " dec ecx",\
 " jns inner",\
 "skipline:",\
 parm [edi][edx][ecx] modify [eax ebx];

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void Tfiller1(int y1, int y2, int left, int right, int tex)
{
 int i;

 for (i=y1;i<y2;i++)
 {
  Tinnerloop(
               (char *)((int)_Buffer+mul960[i+200]+(left>>16)),
               tex,
               ((right - left) >> 16)

              );
  tex   += _outerTexAdder;
  left  += _outerLeftAdder;
  right += _outerRightAdder;
 }
 _savedLeft = left;
 _savedRight = right;
 _savedTex = tex;
}

// -----

void Tfiller2(int y1, int y2)
{
 int left,right,tex;
 int i;

 left  = _savedLeft;
 right = _savedRight;
 tex   = _savedTex;

 for (i=y1;i<y2;i++)
 {
  Tinnerloop(
               (char *)((int)_Buffer+mul960[i+200]+(left>>16)),
               tex,
               ((right - left) >> 16)

              );
  tex   += _outerTexAdder;
  left  += _outerLeftAdder;
  right += _outerRightAdder;
 }
}


// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void Tpoly(sPolygonR *p)
{
 sVertex *v1,*v2,*v3,*temp;
// sGroup *g;
 int y2_y1, y3_y1, y3_y2;
 int x2_x1, x3_x1, x3_x2;
 int x4_x3, x3_x4;
 int slope12, slope13;
 int X4,U4,V4;

 #define SWAP(a,b) { temp=a; a=b; b=temp; }

 #define X1 (int)v1->Pos.x
 #define Y1 (int)v1->Pos.y
 #define U1 (int)v1->tu
 #define V1 (int)v1->tv

 #define X2 (int)v2->Pos.x
 #define Y2 (int)v2->Pos.y
 #define U2 (int)v2->tu
 #define V2 (int)v2->tv

 #define X3 (int)v3->Pos.x
 #define Y3 (int)v3->Pos.y
 #define U3 (int)v3->tu
 #define V3 (int)v3->tv

 v1 = p->v1;
 v2 = p->v2;
 v3 = p->v3;

 if ( ((unsigned)Y1>199) && ((unsigned)Y2>199) && ((unsigned)Y3>199) ) return;
 if ( ((unsigned)X1>319) && ((unsigned)X2>319) && ((unsigned)X3>319) ) return;

 if ( Y1 > Y2 ) SWAP(v1,v2);
 if ( Y1 > Y3 ) SWAP(v1,v3);
 if ( Y2 > Y3 ) SWAP(v2,v3);

 y3_y1 = (Y3 - Y1);
 if (y3_y1 == 0) return;
 if ((Y1 == Y2) && (X1 > X2)) SWAP(v1,v2);
 if ((Y2 == Y3) && (X2 > X3)) SWAP(v2,v3);

 _Texture = p->group->texture;

// poly is one of the following:
// FlatLeft  FlatRight  FlatTop  FlatBottom
// ----------------------------------------
//  1         1
//   2       2          1 2       1
// 3           3         3       2 3

 y2_y1 = (Y2 - Y1);
 if (y2_y1 == 0) goto FlatTop;
 if ((Y3 - Y2) == 0) goto FlatBottom;
 slope12 = ((X2 - X1)<<16) / y2_y1;
 slope13 = ((X3 - X1)<<16) / y3_y1;
 if (slope12 == slope13) return;
 if ( slope12 > slope13 ) goto FlatLeft;
 goto FlatRight;

// ..........................................................................
 FlatTop:
 // 1 2
 //  3
 {
  x2_x1 = ( X2 - X1 );
  if (x2_x1 == 0) return;
  _innerTexAdder   = ((short int)(((U2-U1)<<8)/x2_x1) << 16)
                    + (short int)(((V2-V1)<<8)/x2_x1);
  _outerTexAdder   = ((short int)(((U3-U1)<<8)/y3_y1) << 16)
                    + (short int)(((V3-V1)<<8)/y3_y1);
  _outerLeftAdder  = ((X3-X1)<<16) / y3_y1;
  _outerRightAdder = ((X3-X2)<<16) / y3_y1;
  Tfiller1(Y1,Y3,(X1<<16),
                  (X2<<16),
                  ((U1<<24)+(V1<<8)));
  goto FinitoT;
 }
// ..........................................................................
 FlatBottom:
 //  1
 // 2 3
 {
  x3_x2 = ( X3 - X2 );
  if (x3_x2 == 0) return;
  _innerTexAdder   = ((short int)(((U3-U2)<<8)/x3_x2) << 16)
                    + (short int)(((V3-V2)<<8)/x3_x2);
  _outerTexAdder   = ((short int)(((U2-U1)<<8)/y3_y1) << 16)
                    + (short int)(((V2-V1)<<8)/y3_y1);
  _outerLeftAdder  = ((X2-X1)<<16) / y2_y1;
  _outerRightAdder = ((X3-X1)<<16) / y3_y1;
  Tfiller1(Y1,Y3,(X1<<16),
                  (X1<<16),
                  ((U1<<24)+(V1<<8)));
  goto FinitoT;
 }
// ..........................................................................
 FlatLeft:
 //  1            1
 //   2             2
 // 3 (4)          3
 {
  X4 = X1 + (( slope12 * y3_y1) >> 16);
  U4 = U1 + (((U2-U1) * y3_y1) / y2_y1);
  V4 = V1 + (((V2-V1) * y3_y1) / y2_y1);

  x4_x3 = ( X4 - X3 );
  if (x4_x3 == 0) return;
  _innerTexAdder   = ((short int)(((U4-U3)<<8)/x4_x3) << 16)
                    + (short int)(((V4-V3)<<8)/x4_x3);
  _outerTexAdder   = ((short int)(((U3-U1)<<8)/y3_y1) << 16)
                    + (short int)(((V3-V1)<<8)/y3_y1);
  _outerLeftAdder  = slope13;
  _outerRightAdder = slope12;
  Tfiller1(Y1,Y2,(X1<<16),
                  (X1<<16),
                  ((U1<<24)+(V1<<8)));
  _outerRightAdder = ((X3-X2)<<16) / ( Y3 - Y2 );
  Tfiller2(Y2,Y3);
  goto FinitoT;
 }
// ..........................................................................
 FlatRight:
 //   1
 //  2
 //(4) 3
 {
  X4 = X1 + (( slope12 * y3_y1) >> 16);
  U4 = U1 + (((U2-U1) * y3_y1) / y2_y1);
  V4 = V1 + (((V2-V1) * y3_y1) / y2_y1);

  y3_y2 = ( Y3 - Y2 );
  x3_x4 = ( X3 - X4 );
  if (x3_x4 == 0) return;

  _innerTexAdder   = ((short int)(((U3-U4)<<8)/x3_x4) << 16)
                    + (short int)(((V3-V4)<<8)/x3_x4);
  _outerTexAdder   = ((short int)(((U2-U1)<<8)/y2_y1) << 16)
                    + (short int)(((V2-V1)<<8)/y2_y1);
  _outerLeftAdder  = slope12;
  _outerRightAdder = slope13;
  Tfiller1(Y1,Y2,(X1<<16),
                  (X1<<16),
                  ((U1<<24)+(V1<<8)));
  _outerTexAdder   = ((short int)(((U3-U2)<<8)/y3_y2) << 16)
                    + (short int)(((V3-V2)<<8)/y3_y2);
  _outerLeftAdder  = ((X3-X2)<<16) / y3_y2;
  Tfiller2(Y2,Y3);
  goto FinitoT;
 }
// ..........................................................................
 FinitoT:
 ;
}

// ±±±±± Math ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

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

void Matrix_Rotation(float *m, int xang, int yang, int zang)
{
 float s1,s2,s3,c1,c2,c3;

 xang &= (TABLESIZE-1);
 yang &= (TABLESIZE-1);
 zang &= (TABLESIZE-1);

 s1 = sine[xang];
 s2 = sine[yang];
 s3 = sine[zang];
 c1 = cosine[xang];
 c2 = cosine[yang];
 c3 = cosine[zang];
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

// ----- Vector -------------------------------------------------------------

// Normalize vector
sVector Vector_Normalize(sVector v)
{
 float len;
 sVector r;
 len = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
 if (len!=0)
 {
  r.x = (v.x/len);
  r.y = (v.y/len);
  r.z = (v.z/len);
 }
 else
 {
	r.x = 0; r.y = 0; r.z = 0;
 }
 return r;
}

// -----

// dot product of two vectors
float Vector_Dot(sVector v1, sVector v2)
{
 return ( v1.x*v2.x + v1.y*v2.y + v1.z*v2.z );
}

// -----

// cross product of two vectors
sVector Vector_Cross(sVector v1, sVector v2)
{
 sVector r;
 r.x = v1.y*v2.z - v1.z*v2.y;
 r.y = v1.z*v2.x - v1.x*v2.z;
 r.z = v1.x*v2.y - v1.y*v2.x;
 return r;
}

// -----

// add two vectors together
sVector Vector_Add(sVector v1, sVector v2)
{
 sVector r;
 r.x = v1.x + v2.x;
 r.y = v1.y + v2.y;
 r.z = v1.z + v2.z;
 return r;
}

// -----

// subtract vector v2 from vector v1
sVector Vector_Sub(sVector v1, sVector v2)
{
 sVector r;
 r.x = v1.x - v2.x;
 r.y = v1.y - v2.y;
 r.z = v1.z - v2.z;
 return r;
}

// -----

// translate vector by matrix
sVector Vector_Trans(sVector v, float *m)
{
 sVector r;
 r.x = v.x*m[ 0] + v.y*m[ 1] + v.z*m[ 2] + m[ 3];
 r.y = v.x*m[ 4] + v.y*m[ 5] + v.z*m[ 6] + m[ 7];
 r.z = v.x*m[ 8] + v.y*m[ 9] + v.z*m[10] + m[11];
 return r;
}

// translate vector by matrix (rotation only)
sVector Vector_Trans3x3(sVector v, float *m)
{
 sVector r;
 r.x = v.x*m[ 0] + v.y*m[ 1] + v.z*m[ 2];
 r.y = v.x*m[ 4] + v.y*m[ 5] + v.z*m[ 6];
 r.z = v.x*m[ 8] + v.y*m[ 9] + v.z*m[10];
 return r;
}

// ±±±±± Init ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

// Calculate sin/cos tables.
void Init_SinCos(void)
{
 int i;
 float realangle = -PI;

// sine = (float *)malloc(TABLESIZE * sizeof(float));     // move to Engine_Init
// cosine = (float *)malloc(TABLESIZE * sizeof(float));
 for (i = 0; i < TABLESIZE; i++)
 {
  sine[i]   = sin(realangle);
  cosine[i] = cos(realangle);
  realangle = realangle + ((PI*2)/TABLESIZE);
 }
}

// --------------------------------------------------------------------------

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
 #ifdef SAVE_SHADING_TABLE
  FILE *fp;
 #endif
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

// --------------------------------------------------------------------------
// unsigned char PhongIntensity[129];

void Calc_PhongIntensity(float L, float F, float D, float S, float G)
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
  c = ( D*cos(realangle)*L*F + S*pow(cos(realangle),G)*L ) * 63;
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
 #ifdef SAVE_PHONG_TABLE
  FILE *fp;
 #endif
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
   c = 127-sqrt((x-128)*(x-128) + (y-128)*(y-128));
   if (c > 127) c = 127;
   if (c < 14) c = 14;

   _PhongMap[y*256+x] = c >> 1;
  }
 }
}

// --------------------------------------------------------------------------

void Engine_Init(int maxpolys)
{
 FILE *fp;
 int i;

 _Buffer = (char *)malloc(64000*9);
 memset(_Buffer,31,(64000*9));
 _ShadeTable= (char *)malloc(32768);
 _PhongMap = (char *)malloc(65536);
 World.RenderList = (sPolygonR **)malloc(maxpolys*sizeof(sPolygonR **));
 World.Sorted     = (sPolygonR **)malloc(maxpolys*sizeof(sPolygonR **));
 sine   = (float *)malloc(TABLESIZE*sizeof(float));
 cosine = (float *)malloc(TABLESIZE*sizeof(float));
 Init_SinCos();
 for (i=0;i<600;i++)
 {
  mul960[i] = (i*960 + 320);
 }
 LightAdd = 128;
}

// --------------------------------------------------------------------------

void Engine_Exit(void)
{
 free(World.RenderList);
 free(World.Sorted);
 free(sine);
 free(cosine);
 free(_Buffer);
 free(_ShadeTable);
 free(_PhongMap);

}

// ±±±±± Light ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

// Real fake light.... Using phongMap and X/Y as indexes into this.
void CalcLight_Camera(sObject *o)
{
 int i;
 int xx,yy;
 sVertex *vertex;
 sVertexR *vertexR;
 sVector vector;

 vertex = o->Vertices;
 vertexR = o->VerticesR;
 for (i=0;i<o->NumVertices;i++)
 {
//  vector = Vector_Trans3x3( vertex->Normal, &o->m);
  vector.x =   vertex->Normal.x*o->m[ 0]
             + vertex->Normal.y*o->m[ 1]
             + vertex->Normal.z*o->m[ 2];
  vector.y =   vertex->Normal.x*o->m[ 4]
             + vertex->Normal.y*o->m[ 5]
             + vertex->Normal.z*o->m[ 6];
  vector.z =   vertex->Normal.x*o->m[ 8]
             + vertex->Normal.y*o->m[ 9]
             + vertex->Normal.z*o->m[10];
  xx = (int)vector.x + LightAdd;
  yy = (int)vector.y + LightAdd;
  vertexR->c = _PhongMap[(yy*256+xx) & 0xFFFF];
  vertex++;
  vertexR++;
 }
}

// -----

void CalcLight_Directional(sObject *o, sGroup *g)
{
 int i,n;
 sVector vector;
 int xx,yy;

 for (i=0;i<g->NumVertices;i++)
 {
  n = g->VertexList[i];
  vector = Vector_Trans3x3( o->Vertices[n].Normal, &o->m);
  vector = Vector_Sub(vector,World.Lights->Rot);
  xx = (int)vector.x + LightAdd;
  yy = (int)vector.y + LightAdd;
  o->VerticesR[n].c = _PhongMap[(yy*256+xx) & 0xFFFF];
 }
}

// -----

// Real fake light.... Using phongMap and X/Y as indexes into this.
void CalcLight_EnvMap(sObject *o, sGroup *g)
{
 int i,n;
 sVector vector;

 for (i=0;i<g->NumVertices;i++)
 {
  n = g->VertexList[i];
  vector = Vector_Trans3x3( o->Vertices[n].Normal, &o->m);
  o->VerticesR[n].tu = ((int)vector.x + LightAdd);
  o->VerticesR[n].tv = ((int)vector.y + LightAdd);
 }
}


// ±±±±± Engine ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void Camera_CreateMatrix_Free(sCamera *c)
{
 float mm[16];
 Matrix_Translation(c->m,-(c->Pos.x),-(c->Pos.y),-(c->Pos.z));
 Matrix_Rotation(&mm,-(c->Rot.x),-(c->Rot.y),-(c->Rot.z));
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
}


// --------------------------------------------------------------------------

// Returns 1 if the object's bounding-box is visible in ViewFrustum, else 0
int Object_CheckBoundingSphere(sObject *o, sCamera *c)
{
 sVector s1,s2;
 float xcompare,ycompare;
 int r;

 r = 1;

 s1.x = o->Pos.x;
 s1.y = o->Pos.y;
 s1.z = o->Pos.z;

 s2 = Vector_Trans(s1, c->m);

 if (s2.z + o->BBoxRadius < minclip_z) r = 0;
 if (s2.z - o->BBoxRadius > maxclip_z) r = 0;

 xcompare = halfscreenwidth_vd * s2.z;
 if (s2.x + o->BBoxRadius < -xcompare) r = 0;
 if (s2.x - o->BBoxRadius >  xcompare) r = 0;

 ycompare = halfscreenheight_vd * s2.z;
 if (s2.y + o->BBoxRadius < -ycompare) r = 0;
 if (s2.y - o->BBoxRadius >  ycompare) r = 0;

 return r;
}

// --------------------------------------------------------------------------
// ByteSort the "world"-object
// --------------------------------------------------------------------------
void SortWorldObject( void )
{
 int i;
 short int a,b,d;

 // clear numbers array
 memset( Numbers,0,sizeof (Numbers));
// Count number of each byte-value
 for ( i=0;i<World.VisiblePolys;i++ )
 {
  Numbers[ World.RenderList[i]->z & 255 ]++;
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
 for ( i=0;i<World.VisiblePolys;i++ )
 {
  a = World.RenderList[i]->z & 255; //sorttable[i].sortvalue & 255;
  d = Numbers[a];
  World.Sorted[d] = World.RenderList[i];
  Numbers[a]++;
 }
// -
// Now, do the same, but with high byte...
// -
 // clear numbers array
 memset( Numbers,0,sizeof (Numbers));
// Count number of each byte-value
 for ( i=0;i<World.VisiblePolys;i++ )
 {
  Numbers[ (World.Sorted[i]->z & 0xff00) >> 8 ]++;
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
 for ( i=0;i<World.VisiblePolys;i++ )
 {
  a = (World.Sorted[i]->z & 0xff00) >> 8; //sorttable[i].sortvalue & 255;
  d = Numbers[a];
  World.RenderList[d] = World.Sorted[i];
  Numbers[a]++;
 }
}

// --------------------------------------------------------------------------
// Recursively handle a group and all sub-groups
// --------------------------------------------------------------------------
void HandleGroup( sObject *o,sGroup *g)
{
 int i,n;
 sPolygonR *polyR;
 sVertex *v1,*v2,*v3;
 sVector vector;

// World.VisiblePolys = 0;
 while (g!=NULL)
 {
// --------------------
// rotate all vertices "attached" to group
// --------------------
  for (i=0;i<g->NumVertices;i++)
  {
   n = g->VertexList[i];
   vector = Vector_Trans( o->Vertices[n].Pos, &o->m);
   if (vector.z <= 0)
   {
    o->VerticesR[n].Pos.z = -1;
   }
   else
   {
    o->VerticesR[n].Pos.z = vector.z;
    vector.z += DISTANCE;
    o->VerticesR[n].Pos.x = ((vector.x * PROJ_PARAM) / (vector.z)) + 160;
    o->VerticesR[n].Pos.y = ((vector.y * PROJ_PARAM) / (vector.z)) + 100;
   }
  }
// --------------------
  switch(g->RenderMethod)
  {
   case RENDER_GouraudTexture : CalcLight_Directional(o,g);
                                break;
   case RENDER_EnvMap         : CalcLight_EnvMap(o,g);
                                break;
  }
// --------------------
  polyR = g->PolygonsR;
  for (i=0 ; i<g->NumPolys ; i++)
  {
   v1 = polyR->v1;
   v2 = polyR->v2;
   v3 = polyR->v3;
   // if any of the Z-coordinates are zero or less, skip the entire poly
   if (    (v1->Pos.z != -1)
        && (v2->Pos.z != -1)
        && (v3->Pos.z != -1))
   {
    // check z-component of face-normal
    if (   (v2->Pos.x - v1->Pos.x) * (v3->Pos.y - v1->Pos.y)
         - (v3->Pos.x - v1->Pos.x) * (v2->Pos.y - v1->Pos.y) <= 0 )

    {
     polyR->z = (int)((v1->Pos.z + v2->Pos.z + v3->Pos.z)*Z_MULT);
     World.RenderList[ World.VisiblePolys ] = polyR;
     World.VisiblePolys++;
     polyR->group = g;
    }
   }
   polyR++;
  }
  g = g->Next;
 }
}

// --------------------------------------------------------------------------
// Recursively handle an object and all child- and branch-objects
// to render "world", start with:
//   HandleObject(World->Object);
// --------------------------------------------------------------------------
void HandleObject( sObject *o)
{
 int i,j;
 sVector Light;

 if ( o == NULL) return;
// if (Object_CheckBoundingSphere( o, World.Camera) == 1)
 {
  if (o->Flags & OBJECT_Active != 0)
  {
   Object_CreateMatrix( o, World.Camera );
   HandleGroup( o,o->Groups );
  }
 }
 HandleObject( o->Child  );
 HandleObject( o->Branch );
}

// --------------------------------------------------------------------------

// Renders the entire scene
void Render(void)
{
 int i;
 int cc;

 World.VisiblePolys = 0;
 switch(World.cameratype)
 {
  case 0 : Camera_CreateMatrix_Free( World.Camera );
           break;
//  case 1 : Camera_CreateMatrix_Aim( World.Camera );
//           break;
 }
 HandleObject( World.Objects );
 SortWorldObject();
// for (i=0 ; i<World.VisiblePolys ; i++ )
 for (i=World.VisiblePolys-1 ; i>=0 ; i-- )
 {
  switch(World.RenderList[i]->group->RenderMethod)
  {
   case RENDER_GouraudTexture : GTpoly( World.RenderList[i] );
                                break;
   case RENDER_EnvMap         : Tpoly( World.RenderList[i] );
                                break;
  }
 }
}

// ±±±±± API ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

// ----- Object -------------------------------------------------------------

/*
 O <- Object
 | <- Child
 O---O---O
 |   |   |   --- Branch
 O-O O-O O  |
 |       |  v
 O-O     O-O-O

3DO file format

 char header[4]     // '3DO'+#0
 int  NumObjs       // Number of objects in file (ignored)
 int  Sphere        // bounding sphere radius
 int  NumVertices   // Number of vertices in object
 {
  short int v.x
  short int v.y
  short int v.z
 }
 int  NumTEXVert    // Number of tex vertices (U/V)
 {
  char u
  char v
 }
 int NumGroups      // Number of groups in object (ignored, only 1 used)
 {
  int RenderMethod  // Group's render method
  int NumPolys      // Number of polygons in group
  {
   short int a
   short int b
   short int c
  }
 }
*/

sObject *Object_Load(char *s)
{
 FILE *fp;
 int i,j,k;
 int temp;
 short int stemp;
 char texname[12];
 sObject *O;
 sVertex *V;
 sVertexR *RV;
 sPolygon *P;
 sPolygonR *RP;
 sGroup *G;
 int Num;
 int NumObjects;
 int NumGroups;
 int NumVerticesInGroup;
 int pp,vv,nn;
 char u,v;
 short int *PolyData;

 fp = fopen(s,"rb");
 // Read header
 fread(&temp,4,1,fp);
 // Check if header is valid
 if ( temp != 0x33444f00 )
 {
  printf("Error! %s is not a valid 3DO file!",s);
  fclose(fp);
  exit(1);
 }
 // Read number of objects in file, but ignore it for now
 fread(&NumObjects,4,1,fp);
 // Allocate space for Object
 O = (sObject *)malloc(sizeof(sObject));
 O->Child = NULL;
 O->Branch = NULL;
 O->Flags = OBJECT_Active;
 O->Pos.x = 0;  O->Pos.y = 0;  O->Pos.z = 0;
 O->Rot.x = 0;  O->Rot.y = 0;  O->Rot.z = 0;
 // Read Bounding sphere radius
 fread(&temp,4,1,fp);
 O->BBoxRadius = temp;//65536;
 // Read number of vertices
 fread(&Num,4,1,fp);
 O->NumVertices = Num;
 // Allocate space for vertices
 V = (sVertex *)malloc(Num*sizeof(sVertex));
 O->Vertices = V;
 RV = (sVertexR *)malloc(Num*sizeof(sVertexR));
 O->VerticesR = RV;
 // Read all POS vertices
 for (i=0;i<Num;i++)
 {
  fread(&stemp,2,1,fp);
  V[i].Pos.x = stemp;
  fread(&stemp,2,1,fp);
  V[i].Pos.y = stemp;
  fread(&stemp,2,1,fp);
  V[i].Pos.z = stemp;
  V[i].tu = 0;
  V[i].tv = 0;
  V[i].c = 63;
 }
 // Read number of TEX vertices
 fread(&Num,4,1,fp);
 // Read all TEX vertices
 for (i=0;i<Num;i++)
 {
  fread(&u,1,1,fp);
  V[i].tu = u;
  RV[i].tu = u;
  fread(&v,1,1,fp);
  V[i].tv = v;
  RV[i].tv = v;
 }
 // Read number of vertex normals
 fread(&Num,4,1,fp);
 // Read all TEX vertices
 for (i=0;i<Num;i++)
 {
  fread(&stemp,2,1,fp);
  V[i].Normal.x = stemp;
  fread(&stemp,2,1,fp);
  V[i].Normal.y = stemp;
  fread(&stemp,2,1,fp);
  V[i].Normal.z = stemp;
 }
 // Read number of groups in object. Ignore for now. Use only 1-group objects
 fread(&NumGroups,4,1,fp);
 G = (sGroup *)malloc(Num*sizeof(sGroup));
 O->Groups = G;
 for (j=0;j<NumGroups;j++)
 {
  // Read group's render method
  fread(&G[j].RenderMethod,4,1,fp);
  G[j].Flags = 0;
  // Read number of polygons in group
  fread(&Num,4,1,fp);

  G[j].NumPolys = Num;
  P = (sPolygon *)malloc(Num*sizeof(sPolygon));
  G[j].Polygons = P;
  RP = (sPolygonR *)malloc(Num*sizeof(sPolygonR));
  G[j].PolygonsR = RP;
  PolyData = (short int *)malloc(Num*sizeof(short int)*3);
  fread(PolyData,2,Num*3,fp);
  for (i=0;i<Num;i++)
  {
   stemp = PolyData[i*3];
   P[i].v1 = &V[stemp];
   RP[i].v1 = &RV[stemp];
   stemp = PolyData[i*3+1];
   P[i].v2 = &V[stemp];
   RP[i].v2 = &RV[stemp];
   stemp = PolyData[i*3+2];
   P[i].v3 = &V[stemp];
   RP[i].v3 = &RV[stemp];
   RP[i].group = G;
  }
// --------------------
// set up VerticeList
  // count number of vertices in group
  NumVerticesInGroup=0;
  for (vv=0;vv<O->NumVertices;vv++)
  {
   for (pp=0;pp<G[j].NumPolys;pp++)
   {
    if ((PolyData[pp*3]==vv) || (PolyData[pp*3+1]==vv) || (PolyData[pp*3+2]==vv))
    {
     NumVerticesInGroup++;
     break;
    }
   }
  }
  G[j].NumVertices = NumVerticesInGroup;
  G[j].VertexList = (int *)malloc(NumVerticesInGroup*sizeof(int));
  nn=0;
  for (vv=0;vv<O->NumVertices;vv++)
  {
   for (pp=0;pp<G[j].NumPolys;pp++)
   {
    if ((PolyData[pp*3]==vv) || (PolyData[pp*3+1]==vv) || (PolyData[pp*3+2]==vv))
    {
     G[j].VertexList[nn] = vv;
     nn++;
     break;
    }
   }
  }
  free(PolyData);
// --------------------
  if ((j+1)!=NumGroups) G[j].Next = &(G[j+1]); else G[j].Next = NULL;
 }
 fclose(fp);
 return O;
}

// --------------------------------------------------------------------------

void Object_Free(sObject *o)
{
 sGroup *g,*gnext;
/*
 free:
 - Child / Branch
 - object
   - vertices
   - groups
     - polys
*/
 if (o == NULL) return;
 if (o->Child != NULL) Object_Free (o->Child);
 if (o->Branch != NULL) Object_Free (o->Branch);

 if (o->Vertices != NULL) free(o->Vertices);
 if (o->VerticesR != NULL) free(o->VerticesR);
 g = o->Groups;
 while (g != NULL)
 {
  if (g->Polygons != NULL) free (g->Polygons);
  if (g->PolygonsR != NULL) free (g->PolygonsR);
  gnext = g->Next;
  free(g);
  g = gnext;
 }
 o = NULL;
}

// --------------------------------------------------------------------------

void Object_Set_Pos(sObject *o, float x, float y, float z)
{
 o->Pos.x = x;
 o->Pos.y = y;
 o->Pos.z = z;
}

// --------------------------------------------------------------------------

void Object_Set_Rot(sObject *o, int x, int y, int z)
{
 o->Rot.x = x;
 o->Rot.y = y;
 o->Rot.z = z;
}

// --------------------------------------------------------------------------

void Object_Add_Pos(sObject *o, float x, float y, float z)
{
 o->Pos.x += x;
 o->Pos.y += y;
 o->Pos.z += z;
}

// --------------------------------------------------------------------------

void Object_Add_Rot(sObject *o, int x, int y, int z)
{
 o->Rot.x += x;
 o->Rot.y += y;
 o->Rot.z += z;
}

// --------------------------------------------------------------------------

sGroup *Object_Get_Group(sObject *o, int num)
{
 int i;
 sGroup *g;

 i=0;
 g = o->Groups;
 while (i!= num)
 {
  if (g->Next != NULL) g = g->Next;
  i++;
 }
 return g;
}

// --------------------------------------------------------------------------

void Object_Set_Flags(sObject *o, int flags)
{
 o->Flags = flags;
}

// ----- GROUP --------------------------------------------------------------

void Group_Set_Texture(sGroup *g, char *tex)
{
 g->texture = tex;
}

// --------------------------------------------------------------------------

void Group_Set_RenderMethod(sGroup *g, int method)
{
 g->RenderMethod = method;
}

// --------------------------------------------------------------------------

void Group_Set_Flags(sGroup *g, int flags)
{
 g->Flags = flags;
}

// ----- CAMERA -------------------------------------------------------------

void Camera_Set_Pos(sCamera *c, float x, float y, float z)
{
 c->Pos.x = x;
 c->Pos.y = y;
 c->Pos.z = z;
}

// --------------------------------------------------------------------------

void Camera_Set_Rot(sCamera *c, int x, int y, int z)
{
 c->Rot.x = x;
 c->Rot.y = y;
 c->Rot.z = z;
}

// --------------------------------------------------------------------------

void Camera_Set_Target(sCamera *c, float x, float y, float z)
{
 c->Target.x = x;
 c->Target.y = y;
 c->Target.z = z;
}

// --------------------------------------------------------------------------

void Camera_Add_Pos(sCamera *c, float x, float y, float z)
{
 c->Pos.x += x;
 c->Pos.y += y;
 c->Pos.z += z;
}

// --------------------------------------------------------------------------

void Camera_Add_Rot(sCamera *c, int x, int y, int z)
{
 c->Rot.x += x;
 c->Rot.y += y;
 c->Rot.z += z;
}

// --------------------------------------------------------------------------

void Camera_Add_Target(sCamera *c, float x, float y, float z)
{
 c->Target.x += x;
 c->Target.y += y;
 c->Target.z += z;
}

// ----- TEXTURE ------------------------------------------------------------

char *Texture_Load(char *s)
{
 char *tex;
 FILE *fp;

 tex = (char *)malloc(65536);
 fp = fopen(s,"rb");
 fread(tex,1,65536,fp);
 fclose(fp);
 return tex;
}

void Texture_Free(char *tex)
{
 free(tex);
}

// ----- PALETTE ------------------------------------------------------------

char *Palette_Load(char *s)
{
 char *pal;
 FILE *fp;

 pal = (char *)malloc(65536);
 fp = fopen(s,"rb");
 fread(pal,1,768,fp);
 fclose(fp);
 return pal;
}

void Palette_Free(char *pal)
{
 free(pal);
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

