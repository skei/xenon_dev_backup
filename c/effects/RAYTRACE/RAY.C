 #include "vbe2.h"
 #include <math.h>
 #include <conio.h>

//
// --- define ---------------------------------------------------------------
//

#define X_RES 80
#define Y_RES 50
#define ACCEPTED_ERROR 0.00001

#define MAX_SPHERES 10

#define SOLN    1
#define NO_SOLN 0

//
// --- typedef --------------------------------------------------------------
//

typedef struct sVector
 {
  float x,y,z;
 } sVector;

 // ---

typedef struct sCoord
 {
  float x,y,z;
 } sCoord;

 // ---

typedef struct sRay
 {
  sCoord pt;
  sVector dir;
 } sRay;


 // ---

typedef struct sScreen
 {
  float  xmin;
  float  ymin;
  float  xmax;
  float  ymax;
 } sScreen;

 // ---

typedef struct sColor
 {
  float r,g,b;
 } sColor;

 // ---

typedef struct sSphere
 {
  sCoord ctr;
  float radius;

  sCoord Intersection;
  float distance;
  int flag;
 } sSphere;


//
// --- variables ------------------------------------------------------------
//

unsigned short int *VesaP;
long sqrttab[0x100]; // declare table of square roots

sVector scrHoriz;
sVector scrVert;

int GlobalNumSpheres;
sSphere spheres[MAX_SPHERES];

sVector Light;


//
// --- initialized variables ---
//

sVector viewDir = {0.0,0.0,1.0};
sVector scrUp = {0.0,1.0,0.0};      // View Up Vector
sVector lookAt = {0.0, 0.0, 100.0};  // View Reference Point
sScreen virtScreen = {-1.5, -1.2 , 1.5, 1.2};
sCoord scrOrigin = {0,-1.0,1.0};

sColor Ia = {1.0,0.0,0.0};   // Ambient Light Intensity
sColor Id = {0.0,1.0,1.0};   // Diffuse Light Intensity

float ka = 0.1;   // Ambient Light coefficient
float kd = 0.9;   // Diffuse Light coefficient


//
// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//

//
// --- init functions ---
//

void BuildSqrtTable(void)
 {
  unsigned short i;
  float f;
  unsigned int *fi = (unsigned*)&f;   // To access the bits of a float in
                                     // C quickly we must misuse pointers
  for (i = 0; i <= 0x7f; i++)
  {
   *fi = 0;
   // Build a float with the bit pattern i as mantissa
   // and an exponent of 0, stored as 127
   *fi = (i << 16) | (127 << 23);
   f = sqrt(f);
   // Take the square root then strip the first 7 bits of
   // the mantissa into the table
   sqrttab[i] = (*fi & 0x7fffff);
   // Repeat the process, this time with an exponent of 1,
   // stored as 128
   *fi = 0;
   *fi = (i << 16) | (128 << 23);
   f = sqrt(f);
   sqrttab[i+0x80] = (*fi & 0x7fffff);
  }
 }

 // --------------------------------------------------------------------------

 //extern float FastSqrt(float a);
float FastSqrt(float a);
 #pragma aux FastSqrt modify nomemory [EBX EDX] parm [EAX] value no8087 = \
 "   MOV     EDX,EAX              " \
 "   AND     EAX,0x007FFFFF       " \
 "   SUB     EDX,0x3F800000       " \
 "   MOV     EBX,EDX              " \
 "   AND     EDX,0xFF000000       " \
 "   SAR     EDX,1                " \
 "   AND     EBX,0x00800000       " \
 "   ADD     EDX,0x3F800000       " \
 "   OR      EAX,EBX              " \
 "   SHR     EAX,16               " \
 "   MOV     EBX,OFFSET sqrttab   " \
 "   DB      0x8B, 0x04, 0x83     " \
 "   ADD     EAX,EDX              "

 // -------------------------------------------------------------------------

//
// --- math functions ---
//

float DotProduct( sVector *v1, sVector *v2)
 {
  return (v1->x*v2->x) + (v1->y*v2->y) + ( v1->z* v2->z);
 }

 // ---

void Normalize(sVector *v1, sVector *result)
 {
  float mag,invmag;

  mag = FastSqrt( v1->x*v1->x + v1->y*v1->y + v1->z*v1->z);
  if (mag != 0)
  {
   invmag = 1/mag;
   result->x = v1->x * invmag;
   result->y = v1->y * invmag;
   result->z = v1->z * invmag;
  }
 }

 // ---

void CrossProduct (sVector *v1, sVector *v2, sVector *result)
 {
  result->x = (v1->y*v2->z) - (v1->z*v2->y);
  result->y = (v1->z*v2->x) - (v1->x*v2->z);
  result->z = (v1->x*v2->y) - (v1->y*v2->x);
 }

 // ---

void VectorSub( sVector *v1, sVector *v2, sVector *result )
 {
  result->x = v1->x - v2->x;
  result->y = v1->y - v2->y;
  result->z = v1->z - v2->z;
 }

 // ---

