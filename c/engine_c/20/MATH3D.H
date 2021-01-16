#ifndef math3d_h_included
#define math3d_h_included

// --------------------------------------------------------------------------
// structures
// --------------------------------------------------------------------------

typedef struct          // Coordinate
{
 int x,y,z;       			// fixed point >> 14 (DIVD)
} vector;

// --------------------------------------------------------------------------
// variables
// --------------------------------------------------------------------------

extern int sine[];
extern int cosine[];

// --------------------------------------------------------------------------
// functions
// --------------------------------------------------------------------------

// fixed point multiplication...
int fixedmul2(int, int);
int fixedmul3(int, int, int);

// Calculate sin/cos tables.
void calc_sincos(void);

// ----------------
// Matrix functions
// ----------------

void Matrix_Identity(int *m);
void Matrix_Translation(int *m, int tx, int ty, int tz);
void Matrix_Rotation(int *m,int xang, int yang, int zang);
void Matrix_Mult(int *a, int *b);
void Matrix_Process(int *m, int *src, int *dst, int num);
// Copies the contents of m2 into m1
void Matrix_Copy(int *m1, int *m2);

// ----------------
// Vector functions
// ----------------

// Normalize vector
vector Vector_Norm(vector v);
// dot product of two vectors
int Vector_Dot(vector v1, vector v2);
// cross product of two vectors
vector Vector_Cross(vector v1, vector v2);
// add two vectors together
vector Vector_Add(vector v1, vector v2);
// subtract vector v2 from vector v1
vector Vector_Sub(vector v1, vector v2);
// translate vector by (rotation) matrix
vector Vector_Trans(vector v, int *m);

#endif
