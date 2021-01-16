 #include <math.h>
 #include "gfx.c"

 /* ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 LOG. Started 15.06.99 (08:00)

 15.06.99 (22:00)
   First working version. spheres only, shadows, ambient, diffuse, specular
   shading, reflections, refractions.
 16.06.99 (01.00)
   Tried some optimizations. Did a few in the sphere intersection routine,
   and changed a few divs to the inverse *1/n...
   Removed the FaastSqrt stuff, as the result became fucking ugly! Not
   precise enough...
 16.06.99 ((11:30)
   First prototype of CSG (Constructive Solid Geometry) working!
   Union of two spheres can be calculated now...
   Awkward and crummy interface and usage right now...
   Will redefine the tObject structure, and the union should have a
   ptr to another tObjct structure...
   The raytracer traces the first numObj objects in the list, so we malloc
   more tobject's than we need and insert these kinds of objects after the
   first numObj objects...
 16.06.99 (17:30)
   Now CSG is working a bit better... Now we have UnionAnd and UnionSub. We
   can take one sphere and subtract from the other... And ot works... Don't
   know about the speed yet, but....
 16.06.99 (22:00)
   Code for planes inserted. Wonder if the CSG stuff works with planes ???

 -----

   Next thing to to is to rewrite the entire RayTracer engine, to simplify
   the interface. A structure of object-specific functions, and calling
   the function by indexing the object-type into an array of these
   functions... Would simplify things a LOT... And have to find a standard
   way of using functions like: Intersection, GetNormal, GetT (CSG), etc...
   Also objects could have stuff like this, perhaps

   Todo/bugs
   - refractive objects cast full-blocking shadows! Perhaps they should
     filter the light in some way?
   - refraction index = 1 cause a vertical line at x = 0.. Look at the
     refr. calculation routine...
   - look at the plane stuff.. Something seems wrong
   - Seems like something weird is going on, or that I have mixed up some
     vector systms here and there ????  The Normal vector for the plane
     doesn't seem to work when used how I thought they should be... Have to
     investigate a bit here...
   - some easier way to setup objects and scene... Perhaps even a scene-file
     (coompatible with some other format???)
   - polygons... check plane first, project to 2d, point in triangle test..
   - fog/haze and other atmospheric effects
   - distance attenuation in lighting formulas
   - mapping
   - blobs and other object. quadric surfaces?



 ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ */



//
// ÄÄÄ defines ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//


#define PI 3.1415926535

#define MAXLEVEL       3
#define SHADOWSTRENGTH 0.3

#define GFX_X 640
#define GFX_Y 480

#define CALC_X 640
#define CALC_Y 480

 int numintersections;
 int numintersectionstried;

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

 #define OBJ_IGNORE   0
 #define OBJ_SPHERE   1
 #define OBJ_PLANE    2
 #define OBJ_UNIONAND 3
 #define OBJ_UNIONSUB 4

typedef struct tObject
 {
  int type;
  // sphere data
  tVector c;
  float   r;
  // plane
  float   aa,bb,cc,dd;
  // union
  int     o2;           // the object to intersect with this
  int     closest;
  // surface
  tColor  ambient;
  tColor  diffuse;
  tColor  specular;
  float   shinyness;
  float   reflection_level;
  float   refraction_level;
  float   refraction_index;
  // optimizations
  float   r2;
  float   invindex2;
  int     lastshadow;
 } tObject;

 // ---

typedef struct tLight
 {
  tVector pos;
 } tLight;

 // ---

typedef struct tRay
 {
  tVector  o;     // Ray.origin
  tVector  d;     // Ray.direction
 } tRay;

//
// ÄÄÄ variables ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

int      numLight;
tLight  *LIGHTS;
int      numObj;
tObject *OBJECTS;

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
void  Refract(tVector *dst, tVector *n, tVector *v1, float index, float invindex2)
 {
  float   p,t;
  float invindex;

  invindex = 1/index;
  dst->x = 0;
  dst->y = 0;
  dst->z = 0;
  p = DotProduct(n, v1);
  if (p<0)
  {
   // t = 1 - ( 1 - p*p ) / ( index*index );
   t = 1 - ( 1 - p*p ) * invindex2;
   if( t <= 0 ) return; // v2
   t = -p/index - sqrt(t);
  }
  else
  {
   index = invindex;
   t = 1 - ( 1 - p*p ) / ( index*index );
   if( t <= 0 ) return;// v2;
   t = -p/index + sqrt(t);
	}
  dst->x = (v1->x * (invindex)) + n->x * t;
  dst->y = (v1->y * (invindex)) + n->y * t;
  dst->z = (v1->z * (invindex)) + n->z * t;
 }

