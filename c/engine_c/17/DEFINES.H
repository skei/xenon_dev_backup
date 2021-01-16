#ifndef defines_h_included
#define defines_h_included

#define WORD unsigned short int
#define DWORD unsigned int

#define DIVD 14                 // Fixed Point 2^14
#define DIVD_NUM 16384          // 2^14 = 16384
#define TABLESIZE 4096          // Size of sin/cos tables

#define MAXCOORDS 2048          // Max number of coordinates
#define MAXPOLYS 2048           // Max number of polys
#define PI 3.1415926535

#define DISTANCE 256    // Distance from eye/camera to projection plane (screen)..
												// Kinda like a zoom parameter
#define PROJ_PARAM 9		// Shift left value for projection (constant for screen)

#endif

