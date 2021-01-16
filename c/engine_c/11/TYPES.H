#ifndef types_h_included
#define types_h_included


// --- some common structures ---

typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef unsigned int DWORD;

// -----

typedef struct
{
 int x,y,z;
} sVector;

// --------------------------------------------------------------------------

// --- OBJECT structure ---
typedef struct
{
 int status;            // 0/1 (not)/active
 sVector pos;           // position of object (world space)
 sVector ang;           // rotation angles
 int m[16];             // Calculated matrix
 int e[16];             // Calculated eye matrix
 sVector sphere;        // Bounding sphere position
 int radius;            // Bounding sphere radius
 int numcoords;         // Number of Vertexes/Coordinates
 int numpolys;          // Number of Polys/Faces
 int *coords;           // Pointer to list of coordinates
 int *polys;            // Pointer to polygon data
 int *vertex_n;         // Pointer to vertex normals
 int *face_n;           // Pointer to face normals
 BYTE *texture;         // object texture
} sObject

// --- LIGHT structure ---
typedef struct
{
 sVector pos;           // position
} sLight;

// --- CAMERA structure ---
typedef struct
{
 sVector pos;           // position of camera in world
 sVector ang;           // camera angle
 sVector aim;           // camera look-at target
 int m[16];             // space for camera matrix
} sCamera;

// --- WORLD structure ---
typedef struct
{
 totalpolys;            // total number of polys to render in a scene
 sCamera cam;           // the camera
 sObject objects;       // pointer to 1st object
 sLight lights;
} sWorld;

// --- RENDERLIST structure ---
typedef struct
{
 int x1,y1,u1,v1,c1;
 int x2,y2,u2,v2,c2;
 int x3,y3,u3,v3,c3;
 BYTE *texture;
} sRender


#endif
