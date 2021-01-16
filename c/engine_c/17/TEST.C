#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>

#include "defines.h"
#include "types.h"
#include "matrix.h"
#include "textri.h"
#include "bytesort.h"
#include "vars.h"

#include "torus1.inc"
//#include "face4.inc"

// set video mode
void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

void SetPalette(char *p)
{
 int i;
 outp(0x3c8,0);
 for (i=0;i<768;i++)
 {
  outp(0x3c9,p[i]);
 }
}


// Clears gfx screen

void ClearScreen(char clr)
{
 memset((char *)0xA0000,clr,64000);
}

// -----------------------------------

void InitEngine(void)
{
 int i;
 float realangle = -PI;

 printf("\nInitializing aRender... :) ...\n\n");
// Calculate sin/cos tables.
 for (i = 0; i < TABLESIZE; i++)
 {
  sine[i] =  DIVD_NUM * sin(realangle);
  cosine[i] = DIVD_NUM * cos(realangle);
  realangle = realangle + ((PI*2)/TABLESIZE);
 }
}

// ----------------------------------------------------------------------

void ProjectVertexNormals (struct objstruc *o)
{
 int i;
 int xt,yt;
 for(i=0;i<o->numcoords;i++)
 {
  xt =  rotatedvertexnormals[i*3];
	yt =  rotatedvertexnormals[i*3+1];
  texcoords[i*2+1] = (unsigned short int) (xt+lightx);
  texcoords[i*2  ] = (unsigned short int) (yt+lighty);
 }
}

// -----

void ProjectCoords (struct objstruc *o)
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
   tricoords[i*2+1] = (((xt << PROJ_PARAM) / (zt)) + 160);
   tricoords[i*2  ] = (((yt << PROJ_PARAM) / (zt)) + 100);
  }
 }
}

// -----

void CopyBuffer(void)
{
 int i;
 char *p = bfr+(char *)(200*960+320);
 char *s = (char *)0xA0000;

 for (i=0;i<200;i++)
 {
  memcpy(s,p,320);
  p+=960;
  s+=320;
 }
}

// -----

void ClearBuffer(char c)
{
 int i;
 char *p = bfr+(char *)(200*960+320);

 for (i=0;i<200;i++)
 {
  memset(p,c,320);
  p+=960;
 }
}

// -----

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// Load texture and plette from disk
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
void LoadTexture()
{
 FILE *fp;
// read texture from file
 fp = fopen("texture.raw","rb");
 fread(texture,1,65536,fp);
 fclose(fp);
// read palette from file
 fp = fopen("palette.raw","rb");
 fread(palette,1,768,fp);
 fclose(fp);
}

// -------------------------------------------------------------------------

void DrawTextureObject(struct objstruc *o,int nv)
{
 int i;
 unsigned int polynum;
 int c1,c2,c3;
// for (i=0;i<nv;i++)
 for (i=nv-1;i>=0;i--)
 {
  polynum = (sorttable[i].polynumber);         // polynum = # of polygon to draw

  c1 = o->polys[polynum*3];
  c2 = o->polys[polynum*3+1];
  c3 = o->polys[polynum*3+2];

  textri(c1*4,c2*4,c3*4);
 }
}

// ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void main(void)
{
 lightx = 64;
 lighty = 64;
 lxadd = 2;
 lyadd = 1;
 test.xang = 3072;
 test.yang = 2048;
 test.zang = 2048;
 test.xpos = 0;
 test.ypos = 0;
 test.zpos = 1500;
 cam.xang = 0;
 cam.yang = 0;
 cam.zang = 0;
 cam.xpos = 0;
 cam.ypos = 0;
 cam.zpos = -300;
 InitEngine();
 LoadTexture();
// usetexture : pointer to 256x256 texture
 buffer = &bfr;
 usetexture1 = &texture;
 SetMode(0x13);
 SetPalette(palette);
// -----
 while(!kbhit())
 {
// Update angles
  test.xang+=40;
  test.xang&=4095;
  test.yang+=20;
  test.yang&=4095;
  test.zang+=30;
  test.zang&=4095;
/*
  cam.xang+=0;
  cam.xang&=4095;
  cam.yang+=15;
  cam.yang&=4095;
  cam.zang+=0;
  cam.zang&=4095;
*/
  init_translation(&cam.m[0],-cam.xpos,-cam.ypos,-cam.zpos);
  init_rotation(mat,-cam.xang,-cam.yang,-cam.zang);
  mult_matrix(&cam.m[0],mat);
  init_rotation(&test.m[0],test.xang,test.yang,test.zang);
  process_matrix(&test.m[0],test.vertex_n,rotatedvertexnormals,test.numcoords);
  copy_matrix(ematrix,&test.m[0]);
  ematrix[3*4+0] += (test.xpos);
  ematrix[3*4+1] += (test.ypos);
  ematrix[3*4+2] += (test.zpos);
  mult_matrix(ematrix,&cam.m[0]);
  process_matrix(ematrix,test.coords,rotated,test.numcoords);

  ProjectCoords(&test);
  ProjectVertexNormals(&test);
  numvisiblepolys = sortsetup(&test);
  bytesort(numvisiblepolys);

  ClearBuffer(1);
  DrawTextureObject(&test,numvisiblepolys);
  CopyBuffer();
 }
 getch();
 SetMode(3);
}

