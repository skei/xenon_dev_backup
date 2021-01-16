 #ifndef engine_h_included
 #define engine_h_included
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 #include <stdio.h>       // for the FILE *fp stuff in 3DS loaders

//
// ±±±±± defines ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

#define UVmultiplier 255
#define EnvMapMultiplier 127

#define ShadowSize 256

 // float -> int

#define S 65536.0
#define MAGIC (((S*S*16)+(S*.5))*S)
#define MAGICfixed (MAGIC/65536.0)

 //#define ENGINE_QUAT_SLERP
#define ENGINE_SWAP_YZ
#define ENGINE_DEBUG              // debug mode
 // change to #define DEBUG(x) //x to remove all debug stuff
#define DEBUG(x) x

#define NULL 0
#define PI 3.1415926535
#define EPSILON 1.0e-6
#define qX 0
#define qY 1
#define qZ 2
#define qW 3
#define OK 0
#define ERROR 1
#define    TRUE 1
#define     YES 1
#define   FALSE 0
#define      NO 0


// --- track types ---
#define TrackTypeCamera     1     // camera track
#define TrackTypeCameratgt  2     // camera target track
#define TrackTypeLight      4     // light track
#define TrackTypeSpotlight  8     // spotlight track
#define TrackTypeLighttgt   16    // light target track
#define TrackTypeObject     32    // object track
#define TrackTypeAmbient    64    // ambient track

// --- key types ---
#define KeyTypePos          1     // position track
#define KeyTypeRotate       2     // rotation track
#define KeyTypeScale        3     // scale track
#define KeyTypeFov          4     // fov track
#define KeyTypeRoll         5     // roll track
#define KeyTypeColor        6     // color track
#define KeyTypeMorph        7     // morph track
#define KeyTypeHide         8     // hide track

// --- Node types ---
#define NodeTypeCamera      1     // camera     sCAMERA
#define NodeTypeLight       2     // light      sLIGHT
#define NodeTypeObject      4     // object     sOBJECT
#define NodeTypeMaterial    8     // material   sMATERIAL
#define NodeTypeAmbient     16    // ambient    sAMBIENT

// --- Flags ---
#define FlagHierarchy       1     // hierarchical transformations
#define FlagCalcnorm        2     // calculate normals
#define FlagDomorph         4     // internal object morph
#define FlagTransform       8     // internal transformations
#define FlagNormLerp        16    // morph: lerp normals
#define FlagNormCalc        32    // morph: recalculate normals

// --- Material Flags ---
#define MatFlagTwoSided     1     // two sided
#define MatFlagSoften       2     // soften
#define MatFlagWire         4     // wireframe rendering
#define MatFlagTransAdd     8     // transparency add

// --- Material shading ---
#define MatShadeFlagFlat    1     // flat shading
#define MatShadeFlagGouraud 2     // gouraud shading
#define MatShadeFlagPhong   3     // phong shading
#define MatShadeFlagMetal   4     // metal shading

// --- Face/Polygon flags ---
#define FaceFlagWrapU       1     // face has texture wrapping (u)
#define FaceFlagWrapV       2     // face has texture wrapping (v)

// --- Object flags ---
#define ObjFlagHidden       1     // object is hidden
#define ObjFlagChidden      2     // object is always hidden
#define ObjFlagDummy        4     // object is dummy
#define ObjFlagMorph        8     // object is morphing
#define ObjFlagMapped      16     // THS: Object is mapped

// --- Light flags ---
#define LightFlagOmni       1     // light is omni
#define LightFlagSpot       2     // light is spotlight

// --- Vertex flags ---
#define VertexFlagVisible    1     // vertex is visible
#define VertexFlagEnvMapDone 2      // ENVMAP U/V already calculated

//
// ±±±±± types ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;
typedef signed char    sbyte;
typedef signed short   sword;
typedef signed long    sdword;

typedef int bool;

 // ---

typedef struct sRGB
 {
  float r,g,b;            // 0..1;
 } sRGB;

 // ---

typedef struct sMorph
 {
  int   from, to;         // morph: from/to object
  float stage;            // morph stage (0 -> 1.0)
 } sMorph;

 // ---

