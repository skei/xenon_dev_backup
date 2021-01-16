#include "raytrace.h"
#include "vbe2.h"
#include <math.h>
#include <conio.h>

unsigned short int *VesaP;

long sqrttab[0x100]; // declare table of square roots

// --------------------------------------------------------------------------

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

extern float FastSqrt(float a);
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

// --------------------------------------------------------------------------

// Magnitude determines the magnitude of a vector
float Magnitude(VectorType v1)
{
 float result;

 result = FastSqrt/*sqrt*/( v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
 return(result);
}

// --------------------------------------------------------------------------

// P1minusP2 finds the vector resulting from the difference of two points
VectorType P1minusP2 (coordType p1, coordType p2)
{
 VectorType result;

 result.x = p1.x - p2.x;
 result.y = p1.y - p2.y;
 result.z = p1.z - p2.z;
 return(result);
}


// --------------------------------------------------------------------------

// DotProduct  determines the dot product two vectors
float DotProduct( VectorType v1, VectorType v2)
{
 float result;

 result = (v1.x*v2.x) + (v1.y*v2.y) + ( v1.z* v2.z);
 return (result);
}

// --------------------------------------------------------------------------

// Normalize  returns the normalized vector of v1
VectorType Normalize(VectorType v1)
{
 VectorType      result;
 float mag,invmag;

 // mag = Magnitude(v1);
 mag = FastSqrt/*sqrt*/( v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
 invmag = 1/mag;
 result.x = v1.x * invmag;
 result.y = v1.y * invmag;
 result.z = v1.z * invmag;
 return (result);
}

// --------------------------------------------------------------------------

// CrosProduct determines the corss product of vector 1(v1) with vector 2 (v2)
VectorType CrossProduct (VectorType v1, VectorType v2)
{
 VectorType result;

 result.x = (v1.y*v2.z) - (v1.z*v2.y);
 result.y = (v1.z*v2.x) - (v1.x*v2.z);
 result.z = (v1.x*v2.y) - (v1.y*v2.x);
 return (result);
}

// --------------------------------------------------------------------------

// Intensity calculates the intensity based on the normal vector and the
// light direction vector.
IntensityChar Intensity(VectorType N)
{
 VectorType      normN;
 VectorType      normL;
 IntensityChar   calculated_intensity;
 float          cosTheta;       /*cos of angle between N and L*/

 /* Intensity: I= IaKa + IdKd(N*L)
    where:
     Ia = ambient light intensity
     Ka = ambient coefficient
     Id = directional light intensity
     Kd = difuse light coefficient
     N  = normal to the surface
     L  = light direction
 */

 // need to normalize the vectors
 normN = Normalize(N);
 normL = Normalize(L);
 cosTheta = DotProduct(normN,normL);
 // if the dotProduct is negative it implies a  surface on the
 // back side of an object therefor we can't see it and do not
 // need to calculate an intensity for it.
 if (cosTheta >= 0.0)
 {
  // Calculate r intensity
  calculated_intensity.rint = Ia.rint*ka + Id.rint*kd*cosTheta;
  if (calculated_intensity.rint > 1.0) calculated_intensity.rint = 1.0;
  // Calculate g intensity
  calculated_intensity.gint = Ia.gint*ka + Id.gint*kd*cosTheta;
  if (calculated_intensity.gint > 1.0) calculated_intensity.gint = 1.0;
  // Calculate b intensity
  calculated_intensity.bint = Ia.bint*ka + Id.bint*kd*cosTheta;
  if (calculated_intensity.bint > 1.0) calculated_intensity.bint = 1.0;
 }
 else
 {
  calculated_intensity.rint = 0.0;
  calculated_intensity.bint = 0.0;
  calculated_intensity.gint = 0.0;
 }
 return( calculated_intensity);
}

// --------------------------------------------------------------------------

// RayPlaneInter  calculates the intersection of a ray with a plane.
// A flag is used to indicate if an intersection was found

coordType  RayPlaneInter (rayType ray, planeType plane,float *soln, int *flag)
{
 /* To determine the intersection of a plane and a ray one
    must simultaneously solve the ray and plane equation
    Ray     X = X0 + s*u
    Plane   N*X - D = 0;
    The solution is
     s = - (N*X0 - D) /N*u
    where:
     N = the normal to the plane
     X0 = the starting ray position
     D = planes dist from origin
     u = ray direction
    Once the solution has been found (s) it must be plugged into the
    ray equation to find the intersection.
 */
 VectorType      N;
 coordType       X0;
 float          s;
 VectorType      u;
 float          D;
 coordType       intersection;
 VectorType      vect_to_point;

 // init variables
 *flag = SOLN;
 N = plane.norm;
 X0 = ray.pt;
 u = ray.dir;
 D = plane.dist_from_origin;
 // create a vector from pt in direction of ray  for use in dot product
 vect_to_point.x = X0.x;
 vect_to_point.y = X0.y;
 vect_to_point.z = X0.z;
 if ( DotProduct(N,u) == 0.0 )
 { // No solution
  *flag = NO_SOLN;
 }
 else
 {
  s = - ( (DotProduct(N,vect_to_point) - D) / DotProduct(N,u) );
  if (s >= ACCEPTED_ERROR)
  {
   intersection.x = X0.x + s*u.x;
   intersection.y = X0.y + s*u.y;
   intersection.z = X0.z + s*u.z;
  }
  else *flag = NO_SOLN;
 }
 *soln = s;
 return(intersection);
}

// --------------------------------------------------------------------------

//  RaySphereInter calculates the intersection of a ray
//  with a sphere.  A flag is used to indicate if the intersection was found.
coordType  RaySphereInter (rayType ray, sphereType sphere, float *soln, int *flag)
{
 /* To determine the intersection of a sphere and a ray one must
    simulatneously solve the ray equation and the sphere equation:
    Ray      X = X0 + s*u
    Sphere   |Ctr - X0|^2 - R^2 = 0
    The quadratic equ. =
     s^2(u*u) + 2 s u*(C-X0) + mag(C-X0)^2 + R^2
    Where:
     a = u*u
     b = 2 u*(C-X0)
     c = (mag(C-X0))^2 + R^2
    The soln = s = u*(C-X0) +/- sqrt( (u*(C-X0))^2 - (mag(C-X0))^2 + R^2)
    Once the solution has been found (s) it must be plugged into the
    ray equation to find the intersection.
 */
 coordType       X;              /*this will be the intersection pt*/
 coordType       X0;
 coordType       center;         /*center of sphere*/
 float          R;              /*radius*/
 VectorType      u;
 VectorType      deltaX;         /*X0 - C*/
 float          det;            /* b^2 - mag(deltaX)^2 + R^2*/
 float          a,b,c;
 float          s1,s2;          /*solution*/
 int             tmp;
 float _a;

 // Assume that a solution will be found.
 *flag = SOLN;
 u = Normalize(ray.dir);
 X0 = ray.pt;
 center = sphere.ctr;
 deltaX = P1minusP2(X0,center);
 R = sphere.radius;
 /* Determine the number of solutions by calculating a determinent
    In this case the determinent is : b^2 - mag(deltaX)^2 + R^2
    If the determinent :
     >0 2 solutions
     =0 1 solution
     <0 no solutions
 */
 /* a = DotProduct(u,u); */
    /* Note: this value should be 1.0 if a
       normalized vector is used.  I
       calculate it anyway just in case.
       I also just ralized that I don't use
       it anywhere so I guess later this can
       be modified to accept none normalized
       vectors
    */
 b = DotProduct(u,deltaX);
// c =   Magnitude(deltaX)
//     * Magnitude(deltaX) - R*R;
 _a = FastSqrt( deltaX.x*deltaX.x + deltaX.y*deltaX.y + deltaX.z*deltaX.z);
 c = _a*_a - R*R;

 det = b*b - c;
 if (det == 0)
 {
  s1 = -b;
  *soln = s1;
  X.x = X0.x + s1*u.x;
  X.y = X0.y + s1*u.y;
  X.z = X0.z + s1*u.z;
 }
 else if (det > 0)
 {
  s1 = (-b + FastSqrt/*sqrt*/(det));
  s2 = (-b - FastSqrt/*sqrt*/(det));
  if ((s1 < s2) && (s1 > 0))
  {
   *soln = s1;
   X.x = X0.x + s1*u.x;
   X.y = X0.y + s1*u.y;
   X.z = X0.z + s1*u.z;
  }
  else
  {
   *soln = s2;
   X.x = X0.x + s2*u.x;
   X.y = X0.y + s2*u.y;
   X.z = X0.z + s2*u.z;
  }
 }
 else
 {
  *flag = NO_SOLN;
 }
 return (X);
}

// --------------------------------------------------------------------------

ColorChar RayTrace(coordType startloc, int depth)
{
 coordType       inter,inter1,inter2;
 ColorChar       color;
 int             flag1,flag2;
 rayType         ray1;
 IntensityChar   I;
 VectorType      normal;
 int             intersection_found;
 float          dist1,dist2;

 ray1.pt = startloc;
 ray1.dir = viewDir;
 intersection_found = SOLN;
 // does ray hit sphere 1
 inter1 = RaySphereInter(ray1, sphere[1], &dist1, &flag1);
 inter2 = RaySphereInter(ray1, sphere[2], &dist2, &flag2);

 // determine which intersection is closest to screen
 if (flag1 == SOLN)
 {
  if (flag2 == SOLN)
  {
   // need to determine which is closest
   if (dist1 < dist2)
   {
    inter = inter1;
    // compute the normal at that intersection -  it is the coord!!
    normal.x = inter.x;
    normal.y = inter.y;
    normal.z = inter.z;
   }
   else
   {
    inter = inter2;
    normal.x = inter.x;
    normal.y = inter.y;
    normal.z = inter.z;
//    normal = plane1.norm;
   }
  }
  else
  // intersection is with sphere 1 only
  {
   inter = inter1;
   // compute the normal at that intersection -  it is the coord!!
   normal.x = inter.x;
   normal.y = inter.y;
   normal.z = inter.z;
  }
 }
 else if (flag2 == SOLN) // check intersection with sphere 2 (plane)
 {
  inter = inter2;         // intersection is with plane
  normal.x = inter.x;
  normal.y = inter.y;
  normal.z = inter.z;
//  normal = plane1.norm;
 }
 else // no intersections
 {
  intersection_found = NO_SOLN;
 }
 if (intersection_found == SOLN)
 {
  // determinine light intensity (shading) based on normal and light
  I = Intensity(normal);
  color.gc = (31*I.gint);
  color.bc = (31*I.bint);
  color.rc = (31*I.rint);
 }
 // if no then intensity is background
 else color = backColor;
 // return color
 return(color);
}

// --------------------------------------------------------------------------

void Render()
{
 coordType first_pix_loc;
 float xincr, yincr;
 int i,j;
 coordType cur_pix_loc;
 ColorChar cur_pix_int;

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
   cur_pix_int = RayTrace(cur_pix_loc, 1);

   VesaP[j*320+i] =   ((((int)cur_pix_int.rc))<<10)
                    + ((((int)cur_pix_int.gc))<< 5)
                    + ((((int)cur_pix_int.bc))<< 0);
  }
 }
}