//
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

//
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

float Intersect_Plane(tRay *R, int o)
 {
  float t,aa,bb,cc,dd;
  tObject *P;

  // aa,bb,cc = normal ???

  P = &(OBJECTS[o]);
  t = - (P->aa * R->d.x + P->bb * R->d.y + P->cc * R->d.z);
  // parallell?
  if (t <= 0) return -1;
  t = (( P->dd + P->aa * R->o.x + P->bb * R->o.y + P->cc * R->o.z) / t);
  if ( t <= 0 ) return -1;
  return t;
 }

 // ---

float Intersect_Sphere(tRay *R, int o)
 {
  tObject *S;
  float A,B,C,D;
  float t1,t2,sq;
  float rox_scx,roy_scy,roz_scz;

  numintersectionstried++;
  S = &(OBJECTS[o]);
  rox_scx = R->o.x - S->c.x;
  roy_scy = R->o.y - S->c.y;
  roz_scz = R->o.z - S->c.z;

  A = R->d.x * R->d.x + R->d.y * R->d.y + R->d.z * R->d.z;
  // A = 1;
  //B = 2 * (   R->d.x * ( R->o.x - S->c.x )
  //          + R->d.y * ( R->o.y - S->c.y )
  //          + R->d.z * ( R->o.z - S->c.z ) );
  B = 2 * (   R->d.x * rox_scx
            + R->d.y * roy_scy
            + R->d.z * roz_scz );
  //C = (  (R->o.x - S->c.x) * (R->o.x - S->c.x)
  //     + (R->o.y - S->c.y) * (R->o.y - S->c.y)
  //     + (R->o.z - S->c.z) * (R->o.z - S->c.z) ) - (S->r * S->r);
  C = (  rox_scx * rox_scx
       + roy_scy * roy_scy
       + roz_scz * roz_scz) - S->r2;
  //D = B*B - 4*A*C;
  D = B*B - 4*C;
  if (D<0) return -1;
  numintersections++;
  sq = sqrt(D);
  //t1 =  (-B + sq ) * (A/2);
  //t2 =  (-B - sq ) * (A/2);
  t1 =  (-B + sq ) * 0.5;
  t2 =  (-B - sq ) * 0.5;
  if ( (t1 < t2) && (t1>0) ) return t1;
  else return t2;
 }

 // ---

 /* TESTING (CSG)

  Sett ovenfra

             t  t
             0  1
             °°°°
      -----> °°±±²
             °°±±²
               ²²²

  Hvis vi f†r en intersection mellom t0 og t1, kan vi gj›re lit av hvert.
  Skal vi beholde:
  ° bare "source"
  ± begge
  ² bare "mod"
  Hvis vi skal ha begge:
  - Vi finner intersection med °
  - Vi finner intersection med ²
  - Hvis t'ene overlapper, har vi en union.
  - Normalen kan vi finne v.hj.a source/mod objekt, og ved † sjekke
  - Vi finner alle 4 t'er (at0,at1,bt0,bt1
    Sorterer dem, og vi skal ta vare p† to av dem...
    union (begge):  at1,bt0

  1. test intersection med source, vi f†r t1,t2
  2. test intersection med dest           t3,t3
  3. union _ t2,t3 normal = normalen p† det objektet t2/t2 "h›rer" til...

 */


 // calculates both t's...t1 = closest, t2 = farthest
 // returns 0 = no intersection
 //         1 = both intersections