typedef struct sVector
 {
  float x,y,z;
 } sVector;

 // ---

typedef float sMatrix[3][4];

 // ---

typedef struct sQuat
 {
  float w,x,y,z;
 } sQuat;

// --- objects --------------------------------------------------------------

typedef struct sVertex
 {
  // --- Vertex flags ---
  unsigned int Visible    : 1;
  unsigned int EnvMapDone : 1;
  // int Flags;
  sVector Pos;            // Position
  sVector rPos;           // rotated Position
  sVector pPos;           // projected position
  sVector Normal;         // Vertex normal
  sVector rNormal;        // rotated Vertex normal
  float u,v,c;
  float inv_z,inv_u,inv_v;
  int ceil_y;
  float prestep;          // prestep value for subpixel precision mapping
  int clip_visible;
 } sVertex;

 // ---

typedef struct sMap
 {
  int Flags;
  char *File;
  float Uscale,Vscale;
  float Uoffset,Voffset;
  float RotAngle;
  char *BitMap;
 } sMap;

 // ---

typedef struct sMaterial
 {
  unsigned int TEXTURE : 1;
  unsigned int BUMP    : 1;
  unsigned int ENVMAP  : 1;
  int Flags;
  int Id;
  char *Name;
  int Shading;            // shading, flags (flat,gouraud,phong,metal)
  int MappingType;        // checked in Engine_Draw to call correct mapper... ??
  sRGB Ambient;          // ambient color
  sRGB Diffuse;          // diffuse color
  sRGB Specular;         // specular color
  sRGB Shininess;        // shininess
  sRGB ShinStrength;    // snininess strength
  sRGB Transparency;     // transparency
  sRGB TransFalloff;    // transparency falloff
  sRGB RefBlur;          // reflection blur
  sRGB SelfIllum;       // self illuminance
  sMap Texture;          // texture map
  sMap Bump;             // bump map
  sMap Reflection;       // reflection map
 } sMaterial;

 // ---

typedef struct sAmbient
 {
  int Id;
  char *Name;
  sRGB Color;
 } sAmbient;

 // ---

typedef struct sPolygon
 {
  int Flags;
  sVertex *v[3];                // v[6] for clipping?
  sVector Normal;
  int MaterialId;
  sMaterial *Material;
  int SortZ;                    // check this!!!! floats checked like int's if all same sign!
 } sPolygon;

 // ---

typedef struct sLight
 {
  int Flags;
  int Id;
  char *Name;
  sVector Pos;
  sRGB Color;
  float Roll;           // for spotlight
  sVector Target;       // for spotlight
  float Hotspot;
  float Falloff;
  int Parent1,Parent2;
 } sLight;

 // ---

typedef struct sCamera
 {
  int Flags;
  int Id;
  char *Name;
  sVector Pos;
  sVector Target;
  float Fov;
  float Roll;
  float SizeX,SizeY;
  float PerspX,PerspY;
  float Aspectratio;
  sMatrix CameraMat;
  sMatrix M;
  int Parent1,Parent2;
 } sCamera;

 // ---

typedef struct sObject
 {
  int Flags;
  int Id;
  char *Name;
  int NumVertices;
  sVertex *Vertices;
  int NumPolygons;
  sPolygon *Polygons;
  sVector Pivot;
  sMatrix Matrix;
  sMatrix RotMatrix;
  sVector Translate;
  sVector Scale;
  sQuat Rotate;
  int Parent;
 } sObject;

 // ---

typedef struct sNode
 {
  int Type;
  void *Object;
  struct sNode *Next, *Prev;
 } sNode;

// --- Keyframer ------------------------------------------------------------

typedef union kfData
 {
  int     _int;
  float   _float;
  sVector _vector;
  sQuat   _quat;
 } kfData;

 // ---

typedef struct kfKey
 {
  float Frame;
  float Tens,Bias,Cont;
  float EaseTo,EaseFrom;
  kfData Val;
  float dsa, dsb, dsc, dsd;
  float dda, ddb, ddc, ddd;
  struct kfKey *Next, *Prev;

 } kfKey;

 // ---

