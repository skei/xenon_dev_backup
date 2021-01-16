typedef struct sVector
 {
  float x,y,z;
 } sVector;

typedef struct sVertex
 {
  float x,y,z;
 } sVertex;

typedef struct sEdge
 {
  sVertex *v1,*v2;
 } sEdge;

typedef struct sPolygon
 {
  int NumEdges;
  sEdge *Edges;
  char color;
 } sPolygon;

typedef struct sETentry
 {
  float X,Xadder;
  sPolygon *Poly;
  struct sETentry *Next,*Prev;
 } sETentry;

// ---


sETentry ET[200];   // en for hver scanline
sETentry *AET;

void AddPolygon (sPolygon *P)
 {
  int i;
  sVertex *vert1,*vert2,*vtemp;


  for (i=0;i<P->NumEdges;i++)
  {
   vert1 = P->Edges[i].v1;
   vert2 = P->Edges[i].v2;

   if (vert1.y != vert2.y)
   {
    if (vert1.y > vert2.y)
    {
     vtemp = vert1;
     vert1 = vert2;
     vert2 = vtemp;
    }
    AddEdge(
  }
 }

