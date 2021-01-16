 // Nytt design/interfact - Art v0.2á

 #include <math.h>
 #include "gfx.c"

 /* ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 17.06.99 (15:00)
   Started rewriting the raytracer...

 Ideas:

   flags: specular, shadow cast/receive, etc, etc... to speed up things

 ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ */

//
// ÄÄÄ defines ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

#define REALTIME

 // 8x8 grid
#define GRID   8
 // 4 rays per grid-square
#define JITTER 4

#define NULL           0
#define PI             3.1415926535
#define PI2            (PI*2)
#define MAXLEVEL       2
#define SHADOWSTRENGTH 0.3

//
// ÄÄÄ typedefs ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

typedef struct tColor
 {
  float r,g,b;
 } tColor;

 // ---

typedef struct tVector
 {
  float x,y,z;
 } tVector;

 // ---

 #define MAT_PLAIN   0
 #define MAT_CHECKER 1

 // ---

typedef struct tMaterial
 {
  unsigned int flags;
  tColor  ambient;
  tColor  diffuse;
  tColor  diffuse2;
  tColor  specular;
  float   specular_width;
  float   reflection_level;
  float   transparency;
  float   refraction_index;
  float   scale;
 } tMaterial;

 // ---

typedef struct tObject
 {
  int type;
  tVector p;        // Sphere.center / plane normal
  float   r;        // sphere.radius / plane.distancefromoriginalongnormal
  tMaterial      *material;
  struct tObject *next;
  // function pointers
 } tObject;

 // ---

typedef struct tLight
 {
  tVector        pos;
  tColor         color;
  struct tLight *next;
 } tLight;

 // ---

typedef struct tRay
 {
  tVector  o;     // Ray.origin
  tVector  d;     // Ray.direction
 } tRay;

 // ---

typedef struct tCamera
 {
  float   lookDistance; // distance to view plane
  float   hfov,vfov;    // horizontal & vertical Field Of View
  int     xres,yres;    // screen resolution
  float   aspect;       // aspect ratio
  tVector dir;          // view direction
  //
  float  xdir,ydir;
  tVector LLdir;
 } tCamera;

 // ---

typedef struct tScene
 {
  tCamera   *Camera;
  tObject   *Objects;
  tLight    *Lights;
 } tScene;

 // ---

//
// ÄÄÄ variables ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

tScene Scene;

//
// ÄÄÄ vector math ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

float DotProduct(tVector *v1, tVector *v2)
 {
  return (v1->x*v2->x) + (v1->y*v2->y) + ( v1->z* v2->z);
 }

 // ---

void  CrossProduct (tVector *dst, tVector *v1, tVector *v2)
 {
  dst->x = (v1->y*v2->z) - (v1->z*v2->y);
  dst->y = (v1->z*v2->x) - (v1->x*v2->z);
  dst->z = (v1->x*v2->y) - (v1->y*v2->x);
 }

 // ---

float Length(tVector *v)
 {
  float f;

  f = (v->x*v->x) + (v->y*v->y) + (v->z*v->z);
  if (f > 0) return sqrt(f);

  return 0;
 }

 // ---

void  Normalize(tVector *dst, tVector *src)
 {
  float f,invf;

  f = sqrt( (src->x*src->x) + (src->y*src->y) + (src->z*src->z) );
  invf = 1/f;

  dst->x = src->x * invf;
  dst->y = src->y * invf;
  dst->z = src->z * invf;
 }

 // ---

 // calculate the reflection vector.
void  Reflect(tVector *dst, tVector *n, tVector *v1)
 {
  float a;
  a = 2 * DotProduct(n, v1);
  dst->x = v1->x - (n->x * a);
  dst->y = v1->y - (n->y * a);
  dst->z = v1->z - (n->z * a);
 }

 // ---

 // calculate the refracted vector.
