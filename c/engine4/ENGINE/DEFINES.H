#ifndef defines_h_included
#define defines_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#define NULL 0
#define PI 3.1415926535

#define TABLESIZE 4096      // size of sin/cos table
#define DIVD 14             // fixed point precision. number of decimal bits
#define DIVD_NUM 16384      // fixed point 1

#define DISTANCE 128        // distance from eye/camera to projection plane (screen)
                            // kind of like a zoom parameter
#define PROJ_PARAM 9        // shift left value for projection
#define PROJ_PARAM_value 512

#define viewdistance PROJ_PARAM_value
#define halfscreenwidth_vd  ((320/2) << 8)/viewdistance
#define halfscreenheight_vd ((200/2) << 8)/viewdistance
#define minclip_z 10
#define maxclip_z 4000

// object.status values
#define OBJECT_ACTIVE 1

// object load
#define FILE_HEADER 0x41334430       // 'A3D0'

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif

