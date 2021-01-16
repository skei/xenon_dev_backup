// 3DS/ASC -> A3D converter

#include "defines.h"
#include "math.h"
#include "types.h"
#include <stdlib.h>   // exit
#include <stdio.h>    // file functions


// 컴컴� Global variables 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

unsigned int FILE_HEADER = 0x41334430       // 'A3D0'

/* 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

A3D file format

int   fileheader
int   number_of_objects_in_file
{
 int        object_pos
 int        object_rot
 int        bounding_sphere_radius
 int        number_of_vertices_in_object (NV)
 short int  vertice_data (x,y,z,u,v) (NU*5 short int's)
 int        number_of_groups_in_object
 {
  int       render_method
  int       bounding_box_radius
  char      texture file name (12 chars)
  char      group color (if no texture)
  int       number_of_polygons_in_group (NP)
  short int poly data (a,b,c) (NP*5 short int's)
 }
}

컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴 */

void SaveFile( char *filename)
{
 FILE *fp;

 fp = fopen(filename,"wb");
 fwrite(&FILE_HEADER,4,1,fp);


}

