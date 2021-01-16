 #ifndef engine_h_included
 #define engine_h_included
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 #include <math.h>

//
// ÄÄÄ DEFINES ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

#define    NULL 0
#define      PI 3.1415926535
#define EPSILON 1.0E-6      /* a tiny number */
#define    TRUE 1
#define     YES 1
#define   FALSE 0
#define      NO 0

// ÄÄÄÄÄ

 // change to #define DEBUG(x) //x to remove all debug stuff
//#define DEBUG(x) x
#define DEBUG(x) //x

#define qX 0
#define qY 1
#define qZ 2
#define qW 3

 // Conversion from degrees to radians
#define DEG2RAD(deg) ((deg)*PI/180.0)
 // Calculate focal distance from field of vision
#define FOV2FOC(width,angle) (((width)*cos((angle)/2.0))/(sin((angle)/2.0)*2))

// ÄÄÄ

 // float -> int

#define SSS 65536.0
#define MAGIC (((SSS*SSS*16)+(SSS*.5))*SSS)
#define MAGICfixed (MAGIC/65536.0)

// ÄÄÄ FLAGS ÄÄÄ

#define TRACKflag_loop     1

#define OBJflag_active     1
#define OBJflag_mapped     2

#define POLYflag_material  1

#define LIGHTflag_active   1

#define CAMERAflag_active  1

#define MATflag_texture    1
#define MATflag_opacity    2
#define MATflag_reflection 4
#define MATflag_bump       8

// ÄÄÄ TYPES ÄÄÄ

#define LIGHTtype_omni     1
#define LIGHTtype_spot     2

#define CAMERAtype_free    1
#define CAMERAtype_uvn     2
#define CAMERAtype_3ds     3

#define OBJECTtype_free    1
#define OBJECTtype_3ds     1

//
// ÄÄÄ TYPES ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

typedef struct sVector
 {
  float x,y,z;
 } sVector;

 // ÄÄÄ

typedef union sKeyData
 {
  int     _int;
  float   _float;
  sVector _vector;
  float   _quat[4];
 } sKeyData;

 // ÄÄÄ

typedef struct sKey
 {
  float        Frame;                  // current frame
  float        Tens, Bias, Cont;       // tension, bias, continuity
  float        EaseTo, EaseFrom;       // ease to, ease from
  sKeyData     Val;                    // the interpolated values
  float        dsa, dsb, dsc, dsd, dda, ddb, ddc, ddd;
  float        ds[4], dd[4];
  float        qa[4];
  struct sKey *Next, *Prev;   // just points to [i-1] and [i+1] because of spline routines :-/ fix???
 } sKey;

 // ÄÄÄ

typedef struct sTrack
 {
  //unsigned int Flags;
  int    Flags;
  int    NumKeys;                       // number of keys
  float  Frames;                        // number of frames
  sKey  *Keys;                          // the track
  sKey  *LastKey;                       // pointer to last used key
 } sTrack;

 // ÄÄÄ

typedef struct sNormal
 {
  sVector Pos;
  sVector PosR;
  float u,v;
 } sNormal;

 // ÄÄÄ

typedef struct sVertex
 {
  unsigned int Flags;
  sVector Pos;
  sVector PosR;           // rotated
  sVector PosP;           // projected
  sNormal *Normal;
  int clip_visible;
  int ceil_y;
  float prestep;          // prestep value for subpixel precision mapping
 } sVertex;

 // ÄÄÄ

typedef struct sMaterial
 {
  unsigned int Flags;
  int Type;
  char Name[20];
  char FileNameTexture[20];     // texture map
  char FileNameOpacity[20];     // alpha map
  char FileNameReflection[20];  // env map
  char FileNameBump[20];        // bump map
  char *Texture;
  char *Opacity;
  char *Reflection;
  char *Bump;
  int Color;
  struct sMaterial *Next;
 } sMaterial;

 // ÄÄÄ

typedef struct sPolygon
 {
  unsigned int Flags;
  int NumVertices;
  int A,B,C;
  sVertex *v[3];        // eller flere for clipping. b†de 2d clip & z-clip
  sNormal *Normal;
  sMaterial *Material;
  int Z;
  struct sObject *Obj;
 } sPolygon;

 // ÄÄÄ

