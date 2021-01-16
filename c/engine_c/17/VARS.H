#ifndef vars_h_included
#define vars_h_included

#include "defines.h"
#include "types.h"

extern struct camstruc cam;

extern int omatrix[16];
extern int ematrix[16];
extern int cmatrix[16];
extern int mat[16];

// Polyfillers
extern short int tricoords[MAXCOORDS*2];
extern short int texcoords[MAXCOORDS*2];
extern char bfr[65536*9];      // Background buffer
extern char *buffer;
extern char *usetexture1;
extern char texture[65536];
extern char palette[768];
// Matrix/Rotation
extern int visible;
extern int numbers[256];
extern int zcoords[MAXCOORDS];
extern int numvisiblepolys;
extern int rotated[MAXCOORDS*3];
extern int rotatedfacenormals[MAXPOLYS*3];
extern int rotatedvertexnormals[MAXCOORDS*3];
// ByteSort
extern struct sortstruc sorttable[MAXPOLYS];
extern struct sortstruc sorted[MAXPOLYS];
// Sine/Cosine tables
extern int sine[TABLESIZE];
extern int cosine[TABLESIZE];

extern int ymin;
extern int ymax;

extern int lightx;
extern int lighty;
extern int lxadd;
extern int lyadd;


#endif
