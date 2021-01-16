 #ifndef engine_h_included
 #define engine_h_included
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

//
// ±±±±± Defines ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

#define    NULL 0
#define      PI 3.1415926535
#define EPSILON 1.0E-6      /* a tiny number */
#define    TRUE 1
#define     YES 1
#define   FALSE 0
#define      NO 0

#define qX 0
#define qY 1
#define qZ 2
#define qW 3

 // float -> int

#define S 65536.0
#define MAGIC (((S*S*16)+(S*.5))*S)
#define MAGICfixed (MAGIC/65536.0)

// ÄÄÄÄÄ

 // change to #define DEBUG(x) //x to remove all debug stuff
#define DEBUG(x) x

//
// ±±±±± Structures ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

typedef unsigned int bool;

typedef struct sVector
 {
  float x,y,z;
 } sVector;

 // ÄÄÄÄÄ

typedef struct sQuaternion
 {
  float x,y,z,w;
 } sQuaternion;

 // ÄÄÄÄÄ

typedef struct sKey
 {
  float   x,  y,  z,  w;
  float ApX,ApY,ApZ,ApW;
  float BpX,BpY,BpZ,BpW;
  float CpX,CpY,CpZ,CpW;
  float DpX,DpY,DpZ,DpW;
 } sKey;

 // ÄÄÄÄÄ

typedef struct sKeyFrame
 {
  // --- flags ---
  unsigned int KF_w : 1;
  unsigned int KF_x : 1;
  unsigned int KF_y : 1;
  unsigned int KF_z : 1;
  // flags from 3DS
  unsigned int KF_Track_Single : 1; // 1  | 0 = single
  unsigned int KF_Track_Loop   : 1; // 2  | 3 = loop
  unsigned int KF_Track_Repeat : 1; // 4  | 2 = repeat
  unsigned int KF_Lock_X       : 1; // 8
  unsigned int KF_Lock_Y       : 1; // 16
  unsigned int KF_Lock_Z       : 1; // 32
  unsigned int KF_No_Link_X    : 1; // 128
  unsigned int KF_No_Link_Y    : 1; // 256
  unsigned int KF_No_Link_Z    : 1; // 512

  float CurrentPos;
  float Speed;
  int CurrentKey;       // Current Segment
  int NumKeys;          // Number of keys in this KeyFrame
  int *FrameNumber;     // malloc'ed array of frame numbers
  sKey *Key;            // The values
 } sKeyFrame;

 // ÄÄÄÄÄ

typedef struct sMaterial
 {
  // --- flags ---
  unsigned int MATERIAL_texture : 1;
  // ---
  char Name[20];          // Material name
  char FileName[20];      // Material file name
  int Color;
  char *Tex1;             // ptr to texture/buffer 1
  char *Tex2;             // Ptr to texture 2 (bump/alpha/whatever)
  struct sMaterial *Next; // Next material in linked list
 } sMaterial;

 // ÄÄÄÄÄ

typedef struct sVertex
 {
  // --- flags ---
  // unsigned int VERTEX_visible : 1;
  // ---
  int VERTEX_visible;
  sVector Pos;            // Vertex coordinates
  sVector PosR;           // Rotated vertex coordinates
  sVector Normal;         // Vertex-normal
  float u,v,c;            // mapping & shading
  int ceil_y;
  float prestep;          // prestep value for subpixel precision mapping
  int clip_visible;
 } sVertex;

 // ÄÄÄÄÄ

typedef struct sPolygon
 {
  // --- flags ---
  unsigned int POLYGON_material : 1;
  // ---
  sVector Normal;         // Polygon normal
  int z;                  // sum of Z for all vertices.. Used for sorting
  sVertex *v[3];          // The three vertices making up the poly
  sMaterial *Material;    // Material used by poly
 } sPolygon;

 // ÄÄÄÄÄ

typedef struct sObject
 {
  // --- flags ---
  unsigned int OBJECT_active   : 1;
  unsigned int OBJECT_dummy    : 1;
  unsigned int OBJECT_keyframe : 1;
  unsigned int OBJECT_mapped   : 1;

  // ---
  char Name[20];          // Object name
  sVector Pos;            // Position relative to parent (world/object)
  sVector Rot;            // Rotation relative to parent
  float o2w[16];          // Object -> World matrix
  float m[16];            // Object -> Camera matrix
  float m_3ds[16];        // the matrix from 3ds... Rename this one!!!
  float q[4];             // quaternion (animation)
  sVector WorldPos;       // Position in world
  float SphereRadius;     // Bounding Sphere radius
  int NumVertices;        // Number of vertices in object
  sVertex *Vertices;      // Ptr to malloc'ed list of sVertex's
  int NumPolygons;        // Number of polygons in object
  sPolygon *Polygons;     // Ptr to malloc'ed list of sPolygon's
  struct sObject *Prev;   // Previous object in double-linked list
  struct sObject *Next;   // Next object in double-linked list
  struct sObject *Parent; // Ptr to parent object (which this is relative to)
  int HierarchyNumber;    // Hierarchy number from 3DS
  int ParentNumber;       // Hierarchy number of parent.. -1 = no parent
  sVector Pivot;          // Pivot point
  sKeyFrame KF_Pos;       // Position keyframe
  sKeyFrame KF_Rot;       // Rotation keyframe
 } sObject;

 // ÄÄÄÄÄ

typedef struct sLight
 {
  // --- flags ---
  unsigned int LIGHT_active   : 1;
  unsigned int LIGHT_keyframe : 1;
  // ---
  char Name[20];          // Light name
  sVector Pos;            // Position, for Omni/point light
  sVector Direction;      // Direction, for directional light
  //float m[16];            // Matrix
  float Strength;         // How bright/strong this light is
  float MaxRange;         // Max illumination range
  struct sLight *Next;    // Next light in linked list
 } sLight;

 // ÄÄÄÄÄ

typedef struct sCamera
 {
  // --- flags ---
  unsigned int CAMERA_active   : 1;
  unsigned int CAMERA_keyframe : 1;
  // ---
  char Name[20];                // Camera name
  int Type;                     // 0 = free, 1 = aim/uvn,2 =
  sVector Pos;                  // Position in World (or relative to parent???)
  sVector Rot;                  // Rotation around its own origo
  sVector Target;               // Camera point-at
  sVector UpVector;             // Up-vector
  float Bank;                   // Roll/bank
  float FOV;                    // Field Of View
  float m[16];                  // Camera matrix
  struct sCamera *Next;         // Not needed? Just for consistence
  struct sCamera *Parent;       //
  int HierarchyNumber;          //
  int ParentNumber;             //
  sVector Pivot;                // Pivot point
  sKeyFrame KF_Pos;             // Position keyframe (vector)
  sKeyFrame KF_Target;          // Target keyframe (quaternion)
  sKeyFrame KF_Fov;             // FOV keyframe (1 float)
  sKeyFrame KF_Bank;            // Bank keyframe (1 float)
 } sCamera;

 // ÄÄÄÄÄ

typedef struct sWorld
 {
  // --- flags ---
  unsigned int WORLD_anim : 1;
  // ---
  int NumCameras;         // Number of cameras in world
  sCamera *Cameras;       // Cameras
  sCamera *Camera;        // _THE_ active camera!
  int NumLights;          // Number of lights in world
  sLight *Lights;         // Lights
  int NumObjects;         // Number of objects in world
  sObject *Objects;       // Objects
  int NumMaterials;       // Number of materials in world
  sMaterial *Materials;   // Materials
  float AmbientLight;     // Ambient light value
 // float viewdistance;     // ViewDistance. Used in 3D->2D project. Calculated from FOV
  float ViewDistanceX;
  float ViewDistanceY;
  int VisibleObjects;     // Number of visible objects on screen. Mainly for debugging
  int VisiblePolys;       // Number of visible polys on screen
  sPolygon **RenderList;  // ptr to list of ptr's to polygons
  sPolygon **Sorted;
  int StartFrame;         // Start frame for 3DS animation
  int EndFrame;           // End frame for 3DS animation
  float CurrentPos;
  float Speed;
 } sWorld;

//
// ±±±±± Global variables :-/ ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

extern char *_Buffer;    // Background Buffer to draw to
extern char *_Screen;    // _the_ screen

extern int MinX,MaxX,MinY,MaxY;      // Clipping rectangle
extern int ScreenWidth,ScreenHeight;  // ViewPort size

extern sWorld *World;

extern int TopClip;
extern int BotClip;
extern int LeftClip;
extern int RightClip;


//
// ±±±±± FILE.H ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void Load3DS( char *filename, sWorld *W );
void Parse3DS( char *buffer, int BufferSize, sWorld *World );

//
// ±±±±± POLY2.H ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void Poly_Tex( sPolygon *P );

//
// ±±±±± Misc / Help / Support ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void *GetMem( unsigned int Size );

 //#define NEW(TYPE,NUM) { (s ## TYPE *)GetMem(NUM*sizeof(s ## TYPE)); }
 // test=NEW(Float,5) = { test = (sFloat *)GetMem(num*sizeof(sFloat)); }

