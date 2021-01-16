 #define NULL 0

//
// --------------------------------------------------------------------------
// types
// --------------------------------------------------------------------------
//

typedef struct sVector
 {
  float x,y,z;
 } sVector;

typedef struct sVertex
 {
  sVector Pos;
  sVector Normal;
 } sVertex;

 // ---

typedef struct sCorner
 {
  float value;
  sVector Pos;
 } sCorner;

 // ---

typedef struct sEdge
 {
  sCorner *Corner1, *Corner2;
  sVertex *Vertex;              // NULL if  not yet calculated
 } sEdge;

 // ---

typedef struct sCube
 {
  int flags;
  sCorner Corner[8];
  sEdge *Edge[12];
  struct sCube *SubCube[8];
 } sCube;

//
// --------------------------------------------------------------------------
// Variables
// --------------------------------------------------------------------------
//

 // Pre-allocated lists to fetch from...
int MaxCubes,CurrentCube;
sCube *GlobalCubeList;

int MaxVertices,CurrentVertex;
sVertex *GlobalVertexList;

int edgeTable[256];
int triTable[256][16];
int CurrentVertex;
int TestTabell[36] =
 {
  1   ,0,1,
  2   ,1,2,
  4   ,2,3,
  8   ,3,0,
  16  ,4,5,
  32  ,5,6,
  64  ,6,7,
  128 ,7,4,
  256 ,0,4,
  512 ,1,5,
  1024,2,6,
  2048,3,7
 };

//
// --------------------------------------------------------------------------
// support
// --------------------------------------------------------------------------
//

void error(char *s)
 {
  printf("%s\n",s);
  exit(1);
 }

//
// --------------------------------------------------------------------------
// Polygonization
// --------------------------------------------------------------------------
//

void VertexInterp(float IsoLevel, sVector *V,
                  sVector *v1,  sVector *v2,
                  float value1, float value2)
 {
  float mu;

  if (fabs(IsoLevel-value1) < 0.00001)
  {
   V->x = v1->x;
   V->y = v1->y;
   V->z = v1->z;
   return;
  }
  if (fabs(IsoLevel-value2) < 0.00001)
  {
   V->x = v2->x;
   V->y = v2->y;
   V->z = v2->z;
   return;
  }
  if (fabs(value1-value2) < 0.00001)
  {
   V->x = v1->x;
   V->y = v1->y;
   V->z = v1->z;
   return;
  }
  mu = (IsoLevel - value1) / (value2 - value1);
  V->x = v1->x + ( mu * (v2->x - v1->x) );
  V->y = v1->y + ( mu * (v2->y - v1->y) );
  V->z = v1->z + ( mu * (v2->z - v1->z) );
 }

 // -------------------------------------------------------------------------

int Polygonise (sCube *C,float IsoLevel)
 {
  int cubeindex;
  sVertex *V;
  int ind,j,a,b;
  int NumPolys;

  NumPolys = 0;

  cubeindex = 0;
  if (C->Corner[0].value < IsoLevel) cubeindex |= 1;
  if (C->Corner[1].value < IsoLevel) cubeindex |= 2;
  if (C->Corner[2].value < IsoLevel) cubeindex |= 4;
  if (C->Corner[3].value < IsoLevel) cubeindex |= 8;
  if (C->Corner[4].value < IsoLevel) cubeindex |= 16;
  if (C->Corner[5].value < IsoLevel) cubeindex |= 32;
  if (C->Corner[6].value < IsoLevel) cubeindex |= 64;
  if (C->Corner[7].value < IsoLevel) cubeindex |= 128;

  // cube is entirely in/out-side of surface
  if (edgeTable[cubeindex] == 0) return(0);

  ind = edgeTable[cubeindex];

  for (j=0;j<12;j++)
  {
   if ( ind & TestTabell[j*3] )
   {
    if (C->Edge[j]->Vertex == NULL)  // not yet calculated
    {
     V = &(GlobalVertexList[CurrentVertex++]);
     C->Edge[j]->Vertex = V;
     a = TestTabell[j*3+1];
     b = TestTabell[j*3+2];
     VertexInterp(IsoLevel,&(V->Pos),
                  &(C->Corner[a].Pos), &(C->Corner[b].Pos),
                    C->Corner[a].value,  C->Corner[b].value);
    }
   }
  }
  // triangularize polygon into triangles, insert in GlobalPolyLisr or
  // something, return number of polys
  return NumPolys;
 }

//
// --------------------------------------------------------------------------
// Octree/Cube filling
// --------------------------------------------------------------------------
//

float CalcIso(float x, float y, float z)
 {
  float value;

  return value;
 }

// ---

sCube *NewCube(float x1, float y1, float z1, float x2, float y2, flaot z2);
 {
  sCube *C;

  if (CurrentCube >= MaxCubes) error ("No Cubes left in GlobalCubeList");
  C = &(GlobalCubeList[CurrentCube++]);

  C->Corner[0].Pos.x = x1;
  C->Corner[0].Pos.y = y2;
  C->Corner[0].Pos.z =
  // calc Isovalues for corners
  return C;
 }



void Implicit (float x, float y, float z,float IsoValue)
 {
  sCube *C;

  C = NewCube
  // test all corners
  // divide cube in 8 smaller ones..
  // if the two Isovalues of an edge changes sign, subdivide the cube further
  // until a specified number of iterations has been met
  // insert new edges into cube as necessary, and mark Full, Empty or Partial
  // Partial = subdivide further
 }
