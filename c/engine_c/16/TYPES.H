#ifndef types_h_included
#define types_h_included

#include "defines.h"

typedef struct          // Coordinate
{
 int x,y,z;
} coord;

// -----

typedef struct
{
 int num;              // number of cordinates in spline.
 int oClosed;           // If TRUE, then a closed spline. If FALSE, it's open.
 float bendsize;         // How soft the spline shall be. 0==Straight line, 1==very soft "bends".
 coord p[spline_size];   // The cordinates of the spline
} spline;

// -----

typedef struct
{
 spline s[MAXCOORDS];
} morphstruc;

// -----


#ifdef VisCVersion
 typedef struct
 {						// Screen structure
  int width;
  int height;
  unsigned short * screen;
 } screen_S;
#else
 typedef struct
 {						// Screen structure
  int width;
  int height;
  unsigned char * screen;
 } screen_S;
#endif

// Camera structure

typedef struct
{
 int status;
 int xang,yang,zang;
 int xpos,ypos,zpos;
 int targetx,targety,targetz;
 float upx,upy,upz;
 float Bank;
 float m[16];
} camstruc;

// object structure

typedef struct
{
 int status;                    // 0/1 (not)/active
 struct objstruc *next;         // pointer to next object
 struct objstruc *prev;         // pointer to prevois object
 int rottype;                   // rotation type (face/vertex)
 int shadetype;                 // shading type (flat, gouraud, phong, etc)
 int xpos,ypos,zpos;            // Object's center in 3D
 int xang,yang,zang;            // Object's rotation angles
 float m[16];                   // Calculated matrix
 float e[16];                   // Calculated eye matrix
 int bbox[24];                  // Bounding box
 int numcoords;                 // Number of Vertexes/Coordinates
 int numpolys;                  // Number of Polys/Faces
 int *coords;                   // Pointer to list of coordinates
 int *polys;                    // Pointer to polygon data
 int *vertex_n;                 // Pointer to vertex normals
 int *face_n;                   // Pointer to face normals
 char *phongmap;                // Pointer to PhongMap
 int *bump_c;                   // Pointer to bump coordinates
 unsigned short int *bumpmap;   // Pointer to BumpMap
 int *tex_c;                    // Pointer to Texture coordinates
 char *texmap;                  // Pointer to texture map
} objstruc;

#endif
