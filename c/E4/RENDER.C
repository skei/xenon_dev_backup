
// ÄÄÄÄÄ #define ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

#define NULL 0
#define FALSE 0
#define TRUE 1

// ÄÄÄÄÄ Typedef ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

// a vector/cordinate
typedef struct sVector
{
 int x,y,z;                 // x,y,z components of vector
} sVector;

// -----

// defines a polygon
typedef struct sPolygon
{
 sVertex *v1,*v2,*v3;
 sGroup *group;
 int z;
} sPolygon;

// -----

// defines one of the three vertices in a polygon
typedef struct sVertex
{
 sVector Position;          // vertex coordinate
 sVector Normal;            // Vertex normal
 sVector rPosition;         // space for rotated vertex coordinate
 sVector rNormal;           // space for rotated vertex normal
 int tu,tv;                 // texture U & V
 int c;                     // Vertex color/intensity
 struct sVertex *Next;      // Pointer to next vertex
} sVertex;

// -----

// defines one of the groups an object can consist of
typedef struct sGroup
{
 int shadingtype;           // shading type (gouraud, flat, gouraud texture, etc)
 int fCount;                // Number of faces in group
 sPolygon *PolyData;             // pointer to "poly-connect" list
 char *Texture;             // pointer to texture
 struct sGroup *Next;       // pointer to Child/next node
 int matrix[16];            // space for rotation matrix
} sGroup;

// -----

// the entire object
typedef struct sObject
{
 sVector Position;          // object center in world space
 sVector Rotation;          // object rotation in object space
 sGroup* Groups;            // poiner to group-list
 sVertex *Vertices;         // pointer to vertex list
 int bSphereRadius;         // radius of bounding sphere
 int matrix[16];            // space for object matrix
 struct sObject *Child;     // pointer to "parallell branch"
 struct sObject *Branch;    // pointer to "parallell branch"
} sObject;

// ÄÄÄÄÄ Global variables ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ



// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

// Recursively handle a group and all sub-groups
void RenderGroup( struct sGroup *g)
{
 if (g!=NULL)
 {
  // Chech each polygon in group...
  // if visible, add to WorldPolys list
  RenderGroup(g->Next);
 }
}

// Recursively handle an object and all Child/Branch objects
// Start rendering of world with: RenderObject(World->Objects) or something
void RenderObject( struct sObject *o)
{
 if (o!=NULL)
 {
  // check if object visible (check bounding sphere)
  // Calculate matrix
  RenderGroup(o->Groups);     // renders an object
  RenderObject(o->Child);     // process child objects
  RenderObject(o->Branch);    // process "neighbour" objects
 }
}

// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ

void main(void)
{
}
