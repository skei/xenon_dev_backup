 #ifndef types_h_included
 #define types_h_included
 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

typedef struct sVector
 {
  float x,y,z;
 } sVector;

 // -----

typedef struct sVertex
 {
  sVector Pos;
 } sVertex;

 // -----

typedef struct sPolygon
 {
  sVertex *v1,*v2,*v3;
 } sPolygon;

 // -----


typedef struct sObject
 {
  int Flags;
  int NumVertices;
  sVertex *Vertices;
  int NumPolys;
  sPolygon *Polygons;
  struct sObject *Child;
  struct sObject *Parent;
 } sObject;

 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
 #endif
