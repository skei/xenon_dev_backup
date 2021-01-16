#define ScreenHeight 200

// ---

 // float -> int

#define S 65536.0
#define MAGIC (((S*S*16)+(S*.5))*S)
#define MAGICfixed (MAGIC/65536.0)

#pragma inline float2int;
 static long float2int( float d )
 {
  double dtemp = MAGIC+d;
  return (*(long *)&dtemp)-0x80000000;
 }

 // 컴컴컴컴컴

#pragma inline float2fixed;
 static long float2fixed( float d )
 {
  double dtemp = MAGICfixed+d;
  return (*(long *)&dtemp)-0x80000000;
 }

 // 컴컴컴컴컴

#pragma inline CEIL;
 static int CEIL(float f)
 {
  int i,j;

  i = float2fixed(f);
  j = i >> 16;
  if ((i&0xffff) != 0) j+=1;
  return j;
 }

// ---

typedef struct sVector
 {
  float x,y,z;
 } sVector;

typedef struct sVertex
 {
  sVector pPos;
  float prestep;
  int ceil_y;
  int y;
 } sVertex;

typedef struct sPolygon
 {
  sVertex *v[3];
 } sPolygon;

typedef struct sPolyEdge
 {
  int LeftX,RightX;
  int u,v;
  int c,z;
 } sPolyEdge;

// ---

sPolyEdge Edge[ScreenHeight];

// ---

void ScanRightX(sVertex *v1, sVertex *v2)
 {
  float height,xslope;
  int x,x_slope;
  sPolyEdge *b;
  int h;

  // v1 = top vertex
  // v2 = bottom vertex
  h = v2->ceil_y - v1->ceil_y;
  if (h <= 0) return;
  height =  v2->pPos.y - v1->pPos.y;
  xslope = (v2->pPos.x - v1->pPos.x) / height;
  x_slope = float2fixed(xslope);
  x = (1<<15) + float2fixed(v1->pPos.x+(v1->prestep * xslope));
  b = &(Edge[v1->ceil_y]);

  do
  {
   b->RightX = x;
   x+=x_slope;
  } while (--h);
 }

 // ---

void ScanLeftX(sVertex *v1, sVertex *v2)
 {
  float height,xslope;
  int x,x_slope;
  sPolyEdge *b;
  int h;

  // v1 = top vertex
  // v2 = bottom vertex
  h = v2->ceil_y - v1->ceil_y;
  if (h <= 0) return;
  height =  v2->pPos.y - v1->pPos.y;
  xslope = (v2->pPos.x - v1->pPos.x) / height;
  x_slope = float2fixed(xslope);
  x = (1<<15) + float2fixed(v1->pPos.x+(v1->prestep * xslope));
  b = &(Edge[v1->ceil_y]);

  do
  {
   b->LeftX = x;
   x+=x_slope;
  } while (--h);
 }