void VectorAdd( sVector *v1, sVector *v2, sVector *result )
 {
  result->x = v1->x + v2->x;
  result->y = v1->y + v2->y;
  result->z = v1->z + v2->z;
 }

//
// --- raytrace ---
//

void TraceInit()
 {
  sVector horiz,vert;

  // determine direction of projection and window in view plane
  // Find the vectors which will determine the plane
  CrossProduct( &lookAt, &scrUp,  &horiz );
  CrossProduct( &horiz,  &lookAt, &vert  );
  // normalize the vectors
  Normalize( &horiz, &scrHoriz );
  Normalize( &vert,  &scrVert  );
 }

 // ---

 // calculates the intensity based on the normal vector and the light
 // direction vector.
void CalcIntensity ( sVector *N, sVector *L, sColor *c)
 {
  sVector normN;
  sVector normL;
  float cosTheta;               // cos of angle between N and L

  // Intensity: I= IaKa + IdKd(N*L)
  // where:
  //   Ia = ambient light intensity
  //   Ka = ambient coefficient
  //   Id = directional light intensity
  //   Kd = difuse light coefficient
  //   N  = normal to the surface
  //   L  = light direction

  // need to normalize the vectors
  Normalize( N, &normN );
  Normalize( L, &normL );
  cosTheta = DotProduct(&normN,&normL);

  // if the dotProduct is negative it implies a  surface on the
  // back side of an object therefor we can't see it and do not
  // need to calculate an intensity for it.
  if (cosTheta >= 0.0)
  {

   // Calculate r intensity
   c->r = Ia.r*ka + Id.r*kd*cosTheta;
   if (c->r > 1.0) c->r = 1.0;

   // Calculate g intensity
   c->g = Ia.g*ka + Id.g*kd*cosTheta;
   if (c->g > 1.0) c->g = 1.0;

   // Calculate b intensity
   c->b = Ia.b*ka + Id.b*kd*cosTheta;
   if (c->b > 1.0) c->b = 1.0;

  }
  else
  {
   c->r = 0.0;
   c->g = 0.0;
   c->b = 0.0;
  }
 }

 // ---

 //  RaySphereInter calculates the intersection of a ray
 //  with a sphere. A flag is used to indicate if the intersection was found.
void RaySphereInter ( sRay *ray, sSphere *sphere )
 {
  // To determine the intersection of a sphere and a ray one must
  // simulatneously solve the ray equation and the sphere equation:
  // Ray      X = X0 + s*u
  // Sphere   |Ctr - X0|^2 - R^2 = 0
  // The quadratic equ. =
  //  s^2(u*u) + 2 s u*(C-X0) + mag(C-X0)^2 + R^2
  // Where:
  //  a = u*u
  //  b = 2 u*(C-X0)
  //  c = (mag(C-X0))^2 + R^2
  // The soln = s = u*(C-X0) +/- sqrt( (u*(C-X0))^2 - (mag(C-X0))^2 + R^2)
  // Once the solution has been found (s) it must be plugged into the
  // ray equation to find the intersection.

  sCoord X0;
  sCoord center;  // center of sphere
  float R;        // radius
  sVector u;
  sVector deltaX; // X0 - C
  float det;      // b^2 - mag(deltaX)^2 + R^2
  float a,b,c;
  float s1,s2;    // solution
  int tmp;
  float _a;

  // Assume that a solution will be found.
  sphere->flag = SOLN;
  Normalize(&(ray->dir),&u);
  X0 = ray->pt;
  center = sphere->ctr;
  VectorSub(&X0,&center,&deltaX);
  R = sphere->radius;

  // Determine the number of solutions by calculating a determinent
  // In this case the determinent is : b^2 - mag(deltaX)^2 + R^2
  // If the determinent :
  //  >0 2 solutions
  //  =0 1 solution
  //  <0 no solutions

  // a = DotProduct(u,u);

  // Note: this value should be 1.0 if a
  //       normalized vector is used.  I
  //       calculate it anyway just in case.
  //       I also just ralized that I don't use
  //       it anywhere so I guess later this can
  //       be modified to accept none normalized
  //       vectors

  b = DotProduct(&u,&deltaX);
  // c =   Magnitude(deltaX)
  //     * Magnitude(deltaX) - R*R;
  _a = FastSqrt( deltaX.x*deltaX.x + deltaX.y*deltaX.y + deltaX.z*deltaX.z);
  c = _a*_a - R*R;

  det = b*b - c;
  if (det == 0)
  {
   s1 = -b;
   sphere->distance = s1;
   sphere->Intersection.x = X0.x + s1*u.x;
   sphere->Intersection.y = X0.y + s1*u.y;
   sphere->Intersection.z = X0.z + s1*u.z;
  }
  else if (det > 0)
  {
   s1 = (-b + FastSqrt/*sqrt*/(det));
   s2 = (-b - FastSqrt/*sqrt*/(det));
   if ((s1 < s2) && (s1 > 0))
   {
    sphere->distance = s1;
    sphere->Intersection.x = X0.x + s1*u.x;
    sphere->Intersection.y = X0.y + s1*u.y;
    sphere->Intersection.z = X0.z + s1*u.z;
   }
   else
   {
    sphere->distance = s2;
    sphere->Intersection.x = X0.x + s2*u.x;
    sphere->Intersection.y = X0.y + s2*u.y;
    sphere->Intersection.z = X0.z + s2*u.z;
   }
  }
  else
  {
   sphere->flag = NO_SOLN;
  }
 }

 // ---

