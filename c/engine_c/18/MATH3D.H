// math3d.h

#ifndef math3d_h_included
#define math3d_h_included

typedef struct					// Coordinate
{
 int x,y,z;       			// fixed point >> 14 (DIVD)
} vector;

vector Normalize(vector v);
float DotProduct(vector v1, vector v2);
vector CrossProduct(vector v1, vector v2);

#endif