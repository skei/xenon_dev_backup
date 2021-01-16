#include <stdio.h>    // file
#include <conio.h>  // debug - getch
#include "defines.h"
#include "types.h"
#include "vars.h"
#include "gfx.h"

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

// ²²²²² Object ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²

/* ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
 Loads one object.
 in:    s = ptr to file-name.
 out:   returns ptr to allocated & loaded object
 notes: Only 1-group objects handlet yet...

 file format:

 int file_header
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
 int NumVertices;
 int NumGroups;
 int NumPolys;

 fp = fopen(s,"rb");
 // check if file header is corrrect
 fread(&itemp,4,1,fp);
/* debug */  printf("Object Header: %X\n",itemp);
 if (itemp != OBJECT_HEADER)
 {
  return NULL;
 }
 else
 {
  // allocate memory for object structure
/* debug */  printf("Allocating %i bytes for sObject structure\n",sizeof(sObject));
  O = (sObject *)malloc(sizeof(sObject));
  // some default variables
  O->Pos.x = 0;
  O->Pos.y = 0;
  O->Pos.z = 0;
  O->Rot.x = 0;
  O->Rot.y = 0;
  O->Rot.z = 0;
  O->Child = NULL;
  O->Branch = NULL;
  // read bounding box radius
  fread(&O->BSphereRadius,sizeof(float),1,fp);
/* debug */  printf("Bounding sphere radius: %f\n",O->BSphereRadius);
  // read number of vertices
  fread(&NumVertices,sizeof(int),1,fp);
/* debug */  printf("Number of vertices in object: %i\n",NumVertices);
  O->NumVertices = NumVertices;
/* debug */  printf("Allocating %i bytes for Vertices\n",sizeof(sVertex)*NumVertices);
  O->Vertices = (sVertex *)malloc(sizeof(sVertex)*NumVertices);
  // read all vertices
/* debug */  printf("Reading %i vertices\n----------------\n",NumVertices);
  for (i=0;i<NumVertices;i++)
  {
   fread(&(O->Vertices[i].Pos.x),sizeof(float),1,fp);
   fread(&(O->Vertices[i].Pos.y),sizeof(float),1,fp);
   fread(&(O->Vertices[i].Pos.z),sizeof(float),1,fp);
   fread(&(O->Vertices[i].u),sizeof(int),1,fp);
   fread(&(O->Vertices[i].v),sizeof(int),1,fp);
// /* debug */   printf("Vertex %i: %f,%f,%f : %i,%i\n",i,O->Vertices[i].Pos.x,O->Vertices[i].Pos.y,O->Vertices[i].Pos.z,
// /* debug */                                   O->Vertices[i].u,O->Vertices[i].v);
  }
  // read number of groups.. Ignored... Only 1-group objects yet... :-(
  fread(&NumGroups,sizeof(int),1,fp);
/* debug */  printf("Number of Groups in object: %i\n",NumGroups);
  NumGroups = 1;
// Skal v‘re linked-list, s† allok‚r etterhvert som vi finner grupper i fil...
/* debug */  printf("Allocating %i bytes for Groups\n",sizeof(sGroup));
  G = (sGroup *)malloc(sizeof(sGroup));
  // some default variables
  O->Groups = G;
  G->Material = NULL;
  G->Next = NULL;
  // read number of polygons in group
  fread(&NumPolys,sizeof(int),1,fp);
/* debug */  printf("Number of Polys in group: %i\n",NumPolys);
  G->NumPolys = NumPolys;
/* debug */  printf("Allocating %i bytes for Polygons\n",sizeof(sPolygon)*NumPolys);
  G->Polygons = (sPolygon *)malloc(sizeof(sPolygon)*NumPolys);
  // read all polygons
/* debug */  printf("Reading %i polygons\n----------------\n",NumPolys);
  for (i=0;i<NumPolys;i++)
  {
   fread(&a,4,1,fp);
   fread(&b,4,1,fp);
   fread(&c,4,1,fp);
   G->Polygons[i].v1 = &(O->Vertices[a]);
   G->Polygons[i].v2 = &(O->Vertices[b]);
   G->Polygons[i].v3 = &(O->Vertices[c]);
// /* debug */   printf("Poly %i: %i,%i,%i\n",i,a,b,c);
  }
  fclose(fp);
  return O;
 }
}

/* ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
// Free memory allocated for object object
ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ */
void Group_Free( sGroup *g)
{
 if (g==NULL) return;
 if (g->Polygons != NULL) free (g->Polygons);
 Group_Free(g->Next);
}

// -----

void Object_Free( sObject *o)
{
 int i;

 if (o==NULL) return;
 Group_Free(o->Groups);
 if (o->Vertices != NULL) free (o->Vertices);
 if (o->Groups != NULL) free (o->Groups);

 Object_Free(o->Child);
 Object_Free(o->Branch);

}

// ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²

void Engine_Init( int mode, int maxpolys, int maxmaterials, int w, int h,
                  int projparam, int minz, int maxz)
{
 int i;

 MinX = 0; MaxX = w-1;
 MinY = 0; MaxY = h-1;
 ScreenWidth = w;
 ScreenHeight = h;

 minclip_z = minz;
 maxclip_z = maxz;
 PROJ_PARAM = projparam;
 viewdistance = (1<<PROJ_PARAM);

 halfscreenwidth_vd =  ((ScreenWidth/2) << 8)/viewdistance;
 halfscreenheight_vd = ((ScreenHeight/2) << 8)/viewdistance;

 World.RenderList = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
 World.Sorted     = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));

 Materials = (sMaterial *)malloc(maxmaterials*sizeof(sMaterial));

// sine   = (float *)malloc(TABLESIZE*sizeof(float));
// cosine = (float *)malloc(TABLESIZE*sizeof(float));
// Init_SinCos();

 for (i=0;i<(MaxY+1);i++) MulWidth[i] = (i*ScreenWidth);

 engine_mode = mode;
 switch (engine_mode)
 {
  case 0 : _Screen = (char *)0xA0000;
           SetMode(0x13);
           break;
 }
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void Engine_UnInit(void)
{
 switch (engine_mode)
 {
  case 0 : SetMode(3);
           break;
 }
 if (World.RenderList != NULL) free(World.RenderList);
 if (World.Sorted != NULL) free(World.Sorted);
// if (sine != NULL) free(sine);
// if (cosine != NULL) free(cosine);
 if (Materials != NULL) free (Materials);
// VesaUnInit((char *)VesaPtr);
}

