#include <stdio.h>

#include "defines.h"
#include "types.h"
#include "vars.h"

void Engine_Init(int maxpolys)
{
 FILE *fp;
 int i;

 _Buffer = (WORD *)malloc(64000*2);
 memset(_Buffer,31,(64000*9));
 _ShadeTable= (WORD *)malloc(32768*2);
 _LightMap = (char *)malloc(65536*2);
 World.RenderList = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
 World.Sorted     = (sPolygon **)malloc(maxpolys*sizeof(sPolygon **));
 sine   = (float *)malloc(TABLESIZE*sizeof(float));
 cosine = (float *)malloc(TABLESIZE*sizeof(float));
 Init_SinCos();
 for (i=0;i<200;i++)
 {
  mul320[i] = (i*320);
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
 free(_LightMap);

}

// --------------------------------------------------------------------------

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

/*
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
*/

// --------------------------------------------------------------------------

/*
void Object_Free(sObject *o)
{
 sGroup *g,*gnext;

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
*/

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

/*
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
*/

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

