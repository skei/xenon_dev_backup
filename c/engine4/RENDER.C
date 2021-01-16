/* --------------------------------------------------------------------------
ToDo:

- Sjekk om alt stemmer (spes. pekere til pekere, etc...
- Support routines.. Init/Exit/Alloc, etc...
- 3DS -> A3D (Axon 3D) converter... bin몉. LoadObject
- Camera_CreateMatrix_Aim
- API/Engine rutiner (Object_SetPos/Rot, etc...

-------------------------------------------------------------------------- */

#include <string.h>         // memset
#include <stdio.h>          // file-stuff
#include <stdlib.h>         // exit

#include "defines.h"
#include "types.h"
#include "math3d.h"
#include "vars.h"

#include "gfx.h"

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
// O <- Object
// | <- Child
// O---O---O
// |   |   |   --- Branch
// O-O O-O O  |
// |       |  v
// O-O     O-O-O
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴


// --------------------------------------------------------------------------

void Camera_CreateMatrix_Free(sCamera *c)
{
 int mm[16];
 Matrix_Translation(c->m,-(c->Pos.x),-(c->Pos.y),-(c->Pos.z));
 Matrix_Rotation(mm,-(c->Rot.x),-(c->Rot.y),-(c->Rot.z));
 Matrix_Mult(c->m,mm);
}

// --------------------------------------------------------------------------

void Object_CreateMatrix(sObject *o, sCamera *c)
{
 Matrix_Rotation(o->m,o->Rot.x,o->Rot.y,o->Rot.z);
 Matrix_Copy(o->e,o->m);
 o->e[3*4+0] += (o->Pos.x << DIVD);
 o->e[3*4+1] += (o->Pos.y << DIVD);
 o->e[3*4+2] += (o->Pos.z << DIVD);
 Matrix_Mult(o->e,c->m);
}


// --------------------------------------------------------------------------

// Returns 1 if the object's bounding-box is visible in ViewFrustum, else 0
int Object_CheckBoundingSphere(sObject *o, sCamera *c)
{
 sVector s1,s2;
 int xcompare,ycompare;
 int r;

 r = 1;

 s1.x = o->Pos.x;
 s1.y = o->Pos.y;
 s1.z = o->Pos.z;

 s2 = Vector_Trans(s1, c->m);

 if (s2.z + o->BBoxRadius < minclip_z) r = 0;
 if (s2.z - o->BBoxRadius > maxclip_z) r = 0;

 xcompare = (halfscreenwidth_vd * s2.z) >> 8;
 if (s2.x + o->BBoxRadius < -xcompare) r = 0;
 if (s2.x - o->BBoxRadius >  xcompare) r = 0;

 ycompare = (halfscreenheight_vd * s2.z) >> 8;
 if (s2.y + o->BBoxRadius < -ycompare) r = 0;
 if (s2.y - o->BBoxRadius >  ycompare) r = 0;

 return r;
}

