#ifndef defines_h_included
#define defines_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#define NULL 0
#define FALSE 0
#define TRUE 1
#define PI 3.1415926535

#define DISTANCE 128
// distance from eye/camera to projection plane (screen) "zoom parameter"
#define PROJ_PARAM 512
// value to multiply all Poly.z with (for sorting - radix=integer)
#define Z_MULT 1//256

#define viewdistance PROJ_PARAM
#define halfscreenwidth_vd  (320/2)/viewdistance
#define halfscreenheight_vd (200/2)/viewdistance
#define minclip_z 10
#define maxclip_z 4000
// Size of sin/cos tables
#define TABLESIZE 4096

// Group.RenderMethod
#define RENDER_GouraudTexture 1
#define RENDER_EnvMap         2

// Object.Flags
#define OBJECT_Active 1

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
