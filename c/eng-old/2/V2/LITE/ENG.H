// --------------------------------------------------------------------------

#define PI 3.1415926535
// size of sin/cos tables
#define TABLESIZE 4096

// Value for 3d->2d projection
#define PROJ_VALUE 256
#define PROJ_VALUE2 512

// for converting Z-values to int for radix sort
#define Z_MULT 1

// sObject.Flags
#define OBJECT_Active 1

// --------------------------------------------------------------------------

typedef struct sVector
{
 float x,y,z;
} sVector;

typedef struct sIVector
{
 int x,y,z;
} sIVector;

// 컴컴컴컴컴

typedef struct sVertex
{
 sVector Pos;
 sVector PosR;
 sVector Normal;
 sVector NormalR;
 float u,v;
 int c;
} sVertex;

// 컴컴컴컴컴

typedef struct sPolygon
{
 sVertex *v[3];
 sVector Normal;
 sVector NormalR;
 int z;                   // added Z ( for sorting)
 struct sGroup *Group;           // which group this poly belongs to
} sPolygon;

// 컴컴컴컴컴

typedef struct sGroup
{
 unsigned int Flags;
 int NumPolys;
 sPolygon *Polygons;
 struct sGroup *Next;
} sGroup;

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

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

// 컴컴컴컴컴

typedef struct sCamera
{
 unsigned int Flags;
 sVector Pos;
 sVector Rot;
 sVector Target;
 float Roll;
 float m[16];
} sCamera;

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

typedef struct sWorld
{
 unsigned int Flags;
 sObject *Objects;
 sCamera *Camera;
 int VisiblePolys;
 sPolygon **RenderList;
 sPolygon **Sorted;
} sWorld;


// --------------------------------------------------------------------------

// sin/cos tables.... allocated in init
extern float *sine;
extern float *cosine;

extern unsigned char *_Screen;
extern unsigned char *_Buffer;
extern unsigned char *_Texture;
extern unsigned char *_ShadeTable;

extern int ScreenWidth,ScreenHeight;

extern float viewdistance;
extern float halfscreenwidth_vd;
extern float halfscreenheight_vd;
extern float minclip_z;
extern float maxclip_z;

extern int MinX,MaxX;
extern int MinY,MaxY;


extern sWorld *World;

// --------------------------------------------------------------------------

void Engine_Init(int maxpolys);
void Engine_Exit(void);
sObject *Object_Load( char *s );

void Object_SetPos( sObject *o, float x, float y, float z);
void Object_SetRot( sObject *o, float x, float y, float z);
void Object_AddRot( sObject *o, float x, float y, float z);

void Camera_SetPos( sCamera *c, float x, float y, float z);
void Camera_SetRot( sCamera *c, float x, float y, float z);
void Camera_AddRot( sCamera *c, float x, float y, float z);

// --------------------------------------------------------------------------

void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

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


// --------------------------------------------------------------------------

void Line(int x0, int y0, int x1, int y1, char color);
void DrawChar( int x, int y, unsigned char c, short int color);
void DrawString(int x1, int y1, char *s, short int color);

// --------------------------------------------------------------------------

// (16.16 * 16.16) >> 16
// (16.16 *  8.8 ) >> 16
int fixedmul(int, int);
#pragma aux fixedmul=\
 "imul ebx",\
 "shrd eax,edx,16",\
 parm [eax] [ebx] value [eax] modify [edx];

// (16.16 << 16) / 16.16
// ( 8.8  << 16) / 16.16
int fixeddiv(int,int);
#pragma aux fixeddiv=\
 "cdq",\
 "shld edx,eax,16",\
 "idiv ebx",\
 parm [eax][ebx] value [eax] modify [edx];

void Init_SinCos(void);

void Matrix_Identity(float *m);
void Matrix_Translation(float *m, float tx, float ty, float tz);
void Matrix_Rotation(float *m, float xang, float yang, float zang);
void Matrix_Mult(float *a, float *b);
void Matrix_Copy(float *m1, float *m2);

sVector Vector_Normalize(sVector *v);
float Vector_Dot(sVector *v1, sVector *v2);
sVector Vector_Cross(sVector *v1, sVector *v2);
sVector Vector_Add(sVector *v1, sVector *v2);
sVector Vector_Sub(sVector *v1, sVector *v2);
sVector Vector_Trans(sVector *v, float *m);
sVector Vector_Trans3x3(sVector *v, float *m);

float Vector_Angle( sVector *v1, sVector *v2);

// --------------------------------------------------------------------------

void PolyWireFrame( sPolygon *p );
void TexPoly      ( sPolygon *p );

// --------------------------------------------------------------------------

void Render( void );

// --------------------------------------------------------------------------




