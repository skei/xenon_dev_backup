// --------------------------------------------------------------------------
// defines.h
// defines various aspects of engine
// --------------------------------------------------------------------------
#ifndef defines_h_included
#define defines_h_included

#define VisCVersion
//#define WatcomVersion

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#define spline_size 256 // Maximum number of cordinates in the spline

#define PI 3.1415926535

// max number of coords and polys per object
#define MAXPOLYS 8192
#define MAXCOORDS 8192
#define MAXOBJECTS 256

// size of sin/cos tables
#define TABLESIZE 4096

// fixed point precision... 14 bits fractional part
// #define DIVD 14
// 2^DIVD = fixed point 1.0
// #define DIVD_NUM 16384

// Distance from eye/camera to projection plane (screen)..
// Kinda like a zoom parameter
#define DISTANCE 128
// Shift left value for projection (constant for screen)
#define PROJ_PARAM 9

#endif
