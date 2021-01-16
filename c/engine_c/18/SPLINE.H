// spline.h

#ifndef spline_h_included
#define spline_h_included

#define spline_size 256 // Maximum number of polar cordinates in the spline

typedef struct					// Coordinate
{
 int x,y,z;       			// fixed point >> 14 (DIVD)
} coord;

// -----

typedef struct
{
 int num;              // number of cordinates in spline.
 int oClosed;           // If TRUE, then a closed spline. If FALSE, it's open.
 float bendsize;         // How soft the spline shall be. 0==Straight line, 1==very soft "bends".
 coord p[spline_size];   // The polar cordinates of the spline
} spline;

typedef struct
{
 int num;              // number of cordinates in spline.
 int oClosed;           // If TRUE, then a closed spline. If FALSE, it's open.
 float bendsize;         // How soft the spline shall be. 0==Straight line, 1==very soft "bends".
 float value[spline_size];   // The polar cordinates of the spline
} onespline;


coord get_spline_point(int location, spline * spl);
float get_spline_value(int location, onespline *spl);


#endif

