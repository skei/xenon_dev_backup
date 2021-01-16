#ifndef types_h_included
#define types_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

typedef struct sVector
{
 float x,y,z;
} sVector;

// 컴컴컴컴컴

typedef struct sKeySegment
{
 int TimeStamp;
 sVector Pos;
} sKeySegment;

// 컴컴컴컴컴

typedef struct sKeyFrame
{
 int Flags;     // line/spline, non/wrapping
 float CurrentPos;
// float Speed;
 int CurrentSegment;
 sKeySegment *SplineSegments;
} sKeyFrame;

// To update:
// if ( (int)CurrentPos > SplineSegment[CurrentSegment].TimeStamp)
// {
//  CurrentSegment++;
// }
// Spline : Current, +1, +2

// 컴컴컴컴컴

typedef struct sVertex
{
 sVector Pos;               // vertex pos in 3D space
 sVector PosR;
 sVector PosR3D;            // 3D rotated vertex, not including the +160/100
 sVector Normal;            // vertex normal
// sVector NormalR;
 float u,v;                 // mapping coordinates
 float c;                   // vertex light/shading
} sVertex;

// 컴컴컴컴컴

typedef struct sPolygon
{
 sVertex *v[3];
 sVector Normal;            // face normal
 sVector NormalR;
 int z;                     // Material
 struct sObject *Object;    // Which object this poly belongs to
} sPolygon;

// 컴컴컴컴컴

typedef struct sObject
{
 unsigned int Flags;
 sVector Pivot;             // Pivot point.. Or rotation center
 sVector Pos;               // object pos in world space
 sVector Rot;               // object rotation around its Pivot point
 float m[16];               // object matrix
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

// 컴컴컴컴컴

typedef struct sCamera
{
 unsigned int Flags;
 sVector Pos;
 sVector Rot;
 sVector Target;
 float Roll;
 float m[16];
 sKeyFrame *KeyFramerPos;
 sKeyFrame *KeyFramerRot;
} sCamera;

// 컴컴컴컴컴

typedef struct sLight
{
 unsigned int Flags;
 sVector Pos;
 sVector Rot;
 float m[16];
 sKeyFrame *KeyFramerPos;
 sKeyFrame *KeyFramerRot;
} sLight;

// 컴컴컴컴컴
typedef struct sWorld
{
 unsigned int Flags;
 sObject *Objects;
 sCamera *Camera;
 int NumLights;
 sLight *Lights;
 int VisiblePolys;          // Nunber of visible polys in scene
 sPolygon **RenderList;     // ptr to list of ptr's to polygons
 sPolygon **Sorted;         // same, used for sorting
} sWorld;


// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
