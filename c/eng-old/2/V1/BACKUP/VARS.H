#ifndef vars_h_included
#define vars_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#include "types.h"

extern int Numbers[256];
extern int mul320[200];

extern sWorld World;           // world
extern sCamera Camera;         // camera

extern WORD *_Buffer;
extern char *_Texture;         // texture ptr. used in polyfiller
extern WORD *_ShadeTable;      // used in polyfiller

extern int _outerUadd,_outerVadd,_outerCadd;
extern int _innerUadd,_innerVadd,_innerCadd;
extern int Lslope,Rslope;
extern int _x1,_x2,_u1,_v1,_c1;
extern int Clipping,xclip,yclip;


extern char *_LightMap;
extern unsigned char LightAdd; // center of PhongMap as adder

extern float *sine;
extern float *cosine;


// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