void  Refract(tVector *dst, tVector *n, tVector *v1, float index)
 {
  float p,t;
  float invindex;

  // invindex, and (index*index) can be precalculated for each object...
  invindex = 1/index;
  dst->x   = 0;
  dst->y   = 0;
  dst->z   = 0;
  p = DotProduct(n, v1);
  if (p<0)
  {
   t = 1 - (1-p*p) / (index*index);
   if( t <= 0 ) return;
   t = -p/index - sqrt(t);
  }
  else
  {
   index = invindex;
   t = 1 - (1-p*p) / (index*index);
   if (t<=0) return;
   t = -p/index + sqrt(t);
	}
  dst->x = (v1->x * invindex) + n->x * t;
  dst->y = (v1->y * invindex) + n->y * t;
  dst->z = (v1->z * invindex) + n->z * t;
 }

//
// ÄÄÄ raytracing routines ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

//
// --- intersection ---
//

float Intersect_Plane(tRay *R, tObject *O)
 {
  float t,aa,bb,cc,dd;

  t = - (O->p.x * R->d.x + O->p.y * R->d.y + O->p.z * R->d.z);
  if (t == 0) return -1;
  t = ( O->r + O->p.x * R->o.x + O->p.y * R->o.y + O->p.z * R->o.z) / t;
  if ( t <= 0 ) return -1;
  return t;
 }

 // ---

float Intersect_Sphere(tRay *R, tObject *O)
 {
  float A,B,C,D;
  float t1,t2,sq;
  float rox_scx,roy_scy,roz_scz;

  rox_scx = R->o.x - O->p.x;
  roy_scy = R->o.y - O->p.y;
  roz_scz = R->o.z - O->p.z;

  // A = R->d.x * R->d.x + R->d.y * R->d.y + R->d.z * R->d.z;
  A = 1;
  B = 2 * (   R->d.x * rox_scx
            + R->d.y * roy_scy
            + R->d.z * roz_scz );
  C = (  rox_scx * rox_scx
       + roy_scy * roy_scy
       + roz_scz * roz_scz) - (O->r * O->r);
  D = B*B - 4*C;
  if (D<0) return -1;
  sq = sqrt(D);
  t1 =  (-B + sq ) * 0.5;
  t2 =  (-B - sq ) * 0.5;
  if ( (t1 < t2) && (t1>0) ) return t1;
  else return t2;
 }

//
// --- normals ---
//

void GetNormal_Sphere(tVector *N, tVector *IP, tObject *O)
 {
  N->x = (IP->x - O->p.x) / O->r;
  N->y = (IP->y - O->p.y) / O->r;
  N->z = (IP->z - O->p.z) / O->r;
 }

 // ---

void GetNormal_Plane(tVector *N, tVector *IP, tObject *O)
 {
  N->x = O->p.x;
  N->y = O->p.y;
  N->z = O->p.z;
 }

//
// --- color ---
//

void GetColor_Sphere(tColor *C,tObject *O, tVector *IP, tVector *IN)
 {
  tMaterial *M;
  int x,y;

  M = O->material;
  if (M->flags == MAT_PLAIN)
  {
   C->r = M->diffuse.r;
   C->g = M->diffuse.g;
   C->b = M->diffuse.b;
  }
  else
  {
   if (M->flags & MAT_CHECKER)
   {
    x = M->scale * (int)(0.5 + (IN->x)*0.5);
    y = M->scale * (int)(0.5 + (IN->y)*0.5);
   }
  }
 }

 // ---

void GetColor_Plane(tColor *C,tObject *O, tVector *IP, tVector *IN)
 {
  tMaterial *M;

  M = O->material;
  if (M->flags == MAT_PLAIN)
  {
   C->r = M->diffuse.r;
   C->g = M->diffuse.g;
   C->b = M->diffuse.b;
  }
 }


