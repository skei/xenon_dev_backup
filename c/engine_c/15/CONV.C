// 3DS/ASC -> A3D converter

#include <stdlib.h>   // exit
#include <stdio.h>    // file functions
#include <string.h>
#include <math.h>

typedef unsigned int bool;

#define TRUE 1
#define FALSE 0

#define FILE_HEADER 0x33444f00       // '3DO#0'

#define MAXVERTICES 10000
#define MAXPOLYGONS 10000

#define QUOTE '\"'

// 컴컴� Global variables 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

typedef struct
{
 short int x,y,z;
 char u,v;
} sVertex;

// -----

typedef struct
{
 short int x,y,z;
} sNormal;

typedef struct
{
 short int a,b,c;
} sPolygon;

sVertex  *Vertex;
sPolygon *Polygon;

sNormal *VertexN;
sNormal *PolygonN;

int NumVertices;
int NumPolys;
int Sphere;

int CENTER_OBJECT;
int INCLUDE_TEX;
int INCLUDE_V_NORM;
int INCLUDE_F_NORM;


int LargestPoint;
int LargestTexPoint;


// ----- variables used during loading --------------------------------------

char objname[256];
int Mapped;
char *dummy;

double vertmult;    /* Multiplier for Vertex coordinates */
double vertnmult;		/* Multiplier for Vertex normals */
double facenmult;		/* Multiplier for Face normals */
double texmult;

char *infile, *outfile;

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

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

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

/* ------------- */
/* Load ASC file */
/* ------------- */
void readfile(char *s)
{
 FILE *fp;
 char buf[256];
 int tx,ty,tz,tu,tv;

 Mapped = 0;
 NumPolys = 0;
 NumVertices = 0;
 LargestPoint = -32767;
 LargestTexPoint = -32767;

 fp = fopen (s,"rt");
// Read object name
 while( !instr(buf,"Named object:"))
 {
  fgets ( buf, 255, fp );
 }
 getstring( buf,"Named object:",objname );
 printf("Object found: %s\n",objname);
 while ( !feof( fp ) )
 {
  fgets ( buf, 255, fp );
	if ( instr( buf, "Mapped")) Mapped = 1;
  if ( instr( buf, "Vertex " ) && !instr( buf, "list:" ) )
  {
   tx = (int)(vertmult * getvalue( buf, "X:"));
   ty = (int)(vertmult * getvalue( buf, "Y:"));
   tz = (int)(vertmult * getvalue( buf, "Z:"));
   if (Mapped==1)
   {
	  tu = (int)(texmult * getvalue( buf, "U:"));
    tv = (int)(texmult * getvalue( buf, "V:"));
   }
//   printf("Vertex %i:  X:%f Y:%f Z:%f U:%f V:%f\n",vertexnum,tx,ty,tz,tu,tv);
   if (abs(tx) >  LargestPoint ) LargestPoint = abs(tx);
   if (abs(ty) >  LargestPoint ) LargestPoint = abs(ty);
   if (abs(tz) >  LargestPoint ) LargestPoint = abs(tz);
   if (abs(tu) >  LargestTexPoint ) LargestTexPoint = abs(tu);
   if (abs(tv) >  LargestTexPoint ) LargestTexPoint = abs(tv);

   Vertex[ NumVertices ].x = tx;
   Vertex[ NumVertices ].y = ty;
   Vertex[ NumVertices ].z = tz;
   Vertex[ NumVertices ].u = tu;
   Vertex[ NumVertices ].v = tv;
   NumVertices++;
  }
  if ( instr( buf, "Face " ) && !instr( buf, "list:" ) )
  {
   Polygon[ NumPolys ].a = (int)getvalue( buf, "A:");
   Polygon[ NumPolys ].b = (int)getvalue( buf, "B:");
   Polygon[ NumPolys ].c = (int)getvalue( buf, "C:");
//   Polygon[ NumPolys ].ab = (int)getvalue( buf, "AB:");
//   Polygon[ NumPolys ].bc = (int)getvalue( buf, "BC:");
//   Polygon[ NumPolys ].ca = (int)getvalue( buf, "CA:");
   NumPolys++;
  }
 }
 fclose(fp);
}

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

