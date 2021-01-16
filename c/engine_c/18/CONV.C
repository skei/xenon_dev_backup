/***********************************************/
/* Tor-Helge Skei                              */
/* ASC -> WinTexture3D converter               */
/***********************************************/

typedef unsigned int bool;
#define FALSE 0
#define TRUE 1

#define QUOTE_CHAR '\"'

#define MAXVERTEX 10000
#define MAXFACE 10000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
//#include "funcom.h"

/**************/
/* Structures */
/**************/

struct vertexstruc
{
 int x,y,z,u,v;
};

struct facestruc
{
 int a,b,c,ab,bc,ca;
 char material[256];
};

/*************/
/* Variables */
/*************/

struct vertexstruc vertex[MAXVERTEX];		/* array for vertex data */
struct vertexstruc n_vertex[MAXVERTEX];		/* array for vertex normals */
struct facestruc face[MAXFACE];				/* array for face data */
struct vertexstruc n_face[MAXFACE];			/* array for face normals */

int facenum;							/* Number of faces in ASC file */
int vertexnum;							/* Number of Vertices */

double vertmult;		/* Multiplier for Vertex coordinates */
double vertnmult;		/* Multiplier for Vertex normals */
double facenmult;		/* Multiplier for Face normals */
double texmult;

int px,py,pw,ph;
int width,height;
double u_mul, v_mul;
char mat1[], mat2[];
char *infile, *outfile;
char *objectname;
char objname[256];
int Mapped;

/* */

char *dummy;
char *tmpstr;

int LargestPoint,LargestTexPoint;

/*************/
/* Functions */
/*************/

/* ----------------------------------------------------- */
/* returns TRUE if the string s contains the substring v */
/* ----------------------------------------------------- */
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

/* ---------------------------------------------------- */
/* Returning the value following V, converted to double */
/* ---------------------------------------------------- */
double getvalue ( char *s, char *v )
{
 char *pos;
 double x;
 pos = strstr( s, v );				/* pos -> 1st occurence of V in S */
 pos+=strlen(v);					/* Skip V */
 x = strtod( pos, &dummy );			/* x = convert following string to double */
 return x;
}

/* ------------------------------------------ */
/* Returning the string (in ""'s) following V */
/* ------------------------------------------ */
void getstring( char *s, char *v, char *ret )
{
 char *pos;
 pos = strstr( s, v );				/* pos -> 1st occurence of V in S */
 pos+=strlen(v);					/* Skip V */
 while ( *pos != QUOTE_CHAR ) pos++;
 while ( *pos == QUOTE_CHAR ) pos++;
 while ( *pos != QUOTE_CHAR )
 {
  *ret++ = *pos++;
 }
 *ret = '\0';
}

/* ------------- */
/* Load ASC file */
/* ------------- */
void readfile(char *s)
{
 FILE *fp;
 char buf[256];
 int tx,ty,tz,tu,tv;

 Mapped = 0;
 facenum = 0;
 vertexnum = 0;
 LargestPoint = -32767;
 LargestTexPoint = -32767;

 fp = fopen (s,"rt");
/* Read object name */
 while( !instr(buf,"Named object:"))
 {
  fgets ( buf, 255, fp );
 }
  getstring( buf,"Named object:",objname );

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

   vertex[ vertexnum ].x = tx;
   vertex[ vertexnum ].y = ty;
   vertex[ vertexnum ].z = tz;
   vertex[ vertexnum ].u = tu;
   vertex[ vertexnum ].v = tv;



   vertexnum++;
  }
  if ( instr( buf, "Face " ) && !instr( buf, "list:" ) )
  {
   face[ facenum ].a = (int)getvalue( buf, "A:");
   face[ facenum ].b = (int)getvalue( buf, "B:");
   face[ facenum ].c = (int)getvalue( buf, "C:");
   face[ facenum ].ab = (int)getvalue( buf, "AB:");
   face[ facenum ].bc = (int)getvalue( buf, "BC:");
   face[ facenum ].ca = (int)getvalue( buf, "CA:");
/*
   while (!instr(buf,"Material:"))
   {
    fgets ( buf, 255, fp );
   }
   getstring(buf, "Material:", face[ facenum ].material );
*/
/*   printf("Face %i:    A:%i B:%i C:%i AB:%i BC:%1 CA:%i",facenum
										,face[facenum].a,face[facenum].b,face[facenum].c
										,face[facenum].ab,face[facenum].bc,face[facenum].ca); */
/*   printf("   Material:\"%s\"\n",face[facenum].material); */
   facenum++;
  }
 }
 fclose(fp);
}

/* ----------------- */
/* Save new ASC file */
/* ----------------- */
void writefile(char *s)
{
 FILE *fp;
 int i;

 fp = fopen (s,"wt");
 fprintf(fp,"Named object: \"%s\"\n",objname);
 fprintf(fp,"Tri-mesh, Vertices: %i     Faces: %i\n",vertexnum,facenum);
 fprintf(fp,"Mapped\n");
 fprintf(fp,"Vertex list:\n");
 for (i=0;i<vertexnum;i++)
 {
  fprintf(fp,"Vertex %i:  X:%f     Y:%f     Z:%f     U:%f     V:%f\n",i,vertex[i].x,vertex[i].y,vertex[i].z
																	 ,n_vertex[i].u,n_vertex[i].v);
 }
 fprintf(fp,"Face list:\n");
 for (i=0;i<facenum;i++)
 {
  fprintf(fp,"Face %i:    A:%i B:%i C:%i AB:%i BC:%1 CA:%i\n",i,face[i].a,face[i].b,face[i].c
    		    							              	 ,face[i].ab,face[i].bc,face[i].ca);
  fprintf(fp,"Material:\"%s\"\n",face[i].material);
 }
 fclose(fp);
}

/**********************/
/* Save Include file  */
/**********************/
void writeincfile(char *s)
{
 FILE *fp;
 int i;

 fp = fopen (s,"wt");
 fprintf(fp,"/* Object  : %s*/\n",objname);
 fprintf(fp,"/* Vertices: %i*/\n",vertexnum);
 fprintf(fp,"/* Faces   : %i*/\n",facenum);

 /* Save Vertex data (X,Y,Z) */
 fprintf(fp,"int %s_cor[] = \n{\n",objectname);
 for (i=0;i<(vertexnum-1);i++)
 {
  fprintf(fp,"%i,%i,%i,\n",(int)vertex[i].x,(int)vertex[i].y,(int)vertex[i].z);
 }
 fprintf(fp,"%i,%i,%i\n};\n",(int)vertex[vertexnum-1].x,(int)vertex[vertexnum-1].y,(int)vertex[vertexnum-1].z);

 /* Save Face data (A,B,C,color) */
 fprintf(fp,"int %s_pol[] = \n{\n",objectname);
  for (i=0;i<(facenum-1);i++)
 {
  fprintf(fp,"%i,%i,%i,\n",(int)face[i].a,(int)face[i].b,(int)face[i].c);
 }
 fprintf(fp,"%i,%i,%i\n};\n",(int)face[facenum-1].a,(int)face[facenum-1].b,(int)face[facenum-1].c);

 /* Save Face Normals  (X,Y,Z) */
 fprintf(fp,"int %s_fnor[] = \n{\n",objectname);
  for (i=0;i<(facenum-1);i++)
 {
  fprintf(fp,"%i,%i,%i,\n",(int)n_face[i].x,(int)n_face[i].y,(int)n_face[i].z);
 }
 fprintf(fp,"%i,%i,%i\n};\n",(int)n_face[facenum-1].x,(int)n_face[facenum-1].y,(int)n_face[facenum-1].z);

 /* Save vertex Normals  (X,Y,Z) */
 fprintf(fp,"int %s_vnor[] = \n{\n",objectname);
  for (i=0;i<(vertexnum-1);i++)
 {
  fprintf(fp,"%i,%i,%i,\n",(int)n_vertex[i].x,(int)n_vertex[i].y,(int)n_vertex[i].z);
 }
 fprintf(fp,"%i,%i,%i\n};\n",(int)n_vertex[vertexnum-1].x,(int)n_vertex[vertexnum-1].y,(int)n_vertex[vertexnum-1].z);

/* Save Texture Coordinates  (U,V) */
 fprintf(fp,"int %s_texc[] = \n{\n",objectname);
  for (i=0;i<(vertexnum-1);i++)
 {
  fprintf(fp,"%i,%i,\n",(int)vertex[i].u,(int)vertex[i].v);
 }
 fprintf(fp,"%i,%i\n};\n",(int)vertex[vertexnum-1].u,(int)vertex[vertexnum-1].v);

 fprintf(fp,"\nstruct objstruc %s =\n{\n",objectname);
 fprintf(fp," 1,                 /* status */\n");
 fprintf(fp," 0,                 /* next */\n");
 fprintf(fp," 0,                 /* previous */\n");
 fprintf(fp," 0,                 /* rotation type */\n");
 fprintf(fp," 0,                 /* shading type */\n");
 fprintf(fp," 0,0,0,             /* x/y/z-pos */\n");
 fprintf(fp," 0,0,0,             /* x/y/z-angle */\n");
 fprintf(fp," 0,0,0,0,0,0,0,0,   /* matrix 1:2 */\n");
 fprintf(fp," 0,0,0,0,0,0,0,0,   /* matrix 2:2 */\n");
 fprintf(fp," 0,0,0,             /* matrix const's */\n");
 fprintf(fp," 0,0,0,0,0,0,       /* bounding box */\n");
 fprintf(fp," %i,                /* num.coords */\n",vertexnum);
 fprintf(fp," %i,                /* num.faces */\n",facenum);
 fprintf(fp," &%s_cor[0],        /* -> coords */\n",objectname);
 fprintf(fp," &%s_pol[0],        /* -> polys */\n",objectname);
 fprintf(fp," &%s_vnor[0],       /* -> vertex normals */\n",objectname);
 fprintf(fp," &%s_fnor[0],       /* -> face normals */\n",objectname);
 fprintf(fp," 0,                 /* -> phong map */\n");
 fprintf(fp," 0,                 /* -> bump coords */\n");
 fprintf(fp," 0,                 /* -> bump map */\n");
 fprintf(fp," &%s_texc[0],       /* -> texture coords */\n",objectname);
 fprintf(fp," 0                  /* -> testure map */\n");
 fprintf(fp,"};\n\n");
}

// Calculate Face Normals

void CalcFaceNormals(void)
{
 int i;
 int kx,ky,kz;
 int ax,ay,az;
 int bx,by,bz;
 double leng;

 for (i=0;i<facenum;i++)
 {
//ax:= points^[faces^[i,1],0]-points^[faces^[i,0],0];
//ay:=points^[faces^[i,1],1]-points^[faces^[i,0],1];
//az:=points^[faces^[i,1],2]-points^[faces^[i,0],2];
  ax = (int)(vertex[ face[i].b].x - vertex[ face[i].a].x);
  ay = (int)(vertex[ face[i].b].y - vertex[ face[i].a].y);
  az = (int)(vertex[ face[i].b].z - vertex[ face[i].a].z);
//bx:=points^[faces^[i,2],0]-points^[faces^[i,0],0];
//by:=points^[faces^[i,2],1]-points^[faces^[i,0],1];
//bz:=points^[faces^[i,2],2]-points^[faces^[i,0],2];
  bx = (int)(vertex[ face[i].c].x - vertex[ face[i].a].x);
  by = (int)(vertex[ face[i].c].y - vertex[ face[i].a].y);
  bz = (int)(vertex[ face[i].c].z - vertex[ face[i].a].z);

  kx = (ay*bz)-(az*by);
  ky = (az*bx)-(ax*bz);
  kz = (ax*by)-(ay*bx);
  leng = (sqrt( (double)kx*(double)kx + (double)ky*(double)ky + (double)kz*(double)kz));
  if (leng!=0)
  {
   n_face[i].x = (int)( (double)kx/leng * facenmult ) ;
   n_face[i].y = (int)( (double)ky/leng * facenmult ) ;
   n_face[i].z = (int)( (double)kz/leng * facenmult ) ;
  }
  else
  {
   n_face[i].x = 0;
   n_face[i].y = 0;
   n_face[i].z = 0;
  }
 }
}

