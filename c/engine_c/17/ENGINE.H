#ifndef engine_h_included
#define engine_h_included

#include "defines.h"
#include "types.h"

struct camstruc cam;

int omatrix[16],ematrix[16],cmatrix[16],mat[16];

// Polyfillers
int tricoords[MAXCOORDS*2];
int texcoords[MAXCOORDS*2];
// Matrix/Rotation
int visible;
int numbers[256];
int zcoords[MAXCOORDS];
int numvisiblepolys;
int rotated[MAXCOORDS*3];
int rotatedfacenormals[MAXPOLYS*3];
int rotatedvertexnormals[MAXCOORDS*3];
// ByteSort
struct sortstruc sorttable[MAXPOLYS];
struct sortstruc sorted[MAXPOLYS];
// Sine/Cosine tables
int sine[TABLESIZE];
int cosine[TABLESIZE];

int ymin,ymax;

int lightx,lighty;
int lxadd,lyadd;

char *buffer;

#endif