//
// ÄÄÄ "api" routines ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

 // arrays of object-type specific routines.
 // Call these by indexing the object type into these arrays

 // object types

 #define OBJ_IGNORE   -1
 #define OBJ_SPHERE    0
 #define OBJ_PLANE     1
 #define OBJ_UNIONAND  2
 #define OBJ_UNIONSUB  3


 // returns t of closest intersection, or -1
 float (*intersect[])(tRay *R, tObject *O) =
 {
  Intersect_Sphere,
  Intersect_Plane
 };

 // calculate normal
 void  (*getnormal[])(tVector *N, tVector *IP, tObject *O) =
 {
  GetNormal_Sphere,
  GetNormal_Plane
 };

 // get u/v for texture mapping
 void (*getcolor[])(tColor *C,tObject *O, tVector *IP, tVector *IN) =
 {
  GetColor_Sphere,
  GetColor_Plane,
 };

 // same as intersect, but returns both t's
 // int   (*calct1t2[32])(tRay *R, tObject *O, float *t1, float *t2);
 // int   (*inside[])(tRay *R, tObject *O, float t);

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 //   The first step is to compute two vectors, one in the direction of
 // increasing x coordinates for the image, and one for increasing y coords,
 // that are scaled to be the length of a pixel in each respective direction
 // (aspect ratio calcs go in here). Then compute a vector that points
 // towards the upper left corner of the screen (I will give pseudo code
 // later).
 //   When you have this information, you can quickly create any ray to fire
 // by simply adding appropriately scaled versions of the x and y direction
 // vectors to the vector pointing to the first pixel (of course you want to
 // normalise the final vector.

 // PreProcessing stage:
 //   xdir = (2 * lookDistance * tan(hfov / 2)) / xres
 //   ydir = (2 * lookDistance * tan(vfov / 2)) / yres
 //   ydir *= aspect // Adjust y pixel size given aspect ratio
 //   LLdir = camera.dir - (xdir * xres)/2 - (ydir * yres)/2
 // Computing the ray to fire:
 //   dir = LLdir + xdir * x + ydir * y
 //   dir = dir normalised

void  Camera_Setup(tCamera *C)
 {
  C->xdir = (2 * C->lookDistance * tan(C->hfov / 2)) / C->xres;
  C->ydir = (2 * C->lookDistance * tan(C->vfov / 2)) / C->yres;
  C->ydir *= C->aspect;    // Adjust y pixel size given aspect ratio
  C->LLdir.x = C->dir.x - ((C->xdir * C->xres)/2);
  C->LLdir.y = C->dir.y - ((C->ydir * C->yres)/2);
 }

 // ---

 // return a float between min and max
float frand(float min, float max)
 {
  return min + ((max-min) * (float)rand()/32767);
 }

//
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

 // This is the main function.... Traces a ray, finds intersection,
 // calculates shading, recursively calls Trace for reflected/refracted rays
