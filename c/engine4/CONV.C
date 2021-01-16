// 3DS/ASC -> A3D converter

#include "defines.h"
#include "math.h"
#include "types.h"
#include <stdlib.h>   // exit
#include <stdio.h>    // file functions

#define FILE_HEADER 0x41334430       // 'A3D0'

// ÄÄÄÄÄ Global variables ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

int NumObjects;
int NumVertices;
int bbox;

sVector pos,rot;
short int x,y,z,u,;


// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ


/*
int header 'A3D0' Axon 3Dfile format v0...
int n0 = number_of_objects_in_file
n0 * ³ int object_type (child or branch object)
     ³ int obj_pos_x, obj_pos_y, obj_pos_z
     ³ int obj_rot_x, obj_rot_y, obj_rot_z
     ³ int bounding_box_radius
     ³ int n1 = number_of_vertices_in_object
     ³ n1 * ³ short int vertex_x, vertex_y, vertex_z
     ³      ³ short int vertex_u, vertex_v
     ³ int n2 = Number_of_groups_in_object
     ³ n2 * ³ int n3 = number_of_polygons_in_group
     ³      ³ n3 * ³ int polygon_a, polygon_b, polygon_c
*/

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void Save( char *filename)
{
 FILE *fp;
 int header = FILE_HEADER;
 int temp;
 int i,j,k;

 fp = fopen(filename,"wb");

 fwrite(&header,sizeof(header),1,fp);             // header
 fwrite(&NumObjects,sizeof(NumObjects),1,fp);     // number of objects in file
 for (i=0;i<NumObjects;i++)
 {
  temp = 0;
  fwrite(&temp,sizeof(temp),1,fp);                 // object type (0 = branch, 1 = child)
  fwrite(&pos ,sizeof(pos) ,1,fp);                 // object pos in world space
  fwrite(&rot ,sizeof(rot) ,1,fp);                 // object rotations
  fwrite(&bbox,sizeof(bbox),1,fp);                 // bounding box radius
  fwrite(&NumVertices,sizeof(NumVertices),1,fp);   // number of vertices per object
  for (j=0;j<NumVertices;j++)
  {
   fwrite(&x,2,1,fp);
   fwrite(&y,2,1,fp);
   fwrite(&z,2,1,fp);
   fwrite(&u,2,1,fp);
   fwrite(&v,2,1,fp);
  }
  fwrite(&NumGroups,sizeof(NumGroups),1,fp);   // number of vertices per object

  for (k=0;k<Numgroups;k++)
  {
  }
 }
}


void main( int argc, const char *argv[] )
{
 if ( argc != 3)
 {
  printf("Usage: CONV <in-file> <out-file> ...\n");
  exit;
 }
}
