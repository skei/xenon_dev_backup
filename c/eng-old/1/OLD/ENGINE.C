#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "eng_def.h"
#include "eng_type.h"
#include "eng_vars.h"
#include "eng_math.h"


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


// --------------------------------------------------------------------------

sObject *Object_LoadBuffer( char *LoadedObject )
{
 sObject *O;
 int i;
 int a,b,c;
 int itemp;
 float ftemp;
 int NumVertices;
 int NumPolys;

 fp = LoadedObject;

 // check (and ignore) file header
 f_read(&itemp,4);

 // allocate memory for object structure
 O = (sObject *)malloc(sizeof(sObject));
 assert(O!=0);

 // Some default values:
 O->Flags = OBJECT_active;
 O->Pos.x = 0; O->Pos.y = 0; O->Pos.y = 0;
 O->Rot.x = 0; O->Rot.y = 0; O->Rot.y = 0;
 O->Parent = NULL;
 O->Child = NULL;

 // read bounding box radius
 f_read(&O->BSphereRadius,4);

 // read number of vertices
 f_read(&NumVertices,4);

 O->NumVertices = NumVertices;
 O->Vertices = (sVertex *)malloc(sizeof(sVertex)*NumVertices);
 assert(O->Vertices!=0);

 // read all vertices
 for (i=0;i<NumVertices;i++)
 {
  // read vertex position
  f_read(&(O->Vertices[i].Pos.x),4);
  f_read(&(O->Vertices[i].Pos.y),4);
  f_read(&(O->Vertices[i].Pos.z),4);
  // read vertex U/V
  f_read(&(O->Vertices[i].u),4);
  f_read(&(O->Vertices[i].v),4);
  // read vertex normals
  f_read(&(O->Vertices[i].Normal.x),4);
  f_read(&(O->Vertices[i].Normal.y),4);
  f_read(&(O->Vertices[i].Normal.z),4);

 }
 // read number of polygons in object
 f_read(&NumPolys,4);
 O->NumPolys = NumPolys;
 O->Polygons = (sPolygon *)malloc(sizeof(sPolygon)*NumPolys);
 assert(O->Polygons!=0);

 // read all polygons
 for (i=0;i<NumPolys;i++)
 {
  f_read(&a,4);
  f_read(&b,4);
  f_read(&c,4);
// why was the abc order inverted in old version of engine??? Check 3DS!!!
  O->Polygons[i].v[0] = &(O->Vertices[a]);
  O->Polygons[i].v[1] = &(O->Vertices[b]);
  O->Polygons[i].v[2] = &(O->Vertices[c]);

  f_read(&(O->Polygons[i].Normal.x),4);
  f_read(&(O->Polygons[i].Normal.y),4);
  f_read(&(O->Polygons[i].Normal.z),4);
  O->Polygons[i].Object = O;
 }
 return O;
}

// --------------------------------------------------------------------------

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
 assert(O!=0);
 // Some default values:
 O->Flags = OBJECT_active;
 O->Pos.x = 0; O->Pos.y = 0; O->Pos.y = 0;
 O->Rot.x = 0; O->Rot.y = 0; O->Rot.y = 0;
 O->Parent = NULL;
 O->Child = NULL;
 // read bounding box radius
 fread(&O->BSphereRadius,4,1,fp);
 // read number of vertices
 fread(&NumVertices,4,1,fp);
 O->NumVertices = NumVertices;
 O->Vertices = (sVertex *)malloc(sizeof(sVertex)*NumVertices);
 assert(O->Vertices!=0);
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
 assert(O->Polygons!=0);
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

// --------------------------------------------------------------------------

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

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

// Initialization.. Calc shade tables & phong maps, etc...

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

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

// --------------------------------------------------------------------------
// unsigned char PhongIntensity[129];

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

// --------------------------------------------------------------------------