void RayTrace( sCoord *startloc, int depth, sColor *color)
 {
  sRay ray;
  sVector normal,l;
  int intersection_found;
  int i;
  int SphereNum;
  float distance;

  ray.pt = *startloc;
  ray.dir = viewDir;

  intersection_found = 0;
  distance = 10000;
  SphereNum = 0;

  for (i=0;i<GlobalNumSpheres;i++)
  {
   RaySphereInter( &ray, &(spheres[i]) );
   intersection_found += spheres[i].flag;
   if (    (spheres[i].flag == SOLN)
        && (spheres[i].distance < distance))
   {
    SphereNum = i;
    distance = spheres[i].distance;
   }
  }

  if (intersection_found != 0)
  {

   // make light vector
   VectorSub ( &Light,
               &(spheres[SphereNum].Intersection),
               &l );
   //l.x = 0;
   //l.y = 0;
   //l.z = 1;

   VectorAdd( &(spheres[SphereNum].Intersection),
              &(spheres[SphereNum].ctr),
              &normal);

   normal = (spheres[SphereNum].Intersection);

   // determinine light intensity (shading) based on normal and light
   CalcIntensity ( &normal, &l, color );
  }
  // if no then intensity is background
  else
  {
   color->r = 0;
   color->g = 0;
   color->b = 0;
  }
 }

 // ---

void Render()
 {
  sCoord first_pix_loc;
  float xincr, yincr;
  int i,j;
  sCoord cur_pix_loc;
  sColor c;
  unsigned int RGB;

  // we have the plane and the coordinates, need to determine the starting
  // pixel locations
  first_pix_loc.x =   scrOrigin.x + virtScreen.xmin*scrHoriz.x
                    + virtScreen.ymax*scrVert.x;
  first_pix_loc.y =   scrOrigin.y + virtScreen.xmin*scrHoriz.y
                    + virtScreen.ymax*scrVert.y;
  first_pix_loc.z =   scrOrigin.z + virtScreen.xmin*scrHoriz.z
                    + virtScreen.ymax*scrVert.z;
  // determine the increments, based on the resolution and the screen coords
  xincr = (virtScreen.xmax - virtScreen.xmin) / X_RES;
  yincr = (virtScreen.ymax - virtScreen.ymin) / Y_RES;
  cur_pix_loc = first_pix_loc;

  for (j=0; j<Y_RES; j++)
  {
   cur_pix_loc.x = first_pix_loc.x - (j*yincr)*scrVert.x;
   cur_pix_loc.y = first_pix_loc.y - (j*yincr)*scrVert.y;
   cur_pix_loc.z = first_pix_loc.z - (j*yincr)*scrVert.z;
   for (i=0 ; i<X_RES; i++)
   {
    if (i != 0)
    {
     cur_pix_loc.x = cur_pix_loc.x + (xincr)*scrHoriz.x;
     cur_pix_loc.y = cur_pix_loc.y + (xincr)*scrHoriz.y;
     cur_pix_loc.z = cur_pix_loc.z + (xincr)*scrHoriz.z;
    }
    // trace the ray and get color (intensity ) of pixel
    RayTrace ( &cur_pix_loc, 1, &c );
    VesaP[j*320+i] = (   (((unsigned int)(c.r*31))<<10)
                       + (((unsigned int)(c.g*31))<< 5)
                       + (((unsigned int)(c.b*31))<< 0));

   }
  }
 }



//
// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//

void main( int argc, char *argv[] )
 {
  float s1x,s1y,s1z;
  float s2x,s2y,s2z;

  BuildSqrtTable();
  VesaInit(320,200,15);
  VesaP = (unsigned short int *)VesaPtr;
  Set_Vesa(VesaMode);

  Light.x = 0;
  Light.y = 0;
  Light.z = 100;

  GlobalNumSpheres = 2;

  spheres[0].radius = 0.8;
  spheres[1].radius = 0.7;

  while (!kbhit())
  {
   spheres[0].ctr.x = sin(s1x);
   spheres[0].ctr.y = sin(s1y);
   spheres[0].ctr.z = sin(s1z);

   spheres[1].ctr.x = sin(s2x);
   spheres[1].ctr.y = sin(s2y);
   spheres[1].ctr.z = sin(s2z);

   s1x+= 0.01; s1y+= 0.02; s1z+= 0.03;
   s2x+= 0.03; s2y+= 0.02; s2z+= 0.04;

   TraceInit();
   Render();
  }

  Set_Vesa(3);
  VesaUnInit();
 }

