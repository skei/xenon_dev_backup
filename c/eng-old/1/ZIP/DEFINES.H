#ifndef defines_h_included
#define defines_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

// uncomment to remove all assert checks...
// #define NDEBUG

#define NULL 0
#define PI 3.1415926535
#define FALSE 0
#define TRUE 1

// Value for 3d->2d projection
#define PROJ_VALUE 256
#define PROJ_VALUE2 512

// for converting Z-values to int for radix sort
#define Z_MULT 1

// MATERIAL.Flags

// POLYGON.Flags

// OBJECT.Flags

#define OBJECT_active 1
#define OBJECT_twosided 2
#define OBJECT_inherit 4
#define OBJECT_envmap 65536

// CAMERA.Flags

// LIGHT.Flags

#define LIGHT_directional 1
#define LIGHT_point 2

// WORLD.Flags

#define CAMERA_free 1
#define CAMERA_aim  2

// float -> int

#define S 65536.0
#define MAGIC (((S*S*16)+(S*.5))*S)
#define MAGICfixed (MAGIC/65536.0)


// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif


