// *.ASC converter -> *.AOF (Axon Object File)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

#include <stdlib.h>   // exit
#include <stdio.h>    // file functions
#include <string.h>
#include <math.h>     // sqrt

// --------------------------------------------------------------------------

#define MAXVERTICES 10000
#define MAXPOLYGONS 10000
#define MaterialNameLength 32
#define TRUE 1
#define FALSE 0
#define OBJECT_HEADER 'OBJ0'
#define QUOTE '\"'

typedef unsigned int bool;

typedef struct
{
 float x,y,z;
 float u,v;
} sVertex;

// -----

typedef struct
{
 float x,y,z;
} sNormal;

// -----

typedef struct
{
 int a,b,c;
 int Smoothing;
 char Material[MaterialNameLength];
} sPolygon;

// -----

// ÄÄÄÄÄ Global variables ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

bool CENTER_OBJECT;
bool DEBUG;

bool infile_defined,outfile_defined;

sVertex  *Vertex;
sPolygon *Polygon;

sNormal *VertexN;
sNormal *PolygonN;

char ObjectName[256];
char *infile, *outfile;

int Mapped;
int NumVertices;
int NumPolys;

float LargestPoint;
float LargestTexPoint;

float Sphere;

// ----- variables used during loading --------------------------------------

char *dummy;
float tx,ty,tz,tu,tv;
int a,b,c;

float vertmult;    // Multiplier for Vertex coordinates
float vertnmult;   // Multiplier for Vertex normals
float facenmult;   // Multiplier for Face normals
float texmult;     // texture U/V multiplier

// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// returns TRUE if the string s contains the substring v
// --------------------------------------------------------------------------
bool instr( char *s, char *v )
{
 bool ret;
 ret = FALSE;
 if ( strstr( s, v ) != NULL )
 {
  ret = TRUE;
 }
 return ret;
}

// --------------------------------------------------------------------------
// Returning the value following V, converted to double
// --------------------------------------------------------------------------

double getvalue ( char *s, char *v )
{
 char *pos;
 double x;
 pos = strstr( s, v );        // pos -> 1st occurence of V in S
 pos+=strlen(v);              // Skip V
 x = strtod( pos, &dummy );   // x = convert following string to double
 return x;
}


// --------------------------------------------------------------------------
// Returning the string (in ""'s) following V
// --------------------------------------------------------------------------

void getstring( char *s, char *v, char *ret )
{
 char *pos;
 pos = strstr( s, v );        // pos -> 1st occurence of V in S
 pos+=strlen(v);              // Skip V
 while ( *pos != QUOTE ) pos++;
 while ( *pos == QUOTE ) pos++;
 while ( *pos != QUOTE )
 {
  *ret++ = *pos++;
 }
 *ret = '\0';
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// ± Load ASC file ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void LoadAsc(char *s)
{
 FILE *fp;
 char buf[256];

 Mapped = 0;
 NumPolys = 0;
 NumVertices = 0;

 fp = fopen (s,"rt");

// Read object name
 while( !instr(buf,"Named object:"))
 {
  fgets ( buf, 255, fp );
 }
 getstring( buf,"Named object:",ObjectName);
 printf("Object found: %s\n",ObjectName);
 while ( !feof( fp ) )
 {
  fgets ( buf, 255, fp );
  if ( instr( buf, "Mapped"))
  {
   printf("object is Mapped\n");
   Mapped = 1;
  }

  if ( instr( buf, "Vertex " ) && !instr( buf, "list:" ) )
  {
   tx = (vertmult * getvalue( buf, "X:"));
   ty = (vertmult * getvalue( buf, "Y:"));
   tz = (vertmult * getvalue( buf, "Z:"));
   if (Mapped==1)
   {
    tu = (texmult * getvalue( buf, "U:"));
    tv = (texmult * getvalue( buf, "V:"));
   }
   Vertex[ NumVertices ].x = tx;
   Vertex[ NumVertices ].y = ty;
   Vertex[ NumVertices ].z = tz;
   Vertex[ NumVertices ].u = tu;
   Vertex[ NumVertices ].v = tv;
   if (DEBUG==TRUE) printf("Vertex %i:  X:%f Y:%f Z:%f U:%f V:%f\n",NumVertices,tx,ty,tz,tu,tv);
   NumVertices++;
  }

  if ( instr( buf, "Face " ) && !instr( buf, "list:" ) )
  {
   a = (int)getvalue( buf, "A:");
   b = (int)getvalue( buf, "B:");
   c = (int)getvalue( buf, "C:");
   Polygon[ NumPolys ].a = a;
   Polygon[ NumPolys ].b = b;
   Polygon[ NumPolys ].c = c;
   if (DEBUG==TRUE) printf("Face %i:  A:%i B:%i C:%i\n",NumPolys,a,b,c);
   NumPolys++;
  }

  if (instr( buf, "Material:" ))
  {
   getstring( buf, "Material:", &(Polygon[NumPolys-1].Material));
   if (DEBUG==TRUE) printf ("Material:%s attached to poly %i\n",Polygon[NumPolys-1].Material,NumPolys-1);
  }

  if (instr( buf, "Smoothing:" ))
  {
   Polygon[NumPolys-1].Smoothing = getvalue( buf, "Smoothing:");
   if (DEBUG==TRUE) printf ("Smoohing group: %i\n",Polygon[NumPolys-1].Smoothing);
  }
 }
 fclose(fp);
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// ± Save object file ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

/*

 file format:

 float BSphereRadius
 int NumVertices
 {
  float x,y,z
  int u,v
 }
 int NumGroups
 int NumPolys
 {
  int a,b,c
 }

*/

void SaveFile (char *s)
{
 FILE *fp;
 int itemp;
 float ftemp;
 int i;

 fp = fopen(outfile,"wb");
 itemp = OBJECT_HEADER;
 fwrite(&itemp,4,1,fp);
 fwrite(&Sphere,sizeof(float),1,fp);       // bounding sphere radius
 fwrite(&NumVertices,4,1,fp);
 for (i=0;i<NumVertices;i++)
 {
  fwrite( &Vertex[i].x, sizeof(float), 1, fp );
  fwrite( &Vertex[i].y, sizeof(float), 1, fp );
  fwrite( &Vertex[i].z, sizeof(float), 1, fp );
  fwrite( &Vertex[i].u, sizeof(float), 1, fp );
  fwrite( &Vertex[i].v, sizeof(float), 1, fp );
  // vertex normals
  fwrite( &VertexN[i].x, sizeof(float), 1, fp );
  fwrite( &VertexN[i].y, sizeof(float), 1, fp );
  fwrite( &VertexN[i].z, sizeof(float), 1, fp );

 }
 itemp = 1;
 fwrite(&itemp,4,1,fp);   // only 1 group object for now
 itemp = NumPolys;
 fwrite(&itemp,4,1,fp);   // Number of polys in group
 for (i=0;i<NumPolys;i++)
 {
  fwrite( &Polygon[i].a, 4, 1, fp );
  fwrite( &Polygon[i].b, 4, 1, fp );
  fwrite( &Polygon[i].c, 4, 1, fp );
  fwrite( &PolygonN[i].x, sizeof(float), 1, fp );
  fwrite( &PolygonN[i].y, sizeof(float), 1, fp );
  fwrite( &PolygonN[i].z, sizeof(float), 1, fp );
 }
}

// --------------------------------------------------------------------------

void FindLargestPoint(void)
{
 int i;

 LargestPoint = -32767;
 for (i=0;i<NumVertices;i++)
 {
  if (fabs(Vertex[i].x) > LargestPoint ) LargestPoint = fabs(Vertex[i].x);
  if (fabs(Vertex[i].y) > LargestPoint ) LargestPoint = fabs(Vertex[i].y);
  if (fabs(Vertex[i].z) > LargestPoint ) LargestPoint = fabs(Vertex[i].z);
 }
}

// ----------

void FindLargestTexPoint(void)
{
 int i;

 LargestTexPoint = -32767;
 for (i=0;i<NumVertices;i++)
 {
  if (fabs(Vertex[i].u) > LargestTexPoint ) LargestTexPoint = fabs(Vertex[i].u);
  if (fabs(Vertex[i].v) > LargestTexPoint ) LargestTexPoint = fabs(Vertex[i].v);
 }
}

// ==========================================================================

// --------------------------------------------------------------------------
// Calculate Face Normals
// --------------------------------------------------------------------------
void CalcFaceNormals(void)
{
 int i;
 float kx,ky,kz;
 float ax,ay,az;
 float bx,by,bz;
 float leng;

 for (i=0;i<NumPolys;i++)
 {
  ax = (Vertex[ Polygon[i].b].x - Vertex[ Polygon[i].a].x);
  ay = (Vertex[ Polygon[i].b].y - Vertex[ Polygon[i].a].y);
  az = (Vertex[ Polygon[i].b].z - Vertex[ Polygon[i].a].z);

  bx = (Vertex[ Polygon[i].c].x - Vertex[ Polygon[i].a].x);
  by = (Vertex[ Polygon[i].c].y - Vertex[ Polygon[i].a].y);
  bz = (Vertex[ Polygon[i].c].z - Vertex[ Polygon[i].a].z);

  kx = (ay*bz)-(az*by);
  ky = (az*bx)-(ax*bz);
  kz = (ax*by)-(ay*bx);
  leng = (sqrt( kx*kx + ky*ky + kz*kz));
  if (leng!=0)
  {
   PolygonN[i].x = (kx/leng) * facenmult;
   PolygonN[i].y = (ky/leng) * facenmult;
   PolygonN[i].z = (kz/leng) * facenmult;
  }
  else
  {
   PolygonN[i].x = 0;
   PolygonN[i].y = 0;
   PolygonN[i].z = 0;
  }
 }
}

// --------------------------------------------------------------------------
// Calculate Vertex Normals
// --------------------------------------------------------------------------
void CalcVertexNormals(void)
{
 int i,a,num;
 float ax,ay,az;
 float lengd;

 for (i=0;i<NumVertices;i++)
 {
  ax = 0;
  ay = 0;
  az = 0;
  num = 0;
  for (a=0;a<NumPolys;a++)
  {
   if ( (Polygon[a].a == i) || (Polygon[a].b == i) || (Polygon[a].c == i))
   {
    ax+= PolygonN[a].x;
    ay+= PolygonN[a].y;
    az+= PolygonN[a].z;
    num++;
   }
  }
  if ( num != 0 )
  {
   ax = ax / num;
   ay = ay / num;
   az = az / num;
  }
  lengd = sqrt( ax*ax + ay*ay + az*az);
  if ( lengd != 0)
  {
   VertexN[i].x = (ax/lengd) * vertnmult;
   VertexN[i].y = (ay/lengd) * vertnmult;
   VertexN[i].z = (az/lengd) * vertnmult;
  }
  else
  {
   VertexN[i].x = 0;
   VertexN[i].y = 0;
   VertexN[i].z = 0;
  }
 }
}

// --------------------------------------------------------------------------
// Center object around its average X/Y/Z pos
// --------------------------------------------------------------------------
void CenterObject(void)
{
 int xmax,xmin,ymax,ymin,zmax,zmin;
 int xmove, ymove, zmove;
 int i;
 char yn;

 xmax = -32767; xmin = 32767;
 ymax = -32767; ymin = 32767;
 zmax = -32767; zmin = 32767;
 for (i=0;i<NumVertices;i++)
 {
  if ( Vertex[i].x > xmax ) xmax = Vertex[i].x;
  if ( Vertex[i].x < xmin ) xmin = Vertex[i].x;
  if ( Vertex[i].y > ymax ) ymax = Vertex[i].y;
  if ( Vertex[i].y < ymin ) ymin = Vertex[i].y;
  if ( Vertex[i].z > zmax ) zmax = Vertex[i].z;
  if ( Vertex[i].z < zmin ) zmin = Vertex[i].z;
 }
 Sphere = xmax;
 if (ymax > Sphere) Sphere = ymax;
 if (zmax > Sphere) Sphere = zmax;
 if (fabs(xmin) > Sphere) Sphere = fabs(xmin);
 if (fabs(ymin) > Sphere) Sphere = fabs(ymin);
 if (fabs(zmin) > Sphere) Sphere = fabs(zmin);

 if (CENTER_OBJECT == 1)
 {
  xmove = xmin+((xmax-xmin) >> 1);
  ymove = ymin+((ymax-ymin) >> 1);
  zmove = zmin+((zmax-zmin) >> 1);
  for (i=0;i<NumVertices;i++)
  {
   Vertex[i].x-=xmove;
   Vertex[i].y-=ymove;
   Vertex[i].z-=zmove;
  }
 }

}


// ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²

void main( int argc, const char *argv[] )
{
 int i;

 printf ("\n");
 printf ("ASC converter v0.1á Axon / Inf.Xenon\n\n");

 if ( argc == 1 )
 {
  printf ("USAGE; CONV  [parameters]\n");
  printf ("Parameters (case sensitive):\n");
  printf ("  I $  Input .ASC file\n");
  printf ("  O $  Output file\n");
  printf ("  D    Debug info ON\n");
  printf ("  C    Center object around its own center (average x,y,z coords)\n");
  printf ("  V #  Scale vertices\n");
  printf ("  v #  Scale vertex normals\n");
  printf ("  v #  Scale face normals\n");
  printf ("  t #  Scale texture U/V coordinates\n");
 }
 else
 {
  Vertex   = (sVertex  *)malloc( MAXVERTICES*sizeof(sVertex  ));
  Polygon  = (sPolygon *)malloc( MAXPOLYGONS*sizeof(sPolygon ));
  VertexN  = (sNormal  *)malloc( MAXVERTICES*sizeof(sNormal  ));
  PolygonN = (sNormal  *)malloc( MAXPOLYGONS*sizeof(sNormal  ));

// Set up some default values

  DEBUG = FALSE;
  CENTER_OBJECT = FALSE;

  vertmult = 1.0;
  vertnmult = 1.0;
  facenmult =1.0;
  texmult = 1.0;

  infile_defined = FALSE;
  outfile_defined = FALSE;

  for (i=1;i<argc;i++)
  {
   switch (argv[i][0])
   {
    case 'I' : infile = argv[i+1];
               infile_defined = TRUE;
               i++;
               printf("Input file: %s\n",infile);
               break;
    case 'O' : outfile = argv[i+1];
               outfile_defined = TRUE;
               i++;
               printf("Output file: %s\n",outfile);
               break;
    case 'D' : DEBUG = TRUE;
               break;
    case 'C' : CENTER_OBJECT = TRUE;
               printf("Object(s) will be centered\n");
               break;
    case 'V' : vertmult = strtod(argv[i+1],&dummy);
               i++;
               printf("Vertex multiplier: %f\n",vertmult);
               break;
    case 'v' : vertnmult = strtod(argv[i+1],&dummy);
               i++;
               printf("Vertex Normal multiplier: %f\n",vertnmult);
               break;
    case 'f' : facenmult = strtod(argv[i+1],&dummy);
               i++;
               printf("Face Normal multiplier: %f\n",facenmult);
               break;
    case 't' : texmult = strtod(argv[i+1],&dummy);
               i++;
               printf("Texture U/V multiplier: %f\n",texmult);
               break;

    break;
   }
  }
  printf("\nLoading: %s\n",infile);
  LoadAsc(infile);
  printf("Vertices: %i   Faces: %i\n",NumVertices,NumPolys);
  CenterObject();
  FindLargestPoint();
  FindLargestTexPoint();
  CalcFaceNormals();
  CalcVertexNormals();
  printf("Largest ABS(vertex coordinate): %f\n",LargestPoint);
  printf("Largest ABS(texture coordinate): %f\n",LargestTexPoint);
  printf("Bounding Sphere radius: %f\n",Sphere);
  SaveFile(outfile);

  free( Vertex );
  free( Polygon );
  free( VertexN );
  free( PolygonN );

 }
}