typedef struct kfNode
 {
  int Type, Id;                         // track type, object id
  void *Track, *Object;                 // track / object
  struct kfNode *Child, *Brother;       // hierarchy tree
  struct kfNode *Next, *Prev;           // next/previous node
 } kfNode;

 // ---

typedef struct kfScene
 {
  float Start, End, Current;            // start/end/current frame
  sNode *World, *Wtail;                 // world
  kfNode *Keyframer, *Ktail;            // keyframer
  int VisibleObjects;         // Number of visible objects on screen. Mainly for debugging
  int VisiblePolys;           // Number of visible polys on screen
  sPolygon **RenderList;      // ptr to list of ptr's to polygons
  sPolygon **Sorted;          //
  char *Buffer;
  char *Screen;
  unsigned short int *ShadowBuffer;
  int ScreenWidth;
  int ScreenHeight;
  int TopClip;
  int BotClip;
  int LeftClip;
  int RightClip;
  char *DefaultTexture;
 } kfScene;

// --- keyframe types (for typecasting the Track/Object in kfNode) ----------

typedef struct sCAMERA
 {
  kfKey *pos;                            /* position            */
  kfKey *fov, *roll;                     /* field of view, roll */
 } sCAMERA;

typedef struct sCAMERATGT
 { /* camera target track */
  kfKey *pos;                            /* position */
 } sCAMERATGT;

typedef struct sLIGHT
 { /* light track */
  kfKey *pos;                            /* position */
  kfKey *color;                          /* color    */
 } sLIGHT;

typedef struct sLIGHTTGT
 { /* light target track */
  kfKey *pos;                            /* position */
 } sLIGHTTGT;

typedef struct sSPOTLIGHT
 { /* spotlight track */
  kfKey *pos;                            /* position */
  kfKey *color;                          /* color */
  kfKey *roll;                           /* roll */
 } sSPOTLIGHT;

typedef struct sOBJECT
 { /* object track */
  kfKey *translate;                      /* position */
  kfKey *scale;                          /* scale    */
  kfKey *rotate;                         /* rotation */
  kfKey *morph;                          /* morph    */
  kfKey *hide;                           /* hide     */
 } sOBJECT;

typedef struct sAMBIENT
 { /* ambient track */
  kfKey *color;                          /* color */
 } sAMBIENT;

//
// ±±±±± Global variables ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

extern kfScene *Engine_Scene;
extern int Engine_Flags;

//
// ±±±±± MATH ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

#pragma inline float2int;
 static long float2int( float d )
 {
  double dtemp = MAGIC+d;
  return (*(long *)&dtemp)-0x80000000;
 }

 // ÄÄÄÄÄÄÄÄÄÄ

#pragma inline float2fixed;
 static long float2fixed( float d )
 {
  double dtemp = MAGICfixed+d;
  return (*(long *)&dtemp)-0x80000000;
 }

 // ÄÄÄÄÄÄÄÄÄÄ

#pragma inline CEIL;
 static int CEIL(float f)
 {
  int i,j;

  i = float2fixed(f);
  j = i >> 16;
  if ((i&0xffff) != 0) j+=1;
  return j;
 }


// --- Vector ---

 // create vector (out = [x,y,z]).
void vec_make (float x, float y, float z, sVector *out);
 // zero vector (out = [0,0,0]).
void vec_zero (sVector *out);
 // vector copy (out = a).
void vec_copy (sVector *a, sVector *out);
 // print vector on stdout.
void vec_print (sVector *a);
 // vector addition (out = a+b).
void vec_add (sVector *a, sVector *b, sVector *out);
 // vector substraction (out = a-b).
void vec_sub (sVector *a, sVector *b, sVector *out);
 // vector multiplication (out = a*b).
void vec_mul (sVector *a, sVector *b, sVector *out);
 // vector division (out = a / b).
void vec_div (sVector *a, sVector *b, sVector *out);
 // vector multiplication by a scalar.
void vec_scale (sVector *a, float s, sVector *out);
 // vector negation (out = -a).
void vec_negate (sVector *a, sVector *out);
 // vector compare.
int vec_equal (sVector *a, sVector *b);
 // computes vector magnitude.
float vec_length (sVector *a);
 // computes distance between two vectors.
