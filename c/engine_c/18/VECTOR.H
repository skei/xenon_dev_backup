// vector.h

#ifndef vector_h_included
#define vector_h_included

typedef struct					// Coordinate
{
 int x,y,z;       			// fixed point >> 14 (DIVD)
} vector;

vector Vector_Norm(vector v);
float Vector_Dot(vector v1, vector v2);
vector Vector_Cross(vector v1, vector v2);
vector Vector_Add(vector v1, vector v2);
vector Vector_Sub(vector v1, vector v2);
vector Vector_Trans(vector v, float *m);

#endif