// --------------------------------------------------------------------------
// ByteSort the "world"-object
// --------------------------------------------------------------------------
void SortObject( sPolygon *polys )
{
 int i;
 short int a,b,d;

 // clear numbers array
 memset( Numbers,0,sizeof (Numbers));
// Count number of each byte-value
 for ( i=0;i<World.VisiblePolys;i++ )
 {
  // Huh.... Trodde det burde v몉e polys[i]->z her jeg, men, men...
  Numbers[ polys[i].z & 255 ]++;
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
  a = polys[i].z & 255; //sorttable[i].sortvalue & 255;
  d = Numbers[a];
  World.Sorted[d] = &polys[i];
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
  // Huh.... Trodde det burde v몉e polys[i]->z her jeg, men, men...
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
// Sort according to LOW byte
 for ( i=0;i<World.VisiblePolys;i++ )
 {
  a = (polys[i].z & 0xff00) >> 8; //sorttable[i].sortvalue & 255;
  d = Numbers[a];
  World.RenderList[d] = World.Sorted[i];
  Numbers[a]++;
 }
}

// --------------------------------------------------------------------------
// Recursively handle a group and all sub-groups
// --------------------------------------------------------------------------
void HandleGroup( sGroup *g)
{
 int i;
 sPolygon *poly;
 sVertex *v1,*v2,*v3;

 World.VisiblePolys = 0;
 while (g!=NULL)
 {
  for (i=0 ; i<g->NumPolys ; i++)
  {
   poly = &(g->Polygons[i]);

   v1 = poly->v1;
   v2 = poly->v2;
   v3 = poly->v3;
   // if any of the Z-coordinates are zero or less, skip the entire poly
   if (    (v1->rPos.z != -1)
        && (v2->rPos.z != -1)
        && (v3->rPos.z != -1))
   {
    // check z-component of face-normal
    if (   (v2->rPos.x - v1->rPos.x) * (v3->rPos.y - v1->rPos.y)
         - (v3->rPos.x - v1->rPos.x) * (v2->rPos.y - v1->rPos.y) < 0 )
    {
     World.RenderList[ World.VisiblePolys ] = poly;
     World.VisiblePolys++;
    }
   }
  }
  g = g->Next;
 }
}

// --------------------------------------------------------------------------

// --------------------------
// Transfor_and_project_all_vertices_here      !!!!!
// --------------------------
void Object_Process(sObject *o)
{
 int i,j;
 sGroup *g;
 sVertex *vertex;
 sVector vector;

 for (i=0;i<o->NumVertices;i++)
 {
  vertex = o->Vertices[i];
// process_matrix(o->e,o->coords,rotated,o->numcoords);
  vector = Vector_Trans( vertex->Pos, o->e);
// ProjectCoords(o);
  if (vector.z <= 0)
  {
   vertex->rPos.z = -1;
  }
  else
	{
   vertex->rPos.z = vector.z;
   vector.z += DISTANCE;
   vertex->rPos.x = ((vector.x << PROJ_PARAM) / (vector.z+DISTANCE)) + 128;
   vertex->rPos.y = ((vector.y << PROJ_PARAM) / (vector.z+DISTANCE)) + 128;
  }
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

 if ( o == NULL) return;
 if (Object_CheckBoundingSphere( o, World.Camera) == 1)
 {
  if (o->Status & OBJECT_ACTIVE == OBJECT_ACTIVE)
  {
   Object_CreateMatrix( o, World.Camera );
   Object_Process( o );
   HandleGroup( o->Groups );
  }
 }
 HandleObject( o->Child  );
 HandleObject( o->Branch );
}

// ==========================================================================
// Renders the entire scene
// ==========================================================================

void GTfiller( sPolygon *p )
{
 sVertex *v1,*v2,*v3;

 v1 = p->v1;
 v2 = p->v2;
 v3 = p->v3;

 SlowLine(v1->rPos.x,v1->rPos.y,v2->rPos.x,v2->rPos.y,15);
 SlowLine(v2->rPos.x,v2->rPos.y,v3->rPos.x,v3->rPos.y,15);
 SlowLine(v1->rPos.x,v1->rPos.y,v3->rPos.x,v3->rPos.y,15);
}

void Render(void)
{
 int i;

 switch(World.cameratype)
 {
  case 0 : Camera_CreateMatrix_Free( World.Camera );
           break;
  case 1 : Camera_CreateMatrix_Aim( World.Camera );
           break;
 }
 HandleObject( World.Objects );
// Sort the "world"-object
 SortObject( World.RenderList[0] );
// Render the "world"-object
 for (i=0 ; i<World.VisiblePolys ; i++ )
 {
  GTfiller( World.RenderList[i] );
 }
}

// --------------------------------------------------------------------------

// Simplistic version.
// One object, object type ignored, etc, etc... Returns pointer to allocated & loaded object
sObject *Object_Load( char *f )
{
 FILE *fp;
 sObject *o;
 sVertex *v;
 sGroup *g;
 sPolygon *p;
 int file_id;
 int num_vertices, num_groups, num_polygons;
 int i;
 int v1,v2,v3;

 fopen(f,"rb");
 fread(&file_id,4,1,fp);                                              // read:     'A3D0' ID-header
 o = (sObject *)malloc(sizeof(sObject));                              // allocate: object
 fread(&o->Pos,sizeof(sVector),1,fp);                                 // read:     object position
 fread(&o->Rot,sizeof(sVector),1,fp);                                 // read:     object rotation
 fread(&o->BBoxRadius,4,1,fp);                                        // read:     bounding box radius
 fread(&num_vertices,4,1,fp);                                         // read:     number of vertices in object
 o->Vertices = (sVertex **)malloc(num_vertices*sizeof(sVertex *));    // allocate: array of pointers to sVertex
 v = (sVertex *)malloc(num_vertices*sizeof(sVertex));                 // allocate: array of sVertex'es
 for (i=0;i<num_vertices;i++)
 {
  o->Vertices[i] = &v[i];                                             // pointer in array point to right sVertex structure
  v = o->Vertices[i];
  fread(&v->Pos,sizeof(sVector),1,fp);                                // read:     Vertex position
  fread(&v->tu,4,1,fp);                                               // read:     texture U
  fread(&v->tv,4,1,fp);                                               // read:     texture V
 }
 fread(&num_vertices,4,1,fp);                                         // read:     number of groups in object
// For testing purposes: Only one group per object
 o->Groups = (sGroup *)malloc(sizeof(sGroup));                        // allocate: object group
 g = o->Groups;                                                       // g = Object-group
 fread(&num_polygons,4,1,fp);                                         // read:     number of groups in object
 g->Polygons = (sPolygon *)malloc(num_polygons*sizeof(sPolygon));     // allocate: polygon data
 p = g->Polygons;
 for (i=0;i<num_polygons;i++)
 {
  fread(&v1,4,1,fp);
  fread(&v2,4,1,fp);
  fread(&v3,4,1,fp);
  p[i].v1 = o->Vertices[v1];
  p[i].v2 = o->Vertices[v2];
  p[i].v3 = o->Vertices[v3];
  p[i].group = o->Groups;
 }
 return o;
}
/*
int header 'A3D0' Axon 3Dfile format v0...
int obj_pos_x, obj_pos_y, obj_pos_z
int obj_rot_x, obj_rot_y, obj_rot_z
int bounding_box_radius
int n1 = number_of_vertices_in_object
{
 short int vertex_x, vertex_y, vertex_z
 short int vertex_u, vertex_v
}
int n2 = Number_of_groups_in_object
{
 int n3 = number_of_polygons_in_group
 {
  int polygon_a, polygon_b, polygon_c
 }
}
*/


// ##########################################################################


