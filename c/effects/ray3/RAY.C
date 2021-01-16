// Axon - RayTracing... Hvorfor i #&%Ï+@ finner den ingen intersections?
// --------------------------------------------------------------------------

#include <math.h>

//
// --- defines --------------------------------------------------------------
//


#define ScreenWidth  256
#define ScreenHeight 192

#define TraceScreenWidth 24
#define TraceScreenHeight 18
#define ScreenLeft   -12
#define ScreenTop    -9

#define MaxSpheres 20

//
// --- typedefs -------------------------------------------------------------
//

typedef struct sRay
 {
  float Xo,Yo,Zo;     // Ray origo
  float Xd,Yd,Zd;     // Ray direction
  float Xp,Yp,Zp;     // "screen pixel" ray is going through
  float Length;
 } sRay;

 // ---

typedef struct sSphere
 {
  float Xc,Yc,Zc;       // Sphere center
  float Sr;             // Sphere radius
 } sSphere;

//
// --- variables ------------------------------------------------------------
//

float Xi,Yi,Zi;   // closest intersection so far
float Xn,Yn,Zn;   // normal to --"--
float Ti;

int NumSpheres;
sSphere Spheres[MaxSpheres];

//
// --- functions ------------------------------------------------------------
//

void SetMode(int);
 #pragma aux SetMode=\
  "int 10h",\
 parm [eax];

 // ---

void SetRGB (char,char,char,char);
 #pragma aux SetRGB=\
  "mov dx,3c8h",\
  "out dx,al",\
  "inc dx",\
  "mov al,ah",\
  "out dx,al",\
  "mov al,bl",\
  "out dx,al",\
  "mov al,bh",\
  "out dx,al",\
 parm [al] [ah] [bl] [bh] modify [edx];

// ---

 // check intersection with sphere
 // return time value
int TestSphere(sRay *R, sSphere *S)
 {
  float A,B,C,D;
  float sd;
  float t0,t1;
  float ti;
  float inv;

  // B = 2 * ( ray.direction * (ray.start-sphere.center) )
  A = (R->Xd*R->Xd)+(R->Yd*R->Yd)+(R->Zd*R->Zd);
  B = 2*(  R->Xd*(R->Xo-S->Xc)
         + R->Yd*(R->Yo-S->Yc)
         + R->Zd*(R->Zo-S->Zc) );
  // C = (ray.start-sphere.center)ý - sphere.radiusý
  C =   (R->Xo-S->Xc)*(R->Xo-S->Xc)
      + (R->Yo-S->Yc)*(R->Yo-S->Yc)
      + (R->Zo-S->Zc)*(R->Zo-S->Zc)
      - (S->Sr*S->Sr);
  D = ((B*B) - (4*A*C));
  if (D > 0) return 1;
  else return 0;
   // there is an intersection
   // sd = sqrt(D);
   // t0 = (-B+sd)/(2*A);
   // t1 = (+B+sd)/(2*A);
   // if (t0 < t1) ti = t0; else ti = t1;
   /*
    Ti = ti;
    Xi = R->Xo + R->Xd*Ti;
    Yi = R->Yo + R->Yd*Ti;
    Zi = R->Zo + R->Zd*Ti;
    //// FIND THE NORMAL TO THE SURFACE
    inv = 1/S->Sr;
    Xn = (Xi-S->Xc)*inv;
    Yn = (Yi-S->Yc)*inv;
    Zn = (Zi-S->Zc)*inv;
    //// CALCULATE NORMALIZED RGB COLOR BY MULTIPLYING BY A SHADING FACTOR
    //ShadeFactor = ShadingFactor(HitPt, Norm, ViewDir);
    //Color.Red = Objects[i].Color.Red * ShadeFactor;
    //Color.Green = Objects[i].Color.Green * ShadeFactor;
    //Color.Blue = Objects[i].Color.Blue * ShadeFactor;
   */
 }

//
// ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//

void main(void)
 {
  float Dx,Dy;      // delta x/y for screen
  sRay R;
  int x,y,n;
  float length,inv;


  // --- Set up scene ---

  Spheres[0].Xc = 0;
  Spheres[0].Yc = 0;
  Spheres[0].Zc = 10;
  Spheres[0].Sr = 5;

  Spheres[1].Xc = 5;
  Spheres[1].Yc = 2;
  Spheres[1].Zc = 7;
  Spheres[1].Sr = 10;

  NumSpheres = 2;

  // --- other setup ---

  Dx = TraceScreenWidth/ScreenWidth;
  Dy = TraceScreenHeight/ScreenHeight;


  // eye
  R.Xo = 0;
  R.Yo = 0;
  R.Zo = -20;

  // ---

  SetMode(0x13);

  // starting at upper left
  R.Zp = 0;

  R.Yp = ScreenTop + (0.5*Dy);
  for ( y=0 ; y<ScreenHeight ; y++, R.Yp+=Dy )
  {
   R.Xp = ScreenLeft + (0.5*Dx);
   for ( x=0 ; x<ScreenWidth ; x++, R.Xp+=Dx )
   {
    Ti = 999999;
    // calc & normalize ray-direction
    {
     R.Xd = R.Xp - R.Xo;
     R.Yd = R.Yp - R.Yo;
     R.Zd = R.Zp - R.Zo;
     R.Length = sqrt(R.Xd*R.Xd + R.Yd*R.Yd + R.Zd*R.Zd);
     if (R.Length!=0)
     {
      inv = 1/R.Length;
      R.Xd *= inv;
      R.Yd *= inv;
      R.Zd *= inv;
     }
    }
    // test all spheres
    Ti = 0;
    for (n=0;n<NumSpheres;n++)
    {
     Ti += TestSphere(&R,&(Spheres[n]));
     if (Ti != 0) *(char *)(0xA0000+(y*320+x)) = 1;
      else *(char *)(0xA0000+(y*320+x)) = 8;
    }
   }
  }
  getch();
  SetMode(3);
 }
