#include <math.h>   // sin/cos/etc...
#include <string.h> // memset

#include "defines.h"
#include "vars.h"
#include "types.h"
#include "math3d.h"

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
 int x,y;
 long dist;
 char *p;

 p = _LightMap;
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

 memset(_LightMap,0,65536);
 for (y=0;y<256/*127*/;y++)
 {
  for (x=0;x<256/*127*/;x++)
  {
   c = 127-sqrt((x-128)*(x-128) + (y-128)*(y-128));
   if (c > 127) c = 127;
   if (c < 14) c = 14;

   _LightMap[y*256+x] = c >> 1;
  }
 }
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// Real fake light.... Using phongMap and X/Y as indexes into this.
void CalcLight_Camera(sObject *o)
{
 int i;
 int xx,yy;
 sVertex *vertex;
 sVector vector;

 vertex = o->Vertices;
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
  vertex->c = _LightMap[(yy*256+xx) & 0xFFFF];
  vertex++;
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
  o->Vertices[n].c = _LightMap[(yy*256+xx) & 0xFFFF];
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
  o->Vertices[n].u = ((int)vector.x + LightAdd);
  o->Vertices[n].v = ((int)vector.y + LightAdd);
 }
}

