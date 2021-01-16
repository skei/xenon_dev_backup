// defines.h

#ifndef defines_h_included
#define defines_h_included

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 256

#define DISTANCE 128		// Distance from eye/camera to projection plane (screen)..
												// Kinda like a zoom parameter
#define PROJ_PARAM 9		// Shift left value for projection (constant for screen)

// #define EnMult 255

#define DIVD 14					// Fixed point sixe... 2^14... << & >> 14
#define DIVD_NUM 16384			// Fixed Point 1.00 = 16384
#define TABLESIZE 4096			// Sizeof Sin/Cos tables... 4096 "angles"
#define hicolor(a,b,c) { (WORD)(((a&0xf8)<<7) + ((b&0xf8)<<2) + (c>>3)) }

#define MAXCOORDS 2048
#define MAXPOLYS 2048
#define MAXOBJECTS 256
#define PI 3.1415926535

#endif