typedef struct sObject
 {
  unsigned int Flags;
  int Type;
  int Id;
  char Name[20];
  sVector Pos;
  sVector Rot;
  sVector Scale;
  float m_o2w[16];
  float m_o2c[16];
  float Matrix3DS[16];
  float q[4];
  float SphereRadius;
  int NumVertices;
  sVertex *Vertices;
  int NumPolygons;
  sPolygon *Polygons;
  int NumNormals;
  sNormal *Normals;
  struct sObject *Next,*Prev;
  int ParentId;
  struct sObject *Parent;
  sVector Pivot;
  // - anim -
  sTrack kfPos;
  sTrack kfRot;
  sTrack kfScale;
 } sObject;

 // ÄÄÄ

typedef struct sCamera
 {
  unsigned int Flags;
  int Type;
  int Id;
  char Name[20];
  sVector Pos;
  sVector Rot;
  float m[16];
  sVector Target;
  sVector UpVector;
  float Roll;
  float Fov;
  float SizeX,SizeY;      // 3d->2d projection
  float AspectRatio;
  float PerspX,PerspY;      // 3d->2d projection
  int ParentId;
  struct sCamera *Parent;
  sVector Pivot;                // Pivot point
  struct sCamera *Next;
  // - anim -
  sTrack kfPos;
  sTrack kfTarget;
  sTrack kfFov;
  sTrack kfRoll;
 } sCamera;

 // ÄÄÄ

typedef struct sLight
 {
  unsigned int Flags;
  int Type;
  int Id;
  char Name[20];
  sVector Pos;
  sVector Rot;
  sVector Target;
  float m[16];
  float Strength;
  float MaxRange;
  float HotSpot;
  float FallOff;
  float SpotRoll;       // if rectangular ?
  // char *SpotMap
  // Char *SpotMapFileName
  int ParentId;
  struct sLight *Parent;
  struct sLight *Next;
  // - anim -
  sTrack kfPos;
  sTrack kfTarget;
 } sLight;

 // ÄÄÄ

typedef struct sScene
 {
  int NumObjects;
  sObject *Objects;
  int NumLights;
  sLight *Lights;
  int NumMaterials;
  sMaterial *Materials;
  int NumCameras;
  sCamera *Cameras;
  sCamera *Camera;
  int VisiblePolys;
  int VisibleObjects;
  sPolygon **RenderList;
  sPolygon **Sorted;
  float AmbientLight;
  float ViewDistanceX;     // 3D->2D projection
  float ViewDistanceY;     // for X & Y : AspectRatio fixing
  float ScreenWidth;       //   width
  float ScreenHeight;      // & height of screen (320x200 etc)
  // - anim -
  int StartFrame;         // Start frame for 3DS animation
  int EndFrame;           // End frame for 3DS animation
 } sScene;
 // ÄÄÄ

//
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

//
// ÄÄÄ math ÄÄÄ
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

 // ÄÄÄÄÄÄÄÄÄÄ

void  Vector_Print(sVector *v);
void  Vector_Create(sVector *v, float x,float y,float z);
void  Vector_Zero(sVector *v);
void  Vector_Copy(sVector *vd, sVector *vs);
void  Vector_Add(sVector *vd, sVector *v1, sVector *v2);
void  Vector_Sub(sVector *vd, sVector *v1, sVector *v2);
void  Vector_Mul(sVector *vd, sVector *v1, sVector *v2);
void  Vector_Div(sVector *vd, sVector *v1, sVector *v2);
void  Vector_Scale(sVector *vd, sVector *v, float scale);
void  Vector_Negate(sVector *vd, sVector *vs);
int   Vector_Equal(sVector *v1, sVector *v2);
float Vector_Length(sVector *v);
float Vector_Distance(sVector *v1, sVector *v2);
float Vector_Dot(sVector *v1, sVector *v2);
void  Vector_Cross(sVector *vd, sVector *v1, sVector *v2);
void  Vector_Normalize(sVector *vd, sVector *vs);
void  Vector_Trans(sVector *dest, sVector v, float *m);
void  Vector_Trans3x3(sVector *dest, sVector v, float *m);
void  Vector_Inv_Trans3x3(sVector *dest, sVector v, float *m);

 // ÄÄÄÄÄÄÄÄÄÄ

void Matrix_Identity(float *m);
void Matrix_Translation(float *m, float tx, float ty, float tz);
void Matrix_RotationXYZ(float *m, float xang, float yang, float zang);
void Matrix_RotationX(float *m, float xang);
void Matrix_RotationY(float *m, float yang);
void Matrix_RotationZ(float *m, float zang);
void Matrix_Mul(float *a, float *b);
void Matrix_Copy(float *m1, float *m2);
void Matrix_Inverse( float *d, float *s );
int  Matrix_Inverse2( float *matrix );
void Matrix_AngleAxis ( float *m, float angle, sVector *v );

 // ÄÄÄÄÄÄÄÄÄÄ

void  Quat_Copy( float *out, float *a );
void  Quat_Normalize( float *qd,float *q );
void  Quat_Mul(float *qd,float *qL, float *qR);
void  Quat_Inverse(float *qd,float *q);
void  Quat_ToMatrix(float *q, float *m);
void  Quat_FromAngAxis(float *q,float ang, sVector *axis);
void  Quat_ToAngAxis(float *q, float *ang, sVector *axis);
float Quat_RelAngAxis(float *p, float *q, sVector *axis);
float Quat_Length(float *a);
float Quat_Dot (float *a, float *b);
float Quat_DotUnit (float *a, float *b);
void  Quat_Slerp(float *out, float *a, float *b, float spin, float alpha);
void  Quat_Slerpl (float *out, float *a, float *b, float spin, float alpha);
void  Quat_Negate (float *out, float *a);
void  Quat_Exp(float *out,float *a);
void  Quat_Log(float *out,float *a);
void  Quat_Lndif (float *out, float *a, float *b);

//
// ÄÄ spline ÄÄÄÄ
//

int Spline_Init ( sTrack *track);
int Spline_InitRot (sTrack *track);
int Spline_GetKey_Float (sTrack *track, float frame, float *out);
int Spline_GetKey_Vect ( sTrack *track, float frame, sVector *out);
int Spline_GetKey_Quat (sTrack *track, float frame, float *out);

//
// ÄÄ load3ds ÄÄÄÄ
//

void Load3DS( char *filename, sScene *Scene );
void Parse3DS( char *buffer, int BufferSize, sScene *Scene );

//
// ÄÄ engine ÄÄÄÄ
//

int       *Int_New( int num );
sKey      *Key_New( int num );
sVertex   *Vertex_New( int num );
sNormal   *Normal_New( int num );
sPolygon  *Polygon_New( int num );
sMaterial *Material_New(void);
sMaterial *Material_FindByName( sScene *Scene, char *name );
sLight    *Light_New(void);
sLight *Light_FindByName( sScene *Scene, char *name );

sObject *Object_New(void);
void     Object_SetPos( sObject *o, float x, float y, float z);
void     Object_AddPos( sObject *o, float x, float y, float z);
void     Object_SetRot( sObject *o, float x, float y, float z);
void     Object_AddRot( sObject *o, float x, float y, float z);
void     Object_CalcFaceNormals( sObject *O );
void     Object_CalcVertexNormals( sObject *O );
void     Object_Center( sObject *O );
void     Object_CalcBoundingSphere( sObject *O );
sObject *Object_FindByName( sScene *Scene, char *name );
void     Object_CreateMatrix_Free( sObject *O, sScene *Scene );
void     Object_Handle( sObject *O, sScene *Scene );
void     Object_CheckBoundingSphere(sObject *O, sCamera *C);

sCamera *Camera_New(void);
sCamera *Camera_FindByName( sScene *Scene, char *name );
void     Camera_SetPos( sCamera *c, float x, float y, float z);
void     Camera_SetRot( sCamera *c, float x, float y, float z);
void     Camera_SetTarget( sCamera *c, float x, float y, float z);
void     Camera_AddTarget( sCamera *c, float x, float y, float z);
void     Camera_AddRot( sCamera *c, float x, float y, float z);
void     Camera_AddPos( sCamera *c, float x, float y, float z);
void     Camera_CreateMatrix_Free(sCamera *c);
void     Camera_CreateMatrix_UVN(sCamera *c);
void     Camera_CreateMatrix_3DS(sCamera *C);
void     Camera_SetupFrustum(sCamera *C);

void Yucku_Line(int x0, int y0, int x1, int y1, char color, char *_B);
void Yucku_PolyWireFrame( sPolygon *p );

sScene *Scene_New( int maxpolys );
void    Scene_SortPolys( sScene *Scene );
void    Scene_SetupForRendering(sScene *S);
void    Scene_DrawAllPolys(sScene *S);

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 #endif

