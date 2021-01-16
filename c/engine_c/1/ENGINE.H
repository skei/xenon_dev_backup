 #ifndef engine_h_included
 #define engine_h_included
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

//
// ÄÄÄ Defines ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//


#define    NULL 0
#define      PI 3.1415926535
#define EPSILON 1.0E-6
#define    TRUE 1
#define     YES 1
#define      ON 1
#define   FALSE 0
#define      NO 0

#define qX 0
#define qY 1
#define qZ 2
#define qW 3

// ÄÄÄÄÄ

 // change to #define DEBUG(x) //x to remove all debug stuff

#define DEBUG(x) x
// #define DEBUG(x) //x

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

 // #define OBJflag_active     1
#define OBJflag_mapped     2
#define OBJflag_hidden     4
#define OBJflag_dummy      8
#define OBJflag_ambient    16

 // #define LIGHTflag_active   1

 // #define CAMERAflag_active  1

#define MATflag_texture1   1
#define MATflag_texture2   2
#define MATflag_opacity    4
#define MATflag_reflection 8
#define MATflag_bump       16
#define MATflag_twosided   32
#define MATflag_addtrans   64
#define MATflag_wireframe  128

// ÄÄÄ TYPES ÄÄÄ

#define MATtype_wire       1
#define MATtype_flat       2
#define MATtype_gouraud    3
#define MATtype_phong      4
#define MATtype_metal      5

#define NODEtype_object    1
#define NODEtype_light     2
#define NODEtype_camera    3

#define LIGHTtype_omni     1
#define LIGHTtype_spot     2

#define CAMERAtype_free    1
#define CAMERAtype_uvn     2
#define CAMERAtype_3ds     3

#define OBJECTtype_free    1
#define OBJECTtype_3ds     1

// ÄÄÄ 3DS loader errors ÄÄÄ

#define Error3DS_NoError     0
#define Error3DS_InvalidFile 1

//
// ÄÄÄ Types ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

typedef struct sRGB
 {
  float r,g,b;
 } sRGB;

 // ---

typedef struct sVector
 {
  float x,y,z;
 } sVector;

 // ---

typedef struct sVertex
 {
  sVector   Pos;
  sVector   PosR;   // rotated
  sVector   PosRP;  // rotated & projected
 } sVertex;

 // ---

typedef struct sNormal
 {
  sVector   Pos;
  sVector   PosR;
  float u,v;
 } sNormal;

 // ---

typedef struct sBitmap
 {
  int   Width;
  int   Height;
  int   Bpp;
  int   Size;
  void *Bitmap;
 } sBitmap;

 // ---

typedef struct sMaterial
 {
   unsigned int       Flags;
   int                Type;
   char               Name[20];
   char               FileNameTexture1[20];     // texture map
   char               FileNameTexture2[20];    // texture map
   char               FileNameOpacity[20];     // alpha map
   char               FileNameReflection[20];  // env map
   char               FileNameBump[20];        // bump map
   /*sBitmap*/ char  *Texture1;
   /*sBitmap*/ char  *Texture2;
   /*sBitmap*/ char  *Opacity;
   /*sBitmap*/ char  *Reflection;
   /*sBitmap*/ char  *Bump;
   sRGB               AmbientColor;
   sRGB               DiffuseColor;
   sRGB               SpecularColor;
   int                Color;
   struct sMaterial  *Prev;
   struct sMaterial  *Next;
 } sMaterial;

 // ---

typedef struct sPolygon
 {
  int           A,B,C,Flags;
  unsigned int  SmoothingGroup;
  sVertex      *Vertices[3];
  sNormal      *Normals[3];
  sMaterial    *Material;
  int Z;                              // for sorting
 } sPolygon;

 // ---

typedef union  sKeyData
 {
  int       _int;
  float     _float;
  sVector   _vector;
  float     _quat[4];
  char      _string[11];        // 10 + \0
 } sKeyData;

 // ---

typedef struct sKeyframe
 {
  float             Frame;                  // current frame
  float             Tens;
  float             Bias;
  float             Cont;
  float             EaseTo;
  float             EaseFrom;
  sKeyData          Val;                    // the interpolated values
  float             dsa, dsb, dsc, dsd;
  float             dda, ddb, ddc, ddd;
  //float             ds[4], dd[4];
  //float             qa[4];
  struct sKeyframe *Next;
  struct sKeyframe *Prev;   // just points to [i-1] and [i+1] because of spline routines :-/ fix???
 } sKeyframe;

 // ---

typedef struct sTrack
 {
  int               Flags;
  int               NumKeys;                       // number of keys
  float             Frames;                        // number of frames
  sKeyframe        *Keys;                          // the track
  sKeyframe        *LastKey;                       // pointer to last used key
 } sTrack;

 // ---

typedef struct sNode
 {
  int           Type;
  int           Id;
  int           ParentId;
  char          Name[20];
  struct sNode *Next;         // Next in double linked lsit
  struct sNode *Prev;         // prev --"--

  void         *Data;         // The node object (sObject, sLight or sCamera)

  struct sNode *Parent;       // This node's parent
  struct sNode *Child;        //             child
  struct sNode *Brother;      //             brother
 } sNode;

 // ---

