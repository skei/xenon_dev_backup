 #ifndef e2_h_included
 #define e2_h_included
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

// ²²²²² Defines ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²

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

// VERTEX.Flags

#define VERTEX_visible 1


// OBJECT.Flags

#define OBJECT_active 1
#define OBJECT_twosided 2
#define OBJECT_inherit 4
#define OBJECT_mapped 32768
#define OBJECT_envmap 65536

// CAMERA.Flags

// LIGHT.Flags

#define LIGHT_directional 1
#define LIGHT_point 2
#define LIGHT_point_dist 4

// WORLD.Flags

#define CAMERA_free 1
#define CAMERA_aim  2

// float -> int

#define S 65536.0
#define MAGIC (((S*S*16)+(S*.5))*S)
#define MAGICfixed (MAGIC/65536.0)
#define MAGICfixed12_4 (MAGIC/16.0)

// ²²²²² Types ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²

typedef struct sVector
 {
  float x,y,z;
 } sVector;

 // ÄÄÄÄÄÄÄÄÄÄ

typedef struct sKeySegment
 {
  int TimeStamp;
  sVector Pos;
  sVector Ap,Bp,Cp,Dp;
 } sKeySegment;

 // ÄÄÄÄÄÄÄÄÄÄ

typedef struct sKeyFrame
 {
  int Flags;            // line/spline
  float CurrentPos;
  float Speed;
  int CurrentSegment;
  int NumSegments;
  float BendSize;
  sKeySegment *SplineSegments;
 } sKeyFrame;


 // ÄÄÄÄÄÄÄÄÄÄ

typedef struct sVertex
 {
  int Flags;
  sVector Pos;               // vertex pos in 3D space
  sVector PosR;
  sVector Normal;            // vertex normal
 // int NumFacesSharingVertex;  // Used in Vertex Normal calculations..
 // sVector NormalR;
  float u,v;                 // mapping coordinates
  float c;                   // vertex light/shading
  int ix,iy,iu,iv,ic;        // 16.16 fixed. ceil'ed iy
  int prestep,iy_notceil;    // both 16.16 fixed
 } sVertex;

 // ÄÄÄÄÄÄÄÄÄÄ

typedef struct sPolygon
 {
  sVertex *v[3];
  sVector Normal;            // face normal
  sVector NormalR;
  int z;                     // Material
  struct sObject *Object;    // Which object this poly belongs to
 } sPolygon;

 // ÄÄÄÄÄÄÄÄÄÄ

typedef struct sObject
 {
  unsigned int Flags;
  char ObjectName[20];
 // sVector Pivot;             // Pivot point.. Or rotation center
  sVector Pos;               // object pos in world space
  sVector PosR;             // object center in camera space
  sVector Rot;               // object rotation around its Pivot point
  float m[16];             // Object to world matrix
  float mw[16];               // Object -> World
  float BSphereRadius;       // bounding sphere radius.
  int NumVertices;           // Number of vertices in object
  sVertex *Vertices;         // list of vertices
  int NumPolys;              // Number of polys in object
  sPolygon *Polygons;        // list of polygons
  char *Texture1;            // ptr to texture 1
  char *Texture2;            // ptr to texture 2
  sKeyFrame *KeyFramerPos;
  sKeyFrame *KeyFramerRot;
 // struct sObject *Branch;
 // struct sObject *Child;
  struct sObject *Child;
  struct sObject *Parent;
 } sObject;

 // ÄÄÄÄÄÄÄÄÄÄ

typedef struct sCamera
 {
  unsigned int Flags;
  sVector Pos;
  sVector Rot;
  sVector Target;
  sVector UpVector;
  float Roll;
  float m[16];
  sKeyFrame *KeyFramerPos;
  sKeyFrame *KeyFramerRot;
 } sCamera;

 // ÄÄÄÄÄÄÄÄÄÄ

typedef struct sLight
 {
  unsigned int Flags;
  sVector Direction;
  sVector DirectionR;
  sVector DirectionRobj;
  sVector Pos;
  sVector PosR;
  sVector PosRobj;      // Pos in object space
  float m[16];
  float Strength;
  float MaxRange;
  sKeyFrame *KeyFramerPos;
  sKeyFrame *KeyFramerRot;
 } sLight;

 // ÄÄÄÄÄÄÄÄÄÄ

typedef struct sWorld
 {
  unsigned int Flags;
  sObject *Objects;
  sCamera *Camera;
  int NumLights;
  sLight *Lights;
  int VisiblePolys;          // Nunber of visible polys in scene
  int VisibleObjects;        // for debugging only!!!!!!!!!!!!!!
  float AmbientLight;
  sPolygon **RenderList;     // ptr to list of ptr's to polygons
  sPolygon **Sorted;         // same, used for sorting
 } sWorld;

// ²²²²² Vars ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²

extern char *_Screen;
extern unsigned short int *_VesaScreen;
extern char *_Buffer;
extern unsigned short int *_VesaBuffer;
extern char *_ShadeTable;
extern unsigned short int *_VesaShadeTable;
extern char *_PhongMap;
extern unsigned short int *_VesaPhongMap;

extern char *_Texture1;
extern char *_Texture2;

extern float *_DivTable;

extern int MinX;
extern int MaxX;
extern int MinY;
extern int MaxY;

extern int ScreenWidth;
extern int ScreenHeight;