// Calculate Vertex Normals

void CalcVertexNormals(void)
{
 int i,a,num;
 int ax,ay,az;
 double lengd;

 for (i=0;i<vertexnum;i++)
 {
  ax = 0;
  ay = 0;
  az = 0;
  num = 0;
  for (a=0;a<facenum;a++)
  {
   if ( (face[a].a == i) || (face[a].b == i) || (face[a].c == i))
   {
    ax+= n_face[a].x;
    ay+= n_face[a].y;
    az+= n_face[a].z;
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
   n_vertex[i].x = (int)( ( (double)ax/lengd ) * vertnmult ) ;
   n_vertex[i].y = (int)( ( (double)ay/lengd ) * vertnmult ) ;
   n_vertex[i].z = (int)( ( (double)az/lengd ) * vertnmult ) ;
// fnormals^[i,1]:=   round( ( ay/lengd ) * EnMult ) ;
// texcoords^[i,1] := round(((fnormals^[i,1] * TexMul) / EnMult) + TexMul);
  }
  else
  {
   n_vertex[i].x = 0;
   n_vertex[i].y = 0;
   n_vertex[i].z = 0;
  }
 }
}

void CenterObject(void)
{
 int xmax,xmin,ymax,ymin,zmax,zmin;
 int xmove, ymove, zmove;
 int i;
 char yn;

 xmax = -32767; xmin = 32767;
 ymax = -32767; ymin = 32767;
 zmax = -32767; zmin = 32767;
 for (i=0;i<vertexnum;i++)
 {
  if ( vertex[i].x > xmax ) xmax = vertex[i].x;
  if ( vertex[i].x < xmin ) xmin = vertex[i].x;
  if ( vertex[i].y > ymax ) ymax = vertex[i].y;
  if ( vertex[i].y < ymin ) ymin = vertex[i].y;
  if ( vertex[i].z > zmax ) zmax = vertex[i].z;
  if ( vertex[i].z < zmin ) zmin = vertex[i].z;
 }
 printf ("Center object ? (Y/N)\n");
 yn = getch();
 if (toupper(yn) == 'Y')
 {
  xmove = xmin+((xmax-xmin) >> 1);
  ymove = ymin+((ymax-ymin) >> 1);
  zmove = zmin+((zmax-zmin) >> 1);
  for (i=0;i<vertexnum;i++)
  {
   vertex[i].x-=xmove;
   vertex[i].y-=ymove;
   vertex[i].z-=zmove;
  }
 }
}


void process(void)
{
 CenterObject();
 CalcFaceNormals();
 CalcVertexNormals();
}

/********/
/* MAIN */
/********/

void main( int argc, char *argv[ ], char *envp[ ] )
{
 if ( argc != 8 )
 {
  printf ("ASC converter v0.2... Tor-Helge Skei\n");
  printf ("USAGE; EXE_NAME <IN-file> <OUT-file> VM VNM FNM TM objname\n");
  printf ("\n");
  printf ("VM      = Vertex/coordinate Multiplier\n");
  printf ("VNM     = Vertex Normal multiplier\n");
  printf ("FNM     = Face Normal multiplier\n");
  printf ("TM      = Texture multiplier\n");
  printf ("objname = name of object (in structure\n");
 }
 else
 {
  infile = argv[1];
  outfile = argv[2];
  vertmult = strtod(argv[3],&dummy);
  vertnmult = strtod(argv[4],&dummy);
  facenmult = strtod(argv[5],&dummy);
  texmult = strtod(argv[6],&dummy);
  objectname = argv[7];


  printf("Loading: %s\n",infile);
  readfile(infile);
  printf("Vertices: %i   Faces: %i\n",vertexnum,facenum);
  printf("Largest ABS(vertex coordinate): %i\n",LargestPoint);
  printf("Largest ABS(Texture coordinate): %i\n",LargestTexPoint);
  printf("Processing...");
  process();
  printf("Saving: %s\n",outfile);
  writeincfile(outfile);
  /*  writefile(outfile); */
 }
}

