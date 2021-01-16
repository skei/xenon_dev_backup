#include "defines.h"
#include "types.h"

struct camstruc cam;

int omatrix[16];
int ematrix[16];
int cmatrix[16];
int mat[16];

// Polyfillers
short int tricoords[MAXCOORDS*2];
short int texcoords[MAXCOORDS*2];
char bfr[65536*9];      // Background buffer
// buffer : pointer to 960*600 buffer
char *buffer;
char *usetexture1;
char texture[65536];
char palette[768];
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

int ymin;
int ymax;

int lightx;
int lighty;
int lxadd;
int lyadd;


