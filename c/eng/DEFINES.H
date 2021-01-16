#ifndef defines_h_included
#define defines_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#define PI 3.1415926535
// max number of coords and polys per object
#define MAXPOLYS 8192
#define MAXCOORDS 8192
#define MAXOBJECTS 256
// size of sin/cos tables
#define TABLESIZE 4096
// fixed point precision... 14 bits fractional part
#define DIVD 14
// 2^DIVD = fixed point 1.0
#define DIVD_NUM 16384
// Distance from eye/camera to projection plane (screen)..
// Kinda like a zoom parameter
#define DISTANCE 128
// Shift left value for projection (constant for screen)
#define PROJ_PARAM 9
#define PROJ_PARAM_value 512
// max number of coordinates in spline
#define spline_size 8

#define viewdistance PROJ_PARAM_value
#define halfscreenwidth_vd  ((320/2) << 8)/viewdistance
#define halfscreenheight_vd ((200/2) << 8)/viewdistance
#define minclip_z 10
#define maxclip_z 4000

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