int   Sphere_t1t2(tRay *R, int o, float *t1, float *t2)
 {
  tObject *S;
  float A,B,C,D;
  float sq,_t1,_t2;
  float rox_scx,roy_scy,roz_scz;

  numintersectionstried++;
  S = &(OBJECTS[o]);
  rox_scx = R->o.x - S->c.x;
  roy_scy = R->o.y - S->c.y;
  roz_scz = R->o.z - S->c.z;

  A = R->d.x * R->d.x + R->d.y * R->d.y + R->d.z * R->d.z;
  // A = 1;
  //B = 2 * (   R->d.x * ( R->o.x - S->c.x )
  //          + R->d.y * ( R->o.y - S->c.y )
  //          + R->d.z * ( R->o.z - S->c.z ) );
  B = 2 * (   R->d.x * rox_scx
            + R->d.y * roy_scy
            + R->d.z * roz_scz );
  //C = (  (R->o.x - S->c.x) * (R->o.x - S->c.x)
  //     + (R->o.y - S->c.y) * (R->o.y - S->c.y)
  //     + (R->o.z - S->c.z) * (R->o.z - S->c.z) ) - (S->r * S->r);
  C = (  rox_scx * rox_scx
       + roy_scy * roy_scy
       + roz_scz * roz_scz) - S->r2;
  //D = B*B - 4*A*C;
  D = B*B - 4*C;
  if (D<=0) return 0;
  numintersections++;
  sq = sqrt(D);
  _t1 =  (-B + sq ) * (A/2);
  _t2 =  (-B - sq ) * (A/2);
  if ( (_t1 < _t2) && (_t1>0) )
  {
   *t1 = _t1;
   *t2 = _t2;
  }
  else
  {
   *t1 = _t2;
   *t2 = _t1;
  }
  return 1;
 }

 // ---

float Intersect_UnionAnd(tRay *R, int o)
 {
  float ta1,ta2;
  float tb1,tb2;
  int   n,o2;

  o2 = OBJECTS[o].o2;
  n = Sphere_t1t2(R, o, &ta1, &ta2) + Sphere_t1t2(R, o2, &tb1, &tb2);
  if (n != 2) return -1;
  // ray intersects both objects, so we must find if the closest of ta1 & tb1
  if (ta1 < tb1)
  {
   // use the normal from o1
   OBJECTS[o].closest = o;
   return ta1;
  }
  // use the normal from o1
  OBJECTS[o].closest = o2;
  return tb1;
 }

 // ---

float Intersect_UnionSub(tRay *R, int o)
 {
  float ta1,ta2;
  float tb1,tb2;
  int   n,o2;

  o2 = OBJECTS[o].o2;
  if (Sphere_t1t2(R, o,  &ta1, &ta2) == 0) return -1;
  Sphere_t1t2(R, o2, &tb1, &tb2);
  //if (n != 2) return -1;
  // ray intersects both objects
  if ((ta1 > tb1) && (ta1 < tb2))
  {
   // vi er inne i tb1-tb2, s† vi bruker o2
   OBJECTS[o].closest = -o2;
   return tb2;
  }
  // use the normal from o1
  OBJECTS[o].closest = o;
  return ta1;
 }

//
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

 // This is the main function.... Traces a ray, finds intersection,
 // calculates shading, recursively calls Trace for reflected/refracted rays
