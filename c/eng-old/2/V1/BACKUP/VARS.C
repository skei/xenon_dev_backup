#include "types.h"

int Numbers[256];
int mul320[200];

sWorld World;           // world
sCamera Camera;         // camera

WORD *_Buffer;
char *_Texture;         // int polyfiller
WORD *_ShadeTable;

int _outerUadd,_outerVadd,_outerCadd;
int _innerUadd,_innerVadd,_innerCadd;
int Lslope,Rslope;
int _x1,_x2,_u1,_v1,_c1;
int Clipping,xclip,yclip;

char *_LightMap;
unsigned char LightAdd;            // center of PhongMap as adder

float *sine;
float *cosine;

