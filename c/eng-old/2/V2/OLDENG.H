#ifndef _engine_h_included_
#define _engine_h_included_
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

// ±±±±± Defines ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

#define NULL 0
#define FALSE 0
#define TRUE 1
#define PI 3.1415926535
#define DISTANCE 128
// distance from eye/camera to projection plane (screen) "zoom parameter"
#define PROJ_PARAM 512
// value to multiply all Poly.z with (for sorting - radix=integer)
#define Z_MULT 1//256

#define viewdistance PROJ_PARAM
#define halfscreenwidth_vd  (320/2)/viewdistance
#define halfscreenheight_vd (200/2)/viewdistance
#define minclip_z 10
#define maxclip_z 4000
// Size of sin/cos tables
#define TABLESIZE 4096

// Group.RenderMethod
#define RENDER_GouraudTexture 1
#define RENDER_EnvMap         2

// Object.Flags
#define OBJECT_Active 1

// ±±±±± Types ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

typedef unsigned int boolean;

typedef struct sVector
{
 float x,y,z;                 // x,y,z components of vector
} sVector;

// -----

typedef struct sVertex
{
 sVector Pos;               // vertex coordinate
 sVector Normal;            // Vertex normal
 float tu,tv;                 // texture U & V
 float c;                     // Vertex color/intensity
} sVertex;

// Rotated/Render
typedef struct sVertexR
{
 sVector Pos;
 sVector Normal;
 float tu,tv;
 float c;
} sVertexR;

// -----

typedef struct sPolygon
{
 sVertex *v1,*v2,*v3;       // pointer to sVertex structure
 sVector Normal;
} sPolygon;

typedef struct sPolygonR
{
 sVertex *v1, *v2, *v3;     // Pointer to RENDER vertices
 sVector Normal;
 struct sGroup *group;      // pointer to sGroups structure
 int z;
} sPolygonR;

// -----

typedef struct sGroup
{
 int RenderMethod;          // How to render polygons in group
 int Flags;
 int NumPolys;              // Number of polygons in group
 sPolygon *Polygons;        // -> list of polygons
 sPolygonR *PolygonsR;
 int NumVertices;           // Number of vertices this group uses
 int *VertexList;           // -> List of all vertices this group uses (o->Vertices[i])
 struct sGroup *Next;       // -> Next group in object
 char *texture;
} sGroup;

// -----

typedef struct sObject
{
 int Flags;                 // 0/1 (not)/active
 sVector Pos;               // object center in world space
 sVector Rot;               // object rotation in object space
 float m[16];                 // Calculated matrix
 float BBoxRadius;            // radius of bounding box
 sGroup *Groups;            // -> first group in object
 int NumVertices;
 sVertex *Vertices;         // -> Pointer to array of pointers, each pointing to a sVertex structure
 sVertexR *VerticesR;
 struct sObject *Child;     // -> Child-object
 struct sObject *Branch;    // -> "branch"-object (sister/brother?? :))
} sObject;

// -----

typedef struct sLight
{
 int Flags;
 sVector Pos;
 sVector Rot;
 struct sLight *Next;
} sLight;

typedef struct sCamera
{
 int Flags;
 sVector Pos;
 sVector Rot;
 sVector Target;
 float m[16];
} sCamera;

// -----

typedef struct sWorld
{
 sObject *Objects;
 int cameratype;    // 0 = free pos/rot... 0 = pos/aim
 sCamera *Camera;
 int NumLights;
 sLight *Lights;
 int VisiblePolys;
 sPolygonR **RenderList;    // pointer to array of pointers, each pointing to Polygon structyre
 sPolygonR **Sorted;        // same, but used for sorting
} sWorld;

// ±±±±± Variables ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

extern int Numbers[256];
extern int mul960[600];
extern sWorld World;           // world
extern sCamera Camera;         // camera
extern char *_Buffer;
extern char *_Texture;         // int polyfiller
extern char *_ShadeTable;
extern char *_PhongMap;
extern unsigned char LightAdd;            // center of PhongMap as adder
extern float *sine;
extern float *cosine;
// Poly-filler variables
extern int _innerTexAdder;
extern int _innerColorAdder;
extern int _outerTexAdder;
extern int _outerColorAdder;
extern int _outerLeftAdder;
extern int _outerRightAdder;
extern int _savedLeft;
extern int _savedRight;
extern int _savedTex;
extern int _savedC;

// ±±±±± Graphics ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void SetPalette(char *palette);

void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

void setrgb (char,char,char,char);
#pragma aux setrgb=\
 "mov dx,3c8h",\
 "out dx,al",\
 "inc dx",\
 "mov al,ah",\
 "out dx,al",\
 "mov al,bl",\
 "out dx,al",\
 "mov al,bh",\
 "out dx,al",\
 parm [al] [ah] [bl] [bh] modify [edx];

void waitvr (void);
#pragma aux waitvr =\
 " mov dx,3dah",\
 "wait1:",\
 " in al,dx",\
 " test al,8",\
 " jnz wait1",\
 "wait2:",\
 " in al,dx",\
 " test al,8",\
 " jz wait2",\
 modify [al dx];

