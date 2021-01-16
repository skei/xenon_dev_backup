#ifndef types_h_included
#define types_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#include "defines.h"

typedef struct          // Coordinate
{
 int x,y,z;
} sCoord;

// -----

typedef struct
{
 int x,y,z;
} sVector;

// -----

typedef struct
{
 int num;              // number of cordinates in spline.
 int oClosed;           // If TRUE, then a closed spline. If FALSE, it's open.
 float bendsize;         // How soft the spline shall be. 0==Straight line, 1==very soft "bends".
 sCoord p[spline_size];   // The cordinates of the spline
} sSpline;

// -----

typedef struct
{
 sSpline s[MAXCOORDS];
} sMorph;

// -----

// Camera structure
typedef struct
{
 int status;
 int xang,yang,zang;
 int xpos,ypos,zpos;
 int targetx,targety,targetz;
 float upx,upy,upz;
 float Bank;
 int m[16];
} sCamera;

// ----- object structure -----

typedef struct
{
 int status;                    // 0/1 (not)/active
 void *next;                 // pointer to next object
 void *prev;                 // pointer to prevois object
 int rottype;                   // rotation type (face/vertex)
 int shadetype;                 // shading type (flat, gouraud, phong, etc)
 int xpos,ypos,zpos;            // Object's center in 3D
 int xang,yang,zang;            // Object's rotation angles
 int m[16];                     // Calculated matrix
 int e[16];                     // Calculated eye matrix
 int radius;                    // bounding sphere radius
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
} sObject;

// -----

typedef struct
{
 sObject *Objects;
 int cameratype;    // 0 = free pos/rot... 0 = pos/aim
 sCamera *Camera;
} sWorld;

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
