 #ifndef engine_h_included
 #define engine_h_included
 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

 #include "math3d.h"

//
// 굇� Defines 굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇
//

#define    NULL 0
#define      PI 3.1415926535
#define EPSILON 1.0E-6      /* a tiny number */
#define    TRUE 1
#define     YES 1
#define   FALSE 0
#define      NO 0

// 컴컴�

 // change to #define DEBUG(x) //x to remove all debug stuff
#define DEBUG(x) x

//
// 굇� Types 굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇
//

typedef unsigned int bool;

 // 컴�

typedef union sKeyData
 {
  int     _int;
  float   _float;
  sVector _vector;
  float   _quat[4];
 } sKeyData;

 // 컴�

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

 // 컴�

typedef struct sTrack
 {
  unsigned int TRACK_loop : 1;
  int    Flags;
  int    NumKeys;                       // number of keys
  float  Frames;                        // number of frames
  sKey  *Keys;                          // the track
  sKey  *LastKey;                       // pointer to last used key
 } sTrack;

 // 컴�

typedef struct sVertex
 {
  sVector Pos;
  sVector PosR;
  sVector Normal;
  float u,v,c;            // mapping & shading
  int ceil_y;
  float prestep;          // prestep value for subpixel precision mapping
 } sVertex;

 // 컴�

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

 // 컴�

typedef struct sPolygon
 {
  unsigned int  POLYGON_material : 1;
  sVertex      *v[3];
  sVector       Normal;
  int           z;
  sMaterial    *Material;
 } sPolygon;

 // 컴�

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

 // 컴�

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

 // 컴�

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

 // 컴�

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
// 굇굇� Global variables :-/ 굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇�
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
// 굇� Prototypes 굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇�
//

//
// 컴� misc 컴�
//

int *Int_New( int num );

//
// 컴� key 컴�
//

sKey *Key_New( int num );

//
// 컴� vertex 컴�
//

sVertex *Vertex_New( int num );

//
// 컴� polygon 컴�
//

sPolygon *Polygon_New( int num );

//
// 컴� material 컴�
//

sMaterial *Material_New(void);
sMaterial *Material_FindByName( sScene *Scene, char *name );

//
// 컴� light 컴�
//

sLight *Light_New(void);

//
// 컴� camera 컴�
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
// 컴� object 컴�
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
// 컴� yucku 컴�
//

void Yucku_Line(int x0, int y0, int x1, int y1, char color, char *_B);
void Yucku_PolyWireFrame( sPolygon *p );

//
// 컴� scene 컴�
//

sScene *Scene_New( int maxpolys );
void    Scene_SortPolys( sScene *Scene );
void    Scene_SetupForRendering( sScene *Scene);
void    Scene_DrawAllPolys( sScene *Scene);


//
// 袴� LOAD3DS 袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴
//

void Load3DS( char *filename, sScene *Scene );
void Parse3DS( char *buffer, int BufferSize, sScene *Scene );

//
// 袴� SPLINES 袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴袴
//

int Spline_Init ( sTrack *track);
int Spline_InitRot (sTrack *track);
int Spline_GetKey_Float (sTrack *track, float frame, float *out);
int Spline_GetKey_Vect  (sTrack *track, float frame, sVector *out);
int Spline_GetKey_Quat  (sTrack *track, float frame, float *out);





 /* 컴� Documentation 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴� */

 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
 #endif
