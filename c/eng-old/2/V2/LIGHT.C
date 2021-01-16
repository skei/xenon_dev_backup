#include <math.h>

#include "defines.h"
#include "types.h"
#include "vars.h"

#define PI 3.1415926535

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
   c = 127 - sqrt((x-128)*(x-128) + (y-128)*(y-128));
   if (c >= 127) c = 127;
   if (c <= 14) c = 14;

   _PhongMap[y*256+x] = c >> 1;
  }
 }
}

// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

// Real fake light.... Using phongMap and X/Y as indexes into this.
void CalcLight_Camera(sObject *o)
{
 int i;
 float xx,yy;
 sVertex *vertex;
 sVector vector;

 vertex = o->Vertices;
 for (i=0;i<o->NumVertices;i++)
 {
//  vector = Vector_Trans3x3( vertex->Normal, &o->m);
  vector.x =   vertex->Normal.x * o->m[ 0]
             + vertex->Normal.y * o->m[ 1]
             + vertex->Normal.z * o->m[ 2];

  vector.y =   vertex->Normal.x * o->m[ 4]
             + vertex->Normal.y * o->m[ 5]
             + vertex->Normal.z * o->m[ 6];

  vector.z =   vertex->Normal.x * o->m[ 8]
             + vertex->Normal.y * o->m[ 9]
             + vertex->Normal.z * o->m[10];

  xx = (vector.x*63) + LightAddX;
  yy = (vector.y*63) + LightAddY;
  vertex->c = _PhongMap[(int)(yy*256+xx) & 0xFFFF]*65536.0;
  vertex++;
 }
}