sVector    *NewVector(int num);
sKey       *NewKey(int num);
sMaterial  *NewMaterial(int num);
sVertex    *NewVertex(int num);
sPolygon   *NewPolygon(int num);
sObject    *NewObject(int num);
sLight     *NewLight(int num);
sCamera    *NewCamera(int num);
sWorld     *NewWorld(int num);
int        *NewInt(int num);

//
// ±±±±± Math ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
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

// ----- Matrix -------------------------------------------------------------
void Matrix_Identity(float *m);
void Matrix_Translation(float *m, float tx, float ty, float tz);
void Matrix_Rotation(float *m, float xang, float yang, float zang);
void Matrix_RotationX(float *m, float xang);
void Matrix_RotationY(float *m, float yang);
void Matrix_RotationZ(float *m, float zang);
void Matrix_Mult(float *a, float *b);
void Matrix_Copy(float *m1, float *m2);
void Matrix_Inverse( float *d, float *s );
void Matrix_Inverse2( float *matrix );
void Matrix_AngleAxis ( float *m, float angle, sVector *v );
// ----- Vector -------------------------------------------------------------
void Vector_Normalize(sVector *dest, sVector *v);
float Vector_Dot(sVector v1, sVector v2);
void Vector_Cross(sVector *dest, sVector v1, sVector v2);
void Vector_Add(sVector *dest, sVector v1, sVector v2);
void Vector_Sub(sVector *dest, sVector v1, sVector v2);
void Vector_Trans(sVector *dest, sVector v, float *m);
void Vector_Trans3x3(sVector *dest, sVector v, float *m);
void Vector_inv_Trans3x3(sVector *dest, sVector v, float *m);
float Vector_Distance(sVector *v1, sVector *v2);
float Vector_Length(sVector *v1);
// ----- Quaternion ---------------------------------------------------------
void Quat_Normalize( float *q );
void Quat_Mult(float *qL, float *qR);
void Quat_Inverse(float *q);
void Quat_ToMatrix(float *q, float *m);
void Quat_FromAngAxis(float ang, sVector *axis, float *q);
void Quat_ToAngAxis(float *q, float *ang, sVector *axis);
float Quat_RelAngAxis(float *p, float *q, sVector *axis);

//
// ±±±±± Spline ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void Spline_Init_Value( sKeyFrame *K );
//void Spline_Update( sKeyFrame *K );
//float Spline_GetValue( sKeyFrame *K, sVector *Vector);

//
// ±±±±± Init ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void Engine_Init(void);
void Engine_Exit(void);

//
// ±±±±± Object ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void Object_SetPos( sObject *o, float x, float y, float z);
void Object_AddPos( sObject *o, float x, float y, float z);
void Object_SetRot( sObject *o, float x, float y, float z);
void Object_AddRot( sObject *o, float x, float y, float z);
void Object_CalcFaceNormals( sObject *O );
void Object_CalcVertexNormals( sObject *O );
void Object_Center( sObject *O );
void Object_CalcBoundingSphere( sObject *O );
sObject *Object_FindByName( sWorld *World, char *name );

//
// ±±±±± Camera ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void Camera_SetPos( sCamera *c, float x, float y, float z);
void Camera_SetRot( sCamera *c, float x, float y, float z);
void Camera_SetTarget( sCamera *c, float x, float y, float z);
void Camera_AddTarget( sCamera *c, float x, float y, float z);
void Camera_AddRot( sCamera *c, float x, float y, float z);
void Camera_AddPos( sCamera *c, float x, float y, float z);
sCamera *Camera_FindByName( sWorld *World, char *name );

//
// ±±±±± Material ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

sMaterial *Material_FindByName( sWorld *World, char *name );

//
// ±±±±± Yucku GFX ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void YuckuLine(int x0, int y0, int x1, int y1, char color, char *_B);
void YuckuPolyWireFrame( sPolygon *p );

//
// ±±±±± Render ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void Update_Animation( sWorld *W );
void Camera_CreateMatrix_Free(sCamera *c);
void Camera_CreateMatrix_Aim(sCamera *c);
void Camera_CreateMatrix_Roll( sCamera *C );
void Camera_CreateMatrix_UVN(sCamera *c);
void Object_CreateMatrix_Quat( sObject *O, sWorld *World );
void Object_CreateMatrix_Free( sObject *O, sWorld *World );
void Handle_Object( sObject *O, sWorld *World );
void SortWorldObject( sWorld *World );
void Render_Setup( sWorld *World);
void Render_Draw( sWorld *World);

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 #endif
