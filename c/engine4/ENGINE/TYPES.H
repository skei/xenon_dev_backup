#ifndef types_h_included
#define types_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

typedef struct sVector
{
 int x,y,z;                 // x,y,z components of vector
} sVector;

// -----

// allocate (NumVerticesInObject*sizeof(sVertex)) bytes for following struct
// the number of vertices in an object doesn't change at run-time
typedef struct sVertex
{
 sVector Pos;               // vertex coordinate
 sVector Normal;            // Vertex normal
 sVector rPos;              // space for rotated vertex coordinate
 sVector rNormal;           // space for rotated vertex normal
 int tu,tv;                 // texture U & V
 int c;                     // Vertex color/intensity
} sVertex;

// -----

// allocate (NumPolysInGroup*sizeof(sPolygon)) bytes for following struct
// the number of polygons in a groups doesn't change at run-time
typedef struct sPolygon
{
 sVertex *v1,*v2,*v3;       // pointer to sVertex structure
 sVector Normal;
// sVector rNormal;
 struct sGroup *group;      // pointer to sGroups structure owning this Polygon
 char *texture;
 int z;
} sPolygon;

// -----

// groups may beadded or changed at run-time, so a linked list is used
typedef struct sGroup
{
 int RenderMethod;          // How to render polygons in group
 int BBoxRadius;            // radius of group's bounding box
 char *texture;             // group texture
 char color;                // groups color (if no texture used)
 int NumPolys;              // Number of polygons in group
 sPolygon *Polygons;        // -> list of polygons.
 struct sGroup *Next;       // -> Next group in object
} sGroup;

// -----

// objects may be moved, changed, removed, etc, at run-time, -> linked list
typedef struct sObject
{
 int Status;                // 0/1 (not)/active
 sVector Pos;               // object center in world space
 sVector Rot;               // object rotation in object space
 int m[16];                 // Calculated matrix
 int e[16];                 // Calculated eye matrix
 int BBoxRadius;            // radius of bounding box
 sGroup *Groups;            // -> first group in object
 int NumVertices;
 sVertex **Vertices;        // -> Pointer to array of pointers, each pointing to a sVertex structure
 struct sObject *Child;     // -> Child-object
 struct sObject *Branch;    // -> "branch"-object (sister/brother?? :))
} sObject;

// -----

typedef struct sCamera
{
 int status;
 sVector Pos;
 sVector Rot;
 sVector Target;
 int m[16];
} sCamera;

// -----

typedef struct sWorld
{
 sObject *Objects;
 int cameratype;    // 0 = free pos/rot... 0 = pos/aim
 sCamera *Camera;
 int VisiblePolys;
 sPolygon **RenderList;    // pointer to array of pointers, each pointing to Polygon structyre
 sPolygon **Sorted;        // same, but used for sorting
} sWorld;

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif

/*

 PS:

 int *v[10];    // array of pointers
 int (*v)[10];  // pointer to array
*/