void  Trace(tColor *C, tRay *R, int sender, int level)
 {
  // ----------
  int       num;      // counter
  float     t;        // temp t  (used when finding closest intersection)
  float     tmin;     // min t   (used when finding closest intersection)
  int       obj;      // obj.num (used when finding closest intersection)
  tVector   ip;       // intersection point
  tVector   in;       // intersection normal
  tVector   lv;       // light vector (diffuse)
  tVector   rv;       // reflection/refraction vector
  tRay      tr;       // temp ray (reflection/refraction)
  int       shadowed; // in shadow flag
  float     maxdist;  // shadow maximum distance
  float     diff;     // diffuse level
  float     spec;     // specular level
  tColor    tc;       // temp color (reflection/refraction)

  //float Wr;
  //int Wgrain;
  // ----------

  // if we have reached max recursion level, return without doing anything
  if (level >= MAXLEVEL)
  {
   C->r = 0;
   C->g = 0;
   C->b = 0;
   return;
  }
  // intersection formulas requires a normalized Ray.Direction vector
  Normalize(&R->d,&R->d);
  // iterate through all objects in "scene"
  obj    = -1;
  tmin   = 999999;
  for (num=0;num<numObj;num++)
  {
   // do not intersect objects with themselves
   if (num != sender)
   {
    // fint t for this object
    switch (OBJECTS[num].type)
    {
     case OBJ_SPHERE   : t = Intersect_Sphere(R,num);    break;
     case OBJ_UNIONAND : t = Intersect_UnionAnd(R,num);  break;
     case OBJ_UNIONSUB : t = Intersect_UnionSub(R,num);  break;
     case OBJ_PLANE    : t = Intersect_Plane(R,num);     break;
    }
    if ((t > 0) && (t < tmin))
    {
     tmin = t;
     obj  = num;
    }
   }
  }
  // have we fund any intersections?
  if (obj == -1)
  {
   C->r = 0;
   C->g = 0;
   C->b = 0;
  }
  else
  {
   // --- we have found an intersection ---
   t = tmin;
   // the intersection point
   ip.x = R->o.x + t * R->d.x;
   ip.y = R->o.y + t * R->d.y;
   ip.z = R->o.z + t * R->d.z;

   // --- shadow ---

   shadowed = 1;
   tr.o.x = ip.x;
   tr.o.y = ip.y;
   tr.o.z = ip.z;
   tr.d.x = LIGHTS[0].pos.x - ip.x;
   tr.d.y = LIGHTS[0].pos.y - ip.y;
   tr.d.z = LIGHTS[0].pos.z - ip.z;
   maxdist = Length(&tr.d);
   if (maxdist > 0)
   {
    Normalize(&tr.d,&tr.d);

    /* Didn't work too well ???

    // try the object that previously shadowed first
    num = OBJECTS[obj].lastshadow;
    if (num != -1)
    {
     switch (OBJECTS[num].type)
     {
      case 0 : t = Intersect_Sphere(&tr,num); break;
     }
     if ((t > 0) && (t < maxdist)) goto inshadow;
    }
    */

    // no, so we try the rest of the objects

    for (num=0;num<numObj;num++)
    {
     if ((num != obj)/* && (num != OBJECTS[obj].lastshadow)*/)
     {
      // find t for this object
      switch (OBJECTS[num].type)
      {
       case OBJ_SPHERE   : t = Intersect_Sphere(&tr,num);    break;
       case OBJ_UNIONAND : t = Intersect_UnionAnd(&tr,num);  break;
       case OBJ_UNIONSUB : t = Intersect_UnionSub(&tr,num);  break;
       case OBJ_PLANE    : t = Intersect_Plane(R,num);       break;

      }
      if ((t > 0) && (t < maxdist))
      {
       OBJECTS[obj].lastshadow = num;
       goto inshadow;    // eller ha en goto nnn?
      }
     }
    }

    shadowed = 0;
   }
   inshadow:

   // the intersection normal
   switch (OBJECTS[obj].type)
   {
    case OBJ_SPHERE   : in.x = (ip.x - OBJECTS[obj].c.x) / OBJECTS[obj].r;
                        in.y = (ip.y - OBJECTS[obj].c.y) / OBJECTS[obj].r;
                        in.z = (ip.z - OBJECTS[obj].c.z) / OBJECTS[obj].r;
                        break;
    case OBJ_UNIONAND : in.x = (ip.x - OBJECTS[(OBJECTS[obj].closest)].c.x) / OBJECTS[(OBJECTS[obj].closest)].r;
                        in.y = (ip.y - OBJECTS[(OBJECTS[obj].closest)].c.y) / OBJECTS[(OBJECTS[obj].closest)].r;
                        in.z = (ip.z - OBJECTS[(OBJECTS[obj].closest)].c.z) / OBJECTS[(OBJECTS[obj].closest)].r;
                        break;
    case OBJ_UNIONSUB : if (OBJECTS[obj].closest < 0)
                        {
                         in.x = -((ip.x - OBJECTS[-(OBJECTS[obj].closest)].c.x) / OBJECTS[-(OBJECTS[obj].closest)].r);
                         in.y = -((ip.y - OBJECTS[-(OBJECTS[obj].closest)].c.y) / OBJECTS[-(OBJECTS[obj].closest)].r);
                         in.z = -((ip.z - OBJECTS[-(OBJECTS[obj].closest)].c.z) / OBJECTS[-(OBJECTS[obj].closest)].r);
                        }
                        else
                        {
                         in.x = (ip.x - OBJECTS[(OBJECTS[obj].closest)].c.x) / OBJECTS[(OBJECTS[obj].closest)].r;
                         in.y = (ip.y - OBJECTS[(OBJECTS[obj].closest)].c.y) / OBJECTS[(OBJECTS[obj].closest)].r;
                         in.z = (ip.z - OBJECTS[(OBJECTS[obj].closest)].c.z) / OBJECTS[(OBJECTS[obj].closest)].r;
                        }
                        break;
    case OBJ_PLANE    : in.x = (OBJECTS[obj].aa);  // was -
                        in.y = (OBJECTS[obj].bb);
                        in.z = (OBJECTS[obj].cc);
                        break;
   }

   // --- diffuse ---

   // the vector from light to intersection
   lv.x = LIGHTS[0].pos.x - ip.x;
   lv.y = LIGHTS[0].pos.y - ip.y;
   lv.z = LIGHTS[0].pos.z - ip.z;
   Normalize(&lv,&lv);
   diff = DotProduct(&in,&lv);

   // --- specular ---

   Reflect(&rv, &in, &lv);             // reflect light by intersection normal
   spec = DotProduct(&rv, &R->d);

   // --- calculate the actual color ---

   // ambient color
   C->r = OBJECTS[obj].ambient.r;
   C->g = OBJECTS[obj].ambient.g;
   C->b = OBJECTS[obj].ambient.b;

   if (diff > 0)
   {
    C->r += diff * OBJECTS[obj].diffuse.r;
    C->g += diff * OBJECTS[obj].diffuse.g;
    C->b += diff * OBJECTS[obj].diffuse.b;
   }
   if (spec > 0)
   {
    spec  = pow(spec,OBJECTS[obj].shinyness);    // pow'en her er 'expensive' !!!!
    C->r += spec * OBJECTS[obj].specular.r;      // fins det andre enn phong ???
    C->g += spec * OBJECTS[obj].specular.g;
    C->b += spec * OBJECTS[obj].specular.b;
   }

   // --- reflection ---

   if (OBJECTS[obj].reflection_level > 0)
   {
    Reflect(&rv, &in, &R->d);
    tr.o.x = ip.x;
    tr.o.y = ip.y;
    tr.o.z = ip.z;
    tr.d.x = rv.x;
    tr.d.y = rv.y;
    tr.d.z = rv.z;
    Trace(&tc, &tr, obj,level+1);
    C->r += tc.r * OBJECTS[obj].reflection_level;
    C->g += tc.g * OBJECTS[obj].reflection_level;
    C->b += tc.b * OBJECTS[obj].reflection_level;
   }

   // --- refraction ---

   if (OBJECTS[obj].refraction_level > 0)
   {
    Refract(&rv,&in,&R->d,OBJECTS[obj].refraction_index,OBJECTS[obj].invindex2);
    if ((rv.x != 0) && (rv.y != 0) && (rv.z != 0))
    {
     tr.o.x = ip.x;
     tr.o.y = ip.y;
     tr.o.z = ip.z;
     tr.d.x = rv.x;
     tr.d.y = rv.y;
     tr.d.z = rv.z;
     Trace(&tc,&tr,obj,level+1);
     C->r += tc.r * OBJECTS[obj].refraction_level;
     C->g += tc.g * OBJECTS[obj].refraction_level;
     C->b += tc.b * OBJECTS[obj].refraction_level;
    }
   }

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

   // --- end of found intersection code ---
  }
 }