// --------------------------------------------------------------------------

void TraceInit()
{
 VectorType      horiz;
 VectorType      vert;

 backColor = BLACK;
 // determine direction of projection and window in view plane
 // Find the vectors which will determine the plane
 horiz = CrossProduct( lookAt, scrUp );
 vert = CrossProduct(horiz, lookAt);
 // normalize the vectors
 scrHoriz = Normalize(horiz);
 scrVert = Normalize(vert);
}

// --------------------------------------------------------------------------

main( int argc, char *argv[] )
{
 float s1x,s1y,s1z;
 float s2x,s2y,s2z;

 BuildSqrtTable();
 VesaInit(320,200,15);
 VesaP = (unsigned short int *)VesaPtr;
 Set_Vesa(VesaMode);

 sphere[1].radius = 0.8;
 sphere[2].radius = 0.6;

 while (!kbhit())
 {
  sphere[1].ctr.x = sin(s1x);
  sphere[1].ctr.y = sin(s1y);
  sphere[1].ctr.z = sin(s1z);

  sphere[2].ctr.x = sin(s2x);
  sphere[2].ctr.y = sin(s2y);
  sphere[2].ctr.z = sin(s2z);

  s1x+= 0.01; s1y+= 0.02; s1z+= 0.03;
  s2x+= 0.03; s2y+= 0.02; s2z+= 0.04;

  TraceInit();
  Render();
 }
 Set_Vesa(3);
 VesaUnInit();
 exit(0);
}
