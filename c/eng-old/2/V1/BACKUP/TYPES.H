#ifndef types_h_included
#define types_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned int DWORD;

typedef unsigned int boolean;

typedef struct sVector
{
 float x,y,z;                 // x,y,z components of vector
} sVector;

// -----

typedef struct sVertex
{
 sVector Pos;               // vertex coordinate
 sVector PosR;              // rotated vertex coordinate
 sVector Normal;            // Vertex normal
 sVector NormalR;           // rotated vertex normal
 float u,v;                 // texture U & V
 float c;                   // Vertex color/intensity
} sVertex;


// -----

typedef struct sPolygon
{
 sVertex *v1,*v2,*v3;       // pointer to sVertex structure
 sVector Normal;
 struct sGroup *group;      // pointer to the group this poly belongs to
 int z;
} sPolygon;

// -----

typedef struct sGroup
{
 int RenderMethod;          // How to render polygons in group
 int Flags;
 int NumPolys;              // Number of polygons in group
 sPolygon *Polygons;        // -> list of polygons
 int NumVertices;           // Number of vertices this group uses
 int *VertexList;           // -> List of all vertices this group uses (o->Vertices[i])
 char *texture;
} sGroup;

// -----

typedef struct sObject
{
 int Flags;                 // 0/1 (not)/active
 sVector Pos;               // object center in world space
 sVector Rot;               // object rotation in object space
 float m[16];               // Calculated matrix
 float BSphereRadius;       // radius of bounding sphere
 sGroup *Groups;            // -> groups (array)
 int NumVertices;
 sVertex *Vertices;         // -> Pointer to vertices
 struct sObject *Child;     // -> Child-object
 struct sObject *Branch;    // -> "branch"-object (sister/brother?? :))
} sObject;

// -----

typedef struct sLight
{
 int Flags;
 sVector Pos;
 sVector Rot;
 struct sLight *Next;
} sLight;

typedef struct sCamera
{
 int Flags;
 sVector Pos;
 sVector Rot;
 sVector Target;
 float m[16];
} sCamera;

// -----

typedef struct sWorld
{
 sObject *Objects;
 int cameratype;    // 0 = free pos/rot... 0 = pos/aim
 sCamera *Camera;
 int NumLights;
 sLight *Lights;
 int VisiblePolys;
 sPolygon **RenderList;    // pointer to array of pointers, each pointing to Polygon structyre
 sPolygon **Sorted;        // same, but used for sorting
} sWorld;


// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