//
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

float frand(float min, float max)
 {
  return min + ((max-min) * (float)rand()/32767);
 }

 // ---

void test1(void)
 {
  tRay    R;
  tColor  C;
  int     x,y;
  int     i;

  numintersections = 0;
  numintersectionstried = 0;
  srand(6666);
  numObj = 50;
  OBJECTS = (tObject *)malloc(numObj*sizeof(tObject));
  numLight = 1;
  LIGHTS = (tLight *)malloc(numLight*sizeof(tLight));
  for (i=0;i<numObj;i++)
  {
   OBJECTS[i].type = OBJ_SPHERE;
   OBJECTS[i].c.x              = frand(-400,400);
   OBJECTS[i].c.y              = frand(-300,300);
   OBJECTS[i].c.z              = frand( 800,900);
   OBJECTS[i].r                = frand(25,75);
   OBJECTS[i].r2               = OBJECTS[i].r * OBJECTS[i].r;
   OBJECTS[i].ambient.r        = 0.1;
   OBJECTS[i].ambient.g        = 0.1;
   OBJECTS[i].ambient.b        = 0.1;
   OBJECTS[i].diffuse.r        = frand(0,1);
   OBJECTS[i].diffuse.g        = frand(0,1);
   OBJECTS[i].diffuse.b        = frand(0,1);
   OBJECTS[i].specular.r       = 1.0;
   OBJECTS[i].specular.g       = 1.0;
   OBJECTS[i].specular.b       = 1.0;
   OBJECTS[i].shinyness        = frand(1,64);
   OBJECTS[i].reflection_level = frand(0,0.5);
   OBJECTS[i].refraction_level = frand(0.5,1);
   OBJECTS[i].refraction_index = frand(1,2);
   OBJECTS[i].invindex2        = 1 / (OBJECTS[i].refraction_index * OBJECTS[i].refraction_index);
   OBJECTS[i].lastshadow = -1;

   //OBJECTS[i].diffuse2.r        = frand(0,1);
   //OBJECTS[i].diffuse2.g        = frand(0,1);
   //OBJECTS[i].diffuse2.b        = frand(0,1);
   //OBJECTS[i].Wangle = 1;

  }
  // Light
  LIGHTS[0].pos.x =  1500;
  LIGHTS[0].pos.y =   700;
  LIGHTS[0].pos.z =  0;
  // ---
  // Ray origin
  R.o.x = 0;
  R.o.y = 0;
  R.o.z = 0;

  //while (!kbhit())
  {
   // ---
   for (y=0;y<CALC_Y;y++)
   {
    for (x=0;x<CALC_X;x++)
    {
     // setup ray
     R.d.x = (x - (CALC_X/2)) - R.o.x;
     R.d.y = (y - (CALC_Y/2)) - R.o.y;
     R.d.z = R.o.z + (CALC_X * (4/3)); // 256 : beregn denne v.hj.a X/YSIZE og FOV... ???
     Trace(&C,&R,-1,0);
     BMP_putpixelRGB(Screen,x,y,C.r*255,C.g*255,C.b*255);
    }
   }
   //LIGHTS[0].pos.x -= 50;
  }
  // ---
 }

 // ---

