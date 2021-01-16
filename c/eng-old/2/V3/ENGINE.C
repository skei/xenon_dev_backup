#include <stdio.h>    // file

#include "defines.h"
#include "types.h"
#include "vars.h"

#include "math3d.h"
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void Engine_Init(int maxpolys)
{
 World = (sWorld *)malloc(sizeof(sWorld));
 World->RenderList = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
 World->Sorted     = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
 sine   = (float *)malloc(TABLESIZE*sizeof(float));
 cosine = (float *)malloc(TABLESIZE*sizeof(float));
 Init_SinCos();
}

// --------------------------------------------------------------------------

void Engine_Exit(void)
{
 free(World->RenderList);
 free(World->Sorted);
 free(sine);
 free(cosine);
 free(World);
}


/* ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
 Loads one object.
 in:    s = ptr to file-name.
 out:   returns ptr to allocated & loaded object
 notes: Only 1-group objects handlet yet...

 file format:

 float BSphereRadius
 int NumVertices
 {
  float x,y,z
  int u,v
 }
 int NumGroups
 int NumPolys
 {
  int a,b,c
 }

ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ */
sObject *Object_Load( char *s )
{
 FILE *fp;
 sObject *O;
 sGroup *G;
 int i;
 int a,b,c;
 int itemp;
 float ftemp;
 int NumVertices;
 int NumGroups;
 int NumPolys;

 fp = fopen(s,"rb");
 // check (and ignore) file header
 fread(&itemp,4,1,fp);

 // allocate memory for object structure
 O = (sObject *)malloc(sizeof(sObject));
 // Some default values:
 O->Flags = OBJECT_Active;
 O->Pos.x = 0; O->Pos.y = 0; O->Pos.y = 0;
 O->Rot.x = 0; O->Rot.y = 0; O->Rot.y = 0;
 O->Branch = NULL;
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
  fread(&(O->Vertices[i].Pos.x),4,1,fp);
  fread(&(O->Vertices[i].Pos.y),4,1,fp);
  fread(&(O->Vertices[i].Pos.z),4,1,fp);
  fread(&ftemp,4,1,fp);
  O->Vertices[i].u = float2fixed(ftemp);
  fread(&ftemp,4,1,fp);
  O->Vertices[i].v = float2fixed(ftemp);

  fread(&(O->Vertices[i].Normal.x),4,1,fp);
  fread(&(O->Vertices[i].Normal.y),4,1,fp);
  fread(&(O->Vertices[i].Normal.z),4,1,fp);

 }
 // read number of groups.. Ignored... Only 1-group objects yet... :-(
 fread(&NumGroups,4,1,fp);
 NumGroups = 1;
 G = (sGroup *)malloc(sizeof(sGroup));
 O->Groups = G;
 // Some default values:
 G->Flags = 0;
 G->Next = NULL;
 // read number of polygons in group
 fread(&NumPolys,4,1,fp);
 G->NumPolys = NumPolys;
 G->Polygons = (sPolygon *)malloc(sizeof(sPolygon)*NumPolys);
 // read all polygons
 for (i=0;i<NumPolys;i++)
 {
  fread(&a,4,1,fp);
  fread(&b,4,1,fp);
  fread(&c,4,1,fp);
  G->Polygons[i].v[2] = &(O->Vertices[a]);
  G->Polygons[i].v[1] = &(O->Vertices[b]);
  G->Polygons[i].v[0] = &(O->Vertices[c]);
  fread(&(G->Polygons[i].Normal.x),4,1,fp);
  fread(&(G->Polygons[i].Normal.y),4,1,fp);
  fread(&(G->Polygons[i].Normal.z),4,1,fp);
 }
 close(fp);
 return O;
}

// --------------------------------------------------------------------------

void Object_SetPos( sObject *o, float x, float y, float z)
{
 o->Pos.x = x;
 o->Pos.y = y;
 o->Pos.z = z;
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

void Camera_AddRot( sCamera *c, float x, float y, float z)
{
 c->Rot.x += x;
 c->Rot.y += y;
 c->Rot.z += z;
}

