#ifndef vars_h_included
#define vars_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

// Sine/Cosine tables
extern int sine[TABLESIZE];
extern int cosine[TABLESIZE];
// bytesort variables
extern int numvisiblepolys;
extern int numbers[256];
// env mapping light pos (center of light in texture)
extern int env_lightx, env_lighty;

extern int *rotated;
extern int *sorted;
extern int *rotatedvertexnormals;
extern int *polysort;
extern int *tricoords;
extern int *texcoords;
extern int *zcoords;
extern unsigned char *BackgroundBuffer;

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
