 #ifndef lite_h_included
 #define lite_h_included
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

typedef unsigned char      BYTE;
typedef unsigned short int WORD;

 // ---

typedef struct tVector
 {
  float x,y,z;
 } tVector;

 // ---

typedef struct tVertex
 {
  tVector pos;
  tVector normal;
  tVector posR;
  tVector normalR;
  float   u,v,c;
  int     clip_visible;
  int     ceil_y;
  float   prestep;
 } tVertex;

 // ---

typedef struct tPolygon
 {
  tVertex *v[3];      // denne m† kanskje utvides n†r vi skal adde Clip?
  tVector  normal;
  tVector  normalR;
  BYTE    *texture;
  int      z;
 } tPolygon;

 // ---

typedef struct tObject
 {
  tVector   pos;          // Position in WorldSpace
  tVector   rot;          // Rotation around object's origo (0,0,0)
  int       numvertices;
  tVertex  *vertices;
  int       numpolys;
  tPolygon *polygons;
  float     matrix[16];
 } tObject;

 // ---

typedef struct tLight
 {
  tVector pos;
  float   intensity;
 } tLight;

 // ---

typedef struct tCamera
 {
  tVector pos;
  tVector rot;
  float   matrix[16];
 } tCamera;

 // ---

typedef struct tScene
 {
  int        numobjects;
  tObject   *objects;
  int        numlights;
  tLight    *lights;
  tCamera   *camera;
  int        visiblepolys;
  tPolygon **renderlist;
  tPolygon **sorted;
 } tScene;

//
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

extern void Poly_Tex( tPolygon *P, char *buffer );


 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 #endif