void  Trace(tColor *C, tRay *R, tObject *sender, int level)
 {
  tObject   *O;
  tObject   *Oi;
  tMaterial *M;
  tVector    ip;
  tVector    in;
  tVector    lv;
  tVector    rv;
  tColor     tc;
  tRay       tr;
  float      t;
  float      tmin;
  float      maxdist;
  int        shadowed;
  float      diff;
  float      spec;
  int ix,iy,iz,sum;

  // --- return if we have reached max recursion level ----------------------

  if (level >= MAXLEVEL)
  {
   C->r = 0;
   C->g = 0;
   C->b = 0;
   return;
  }

  // --- find closest intersected object ------------------------------------

  // intersection formulas requires a normalized Ray.Direction vector
  Normalize(&R->d,&R->d);
  O    = NULL;
  tmin = 999999;
  Oi = Scene.Objects;
  while (Oi!=NULL)
  {
   if (Oi != sender) // don't test objects against themselves
   {
    t = intersect[Oi->type](R,Oi);
    if ((t > 0) && (t < tmin))
    {
     tmin = t;
     O    = Oi;
    }
   }
   Oi=Oi->next;
  }

  // --- return if no intersection -----------------------------------------

  // If no objects were intersected, return no color
  if (O == NULL)
  {
   C->r = 0;
   C->g = 0;
   C->b = 0;
   return;
  }

  M = O->material;

  // --- we have found an intersection --------------------------------------

  t = tmin;

  // --- the intersection point ---------------------------------------------

  ip.x = R->o.x + t * R->d.x;
  ip.y = R->o.y + t * R->d.y;
  ip.z = R->o.z + t * R->d.z;

  // --- shadow -------------------------------------------------------------

  shadowed = 1;

  tr.o.x = ip.x;
  tr.o.y = ip.y;
  tr.o.z = ip.z;

  tr.d.x = Scene.Lights->pos.x - ip.x;
  tr.d.y = Scene.Lights->pos.y - ip.y;
  tr.d.z = Scene.Lights->pos.z - ip.z;

  maxdist = Length(&tr.d);

  if (maxdist > 0)
  {
   // We can probably optimize this one a bit further. We just need to
   // divide it by the sqrt of the previously calculated length....
   // make a separate normalize routine, and pass the length ???
   Normalize(&tr.d,&tr.d);

   // Didn't work too well ??? Didn't save too many intersection tests... ??
   //// try the object that previously shadowed first
   //num = OBJECTS[obj].lastshadow;
   //if (num != -1)
   //{
   // switch (OBJECTS[num].type)
   // {
   //  case 0 : t = Intersect_Sphere(&tr,num); break;
   // }
   // if ((t > 0) && (t < maxdist)) goto inshadow;
   //}

   // no, so we try the rest of the objects

   Oi = Scene.Objects;
   while (Oi != NULL)
   {
    if ((Oi != O)/* && (Oi != O->lastshadow)*/)
    {
     t = intersect[Oi->type](&tr,Oi);
     if ((t > 0) && (t < maxdist))
     {
      //O->lastshadow = Oi;

      #ifdef REALTIME
       C->r = M->ambient.r;   // or 0,0,0 ???
       C->g = M->ambient.g;
       C->b = M->ambient.b;
       return;
      #endif

      goto inshadow;
     }
    }
    Oi = Oi->next;
   }
   shadowed = 0;
  }

  inshadow:

  // for realtime usage, we can make all shadowed parts BLACK, just to save
  // some cycles... Unrealistic, but much faster...



  // --- intersection normal ------------------------------------------------

  // get the intersection normal
  getnormal[O->type](&in,&ip,O);

  // --- diffuse ------------------------------------------------------------

  // the vector from light to intersection

  lv.x = tr.d.x;//Scene.Lights->pos.x - ip.x;
  lv.y = tr.d.y;//Scene.Lights->pos.y - ip.y;
  lv.z = tr.d.z;//Scene.Lights->pos.z - ip.z;

  // Normalize(&lv,&lv);

  diff = DotProduct(&in,&lv);

  // --- specular -----------------------------------------------------------

  // hah! Another optimization possibility! The Reflect function calculates
  // the DotProduct between in and lv, and since we have this already...
  Reflect(&rv, &in, &lv); // reflect light by intersection normal
  spec = DotProduct(&rv, &R->d);

  // --- calculate the actual color -----------------------------------------

  C->r = M->ambient.r;
  C->g = M->ambient.g;
  C->b = M->ambient.b;

  if (diff > 0)
  {
   getcolor[O->type](&tc,O,&ip,&in);
   C->r += diff * tc.r;//M->diffuse.r;
   C->g += diff * tc.g;//M->diffuse.g;
   C->b += diff * tc.b;//M->diffuse.b;
  }

  if (spec > 0)
  {
   spec  = pow(spec,M->specular_width);  // pow'en her er 'expensive' !!!!
   C->r += spec * M->specular.r;    // fins det andre enn phong ???
   C->g += spec * M->specular.g;
   C->b += spec * M->specular.b;
  }

  // --- reflection ---------------------------------------------------------

  if (M->reflection_level > 0)
  {
   // the dot prod calculated here (inúR->d) is also needed by Refract
   Reflect(&rv, &in, &R->d);
   tr.o.x = ip.x;
   tr.o.y = ip.y;
   tr.o.z = ip.z;
   tr.d.x = rv.x;
   tr.d.y = rv.y;
   tr.d.z = rv.z;
   Trace(&tc, &tr, O,level+1);
   C->r += tc.r * M->reflection_level;
   C->g += tc.g * M->reflection_level;
   C->b += tc.b * M->reflection_level;
  }

  // --- refraction ---------------------------------------------------------

  // refraction is too expensive for realtime usage ????
  // pr›v oss med refraction-less transparency...

  // more optimizations... The dot product calculated in the reflection
  // routine (inúR->d) is also needed by the Refraction
  tr.d.x = R->d.x;
  tr.d.y = R->d.y;
  tr.d.z = R->d.z;

  if (M->refraction_index > 0)
  {
   Refract(&rv,&in,&R->d,M->refraction_index);
   //if ((rv.x != 0) && (rv.y != 0) && (rv.z != 0))
   //tr.o.x = ip.x;
   //tr.o.y = ip.y;
   //tr.o.z = ip.z;
   tr.d.x = rv.x;
   tr.d.y = rv.y;
   tr.d.z = rv.z;
  }
  if (M->transparency > 0)
  {
   Trace(&tc,&tr,O,level+1);
   C->r += tc.r * M->transparency;
   C->g += tc.g * M->transparency;
   C->b += tc.b * M->transparency;
  }

  // --- apply shadow -------------------------------------------------------

  if (shadowed == 1)
  {
   C->r *= SHADOWSTRENGTH;
   C->g *= SHADOWSTRENGTH;
   C->b *= SHADOWSTRENGTH;
  }

  // 'clamp' colors...
  if (C->r > 1) C->r = 1;
  if (C->g > 1) C->g = 1;
  if (C->b > 1) C->b = 1;
 }