float vec_distance (sVector *a, sVector *b);
 // computes dot product of two vectors.
float vec_dot (sVector *a, sVector *b);
 // computes cross product of two vectors.
void vec_cross (sVector *a, sVector *b, sVector *out);
 // linear interpolation of two vectors.
void vec_lerp (sVector *a, sVector *b, float alpha, sVector *out);
 // linear combination of two vectors.
void vec_combine (sVector *a, sVector *b, float as, float bs, sVector *out);
 // vector normalization.
void vec_normalize (sVector *a, sVector *out);

// --- Matrix ---

 // make identity matrix.
void mat_identity (sMatrix out);
 // clear matrix.
void mat_zero (sMatrix out);
 // matrix copy.
void mat_copy (sMatrix a, sMatrix out);
 // print matrix on stdout.
void mat_print (sMatrix a);
 // matrix addition.
void mat_add (sMatrix a, sMatrix b, sMatrix out);
 // matrix substraction.
void mat_sub (sMatrix a, sMatrix b, sMatrix out);
 // matrix multiplication.
void mat_mul (sMatrix a, sMatrix b, sMatrix out);
 // inverse matrix calculation (non-singular).
int mat_inverse (sMatrix a, sMatrix out);
 // create a translation matrix.
void mat_settrans (sVector *v, sMatrix out);
 // create a scale matrix.
void mat_setscale (sVector *v, sMatrix out);
 // create rotation matrix around X axis.
void mat_rotateX (float ang, sMatrix out);
 // create rotation matrix around Y axis.
void mat_rotateY (float ang, sMatrix out);
 // // create rotation matrix around Z axis.
void mat_rotateZ (float ang, sMatrix out);
 // multiply a vector by matrix (out = [a]*b)
void mat_mulvec (sMatrix a, sVector *b, sVector *out);
 // multiply a vector by upper 3x3 matrix (out = [a]*b)
void mat_mulvec3 (sMatrix a, sVector *b, sVector *out);

// --- Quaternion ---

 // compute quaterion from [angle,axis] representation.
void qt_fromang (float ang, float x, float y, float z, sQuat *out);
 // convert quaterion to [angle,axis] representation.
void qt_toang (sQuat *a, float *ang, float *x, float *y, float *z);
 // create quaternion (out = [w,[x,y,z]])
void qt_make (float w, float x, float y, float z, sQuat *out);
 // make identity quaternion (out = [1,0,0,0]).
void qt_identity (sQuat *out);
 // zero quaternion (out = [0,0,0,0]).
void qt_zero (sQuat *out);
 // quaternion copy (out = a).
void qt_copy (sQuat *a, sQuat *out);
 // print quaternion on stdout.
void qt_print (sQuat *a);
 // quaternion addition (out = a+b).
void qt_add (sQuat *a, sQuat *b, sQuat *out);
 // quaterion substraction (out = a-b).
void qt_sub (sQuat *a, sQuat *b, sQuat *out);
 // quaternion multiplication (out = a*b).
void qt_mul (sQuat *a, sQuat *b, sQuat *out);
 // scale quaternion to a new length.
void qt_scale (sQuat *a, float s, sQuat *out);
 // quaterion division (out = a/b).
void qt_div (sQuat *a, sQuat *b, sQuat *out);
 // quaternion square (out = a^2).
void qt_square (sQuat *a, sQuat *out);
 // quaternion square root (out = sqrt(a)).
void qt_sqrt (sQuat *a, sQuat *out);
 // quaternion normalization.
void qt_normalize (sQuat *a, sQuat *out);
 // form multiplicative inverse of quaternion.
void qt_inverse (sQuat *a, sQuat *out);
 // spherical linear interpolation of quaternions.
void qt_slerp (sQuat *a, sQuat *b, float alpha, sQuat *out);
 // spherical linear interpolation of quaternions with extra spins.
void qt_slerpspin (sQuat *a, sQuat *b, float alpha, int spin, sQuat *out);
 // convert quaternion to rotation matrix.
 //      ( 1-yy-zz , xy+wz   , xz-wy   )
 //  T = ( xy-wz   , 1-xx-zz , yz+wx   )
 //      ( xz+wy   , yz-wx   , 1-xx-yy )
