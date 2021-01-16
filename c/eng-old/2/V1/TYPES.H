#ifndef types_h_included
#define types_h_included
// ��������������������������������������������������������������������������

typedef unsigned int BOOL;
typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned int DWORD;

// ��������������������

typedef struct sVector
{
 float x,y,z;
} sVector;

// ��������������������

typedef struct sVertex
{
 sVector Pos;
 sVector PosR;
 int u,v;                 // fixed point... perhaps float here too?
 int c;
} sVertex;

// ��������������������

typedef struct sPolygon
{
 sVertex *v1, *v2, *v3;
 int z;
} sPolygon;

// ��������������������

typedef struct sMaterial
{
 int Flags;
 char *Texture1;
 char *Texture2;
} sMaterial;

// ��������������������


typedef struct sGroup
{
 int Flags;
 int NumPolys;
 sPolygon *Polygons;
 sMaterial *Material;
} sGroup;

// ��������������������

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

// ��������������������

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

// ��������������������

typedef struct sWorld
{
 int Flags;
 sCamera *Camera;
 sObject *Objects;
 sLight *Lights;
 sMaterial *Materials;
 int VisiblePolys;
 sPolygon **RenderList;
 sPolygon **Sorted;
} sWorld;

// ��������������������������������������������������������������������������
#endif