// 3D engine....

#include "defines.h"
#include "types.h"
#include "math3d.h"
#include "spline.h"
#include "bytesort.h"
#include "api.h"
#include "texmap.h"

#ifdef VisCVersion
 #pragma warning (once:4035)
 #pragma warning (once:4244)
#endif

// ------------------------
// Global variables... :-/
// ------------------------

float omatrix[16];  // object matrix
float cmatrix[16];  // camera matrix
float ematrix[16];  // eye/viewpoint marix
float mat[16];      // temporary

// The following should be allocated run-time instead of wasting space here...
int tricoords[MAXCOORDS*2];
int texcoords[MAXCOORDS*2];
int rotated[MAXCOORDS*3];
int rotatedfacenormals[MAXPOLYS*3];
int rotatedvertexnormals[MAXCOORDS*3];

int numvisiblepolys;     // Number of visible polys
int numvisibleobjects;   // Number of visible objects
int polysort[MAXPOLYS];  // Initial poly-list before sorting, and after
int objsort[MAXOBJECTS]; // same, but for objects

// coordinates for moving lightsource... Position of light-center in phongmap
int lightx;
int lighty;

// -----

void CopyVertexNormals(objstruc *o)
{
 int i;
 int *from = o->tex_c;
 for(i=0;i<o->numcoords;i++)
 {
	texcoords[i*2]   = from[i*2];
  texcoords[i*2+1] = from[i*2+1];
 }
}

// -----

// ----------------------
// project vertex normals
// ----------------------
void ProjectVertexNormals (objstruc *o)
{
 int i;
 int xt,yt;
 for(i=0;i<o->numcoords;i++)
 {
  xt =  rotatedvertexnormals[i*3];
	yt =  rotatedvertexnormals[i*3+1];
  texcoords[i*2]   = xt+lightx;
  texcoords[i*2+1] = yt+lighty;
 }
}

// --------------------------------------------------
// project coordinates 3D (rotated) -> 2D (tricoords)
// --------------------------------------------------
void ProjectCoords (objstruc *o)
{
 int i;
 // int *from = &rotated[0];
 int xt,yt,zt;
 for(i=0;i<o->numcoords;i++)
 {
  xt =  rotated[i*3];
	yt =  rotated[i*3+1];
	zt =  rotated[i*3+2];
  if (zt <= 0)
	{
   zcoords[i] = -1;
	}
  else
	{
   zcoords[i] = zt;
	 zt += DISTANCE;
   tricoords[i*2]   = ((xt << PROJ_PARAM) / (zt)) + 128;
   tricoords[i*2+1] = ((yt << PROJ_PARAM) / (zt)) + 128;
  }
 }
}

// -----

// nv = number of visible polygons
// table = sorted tablr (from bytesort)
// tri -> triangle coordinates
// tex -> texture coordinates
void Object_DrawTexture(screen_S *vp,objstruc *o,int nv, int *table, int *tri, int *tex)
{
// struct screen_S * vp = &buffer;
 int i;
 unsigned int polynum;
 int c1,c2,c3;
// for (i=0;i<nv;i++)
 for (i=nv-1;i>=0;i--)
 {
  polynum = ((unsigned int)(table[i]) >> 16/*.data*/);         // polynum = # of polygon to draw
  c1 = o->polys[polynum*3];
  c2 = o->polys[polynum*3+1];
  c3 = o->polys[polynum*3+2];
  TexMap(vp,c1,c2,c3,tri,tex,o->texmap);
 }
}

// -----

void Object_Project(objstruc *o, camstruc *c)
{
 Matrix_Rotation(o->m,o->xang,o->yang,o->zang);
 Matrix_Copy(o->e,o->m);
 o->e[3*4+0] += o->xpos;
 o->e[3*4+1] += o->ypos;
 o->e[3*4+2] += o->zpos;
 Matrix_Mult(o->e,c->m);
}

// -----

void Object_Draw(screen_S *vp,objstruc *o)
{
 Matrix_Process(o->e,o->coords,rotated,o->numcoords);
 ProjectCoords(o);
 switch(o->rottype)
 {
	case 0 : Matrix_Process(o->m,o->vertex_n,rotatedvertexnormals,o->numcoords);
           ProjectVertexNormals(o);
					 break;
	case 1 : //CopyVertexNormals(o);
					 break;
 }
 numvisiblepolys = polysortsetup(o,polysort,tricoords);
 bytesort(numvisiblepolys,polysort);
 switch(o->shadetype)
 {
	case 0 : Object_DrawTexture(vp,o,numvisiblepolys,polysort,tricoords,texcoords);
	         break;
	case 1 : Object_DrawTexture(vp,o,numvisiblepolys,polysort,tricoords,o->tex_c);
	         break;
 }
}

// -----

void Camera_Process_Aim(camstruc *c)
{
// int mm[16];
 // Matrix_Translation(c->m,-(c->xpos),-(c->ypos),-(c->zpos));
 // create_camera_matrix(c,c->m);
 // Matrix_Rotation(mm,-(c->xang),-(c->yang),-(c->zang));
 // Matrix_Mult(c->m,mm);
 Camera_Create_Lookat(c,c->m/*mm*/);
}

void Camera_Process_Free(camstruc *c)
{
 float mm[16];
 Matrix_Translation(c->m,-(c->xpos),-(c->ypos),-(c->zpos));
 Matrix_Rotation(mm,-(c->xang),-(c->yang),-(c->zang));
 Matrix_Mult(c->m,mm);
}


// -----

void Check_BoundingBox(screen_S *vp,objstruc *o, camstruc *c)
{
 int i;
 int x,y,z;
 int b[8*3];
 int numvis;

 Object_Project(o,c);		 // init matrices
 Matrix_Process(o->e,o->bbox,b,8);
 numvis = 8;
 for (i=0;i<8;i++)
 {
	z = b[i*3+1];
  if (z <= 0)
	{
	 z = 1;
	}
	else
	{
   z += DISTANCE;
	}
  x = ((b[i*3+0] << PROJ_PARAM) / (z)) + 128;
  y = ((b[i*3+1] << PROJ_PARAM) / (z)) + 128;
	if ( ( (unsigned int)x > 319 ) &&
       ( (unsigned int)y > 199 ) &&
			 ( (unsigned int)z > 1000)) numvis--;
 }
 if (numvis != 0)	Object_Draw(vp,o);
}

// -----

void Anim_Init(objstruc *o1,objstruc *o2,objstruc *o3, morphstruc *os)
{
 int i;
 for (i=0;i<o1->numcoords;i++)
 {
	os->s[i].p[0].x = o1->coords[i*3];
	os->s[i].p[0].y = o1->coords[i*3+1];
	os->s[i].p[0].z = o1->coords[i*3+2];
	os->s[i].p[1].x = o2->coords[i*3];
	os->s[i].p[1].y = o2->coords[i*3+1];
	os->s[i].p[1].z = o2->coords[i*3+2];
	os->s[i].p[2].x = o3->coords[i*3];
	os->s[i].p[2].y = o3->coords[i*3+1];
	os->s[i].p[2].z = o3->coords[i*3+2];
  os->s[i].num = 3;
  os->s[i].oClosed = 1;
  os->s[i].bendsize = 1.0;
 }
}

// -----

void Anim_Update(objstruc *anim, int objloc, morphstruc *os)
{
 int i;
 coord objpos;
 objloc&=65535;
 for (i=0;i<anim->numcoords;i++)
 {
	objpos = get_spline_point(objloc, &os->s[i]);
  anim->coords[i*3]   = objpos.x;
  anim->coords[i*3+1] = objpos.y;
  anim->coords[i*3+2] = objpos.z;
	// Re-calculate Face & Vertex normals here :-/
 }
}

