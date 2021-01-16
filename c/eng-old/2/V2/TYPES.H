#ifndef types_h_included
#define types_h_included
// ��������������������������������������������������������������������������

typedef struct sVector
{
 float x,y,z;
} sVector;

typedef struct sIVector
{
 int x,y,z;
} sIVector;

// ����������

typedef struct sVertex
{
 sVector Pos;
 sVector PosR;
 sVector Normal;
 sVector NormalR;
 float u,v;
 float c;
} sVertex;

// ����������

typedef struct sPolygon
{
 sVertex *v[3];
 sVector Normal;
 sVector NormalR;
 int z;                   // added Z ( for sorting)
 struct sGroup *Group;           // which group this poly belongs to
} sPolygon;

// ����������

typedef struct sGroup
{
 unsigned int Flags;
 int NumPolys;
 char *Texture;
 sPolygon *Polygons;
 struct sGroup *Next;
} sGroup;

// ������������������������������

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

// ����������

typedef struct sCamera
{
 unsigned int Flags;
 sVector Pos;
 sVector Rot;
 sVector Target;
 float Roll;
 float m[16];
} sCamera;

// ������������������������������

typedef struct sWorld
{
 unsigned int Flags;
 sObject *Objects;
 sCamera *Camera;
 int VisiblePolys;
 sPolygon **RenderList;
 sPolygon **Sorted;
} sWorld;

// ��������������������������������������������������������������������������
#endif