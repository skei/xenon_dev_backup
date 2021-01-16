// Gouraud shaded Texture Mapping...
// Axon / Xenon & Inf <ÄÍÍÍÄ> (Tor-Helge Skei)

// 10.49 = 10.00 = 95.328 25-pixel polys p/sek

// ÄÄÄÄÄ Testing functions & variables ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

#include <conio.h>      // getch,outp
#include <stdio.h>      // printf
#include <stdlib.h>     // malloc,abs
#include <string.h>     // memset

#include <math.h>       // sqrt

// #pragma inline GTfiller();   Funker noe s†nt, mon tro???

void SetMode(short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [eax];



// ÄÄÄÄÄ Typedef ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

typedef struct
{
 int x,y;
// int px,py,pz;  // position
// int nx,ny,nz;  // normal
 int u,v;
 int c;
} sRenderVertex;

// ÄÄÄÄÄ

typedef struct
{
 sRenderVertex *v1,*v2,*v3;
 char *texture;
} sPoly;

// ÄÄÄÄÄ

sPoly *RenderList;

// ÄÄÄÄÄ

/*

typedef struct
{
 int px,py,pz;  // position
 int nx,ny,nz;  // normal
 int tu,tv;
 int color;
} sVertex;

typedef struct
{
 int vCount;      // vertex count
 int fCount;      // face count
 int *fData;      // face data (array of 3*fCount vertex numbers)
 char *Texture;   // Group texture
 int RenderFlags  // rendering type (flat, gouraud, etc..)
} sGroup;

typedef struct
{
 int xpos,ypos,zpos;
 int xang,yang,zang;
 int NumGroups;
 sGroup *Groups;
} sObject;

*/

// ÄÄÄÄÄ Global variables ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

int mul960[600];

char *palette;
char *tex;

// GTinnerloop
char *_Buffer;
char *_Texture;           // constant for object (group)
char *_ShadeTable;        // constant for palette/texture
int _innerTexAdder;
int _innerColorAdder;
// GTfiller
int _outerTexAdder;
int _outerColorAdder;
int _outerLeftAdder;
int _outerRightAdder;

int _savedLeft;
int _savedRight;
int _savedTex;
int _savedC;

// ÄÄÄÄÄ The polyfiller innerloop ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

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
 " jle skipline",\
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
 " jnz inner",\
 "skipline:",\
 parm [edi][edx][esi][ecx] modify [eax ebx];

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void GTfiller1(int y1, int y2, int left, int right, int tex, int c)
{
 int i;

 if (y2 < 0)   return;
 if (y1 > 199) return;
 if (left  > (319<<16)) return;
 if (right < 0) return;

// maybe remove these
 if (y1 < -199 ) return;
 if (y2 >  399 ) return;

 for (i=y1;i<y2;i++)
 {
  GTinnerloop(
               (char *)((int)_Buffer+mul960[i+200]+(left>>16)),
               tex,
               c,
//               (right >> 16) - (left>>16)
               (right - left) >> 16

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

 if (y2 < 0)   return;
 if (y1 > 199) return;
 if (left  > (319<<16)) return;
 if (right < 0) return;

// maybe remove these
 if (y1 < -199 ) return;
 if (y2 >  399 ) return;

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
//               (right >> 16) - (left>>16)
               (right - left) >> 16

              );
  tex   += _outerTexAdder;
  c     += _outerColorAdder;
  left  += _outerLeftAdder;
  right += _outerRightAdder;
 }
}


// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void GTpoly(sPoly *p)
{
 sRenderVertex *v1,*v2,*v3,*temp;
 int y2_y1, y3_y1, y3_y2;
 int x2_x1, x3_x1, x3_x2;
 int x4_x3, x3_x4;
 int slope12, slope13;
 int X4,U4,V4,C4;

 #define SWAP(a,b) { temp=a; a=b; b=temp; }

 #define X1 v1->x
 #define Y1 v1->y
 #define U1 v1->u
 #define V1 v1->v
 #define C1 v1->c

 #define X2 v2->x
 #define Y2 v2->y
 #define U2 v2->u
 #define V2 v2->v
 #define C2 v2->c

 #define X3 v3->x
 #define Y3 v3->y
 #define U3 v3->u
 #define V3 v3->v
 #define C3 v3->c

 _Texture = p->texture;

 v1 = p->v1;
 v2 = p->v2;
 v3 = p->v3;

 if ( Y1 > Y2 ) SWAP(v1,v2);
 if ( Y1 > Y3 ) SWAP(v1,v3);
 if ( Y2 > Y3 ) SWAP(v2,v3);

 y3_y1 = (Y3 - Y1);
 if (y3_y1 == 0) return;
 if ((Y1 == Y2) && (X1 > X2)) SWAP(v1,v2);
 if ((Y2 == Y3) && (X2 > X3)) SWAP(v2,v3);

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
  _innerColorAdder = ((C2 - C1) << 8) / x2_x1;
  _innerTexAdder   = ((short int)(((U2 - U1)<<8)/x2_x1) << 16)
                    + (short int)(((V2 - V1)<<8)/x2_x1);

  _outerTexAdder   = ((short int)(((U3 - U1)<<8)/y3_y1) << 16)
                    + (short int)(((V3 - V1)<<8)/y3_y1);
  _outerColorAdder = ((C3 - C1) << 8 ) / y3_y1;
  _outerLeftAdder  = slope13;
  _outerRightAdder = ((X3 - X2) << 16) / y3_y1;
  GTfiller1(Y1,Y3,(X1<<16),(X2<<16),((U1<<24)+(V1<<8)),(C1<<8));
  goto FinitoGT;
 }
// ..........................................................................
 FlatBottom:
 //  1
 // 2 3
 {
  x3_x2 = ( X3 - X2 );
  _innerColorAdder = ((C3 - C2) << 8) / x3_x2;
  _innerTexAdder   = ((short int)(((U2 - U1)<<8)/x3_x2) << 16)
                    + (short int)(((V2 - V1)<<8)/x3_x2);

  _outerTexAdder   = ((short int)(((U2 - U1)<<8)/y3_y1) << 16)
                    + (short int)(((V2 - V1)<<8)/y3_y1);
  _outerColorAdder = ((C2 - C1) << 8 ) / y3_y1;
  _outerLeftAdder  = slope12;
  _outerRightAdder = slope13;
  GTfiller1(Y1,Y3,(X1<<16),(X1<<16),((U1<<24)+(V1<<8)),(C1<<8));

  goto FinitoGT;
 }
// ..........................................................................
 FlatLeft:
 //  1
 //   2
 // 3 (4)
 {
  X4 = X1 + (( slope12 * y3_y1) >> 16);
  C4 = C1 + (((C2-C1) * y3_y1) / y2_y1);
  U4 = U1 + (((U2-U1) * y3_y1) / y2_y1);
  V4 = V1 + (((V2-V1) * y3_y1) / y2_y1);

  x4_x3 = ( X4 - X3 );
  _innerColorAdder = ((C4 - C3) << 8) / x4_x3;
  _innerTexAdder   = ((short int)(((U4 - U3)<<8)/x4_x3) << 16)
                    + (short int)(((V4 - V3)<<8)/x4_x3);
  _outerTexAdder   = ((short int)(((U3 - U1)<<8)/y3_y1) << 16)
                    + (short int)(((V3 - V1)<<8)/y3_y1);
  _outerColorAdder = ((C3 - C1) << 8 ) / y3_y1;
  _outerLeftAdder  = slope13;
  _outerRightAdder = slope12;
  GTfiller1(Y1,Y2,(X1<<16),(X1<<16),((U1<<24)+(V1<<8)),(C1<<8));
  _outerRightAdder = ((X3 - X2) << 16) / ( Y3 - Y2 );
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
  _innerColorAdder = ((C3 - C4) << 8) / x3_x4;
  _innerTexAdder   = ((short int)(((U3 - U4)<<8)/x3_x4) << 16)
                    + (short int)(((V3 - V4)<<8)/x3_x4);
  _outerTexAdder   = ((short int)(((U2 - U1)<<8)/y2_y1) << 16)
                    + (short int)(((V2 - V1)<<8)/y2_y1);
  _outerColorAdder = ((C2 - C1) << 8 ) / y2_y1;
  _outerLeftAdder  = slope12;
  _outerRightAdder = slope13;
  GTfiller1(Y1,Y2,(X1<<16),(X1<<16),((U1<<24)+(V1<<8)),(C1<<8));
  _outerTexAdder   = ((short int)(((U3 - U2)<<8)/y3_y2) << 16)
                    + (short int)(((V3 - V2)<<8)/y3_y2);
  _outerColorAdder = ((C3 - C2) << 8 ) / y3_y2;
  _outerLeftAdder  = ((X3 - X2) << 16) / ( y3_y2 );
  GTfiller2(Y2,Y3);
  goto FinitoGT;
 }
// ..........................................................................
 FinitoGT:
 ;
}

// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ

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

// -----

void CalcShadeTable(char *palette, char *ShadeTable)
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

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

// --------------------------------------------------------------------------
void visible(void)
{
}

// --------------------------------------------------------------------------

void main(void)
{
 sRenderVertex v1,v2,v3;
 sPoly p;
 int i;
 FILE *fp;
 char *pp;
 int x,y;

 printf("\n");
 // init mul960 table
 printf("ş Calculating multiplication table\n");
 for (i=0;i<600;i++)
 {
  mul960[i] = ((i-200)*960 + 320);
 }
 printf("ş Loading Palette\n");
 palette = (char *)malloc(768);
 fp = fopen("palette.raw","rb");
 fread(palette,1,768,fp);
 fclose(fp);

 printf("ş Loading Texture\n");
 tex = (char *)malloc(65536);
 fp = fopen("texture.raw","rb");
 fread(tex,1,65536,fp);
 fclose(fp);

 printf("ş Calculating Shade table\n");
 _ShadeTable= (char *)malloc(65536);
 CalcShadeTable(palette,_ShadeTable);

 v1.x = 160;
 v1.y = 10;
 v1.u = 0;
 v1.v = 0;
 v1.c = 0;

 v2.x = 10;
 v2.y = 90;
 v2.u = 127;
 v2.v = 0;
 v2.c = 31;

 v3.x = 310;
 v3.y = 190;
 v3.u = 127;
 v3.v = 127;
 v3.c = 63;

 p.v1 = &v1;
 p.v2 = &v2;
 p.v3 = &v3;
 p.texture = tex;

 printf("ş Allocating background buffer\n");
 _Buffer = (char *)malloc(64000*9);
 memset(_Buffer,0,64000*9);

 SetMode(0x13);
 for (i=0;i<256;i++)
 {
  outp(0x3c8,i);
  outp(0x3c9,palette[i*3]);
  outp(0x3c9,palette[i*3+1]);
  outp(0x3c9,palette[i*3+2]);
 }
// for (i=0;i<1000000;i++)
 {
  GTpoly(&p);
 }
 pp = (char *)0xA0000;
 for (y=0;y<200;y++)
 {
  for (x=0;x<320;x++)
  {
   pp[y*320+x] = _Buffer[mul960[y+200]+x];
  }
 }
 getch();
 SetMode(3);
 free(palette);
 free(tex);
 free(_Buffer);
 free(_ShadeTable);
}

