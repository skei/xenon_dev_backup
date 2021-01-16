// vars.h
// All global variables

#ifndef vars_h_included
#define vars_h_included

#include "defines.h"
#include "types.h"

struct camstruc cam;

float omatrix[16];	// object matrix
float cmatrix[16];	// camera matrix
float ematrix[16];	// eye/viewpoint marix
float mat[16];			// temporary
float aax,aay,aaz;

// The following should be allocated run-time instead of wasting space here...
int tricoords[MAXCOORDS*2];
int texcoords[MAXCOORDS*2];
char colorcoords[MAXCOORDS];
int rotated[MAXCOORDS*3];
int rotatedfacenormals[MAXPOLYS*3];
int rotatedvertexnormals[MAXCOORDS*3];
float sine[TABLESIZE];
float cosine[TABLESIZE];

// unsigned short int ColorMap[65536];
unsigned short int ShadeMap[64*256];
unsigned short int colortable[256];			// To simulate a 256-color mode...
								// use *p = colortable[color] instead of *p = color

struct polystruc poly;
int slope12;
int slope13;
int slope23;

// struct sortstruc sorttable[MAXPOLYS];
// struct sortstruc sorted[MAXPOLYS];
int polysort[MAXPOLYS];
int objsort[MAXOBJECTS];
int sorted[MAXPOLYS];			// used in bytesort...
;
int zcoords[MAXCOORDS];
int numbers[256];
int numvisiblepolys;
int numvisibleobjects;

int ymin,ymax;

int lightx,lighty;
int lxadd,lyadd;


// Yucku stuff for jalla clipping in slow polyfiller....

int clipx1;
int clipy1;
int clipx2;
int clipy2;

int clipz;

int bufferpointer, bufferpointersize;
int *paaaaa;

#endif