void asm_ClearBuffer(void);
// (960*200)+320 = 192320
#pragma aux asm_ClearBuffer=\
 " mov edi,[_Buffer]",\
 " add edi,192320",\
 " xor eax,eax",\
 " mov ecx,200",\
 "l1:",\
 " mov edx,80",\
 "l2:",\
 " mov [edi],eax",\
 " add edi,4",\
 " dec edx",\
 " jnz l2",\
 " add edi,640",\
 " dec ecx",\
 " jnz l1",\
 modify [eax ecx edx edi];

void asm_CopyBuffer(void);
// (960*200)+320 = 192320
#pragma aux asm_CopyBuffer=\
 " mov esi,[_Buffer]",\
 " add esi,192320",\
 " mov edi,0A0000h",\
 " mov ecx,200",\
 "l1:",\
 " mov edx,80",\
 "l2:",\
 " mov eax,[esi]",\
 " add esi,4",\
 " mov [edi],eax",\
 " add edi,4",\
 " dec edx",\
 " jnz l2",\
 " add esi,640",\
 " dec ecx",\
 " jnz l1",\
 modify [eax ecx edx esi edi];

/*
PROC OwnClearBuffer
 mov esi,offset Background
 mov edi,[buffer]
 add edi,(960*200)+320
 mov ecx,200
@@1:
 rept 80
  movsd
 endm
 add edi,640
 dec ecx
 jnz @@1
 ret
ENDP
*/

void SetPalette(char *palette);

// ±±±±± Poly fillers ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void GTpoly(sPolygonR *p);
void Tpoly(sPolygonR *p);

// ±±±±± Math ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

// ----- Matrix -------------------------------------------------------------

void Matrix_Identity(float *m);
void Matrix_Translation(float *m, float tx, float ty, float tz);
void Matrix_Rotation(float *m,int xang, int yang, int zang);
void Matrix_Mult(float *a, float *b);
void Matrix_Copy(float *m1, float *m2);

// ----- Vector -------------------------------------------------------------

sVector Vector_Normalize(sVector v);
float Vector_Dot(sVector v1, sVector v2);
sVector Vector_Cross(sVector v1, sVector v2);
sVector Vector_Add(sVector v1, sVector v2);
sVector Vector_Sub(sVector v1, sVector v2);
sVector Vector_Trans(sVector v, float *m);
sVector Vector_Trans3x3(sVector v, float *m);

// ±±±±± Init ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void Init_SinCos(void);
char ClosestColor(char *palette,char r, char g, char b);
void Calc_ShadeTable(char *palette, char *ShadeTable);
void Calc_ShadeHighLight(char *palette, char *ShadeTable,int diffuse,
                         float rr, float gg, float bb);
void Calc_PhongIntensity(float L, float F, float D, float S, float G);
void Calc_PhongMap(void);
void Calc_GouraudMap(void);
void Engine_Init(int maxpolys);
void Engine_Exit(void);

// ±±±±± Light ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void CalcLight_Camera(sObject *o);
void CalcLight_Directional(sObject *o, sGroup *g);
void CalcLight_EnvMap(sObject *o, sGroup *g);

// ±±±±± Engine ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void Camera_CreateMatrix_Free(sCamera *c);
void Object_CreateMatrix(sObject *o, sCamera *c);
int Object_CheckBoundingSphere(sObject *o, sCamera *c);
void SortWorldObject( void );
void HandleGroup( sObject *o,sGroup *g);
void HandleObject( sObject *o);
void Render(void);

// ±±±±± API ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

// ----- Object -------------------------------------------------------------

sObject *Object_Load(char *s);
void Object_Free(sObject *o);
void Object_Set_Pos(sObject *o, float x, float y, float z);
void Object_Set_Rot(sObject *o, int x, int y, int z);
void Object_Add_Pos(sObject *o, float x, float y, float z);
void Object_Add_Rot(sObject *o, int x, int y, int z);
sGroup *Object_Get_Group(sObject *o, int num);
void Object_Set_Flags(sObject *o, int flags);

// ----- GROUP --------------------------------------------------------------

void Group_Set_Texture(sGroup *g, char *tex);
void Group_Set_RenderMethod(sGroup *g, int method);
void Group_Set_Flags(sGroup *g, int flags);

// ----- CAMERA -------------------------------------------------------------

void Camera_Set_Pos(sCamera *c, float x, float y, float z);
void Camera_Set_Rot(sCamera *c, int x, int y, int z);
void Camera_Set_Target(sCamera *c, float x, float y, float z);
void Camera_Add_Pos(sCamera *c, float x, float y, float z);
void Camera_Add_Rot(sCamera *c, int x, int y, int z);
void Camera_Add_Target(sCamera *c, float x, float y, float z);

// ----- TEXTURE ------------------------------------------------------------

char *Texture_Load(char *s);
void Texture_Free(char *tex);

// ----- PALETTE ------------------------------------------------------------

char *Palette_Load(char *s);
void Palette_Free(char *pal);









// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#endif