void qt_matrix (sQuat *a, sMatrix mat);
 // convert quaternion to inversed rotation matrix.
 //      ( 1-yy-zz , xy-wz   , xz+wy   )
 //  T = ( xy+wz   , 1-xx-zz , yz-wx   )
 //      ( xz-wy   , yz+wx   , 1-xx-yy )
void qt_invmatrix (sQuat *a, sMatrix mat);
 // convert rotation matrix to quaternion.
void qt_frommat (sMatrix mat, sQuat *out);

// --- Spline ---

 // initialize spline key list.
int Spline_Init (kfKey *keys);
 // get spline interpolated float for frame "frame".
int Spline_GetKey_Float (kfKey *keys, float frame, float *out);
 // get spline interpolated vector for frame "frame".
int Spline_GetKey_Vect (kfKey *keys, float frame, sVector *out);
 // get spline interpolated quaternion for frame "frame".
int Spline_GetKey_Quat (kfKey *keys, float frame, sQuat *out);

// --- Camera ---

 // converts 3dstudio lens to fov.
void cam_lens_fov (float lens, float *fov);
 // creates a camera matrix.
void Camera_Update (sCamera *cam);


//
// ±±±±± functions ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

 // frees keys list.
void Engine_FreeKey(kfKey *key);
 // calculates face/vertex normals.
void Engine_CalcNormals (void);
 // transform all objects.
void Engine_DoTransform (void);
 // non-case sensitive string compare.
int strucmp (char *s1, char *s2);

void Engine_Init(int flags);
void Engine_Done(void);
 // return number of frames.
void Engine_GetFrames (float *start, float *end);
 // set current frame number.
void Engine_SetFrame (float frame);
 // return current frame number.
float Engine_GetFrame (void);
 // sets active scene.
void Engine_SetActive (kfScene *scene);
 // get active scene.
kfScene *Engine_GetActiv(void);
 // find object by name.
sNode *Engine_FindByName(char *name);
 // find object by id.
sNode *Engine_FindById(int id);
 // finds first node with attribute "attr".
sNode *Engine_FindFirst (int attr);
 // finds next node with attribute "attr".
sNode *Engine_FindNext (int attr, sNode *node);
 // add object to world list.
int Engine_AddWorld(int type, void *obj);
 // add track to keyframer list.
int Engine_AddTrack (int type, int id, int parent, void *track, void *obj);
 // assigns a key list to track.
int Engine_SetTrack (int type, int id, kfKey *track);
 // allocates a new scene.
kfScene *Engine_AllocScene(int width, int height, int bpp, int maxpolys);
 // loads mesh data from 3ds file "filename" into scene "scene".
int Engine_LoadWorld(char *filename, kfScene *scene);
 // loads motion data from 3ds file "filename" into scene "scene".
int Engine_LoadMotion(char *filename, kfScene *scene);
 // release all memory used by world.
int Engine_FreeWorld(kfScene *scene);
 // release all memory used by keyframer.
int Engine_FreeMotion(kfScene *scene);
 // loads mesh and keyframer data from 3ds file "filename" into scene "scene".
int Engine_LoadScene(char *filename, kfScene *scene);
int Engine_FreeScene(kfScene *scene);
 // return float key at frame "frame".
int Engine_GetKey_Float(kfKey *keys, float frame, float *out);
 // return vector key at frame "frame".
int Engine_GetKey_Vect(kfKey *keys, float frame, sVector *out);
 // return rgb key at frame "frame".
int Engine_GetKey_RGB(kfKey *keys, float frame, sRGB *out);
 // return quaternion key at frame "frame".
int Engine_GetKey_Quat (kfKey *keys, float frame, sQuat *out);
 // update all keyframer data.
int Engine_Update(void);

//
// ±±±±± 3DS loaders ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

 // loads mesh data from 3ds file "filename" into scene "scene".
int Engine_LoadMesh_3DS (FILE *f);

 // loads motion data from 3ds file "filename" into scene "scene".
int Engine_LoadMotion_3DS(FILE *f);

//
// ±±±±± PolyFiller ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void Poly_Tex( sPolygon *P );
void Poly_GourTex( sPolygon *P );

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 #endif
