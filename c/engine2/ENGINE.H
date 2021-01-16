 #ifndef engine_h_included
 #define engine_h_included
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 #include "math3d.h"

//
// ±±± Defines ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
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
#define DEBUG(x) x

//
// ±±± Types ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

typedef unsigned int bool;

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
  struct sKey *Next, *Prev;
 } sKey;

 // ÄÄÄ

typedef struct sTrack
 {
  unsigned int TRACK_loop : 1;
  int    Flags;
  int    NumKeys;                       // number of keys
  float  Frames;                        // number of frames
  sKey  *Keys;                          // the track
  sKey  *LastKey;                       // pointer to last used key
 } sTrack;

 // ÄÄÄ

typedef struct sVertex
 {
  sVector Pos;
  sVector PosR;
  sVector Normal;
  float u,v,c;            // mapping & shading
  int ceil_y;
  float prestep;          // prestep value for subpixel precision mapping
 } sVertex;

 // ÄÄÄ

typedef struct sMaterial
 {
  unsigned int      MATERIAL_texture : 1;
  int               Id;
  char              Name[20];
  char              FileName[20];
  int               Color;
  char             *Texture;
  struct sMaterial *Next;
 } sMaterial;

 // ÄÄÄ

typedef struct sPolygon
 {
  unsigned int  POLYGON_material : 1;
  sVertex      *v[3];
  sVector       Normal;
  int           z;
  sMaterial    *Material;
 } sPolygon;

 // ÄÄÄ

typedef struct sObject
 {
  // - flags -
  unsigned int     OBJECT_active : 1;
  unsigned int     OBJECT_mapped : 1;
  unsigned int     OBJECT_dummy  : 1;
  int              Id;
  char             Name[20];
  int              Type;
  float            SphereRadius;
  sVector          Pos;
  sVector          Rot;
  float            Q[4];
  float            Obj2World[16];
  float            Obj2Camera[16];
  float            Matrix3DS[16];   // Matrix loaded from 3DS file
  int              NumVertices;
  sVertex         *Vertices;
  int              NumPolygons;
  sPolygon        *Polygons;
  struct sObject  *Next,*Prev;
  int              ParentId;
  struct sObject  *Parent;
  sVector Pivot;          // Pivot point
  sTrack           kfPos;
  sTrack           kfRot;
 } sObject;

 // ÄÄÄ

typedef struct sCamera
 {
  unsigned int    CAMERA_active : 1;
  int             Id;
  char            Name[20];
  int             Type;
  sVector         Pos;
  sVector         Rot;
  sVector         Target;
  sVector         UpVector;
  float           Roll;
  float           Fov;
  float           Matrix[16];
  float           SizeX,SizeY;      // 3d->2d projection
  float           AspectRatio;
  float           PerspX,PerspY;      // 3d->2d projection
  struct sCamera *Next;
  int             ParentId;
  sVector Pivot;                // Pivot point
  sTrack          kfPos;
  sTrack          kfTarget;
  sTrack          kfFov;
  sTrack          kfRoll;
 } sCamera;

 // ÄÄÄ

typedef struct sLight
 {
  unsigned int   LIGHT_active : 1;
  int            Id;
  char           Name[20];
  int            Type;
  sVector        Pos;
  sVector        Rot;
  float          Strength;
  float          MaxRange;
  float          Matrix[16];
  struct sLight *Next;
 } sLight;

 // ÄÄÄ

typedef struct sScene
 {
  unsigned int  SCENE_anim : 1;
  int           NumCameras;        // ??? only one camera allowed ???
  sCamera      *Cameras;
  sCamera      *Camera;            // The active camera
  int           NumLights;
  sLight       *Lights;
  int           NumObjects;
  sObject      *Objects;
  int           NumMaterials;
  sMaterial    *Materials;
  float         AmbientLight;
  float         ViewDistanceX;     // 3D->2D projection
  float         ViewDistanceY;     // for X & Y : AspectRatio fixing
  int           VisibleObjects;    // Number of visible objects on screen. Mainly for debugging
  int           VisiblePolys;      // Number of visible polys on screen
  sPolygon    **RenderList;        // ptr to list of ptr's to polygons
  sPolygon    **Sorted;            // --"-- : for sorting
  // - anim -
  int           StartFrame;         // Start frame for 3DS animation
  int           EndFrame;           // End frame for 3DS animation
  float         CurrentPos;
  float         Speed;
 } sScene;

//
// ±±±±± Global variables :-/ ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

extern char *_Buffer;    // Background Buffer to draw to
extern char *_Screen;    // _the_ screen
extern int MinX,MaxX,MinY,MaxY;      // Clipping rectangle
extern int ScreenWidth,ScreenHeight;  // ViewPort size
extern int TopClip;
extern int BotClip;
extern int LeftClip;
extern int RightClip;

//
// ±±± Prototypes ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

//
// ÄÄÄ misc ÄÄÄ
//

int *Int_New( int num );

//
// ÄÄÄ key ÄÄÄ
//

sKey *Key_New( int num );

//
// ÄÄÄ vertex ÄÄÄ
//

sVertex *Vertex_New( int num );

//
// ÄÄÄ polygon ÄÄÄ
//

sPolygon *Polygon_New( int num );

//
// ÄÄÄ material ÄÄÄ
//

sMaterial *Material_New(void);
sMaterial *Material_FindByName( sScene *Scene, char *name );

//
// ÄÄÄ light ÄÄÄ
//

sLight *Light_New(void);

//
// ÄÄÄ camera ÄÄÄ
//

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

//
// ÄÄÄ object ÄÄÄ
//

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

//
// ÄÄÄ yucku ÄÄÄ
//

void Yucku_Line(int x0, int y0, int x1, int y1, char color, char *_B);
void Yucku_PolyWireFrame( sPolygon *p );

//
// ÄÄÄ scene ÄÄÄ
//

sScene *Scene_New( int maxpolys );
void    Scene_SortPolys( sScene *Scene );
void    Scene_SetupForRendering( sScene *Scene);
void    Scene_DrawAllPolys( sScene *Scene);


//
// ÍÍÍ LOAD3DS ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
//

void Load3DS( char *filename, sScene *Scene );
void Parse3DS( char *buffer, int BufferSize, sScene *Scene );

//
// ÍÍÍ SPLINES ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
//

int Spline_Init ( sTrack *track);
int Spline_InitRot (sTrack *track);
int Spline_GetKey_Float (sTrack *track, float frame, float *out);
int Spline_GetKey_Vect  (sTrack *track, float frame, sVector *out);
int Spline_GetKey_Quat  (sTrack *track, float frame, float *out);





 /* ÄÄÄ Documentation ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ */

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 #endif