typedef struct sObject
 {
  unsigned int    Flags;
  int             Type;

  sVector         Pos;
  sVector         Rot;
  sVector         Scale;
  sVector         Pivot;
  float           SphereRadius;

  float           kf_Rotate[4];
  sVector         kf_Scale;
  sVector         kf_Translate;
  float           kf_Matrix[16];

  float           Matrix3DS[9];  // the matrix in the 3DS file      -- debug --
  sVector         Pivot3DS;      // the translation vector in 3DS

  //float           M2w[16];     // object to world
  //float           M2c[16];     // object to camera
  //float           Q[4];

  int             NumVertices;
  sVertex        *Vertices;
  int             NumPolygons;
  sPolygon       *Polygons;
  int             NumNormals;
  sNormal        *Normals;
  // ---

  // - anim -
  sTrack       kfPos;
  sTrack       kfRot;
  sTrack       kfScale;
  sTrack       kfHide;
  sTrack       kfMorph;
 } sObject;

 // ---

typedef struct sLight
 {
  unsigned int    Flags;
  int             Type;

  sVector      Pos;
  sVector      Rot;
  sVector      Target;
  sVector      Pivot;

  float        Matrix[16];

  // char        *SpotMap
  // Char        *SpotMapFileName

  float        Strength;
  float        MaxRange;
  float        HotSpot;
  float        FallOff;
  float        SpotRoll;    // if rectangular ?
  // - anim -
  sTrack       kfPos;
  sTrack       kfTarget;
 } sLight;

 // ---

typedef struct sCamera
 {
  unsigned int Flags;
  int          Type;

  sVector      Pos;
  sVector      Rot;
  sVector      Target;
  sVector      Pivot;
  sVector      UpVector;

  float        Matrix[16];

  float        Roll;
  float        Fov;
  float        SizeX,SizeY;      // 3d->2d projection
  float        AspectRatio;
  float        PerspX,PerspY;    // 3d->2d projection
  float        ViewDistanceX,ViewDistanceY;
  // - anim -
  sTrack       kfPos;
  sTrack       kfTarget;
  sTrack       kfFov;
  sTrack       kfRoll;
 } sCamera;

 // ---

typedef struct sScene
 {
  //int           NumObjects;
  //sObject          *Objects;
  //sObject          *LastObject;
  //int           NumLights;
  //sLight           *Lights;
  //sLight           *LastLight;
  int           NumMaterials;
  sMaterial        *Materials;
  sMaterial        *LastMaterial;
  //int           NumCameras;
  //sCamera          *Cameras;
  //sCamera          *LastCamera;

  int           NumNodes;
  sNode            *Nodes;
  sNode            *LastNode;

  sCamera          *ActiveCamera;

  int               VisiblePolys;
  int               VisibleObjects;
  sPolygon        **RenderList;
  sPolygon        **Sorted;
  float             AmbientLightColor;
  // - 3ds -
  float             MasterScale;
  sRGB              AmbientLightRGB;
  int               UseFog;
  float             NearFogPlaneDistance;
  float             NearFogPlaneDensity;
  float             FarFogPlaneDistance;
  float             FarFogPlaneDensity;
  sRGB              FogColor;
  // - anim -
  float             Frame;
  int               StartFrame;        // Start frame for 3DS animation
  int               EndFrame;          // End frame for 3DS animation
  // ---
  char             *Buffer;

 } sScene;

//
// ÄÄÄ Functions ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

void        Debug            (char *s);
sVertex    *Vertex_New       (int num);
sNormal    *Normal_New       (int num);
sPolygon   *Polygon_New      (int num);
sMaterial  *Material_New     (void);
sKeyframe  *Keyframe_New     (void);
void        Track_AddKeyframe(sTrack *T, sKeyframe *K);
sNode      *Node_New         (void);

//
// --- Object ---
//

sObject *Object_New               (void);
void     Object_CalcFaceNormals   (sObject *O);
void     Object_CalcVertexNormals (sObject *O);
void     Object_CalcBoundingSphere(sObject *O);
void     Object_Center            (sObject *O);
void     Object_Update            (sObject *O);
void     Object_Handle            (sObject *O,sScene *Scene);

//
// --- Light ---
//

sLight *Light_New(void);

//
// --- Camera ---
//

sCamera *Camera_New      (void);
void     Camera_LensToFov(sCamera *C, float l);
void     Camera_Update   (sCamera *C);

//
// --- Scene ---
//

sScene    *Scene_New               (int numpolys);


void       Scene_AddNode           (sScene *S, sNode *N);
sNode     *Scene_NewNode           (sScene *S);
sNode     *Scene_FindNodeByName    (sScene *S, char *name);
sNode     *Scene_FindNodeById      (sScene *S, int id);

void       Scene_AddMaterial       (sScene *S, sMaterial *M);
sMaterial *Scene_FindMaterialByName(sScene *S, char *name);

void       Scene_SortPolys         (sScene *Scene);
void       Scene_Update            (sScene *S);

//
// --- Load3DS ---
//

int Scene_Load3DS (sScene *Scene, char *file);
int Scene_Parse3DS(sScene *Scene, char *Buffer, char *BufferEnd);

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 #endif
