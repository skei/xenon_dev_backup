#ifndef types_h_included
#define types_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

typedef struct sVector
{
 float x,y,z;
} sVector;

typedef struct sIVector
{
 int x,y,z;
} sIVector;

// 컴컴컴컴컴

typedef struct sVertex
{
 sVector Pos;
 sVector PosR;
 sVector Normal;
 sVector NormalR;
 float u,v;
 float c;
} sVertex;

// 컴컴컴컴컴

typedef struct sPolygon
{
 sVertex *v[3];
 sVector Normal;
 sVector NormalR;
 int z;                   // added Z ( for sorting)
 struct sGroup *Group;           // which group this poly belongs to
} sPolygon;

// 컴컴컴컴컴

typedef struct sGroup
{
 unsigned int Flags;
 int NumPolys;
 sPolygon *Polygons;
 struct sGroup *Next;
} sGroup;

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

typedef struct sObject
{
 unsigned int Flags;
 sVector Pos;
 sVector Rot;
 float m[16];
 float BSphereRadius;
 int NumVertices;
 sVertex *Vertices;
 sGroup *Groups;
 struct sObject *Branch;
 struct sObject *Child;
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
} sCamera;

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

typedef struct sWorld
{
 unsigned int Flags;
 sObject *Objects;
 sCamera *Camera;
 int VisiblePolys;
 sPolygon **RenderList;
 sPolygon **Sorted;
} sWorld;

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