//
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
//

 #define GFX_X 320
 #define GFX_Y 200

 #define CALC_X 80
 #define CALC_Y 50

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

tMaterial Material1 =
 {
  MAT_PLAIN,            // flags
  { 0.1, 0.1, 0.1 },    // ambient
  { 0.0, 0.5, 0.9 },    // diffuse
  { 0.0, 0.0, 0.5 },    // diffuse2
  { 1.0, 1.0, 1.0 },    // specular
  8,                    // specular_width
  0.9,                  // reflection_level
  0.5,                  // transparency
  0.0,                  // refraction_index
  1.0                   // scale
 };

tMaterial Material2 =
 {
  MAT_PLAIN,            // flags
  { 0.1, 0.1, 0.1 },    // ambient
  { 0.3, 0.2, 0.8 },    // diffuse
  { 0.0, 0.0, 0.5 },    // diffuse2
  { 1.0, 1.0, 1.0 },    // specular
  32,                   // specular_width
  0.4,                  // reflection_level
  0.8,                  // transparency
  0.0,                  // refraction_index
  1.0                   // scale
 };



tMaterial Material3 =
 {
  MAT_PLAIN,            // flags
  { 0.1, 0.1, 0.1 },    // ambient
  { 0.4, 0.8, 0.0 },    // diffuse
  { 0.0, 0.0, 0.5 },    // diffuse2
  { 1.0, 1.0, 1.0 },    // specular
  32,                   // specular_width
  0.9,                  // reflection_level
  0.3,                  // transparency
  0.0,                  // refraction_index
  1.0                   // scale
 };

 // ---

tLight Light1 =
 {
  { -100, -100, 0 },     // pos
  { 1.0, 1.0, 1.0 },    // color (ignored right now
  NULL                  // next in linked list
 };

tObject Plane1 =
 {
  OBJ_PLANE,            // type
  { 0,-1,0 },           // sphere.center / plane normal
  50,                  // sphere.radius / plane.distancefromorigin
  &Material1,           // material
  NULL                  // next in linked list
 };

tObject Sphere2 =
 {
  OBJ_SPHERE,           // type
  { -50,0,300 },          // sphere.center / plane normal
    40,                   // sphere.radius / plane.distancefromorigin
  &Material2,           // material
  &Plane1               // next in linked list
 };

tObject Sphere1 =
 {
  OBJ_SPHERE,           // type
  { 50,25,300 },          // sphere.center / plane normal
    5,                   // sphere.radius / plane.distancefromorigin
  &Material3,           // material
  &Sphere2               // next in linked list
 };

// ---

 int buffer[80*50];

 #define SCALE75(x) (x - ((x>>2)&0x3f3f3f))
 #define SCALE50(x) (     (x>>1)&0x7f7f7f )
 #define SCALE25(x) (     (x>>2)&0x3f3f3f )

 // ---

 int temp1[320];
 int temp2[320];

 // ---

 #define pset24(d,c)           \
 {                             \
  *d++ = (c & 0xff0000) >> 16; \
  *d++ = (c & 0x00ff00) >> 8;  \
  *d++ =  c & 0x0000ff;        \
 }

 // ---

 // dst = 24-bit image
 // src = INT 24-bit image, high byte ignored...