/*
3DO file format

 char header[4]     // '3DO'+#0
 int  NumObjs       // Number of objects in file (ignored)
 int  Sphere        // bounding sphere radius
 int  NumVertices   // Number of vertices in object
 {
  short int v.x
  short int v.y
  short int v.z
 }
 int  NumTEXVert    // Number of tex vertices (U/V)
 {
  char u
  char v
 }
 int  NumVertNormals  // Number of vertex normals in object
 {
  short int vn.x
  short int vn.y
  short int vn.z
 }

 int NumGroups      // Number of groups in object (ignored, only 1 used)
 {
  int RenderMethod  // Group's render method
  int NumPolys      // Number of polygons in group
  {
   short int a
   short int b
   short int c
  }
 }
*/

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void Save( char *filename)
{
 FILE *fp;
 int header = FILE_HEADER;
 int temp;
 short int stemp;
 int i,j,k;

 fp = fopen(filename,"wb");

 fwrite(&header,4,1,fp);        // header
 temp = 1;
 fwrite(&temp,4,1,fp);          // number of objects in file (ignored)
 fwrite(&Sphere,4,1,fp);        // bounding box radius
 fwrite(&NumVertices,4,1,fp);   // number of vertices in object
 for (j=0;j<NumVertices;j++)
 {
  fwrite(&Vertex[j].x,2,1,fp);
  fwrite(&Vertex[j].y,2,1,fp);
  fwrite(&Vertex[j].z,2,1,fp);
 }

 if (INCLUDE_TEX == 1)
 {
  fwrite(&NumVertices,4,1,fp);  // Number of U/V's ...
  for (j=0;j<NumVertices;j++)
  {
   fwrite(&Vertex[j].u,1,1,fp);
   fwrite(&Vertex[j].v,1,1,fp);
  }
 }
 else
 {
  temp = 0;
  fwrite(&temp,4,1,fp);         // ...  or zero to skip
 }

 if (INCLUDE_V_NORM == 1)
 {
  fwrite(&NumVertices,4,1,fp);  // Number of Vertex normals
  for (j=0;j<NumVertices;j++)
  {
   fwrite(&VertexN[j].x,2,1,fp);
   fwrite(&VertexN[j].y,2,1,fp);
   fwrite(&VertexN[j].z,2,1,fp);
  }
 }
 else
 {
  temp = 0;
  fwrite(&temp,4,1,fp);         // ...  or zero to skip
 }


 temp = 1;
 fwrite(&temp,4,1,fp);          // number of groups in object (ignored)
 temp = 1;
 fwrite(&temp,4,1,fp);          // render method
 fwrite(&NumPolys,4,1,fp);      // Number of polygons
 for (j=0;j<NumPolys;j++)
 {
  fwrite(&Polygon[j].a,2,1,fp);
  fwrite(&Polygon[j].b,2,1,fp);
  fwrite(&Polygon[j].c,2,1,fp);
 }
 close(fp);
}

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

