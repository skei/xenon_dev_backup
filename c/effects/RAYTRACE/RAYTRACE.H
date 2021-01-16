#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define X_RES 160
#define Y_RES 100

#define ACCEPTED_ERROR 0.00001

/*quantized pixel values*/
typedef struct
{
 unsigned char rc;
 unsigned char gc;
 unsigned char bc;
} ColorChar;

ColorChar image[X_RES][Y_RES];  /*storage for all pixel values*/
ColorChar WHITE   = {1,1,1};
ColorChar BLACK = {0,0,0};
ColorChar backColor;

/*
 Remember that the world coordinate system is based on the following
 axis:
  x axis - increasing right horizontal; (left to right)
  y axis - increasing front horizontal  (front to back)
  z axis - increasing up vertical       (bottom to top)
*/

typedef struct vector
{
 float  x;
 float  y;
 float  z;
} VectorType;

VectorType viewDir = {0.0,1.0,-1.0};

VectorType scrUp = {0.0,0.0,1.0};     /* View Up Vector in book*/
VectorType scrHoriz;
VectorType scrVert;

VectorType lookAt = {0.0, 1.0, -1.0};   /* View Reference Point in book*/

/* screen coordinates  -1.5 -> 1.5 i.e 3x3 screen*/
/* screen coordinates specified within plane*/

typedef struct
{
 float  x;
 float  y;
 float  z;
} coordType;

typedef struct screen
{
 float  xmin;
 float  ymin;
 float  xmax;
 float  ymax;
} screenCoord;

screenCoord virtScreen = {-1.5, -1.2 , 1.5, 1.2};

/*define screen in world coordinate system*/
coordType xminWc;
coordType xmaxWC;
coordType ymaxWC;
coordType yminWC;

coordType scrOrigin = {0,-1.0,1.0};

typedef struct
{
 coordType ctr;
 float radius;
} sphereType;

sphereType sphere[5];// =
//{     /*initialize the sphere obj for this proj*/
// 0.0,    /*Center pt - x location*/
// 0.0,    /*Center pt - y location*/
// 0.0,    /*Center pt - z location*/
// 1.0     /*Radius*/
//};

typedef struct
{
 coordType       pt;
 VectorType      dir;
} rayType;

typedef struct
{
 float          dist_from_origin;
 VectorType      norm;
} planeType;

planeType plane1 =
{
 0.0,          /*the dist from origin*/
 0.0,0.0,1.0   /*normal in positive Z direction*/
};

#define SOLN    1
#define NO_SOLN 0

typedef struct
{
 float rint;
 float gint;
 float bint;
} IntensityChar;

IntensityChar Ia = {1.0,0.0,0.0};   /*Ambient Light Intensity*/
IntensityChar Id = {1.0,1.0,1.0};   /*Diffuse Light Intensity*/

float  ka = 0.1; /*Ambient Light coefficient*/
float  kd = 0.9; /*Diffuse Light coefficient*/

VectorType L = {1,-2,2};   /*Light direction vector*/
