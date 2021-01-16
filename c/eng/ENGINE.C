#include "types.h"
#include "vars.h"
#include "spline.h"

// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
// ฑ Engine specific routines ฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑ
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ

// project vertex normals
// r = array of rotated vertex normals (x,y,z)
// t = array of texture coordinates (u,v)
void EnvProjectVertexNormals (sObject *o, int *r, int *t)
{
 int i;
 int xt,yt;
 for(i=0;i<o->numcoords;i++)
 {
  xt =  r[i*3];
  yt =  r[i*3+1];
  t[i*2]   = xt+env_lightx;
  t[i*2+1] = yt+env_lighty;
 }
}

// -----

// project coordinates 3D (rotated) -> 2D (tricoords)
// r = array of rotated coordinates (x,y,z)
// t = array of projected coordinates (x,y)
// z = array of Z-values
void ProjectCoords (sObject *o, int *r, int *t, int *z)
{
 int i;
 // int *from = &rotated[0];
 int xt,yt,zt;
 for(i=0;i<o->numcoords;i++)
 {
  xt =  r[i*3];
  yt =  r[i*3+1];
  zt =  r[i*3+2];
  if (zt <= 0)
	{
   z[i] = -1;
	}
  else
	{
   z[i] = zt;
	 zt += DISTANCE;
   t[i*2]   = ((xt << PROJ_PARAM) / (zt)) + 128;
   t[i*2+1] = ((yt << PROJ_PARAM) / (zt)) + 128;
  }
 }
}

// -----

// nv = number of visible polygons
// table = sorted tablr (from bytesort)
// tri -> triangle coordinates
// tex -> texture coordinates
void Object_DrawTexture(unsigned char *b,sObject *o,int nv, int *table, int *tri, int *tex)
{
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
  TexMap(b,c1,c2,c3,tri,tex,o->texmap);
 }
}

// -----

void Object_Project(sObject *o, sCamera *c)
{
 Matrix_Rotation(o->m,o->xang,o->yang,o->zang);
 Matrix_Copy(o->e,o->m);
 o->e[3*4+0] += (o->xpos << DIVD);
 o->e[3*4+1] += (o->ypos << DIVD);
 o->e[3*4+2] += (o->zpos << DIVD);
 Matrix_Mult(o->e,c->m);
}

// -----

void Object_Draw(unsigned char *b,sObject *o)
{
 Matrix_Process(o->e,o->coords,rotated,o->numcoords);
 ProjectCoords(o,rotated,tricoords,zcoords);
 switch(o->rottype)
 {
  case 0 : Matrix_Process(o->m,o->vertex_n,rotatedvertexnormals,o->numcoords);
           EnvProjectVertexNormals(o,rotatedvertexnormals,texcoords);
					 break;
	case 1 : //CopyVertexNormals(o);
					 break;
 }
 numvisiblepolys = polysortsetup(o,polysort,tricoords,zcoords);
 bytesort(numvisiblepolys,polysort,sorted);
 switch(o->shadetype)
 {
  case 0 : Object_DrawTexture(b,o,numvisiblepolys,polysort,tricoords,texcoords);
	         break;
  case 1 : Object_DrawTexture(b,o,numvisiblepolys,polysort,tricoords,o->tex_c);
	         break;
 }
}

// -----

void Camera_Process_Aim(sCamera *c)
{
// int mm[16];
 // Matrix_Translation(c->m,-(c->xpos),-(c->ypos),-(c->zpos));
 // create_camera_matrix(c,c->m);
 // Matrix_Rotation(mm,-(c->xang),-(c->yang),-(c->zang));
 // Matrix_Mult(c->m,mm);
 Camera_Create_Lookat(c,c->m/*mm*/);
}

void Camera_Process_Free(sCamera *c)
{
 int mm[16];
 Matrix_Translation(c->m,-(c->xpos),-(c->ypos),-(c->zpos));
 Matrix_Rotation(mm,-(c->xang),-(c->yang),-(c->zang));
 Matrix_Mult(c->m,mm);
}


// -----

void Anim_Init(sObject *o1,sObject *o2,sObject *o3, sMorph *os)
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

void Anim_Update(sObject *anim, int objloc, sMorph *os)
{
 int i;
 sCoord objpos;
 objloc&=65535;
 for (i=0;i<anim->numcoords;i++)
 {
	objpos = get_spline_point(objloc, &os->s[i]);
  anim->coords[i*3]   = objpos.x;
  anim->coords[i*3+1] = objpos.y;
  anim->coords[i*3+2] = objpos.z;
  // Re-calculate Face & Vertex normals here
 }
}

// -----

void Init_Engine(void)
{
 rotated              = (int *)malloc(MAXCOORDS*(3*sizeof(int)));
 sorted               = (int *)malloc(MAXCOORDS*(3*sizeof(int)));
 rotatedvertexnormals = (int *)malloc(MAXCOORDS*(3*sizeof(int)));
 polysort             = (int *)malloc(MAXPOLYS *(sizeof(int)));
 tricoords            = (int *)malloc(MAXCOORDS*(3*sizeof(int)));
 texcoords            = (int *)malloc(MAXCOORDS*(3*sizeof(int)));
 zcoords              = (int *)malloc(MAXCOORDS*(sizeof(int)));
 BackgroundBuffer     = (unsigned char *)malloc(64000);
}

void Exit_Engine(void)
{
 free(rotated);
 free(sorted);
 free(rotatedvertexnormals);
 free(polysort);
 free(tricoords);
 free(texcoords);
 free(zcoords);
 free(BackgroundBuffer);
}