// --------------------------------------------------------------------------
// Calculate Face Normals
// --------------------------------------------------------------------------
void CalcFaceNormals(void)
{
 int i;
 int kx,ky,kz;
 int ax,ay,az;
 int bx,by,bz;
 double leng;

 for (i=0;i<NumPolys;i++)
 {
  ax = (int)(Vertex[ Polygon[i].b].x - Vertex[ Polygon[i].a].x);
  ay = (int)(Vertex[ Polygon[i].b].y - Vertex[ Polygon[i].a].y);
  az = (int)(Vertex[ Polygon[i].b].z - Vertex[ Polygon[i].a].z);

  bx = (int)(Vertex[ Polygon[i].c].x - Vertex[ Polygon[i].a].x);
  by = (int)(Vertex[ Polygon[i].c].y - Vertex[ Polygon[i].a].y);
  bz = (int)(Vertex[ Polygon[i].c].z - Vertex[ Polygon[i].a].z);

  kx = (ay*bz)-(az*by);
  ky = (az*bx)-(ax*bz);
  kz = (ax*by)-(ay*bx);
  leng = (sqrt( (double)kx*(double)kx + (double)ky*(double)ky + (double)kz*(double)kz));
  if (leng!=0)
  {
   PolygonN[i].x = (int)( (double)kx/leng * facenmult ) ;
   PolygonN[i].y = (int)( (double)ky/leng * facenmult ) ;
   PolygonN[i].z = (int)( (double)kz/leng * facenmult ) ;
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
 int ax,ay,az;
 double lengd;

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
   VertexN[i].x = (int)( ( (double)ax/lengd ) * vertnmult ) ;
   VertexN[i].y = (int)( ( (double)ay/lengd ) * vertnmult ) ;
   VertexN[i].z = (int)( ( (double)az/lengd ) * vertnmult ) ;
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

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void process(void)
{
 CenterObject();
 CalcFaceNormals();
 CalcVertexNormals();
}

// 같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같

void main( int argc, const char *argv[] )
{
 char yn;

 if ( argc != 7 )
 {
  printf ("ASC -> 3DO converter v0... Axon / Inf.Xenon\n");
  printf ("USAGE; CONV <IN-file> <OUT-file> VM VNM FNM TM\n");
  printf ("VM   = Vertex Multiplier\n");
  printf ("VNM  = Vertex Normal Multiplier\n");
  printf ("FNM  = Face Normal Multiplier\n");
  printf ("TM   = Texture Multiplier\n");
 }
 else
 {
  infile = argv[1];
  outfile = argv[2];
  vertmult = strtod(argv[3],&dummy);
  vertnmult = strtod(argv[4],&dummy);
  facenmult = strtod(argv[5],&dummy);
  texmult = strtod(argv[6],&dummy);

  Vertex  = (sVertex  *)malloc(MAXVERTICES*sizeof(sVertex ));
  Polygon = (sPolygon *)malloc(MAXPOLYGONS*sizeof(sPolygon));
  VertexN  = (sNormal *)malloc(MAXVERTICES*sizeof(sNormal));
  PolygonN = (sNormal *)malloc(MAXPOLYGONS*sizeof(sNormal));

  printf("Loading: %s\n",infile);
  readfile(infile);

  printf("Vertices: %i   Faces: %i\n",NumVertices,NumPolys);
  printf("Largest ABS(vertex coordinate): %i\n",LargestPoint);
  if (Mapped == 1)
  {
   printf("Largest ABS(Texture coordinate): %i\n",LargestTexPoint);
  }
  else
  {
   printf("Object is not mapped...\n");
  }

 CENTER_OBJECT = 0;
 printf ("Center object ? (Y/N) : ");
 yn = getch();
 if (toupper(yn) == 'Y')
 {
  printf("Y\n");
  CENTER_OBJECT = 1;
 } else printf("N\n");

 INCLUDE_TEX = 0;
 if (Mapped == 1)
 {
  printf ("Include texture coordinates? (Y/N) : ");
  yn = getch();
  if (toupper(yn) == 'Y')
  {
   printf("Y\n");
   INCLUDE_TEX = 1;
  } else printf("N\n");
 }

 INCLUDE_V_NORM = 0;
 printf ("Include Vertex Normals? (Y/N) : ");
 yn = getch();
 if (toupper(yn) == 'Y')
 {
  printf("Y\n");
  INCLUDE_V_NORM = 1;
 } else printf("N\n");

 printf("\nProcessing...\n");
 process();

 printf("\nSaving: %s\n",outfile);
 save(outfile);

 free(Vertex);
 free(Polygon);
 free(VertexN);
 free(PolygonN);
 }
}