extern float viewdistance;

extern float min_clip_z;
extern float max_clip_z;

extern float halfscreenwidth_vd;
extern float halfscreenheight_vd;

extern char PhongIntensity[129];
extern char Intensity[128];

extern sWorld *World;
extern sCamera *Camera;

// ²²²²² Math ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²

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

#pragma inline float2fixed12_4;
 static long float2fixed12_4( float d )
 {
  double dtemp = MAGICfixed12_4+d;
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
 // Copies the contents of m2 into m1
void Matrix_Copy(float *m1, float *m2);
void Matrix_Inverse( float *d, float *s );

// ----- Vector -------------------------------------------------------------

 // Normalize vector
void Vector_Normalize(sVector *dest, sVector *v);
 // dot product of two vectors
float Vector_Dot(sVector v1, sVector v2);
 // cross product of two vectors
void Vector_Cross(sVector *dest, sVector v1, sVector v2);
 // add two vectors together
void Vector_Add(sVector *dest, sVector v1, sVector v2);
 // subtract vector v2 from vector v1
void Vector_Sub(sVector *dest, sVector v1, sVector v2);
 // translate vector by matrix
void Vector_Trans(sVector *dest, sVector v, float *m);
 // translate vector by matrix (rotation only)
void Vector_Trans3x3(sVector *dest, sVector v, float *m);
 // translate vector by inverse matrix (rotation only)
void Vector_inv_Trans3x3(sVector *dest, sVector v, float *m);
 // return distance between two vectors/points
float Vector_Distance(sVector *v1, sVector *v2);
 // return length of vector
float Vector_Length(sVector *v1);

// ²²²²² GFX ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²

 void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

 void setrgb(char color, char r,char g, char b);
#pragma aux setrgb=\
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

 void waitvr (void);
#pragma aux waitvr =\
 " mov dx,3dah",\
 "wait1:",\
 " in al,dx",\
 " test al,8",\
 " jnz wait1",\
 "wait2:",\
 " in al,dx",\
 " test al,8",\
 " jz wait2",\
 modify [al dx];

 // --------------------------------------------------------------------------

void Line(int x0, int y0, int x1, int y1, char color, char *_B);
void DrawChar( int x, int y, unsigned char c, short int color,unsigned short int *_B);
void DrawString(int x1, int y1, char *s, short int color,unsigned short int *_B);
void SetPalette( char *p );

// ²²²²² Poly ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²

void PolyWireFrame( sPolygon *p );
void TexPoly      ( sPolygon *p );
void GTexPoly     ( sPolygon *p );

// ²²²²² Spline ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²

void Spline_Init( sKeyFrame *K );
void Spline_GetValues( sVector *V, sKeyFrame *K);
void Spline_Add( sKeyFrame *K );

// ²²²²² Render ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²

void Light_Calc( sObject *o/*, sWorld *w */);
void Handle_Light(sLight *l, sCamera *c);

void Camera_CreateMatrix_Free(sCamera *c);
void Camera_CreateMatrix_Aim(sCamera *c);

void Object_CreateMatrix(sObject *o, sCamera *c);
int Object_Visible(sObject *o, sCamera *c);
void SortWorldObject( sWorld *World );
void Handle_Object( sObject *O, sCamera *C/*, sWorld *W*/ );

void Render_Setup( sWorld *World);
void Render_Draw( sWorld *World );

// ²²²²² Engine ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²

sObject *Load3DS( char *filename, float VertexMult, float UVmult );

void Object_CalcFaceNormals( sObject *O );
void Object_CalcVertexNormals( sObject *O );
void Object_Center( sObject *O );
void Object_CalcBoundingSphere( sObject *O );

sObject *Object_LoadFile( char *filename );

void Object_Free(sObject *O);
void Object_SetHierarchy( sObject *O, sObject *parentO, sObject *childO, int inherit );

void Object_SetPos( sObject *o, float x, float y, float z);
void Object_AddPos( sObject *o, float x, float y, float z);
void Object_SetRot( sObject *o, float x, float y, float z);
void Object_AddRot( sObject *o, float x, float y, float z);

sObject *Object_FindByName( char *name );

void Camera_SetPos( sCamera *c, float x, float y, float z);
void Camera_SetRot( sCamera *c, float x, float y, float z);
void Camera_SetTarget( sCamera *c, float x, float y, float z);
void Camera_AddTarget( sCamera *c, float x, float y, float z);
void Camera_AddRot( sCamera *c, float x, float y, float z);
void Camera_AddPos( sCamera *c, float x, float y, float z);

char ClosestColor(char *palette,char r, char g, char b);
void Calc_ShadeTable(char *palette, char *ShadeTable);
void Calc_ShadeHighLight(char *palette, char *ShadeTable,int diffuse,
                         float rr, float gg, float bb);
void Setup_Intensity(int ambient);
void Calc_PhongIntensity(float _L, float _F, float _D, float _S, float _G);
void Calc_PhongMap(void);
void Calc_GouraudMap(void);

void VesaCalc_ShadeTable(char *palette, unsigned short int *VesaShadeTable);
void VesaCalc_ShadeHighLight(char *palette, unsigned short int *VesaShadeTable,
                             int diffuse, float rr, float gg, float bb);

int Engine_Init(int w, int h, int c, int vesa, int maxpolys);
void Engine_UnInit(int vesa);

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#endif