void test2(void)
 {
  tRay    R;
  tColor  C;
  int     x,y;
  int     i;

  numintersections = 0;
  numintersectionstried = 0;
  srand(6666);
  numObj = 4;
  OBJECTS = (tObject *)malloc(numObj*sizeof(tObject));
  numLight = 1;
  LIGHTS = (tLight *)malloc(numLight*sizeof(tLight));

  OBJECTS[0].type             = OBJ_UNIONSUB;
  OBJECTS[0].c.x              =  0;
  OBJECTS[0].c.y              =  0;
  OBJECTS[0].c.z              =  500;
  OBJECTS[0].r                =  100;
  OBJECTS[0].r2               =  OBJECTS[0].r * OBJECTS[0].r;
  OBJECTS[0].ambient.r        =  0.1;
  OBJECTS[0].ambient.g        =  0.1;
  OBJECTS[0].ambient.b        =  0.1;
  OBJECTS[0].diffuse.r        =  0.8;
  OBJECTS[0].diffuse.g        =  0.2;
  OBJECTS[0].diffuse.b        =  0;
  OBJECTS[0].specular.r       =  1.0;
  OBJECTS[0].specular.g       =  1.0;
  OBJECTS[0].specular.b       =  1.0;
  OBJECTS[0].shinyness        =  32;
  OBJECTS[0].reflection_level =  0.9;
  OBJECTS[0].refraction_level =  0;
  OBJECTS[0].refraction_index =  1;
  OBJECTS[0].invindex2        =  1 / (OBJECTS[0].refraction_index * OBJECTS[0].refraction_index);
  OBJECTS[0].lastshadow       = -1;
  OBJECTS[0].o2               =  3;

  OBJECTS[3].type             = OBJ_SPHERE;
  OBJECTS[3].c.x              =  -20;
  OBJECTS[3].c.y              =  -10;
  OBJECTS[3].c.z              =  480;
  OBJECTS[3].r                =  90;
  OBJECTS[3].r2               =  OBJECTS[3].r * OBJECTS[3].r;
  OBJECTS[3].ambient.r        =  0.1;
  OBJECTS[3].ambient.g        =  0.1;
  OBJECTS[3].ambient.b        =  0.1;
  OBJECTS[3].diffuse.r        =  0;
  OBJECTS[3].diffuse.g        =  0;
  OBJECTS[3].diffuse.b        =  1;
  OBJECTS[3].specular.r       =  1.0;
  OBJECTS[3].specular.g       =  1.0;
  OBJECTS[3].specular.b       =  1.0;
  OBJECTS[3].shinyness        =  32;
  OBJECTS[3].reflection_level =  0;
  OBJECTS[3].refraction_level =  0;
  OBJECTS[3].refraction_index =  1;
  OBJECTS[3].invindex2        =  1 / (OBJECTS[3].refraction_index * OBJECTS[3].refraction_index);
  OBJECTS[3].lastshadow       = -1;
  OBJECTS[3].o2               =  0;

  OBJECTS[1].type             = OBJ_SPHERE;
  OBJECTS[1].c.x              =  70;
  OBJECTS[1].c.y              =  -10;
  OBJECTS[1].c.z              =  450;
  OBJECTS[1].r                =  65;
  OBJECTS[1].r2               =  OBJECTS[1].r * OBJECTS[1].r;
  OBJECTS[1].ambient.r        =  0.1;
  OBJECTS[1].ambient.g        =  0.1;
  OBJECTS[1].ambient.b        =  0.1;
  OBJECTS[1].diffuse.r        =  0.5;
  OBJECTS[1].diffuse.g        =  0.4;
  OBJECTS[1].diffuse.b        =  0.7;
  OBJECTS[1].specular.r       =  1.0;
  OBJECTS[1].specular.g       =  1.0;
  OBJECTS[1].specular.b       =  1.0;
  OBJECTS[1].shinyness        =  32;
  OBJECTS[1].reflection_level =  0.5;
  OBJECTS[1].refraction_level =  0.5;
  OBJECTS[1].refraction_index =  1.0;
  OBJECTS[1].invindex2        =  1 / (OBJECTS[1].refraction_index * OBJECTS[1].refraction_index);
  OBJECTS[1].lastshadow       = -1;

  // left-hand system...

  OBJECTS[2].type             = OBJ_PLANE;
  // Plan.. G†r gjennom 0,0,0.. a,b,c = Normal vektor
  // deretter "flyttes" det d fra 0,0,0

  // ser ut som denne skal v‘re "invers normal" ?????
  OBJECTS[2].aa =  0;
  OBJECTS[2].bb =  -1;
  OBJECTS[2].cc =  0;
  // DD = -(aa*x + bb*y + cc*z)
  // hvor langt langs aa,bb,cc vektoren planet skal v‘re. starter ved 0
  OBJECTS[2].dd =  0;
  OBJECTS[2].ambient.r        =  0.1;
  OBJECTS[2].ambient.g        =  0.1;
  OBJECTS[2].ambient.b        =  0.1;
  OBJECTS[2].diffuse.r        =  0.1;
  OBJECTS[2].diffuse.g        =  0.5;
  OBJECTS[2].diffuse.b        =  0.2;
  OBJECTS[2].specular.r       =  1.0;
  OBJECTS[2].specular.g       =  1.0;
  OBJECTS[2].specular.b       =  1.0;
  OBJECTS[2].shinyness        =  32;
  OBJECTS[2].reflection_level =  0.2;
  OBJECTS[2].refraction_level =  0.0;
  OBJECTS[2].refraction_index =  1.1;
  OBJECTS[2].invindex2        =  1 / (OBJECTS[2].refraction_index * OBJECTS[2].refraction_index);
  OBJECTS[2].lastshadow       = -1;

  LIGHTS[0].pos.x =  100;
  LIGHTS[0].pos.y = -100;
  LIGHTS[0].pos.z =  250;

  numObj = 3;

  // Ray origin
  R.o.x =  0;
  R.o.y = -100;
  R.o.z =  0;

  //while (!kbhit())
  {
   // ---
   for (y=0;y<CALC_Y;y++)
   {
    for (x=0;x<CALC_X;x++)
    {
     // setup ray
     R.d.x = (x - (CALC_X/2)) - R.o.x;
     R.d.y = (y - (CALC_Y/2)) - R.o.y;
     R.d.z = R.o.z + (CALC_X * (4/3)); // 256 : beregn denne v.hj.a X/YSIZE og FOV... ???
     Trace(&C,&R,-1,0);
     BMP_putpixelRGB(Screen,x,y,C.r*255,C.g*255,C.b*255);
    }
   }
   //LIGHTS[0].pos.x -= 50;
  }

 }

 // ---

void main(void)
 {
  GFX_init(GFX_X,GFX_Y);
  test2();
  getch();
  GFX_exit();
  printf("numintersectionstried,numintersections: %i,%i\n",numintersectionstried,numintersections);
 }


