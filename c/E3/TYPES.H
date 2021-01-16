#ifndef types_h_included
#define types_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

typedef struct sVector
{
 float x,y,z;
} sVector;

// 컴컴컴컴컴컴컴컴컴컴

typedef struct sVertex
{
 sVector Pos;
 sVector PosR;
 int u,v;                 // fixed point... perhaps float here too?
 int c;
} sVertex;

// 컴컴컴컴컴컴컴컴컴컴

typedef struct sPolygon
{
 sVertex *v1, *v2, *v3;
 struct sGroup *Group;
 int z;
} sPolygon;

// 컴컴컴컴컴컴컴컴컴컴

typedef struct sMaterial
{
 int Flags;
 char *Texture1;
 char *Texture2;
} sMaterial;

// 컴컴컴컴컴컴컴컴컴컴


typedef struct sGroup
{
 int Flags;
 int NumPolys;
 sPolygon *Polygons;
 sMaterial *Material;
 struct sGroup *Next;
} sGroup;

// 컴컴컴컴컴컴컴컴컴컴

typedef struct sObject
{
 int Flags;
 float m[16];
 sVector Pos;
 sVector Rot;
 float BSphereRadius;
 int NumVertices;
 sVertex *Vertices;
 int NumGroups;
 sGroup *Groups;
 struct sObject *Child;
 struct sObject *Branch;
} sObject;

// 컴컴컴컴컴컴컴컴컴컴

typedef struct sCamera
{
 int Flags;
 float m[16];
 sVector Pos;
 sVector Rot;
 sVector Target;
} sCamera;

typedef struct sLight
{
 int Flags;
 sVector Pos;
} sLight;

// 컴컴컴컴컴컴컴컴컴컴

typedef struct sWorld
{
 int Flags;
 sCamera *Camera;
 sObject *Objects;
 sLight  *Lights;
 sMaterial *Materials;
 int VisiblePolys;
 sPolygon **RenderList;
 sPolygon **Sorted;
} sWorld;


// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
