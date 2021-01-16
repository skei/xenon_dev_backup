#include "defines.h"

// Sine/Cosine tables
int sine[TABLESIZE];
int cosine[TABLESIZE];
// bytesort variables
int numvisiblepolys;
int numbers[256];
// env mapping light pos (center of light in texture)
int env_lightx, env_lighty;

int *rotated;
int *sorted;
int *rotatedvertexnormals;
int *polysort;
int *tricoords;
int *texcoords;
int *zcoords;
unsigned char *BackgroundBuffer;


