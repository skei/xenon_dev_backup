/* --------------------------------------------------------------------------
ToDo:

- Sjekk om alt stemmer (spes. pekere til pekere, etc...
- Support routines.. Init/Exit/Alloc, etc...
- 3DS -> A3D (Axon 3D) converter... bin몉. LoadObject
- Camera_CreateMatrix_Aim
- API/Engine rutiner (Object_SetPos/Rot, etc...
- Load/Save A3D

-------------------------------------------------------------------------- */

#include <string.h>         // memset
#include <stdio.h>          // file-stuff
#include <stdlib.h>         // exit

#include "defines.h"
#include "types.h"
#include "math3d.h"
#include "vars.h"
#include "gtpoly.h"
#include "gfx.h"

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

/*
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
 O <- Object                                o
 | <- Child                                /|\
 O---O---O                                o o o
 |   |   |   --- Branch      <->         /| |\ \
 O-O O-O O  |                           o o o o o
 |       |  v                          / \     /|\
 O-O     O-O-O                        o   o   o o o
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
*/


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

 s1.x = o->Pos.x;
 s1.y = o->Pos.y;
 s1.z = o->Pos.z;

 s2 = Vector_Trans(s1, c->m);

 if (s2.z + o->BBoxRadius < minclip_z) return 0;
 if (s2.z - o->BBoxRadius > maxclip_z) return 0;

 xcompare = (halfscreenwidth_vd * s2.z) >> 8;
 if (s2.x + o->BBoxRadius < -xcompare) return 0;
 if (s2.x - o->BBoxRadius >  xcompare) return 0;

 ycompare = (halfscreenheight_vd * s2.z) >> 8;
 if (s2.y + o->BBoxRadius < -ycompare) return 0;
 if (s2.y - o->BBoxRadius >  ycompare) return 0;

 return 1;
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

 while (g!=NULL)
 {
  for (i=0 ; i<g->NumPolys ; i++)
  {
   poly = &(g->Polygons[i]);      // get adress of Polygon #i

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

 World.VisiblePolys = 0;
 HandleObject( World.Objects );
// Sort the "world"-object
 SortObject( World.RenderList[0] );
// Render the "world"-object
 for (i=0 ; i<World.VisiblePolys ; i++ )
 {
  GTpoly( World.RenderList[i] );
 }
}

// --------------------------------------------------------------------------

// Simplistic version.
// One object, object type ignored, etc, etc... Returns pointer to allocated & loaded object
sObject *Object_Load( char *f )
{
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
 sObject *o;
 sGroup *g;
 sGroup **gptr;
 sPolygon *p;
 FILE *fp;
 int version;
 int num_objects;
 int num_vertices;
 int num_groups;
 int num_polys;
 int i,j,k;
 int v;

 fp = fopen(f,"rb");                                          // Open file
 fread(&version,4,1,fp);                                      // read A3D file version
 o = (sObject *)malloc(sizeof(sObject));                      // Allocate space for object
 fread(&(o->Pos),4,3,fp);                                     // Load object position
 fread(&(o->Rot),4,3,fp);                                     // Load object rotation
 fread(&(o->BBoxRadius),4,1,fp);                              // Bounding box radius
 fread(&(o->NumVertices),4,1,fp);                             // Number of vertices in object
 *(o->Vertices) = malloc(o->NumVertices*sizeof(sVertex));     // allocate space for vertices
 for (i=0;i<o->NumVertices;i++)                               //  |
 {                                                            //  |
  o->Vertices[i] = (sVertex *)malloc(sizeof(sVertex));        //  |
  fread(&(o->Vertices[i]->Pos.x),2,1,fp);                     //  |
  fread(&(o->Vertices[i]->Pos.y),2,1,fp);                     //   > Load all vertices
  fread(&(o->Vertices[i]->Pos.z),2,1,fp);                     //  |
  fread(&(o->Vertices[i]->tu),2,1,fp);                        //  |
  fread(&(o->Vertices[i]->tv),2,1,fp);                        //  |
 }                                                            //  |
 o->Groups = (sGroup *)malloc(sizeof(sGroup));                // allocate memory for group. Always at least one group in object
 fread(&(num_groups),4,1,fp);                                 // Number of groups in object

 gptr = &(o->Groups);                                         // gptr = pointer to DD p (offset p)
 while (num_groups > 0)                                       // repeat until no groups left to load
 {
  g = *gptr;
  g->Polygons = (sPolygon *)malloc(g->NumPolys*sizeof(sPolygon));   // allocate memory
  fread(&(g->NumPolys),4,1,fp);                                // read number of polys in current group
  p = g->Polygons;
  for (i=0;i<g->NumPolys;i++)
  {
   fread(&v,4,1,fp);
   p->v1 = o->Vertices[v];
   fread(&v,4,1,fp);
   p->v2 = o->Vertices[v];
   fread(&v,4,1,fp);
   p->v3 = o->Vertices[v];
  }
  gptr = &(g->Next);
 }
 return o;
}


// ##########################################################################

void main(void)
{
}