void scaleX(int *dst, int *src)
 {
  int x,c1,c2;
  int *d,*s;

  d  = dst;
  s  = src;
  c1 = 0;
  s++;
  for (x=0;x<79;x++)
  {
   c2 = *s++;
   *d++ = c1;
   *d++ = SCALE75(c1) + SCALE25(c2);
   *d++ = SCALE50(c1) + SCALE50(c2);
   *d++ = SCALE25(c1) + SCALE75(c2);
   c1 = c2;
  }
  c2 = 0;
  *d++ = c1;
  *d++ = SCALE75(c1) + SCALE25(c2);
  *d++ = SCALE50(c1) + SCALE50(c2);
  *d++ = SCALE25(c1) + SCALE75(c2);
 }

 // ---

void Scale(byte *dst, int *src)
 {
  int   x,y;
  int   c1,c2;
  byte *d1,*d2,*d3,*d4;
  int  *s1,*s2,*s,*tmp;

  d1 = dst;
  d2 = dst + 960*1;
  d3 = dst + 960*2;
  d4 = dst + 960*3;

  s = src + 80;

  memset(temp1,0,sizeof(temp1));
  s1 = temp1;
  s2 = temp2;
  for (y=0;y<49;y++)
  {
   scaleX(s2,s);
   // interpolate between temp1 & temp2...
   for (x=0;x<320;x++)
   {
    c1 = s1[x];
    c2 = s2[x];
    pset24(d1, c1);
    pset24(d2, SCALE75(c1) + SCALE25(c2) );
    pset24(d3, SCALE50(c1) + SCALE50(c2) );
    pset24(d4, SCALE25(c1) + SCALE75(c2) );
   }
   d1  += (960 * 3);
   d2  += (960 * 3);
   d3  += (960 * 3);
   d4  += (960 * 3);
   s   += 80;
   tmp  = s1;
   s1   = s2;
   s2   = tmp;
  }
  memset(s2,0,sizeof(temp2));
  for (x=0;x<320;x++)
  {
   c1 = s1[x];
   c2 = s2[x];
   pset24(d1, c1);
   pset24(d2, SCALE75(c1) + SCALE25(c2) );
   pset24(d3, SCALE50(c1) + SCALE50(c2) );
   pset24(d4, SCALE25(c1) + SCALE75(c2) );
  }
 }


 // ---

void test1(void)
 {
  tRay    R;
  tColor  C;
  int     x,y;
  int     i;

  Scene.Lights  = &Light1;
  Scene.Objects = &Sphere1;
  // ---
  // Ray origin
  R.o.x = 0;
  R.o.y = 0;
  R.o.z = 0;

  while (!kbhit())
  {
   for (y=0;y<CALC_Y;y++)
   {
    for (x=0;x<CALC_X;x++)
    {
     // setup ray
     R.d.x = (x - (CALC_X/2)) - R.o.x;
     R.d.y = (y - (CALC_Y/2)) - R.o.y;
     R.d.z = R.o.z + (CALC_X * (4/3)); // 256 : beregn denne v.hj.a X/YSIZE og FOV... ???
     Trace(&C,&R,NULL,0);
     //BMP_putpixelRGB(Screen,x,y,C.r*255,C.g*255,C.b*255);
     buffer[y*80+x] = (int)(C.r*255) + ((int)(C.g*255)<<8) + ((int)(C.b*255)<<16);
    }
   }
   Scale((byte *)Vesa_Ptr,buffer);
   Light1.pos.x += 3;
   Material1.diffuse.r += 0.03;
   Sphere1.r    += 0.7;
   Sphere1.p.z  += 1;
   Sphere2.p.x  += 1;
  }
 }

 // ---

void main(void)
 {
  GFX_init(GFX_X,GFX_Y);
  getch();
  test1();
  getch();
  GFX_exit();
 }

 /* ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 40x25 w/jittering... 1000 rays + jittersize

 ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ */
