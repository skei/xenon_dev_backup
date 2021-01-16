#ifndef engine_h_included
#define engine_h_included

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Defines
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

#define PI 3.1415926535
// max number of coords and polys per object
#define MAXPOLYS 8192
#define MAXCOORDS 8192
#define MAXOBJECTS 256
// size of sin/cos tables
#define TABLESIZE 4096
// fixed point precision... 14 bits fractional part
#define DIVD 14
// 2^DIVD = fixed point 1.0
#define DIVD_NUM 16384
// Distance from eye/camera to projection plane (screen)..
// Kinda like a zoom parameter
#define DISTANCE 128
// Shift left value for projection (constant for screen)
#define PROJ_PARAM 9
// max number of coordinates in spline
#define spline_size 8

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Types / structures
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

typedef struct          // Coordinate
{
 int x,y,z;
} sCoord;

// -----

typedef struct
{
 int x,y,z;
} sVector;

typedef struct
{
 int num;              // number of cordinates in spline.
 int oClosed;           // If TRUE, then a closed spline. If FALSE, it's open.
 float bendsize;         // How soft the spline shall be. 0==Straight line, 1==very soft "bends".
 sCoord p[spline_size];   // The cordinates of the spline
} sSpline;

// -----

typedef struct
{
 sSpline s[MAXCOORDS];
} sMorph;

// -----

// Camera structure
typedef struct
{
 int status;
 int xang,yang,zang;
 int xpos,ypos,zpos;
 int targetx,targety,targetz;
 float upx,upy,upz;
 float Bank;
 int m[16];
} sCamera;

// object structure

typedef struct
{
 int status;                    // 0/1 (not)/active
 void *next;                 // pointer to next object
 void *prev;                 // pointer to prevois object
 int rottype;                   // rotation type (face/vertex)
 int shadetype;                 // shading type (flat, gouraud, phong, etc)
 int xpos,ypos,zpos;            // Object's center in 3D
 int xang,yang,zang;            // Object's rotation angles
 int m[16];                     // Calculated matrix
 int e[16];                     // Calculated eye matrix
 int bbox[24];                  // Bounding box
 int numcoords;                 // Number of Vertexes/Coordinates
 int numpolys;                  // Number of Polys/Faces
 int *coords;                   // Pointer to list of coordinates
 int *polys;                    // Pointer to polygon data
 int *vertex_n;                 // Pointer to vertex normals
 int *face_n;                   // Pointer to face normals
 char *phongmap;                // Pointer to PhongMap
 int *bump_c;                   // Pointer to bump coordinates
 unsigned short int *bumpmap;   // Pointer to BumpMap
 int *tex_c;                    // Pointer to Texture coordinates
 char *texmap;                  // Pointer to texture map
} sObject;

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// variables
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

// Sine/Cosine tables
extern int sine[TABLESIZE];
extern int cosine[TABLESIZE];
// bytesort variables
extern int numvisiblepolys;
extern int numbers[256];
// env mapping light pos (center of light in texture)
extern int env_lightx, env_lighty;

extern int *rotated;
extern int *sorted;
extern int *rotatedvertexnormals;
extern int *polysort;
extern int *tricoords;
extern int *texcoords;
extern int *zcoords;
extern unsigned char *BackgroundBuffer;

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Prototypes
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

// Calculate sin/cos tables.
void calc_sincos(void);
void Init_Engine(void);
void Exit_Engine(void);

// ± Matrix routines ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

// Init identity matrix
void Matrix_Identity(int *m);
// Translate (move) matrix
void Matrix_Translation(int *m, int tx, int ty, int tz);
// Premultiplied rotation matrix
void Matrix_Rotation(int *m,int xang, int yang, int zang);
// Multiply matrixes (full 4x4 * 4x4 :-/).    a = a*b
void Matrix_Mult(int *a, int *b);
// Process matrix.
void Matrix_Process(int *m, int *src, int *dst, int num);
// Copies the contents of m2 into m1
void Matrix_Copy(int *m1, int *m2);

// ± Vector routines ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

// Normalize vector
sVector Vector_Norm(sVector v);
// dot product of two vectors
int Vector_Dot(sVector v1, sVector v2);
// cross product of two vectors
sVector Vector_Cross(sVector v1, sVector v2);
// add two vectors together
sVector Vector_Add(sVector v1, sVector v2);
// subtract vector v2 from vector v1
sVector Vector_Sub(sVector v1, sVector v2);
// translate vector by (rotation) matrix
sVector Vector_Trans(sVector v, int *m);

// ± Bytesort routines ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

// table = table to sort
// t = array of coordinates
// z = array of Z values
int polysortsetup(sObject *o, int *table, int *t, int *z);
void bytesort(int numvis, int *table, int *sorted);

// ± Object handling routines ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

sObject* Object_Init(void);
sObject* Object_Copy(sObject *o);
void Object_Set_Rot(sObject *o, int xa, int ya, int za);
void Object_Set_Pos(sObject *o, int x, int y, int z);
void Object_Add_Rot(sObject *o, int xa, int ya, int za);
void Object_Add_Pos(sObject *o, int x, int y, int z);
void Object_Set_Mapping(sObject *o, int t, char *texture);
char* Load_Texture(char *f);
char* Load_Palette(char *f);

// ± Camera handling routines ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void Camera_Set_Rot(sCamera *c, int xa, int ya, int za);
void Camera_Set_Pos(sCamera *c, int x, int y, int z);
void Camera_Add_Rot(sCamera *c, int xa, int ya, int za);
void Camera_Add_Pos(sCamera *c, int x, int y, int z);
void Camera_Set_Target(sCamera *c, int x, int y, int z);
void Camera_Create_Lookat(sCamera *c, int *m);

// ± Spline routines ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

// location = pos in spline (0..65535)
sCoord get_spline_point(int location, sSpline *spl);

// ± Engine specific routines ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

// project vertex normals
// r = array of rotated vertex normals (x,y,z)
// t = array of texture coordinates (u,v)
void EnvProjectVertexNormals (sObject *o, int *r, int *t);
// project coordinates 3D (rotated) -> 2D (tricoords)
// r = array of rotated coordinates (x,y,z)
// t = array of projected coordinates (x,y)
// z = array of Z-values
void ProjectCoords (sObject *o, int *r, int *t, int *z);
// nv = number of visible polygons
// table = sorted tablr (from bytesort)
// tri -> triangle coordinates
// tex -> texture coordinates
void Object_DrawTexture(unsigned char *b,sObject *o,int nv, int *table, int *tri, int *tex);
// Project object from object space to screen space
void Object_Project(sObject *o, sCamera *c);
void Object_Draw(unsigned char *b,sObject *o);
void Camera_Process_Aim(sCamera *c);
void Camera_Process_Free(sCamera *c);
void Check_BoundingBox(unsigned char *buf,sObject *o, sCamera *c);
void Anim_Init(sObject *o1,sObject *o2,sObject *o3, sMorph *os);
void Anim_Update(sObject *anim, int objloc, sMorph *os);

// ± Texture mapping routines ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void TexMap(unsigned char *buf,int c1, int c2, int c3, int *tric, int *texc, unsigned char *Texture);

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#endif
