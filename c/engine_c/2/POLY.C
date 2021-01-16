 // edge buffer

 #define S 65536.0
 #define MAGIC (((S*S*16)+(S*.5))*S)
 #define MAGICfixed (MAGIC/65536.0)

#pragma inline float2int;
 static long float2int( float d )
 {
  double dtemp = MAGIC+d;
  return (*(long *)&dtemp)-0x80000000;
 }

 // ---

 // konverterer til 16.16 fixed point
#pragma inline float2fixed;
 static long float2fixed( float d )
 {
  double dtemp = MAGICfixed+d;
  return (*(long *)&dtemp)-0x80000000;
 }

 // ---

 // ceil
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

typedef struct sVertex
 {
  float x,y,z;
  float u,v;
  float c;
 } sVertex;

 // ---

typedef struct sEdgetype
 {
  int x,z,u,v,c;
 } sEdgetype;

 // ---

sEdgetype Edgebuffer[200];

//
// ---
//

void ScanEdge(sVertex *v1, sVertex *v2)
 {
  float delta;
  int fixdelta,height;
  int x,y;

  delta = (v2->x - v1->x) / (v2->y - v1->y);
  fixdelta = float2fixed(delta);
  height = v2->y - v1->y;
  x = float2fixed(v1->x);
  y = float2int(v1->y);
  while(height!=0)
  {
   Edgebuffer[y].x = x;
   x += fixdelta;
   height--;
  }
 }
