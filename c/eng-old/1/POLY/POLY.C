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

#pragma inline float2fixed12_4;
 static long float2fixed12_4( float d )
 {
  double dtemp = MAGICfixed12_4+d;
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

// --------------------------------------------------------------------------

typedef struct sVertex
{
 float x,y,z;
 float u,v,c;
 int ix,iy;
 int iu,iv,ic;
 int prestep;
} sVertex;

 // 컴컴컴컴컴

typedef struct sPolygon
{
 int NumVertices;
 sVertex *v[6];     // May be up to 6 after clipping
} sPolygon;

// --------------------------------------------------------------------------

sPolygon *Poly;
char *Texture;

int left_height, right_height;
int left_vertex, right_vertex;
int end_vertex;
int NumVertices;
int left_x_slope,right_x_slope;

// --------------------------------------------------------------------------

void FlatLeft(void)
{
}

// --------------------------------------------------------------------------

void FlatRight(void)
{
 int n;

 n = right_vertex - 1;
 if (n < 0) n = NumVertices-1;

 right_height = (Poly->v[n]->iy - Poly->v[right_vertex]->iy);
 if (right_height <= 0) return;

 right_x_slope = float2fixed(  (   (Poly->v[right_vertex]->x - Poly->v[n]->x)
                             / (Poly->v[n]->x - Poly->v[right_vertex]->x) );

 right_x = Poly->v[n]->ix + (right_x_slope*Poly->v[right_vertex]->prestep);

}

// --------------------------------------------------------------------------

void FlatPoly( sPolygon *P )
{
 int i;
 int miny,maxy;
 int top,bot;

 Poly = P;
// Texture = P->Object->texture;

 miny = 320;
 maxy = 0;
 top = 0;
 bot = P->NumVertices-1;

 // Find top/bottom vertex

 for (i=0;i<P->NumVertices;i++)
 {
  if (P->v[i]->iy > maxy) // find max_y
  {
   maxy = P->v[i]->iy;
   top = i;
  }
  if (P->v[i]->iy < miny) // find min_y
  {
   miny = P->v[i]->iy;
   bot = i;
  }
 }

 left_vertex  = top;    // Left side starting vertex
 right_vertex = top;    // Right side starting vertex
 end_vertex   = bot;    // Last vertex in array
 NumVertices = P->NumVertices;

 // Start drawing... left = v[yop] -> v[top-1, wrap]
 //                 right = v[top] -> v[top+1, wrap]

 // Find Left slope & height
 do
 {
  if (right_vertex == end_vertex) return;
  FlatLeft();
 } while (left_height <= 0);
 // Find Right slope & height
 do
 {
  if (left_vertex == end_vertex) return;
  FlatRight();
 } while (right_height <= 0);

 // Draw the poly
 for(;;)
 {
 }
}


/*

1. Find top & bottom. CEIL y's check ceil'ed...
2. Left = (top-1) wrap around NumPolys
3. Right = (top+1) wrap around NumPolys








*/
