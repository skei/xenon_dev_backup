#include <malloc.h>
#include <memory.h>
#include <math.h>
#include <stdio.h>
#include "defines.h"			 // all #define's
#include "types.h"         // mainly structures
#include "math3d.h"

#ifdef VisCVersion
 #pragma warning (once:4035)
 #pragma warning (once:4244)
#endif

// -------
// Objects
// -------

objstruc* Object_Init(void)
{
 objstruc *newo;

 newo = malloc(sizeof(objstruc));
 if (newo!=0)
 {
  memset(newo,0,sizeof(objstruc));
 }
 return newo;
}

// -----

objstruc* Object_Copy(objstruc *o)
{
 objstruc *newo;

 newo = malloc(sizeof(objstruc));
 if (newo!=0 )
 {
  memcpy(newo,o,sizeof(objstruc));
 }
 return newo;
}

// -----

void Object_Set_Rot(objstruc *o, int xa, int ya, int za)
{
 o->xang = xa;
 o->yang = ya;
 o->zang = za;
}

// -----

void Object_Set_Pos(objstruc *o, int x, int y, int z)
{
 o->xpos = x;
 o->ypos = y;
 o->zpos = z;
}

// -----

void Object_Add_Rot(objstruc *o, int xa, int ya, int za)
{
 o->xang += xa;
 o->yang += ya;
 o->zang += za;
}

// -----

void Object_Add_Pos(objstruc *o, int x, int y, int z)
{
 o->xpos += x;
 o->ypos += y;
 o->zpos += z;
}

// -----

void Object_Set_Mapping(objstruc *o, int t, char *texture)
{
 switch(t)
 {
// Environment mapping
	case 0 : o->rottype = 0;
	         o-> shadetype = 0;
					 o->texmap = texture;
	         break;
// Standard texture mapping
	case 1 : o->rottype = 1;
	         o-> shadetype = 1;
					 o->texmap = texture;
	         break;
 }
}

char* Load_Texture(char *f)
{
 char *r;
 FILE *fp;

 r = malloc(65536);
 fp = fopen (f, "rb");
 fread (r, 1, 65535, fp);
 fclose (fp);
 return r;
}

char* Load_Palette(char *f)
{
 char *r;
 FILE *fp;

 r = malloc(768);
 fp = fopen (f, "rb");
 fread (r, 1, 768, fp);
 fclose (fp);
 return r;
}


// ------
// Camera
// ------

void Camera_Set_Rot(camstruc *c, int xa, int ya, int za)
{
 c->xang = xa;
 c->yang = ya;
 c->zang = za;
}

// -----

void Camera_Set_Pos(camstruc *c, int x, int y, int z)
{
 c->xpos = x;
 c->ypos = y;
 c->zpos = z;
}

// -----

void Camera_Add_Rot(camstruc *c, int xa, int ya, int za)
{
 c->xang += xa;
 c->yang += ya;
 c->zang += za;
}

// -----

void Camera_Add_Pos(camstruc *c, int x, int y, int z)
{
 c->xpos += x;
 c->ypos += y;
 c->zpos += z;
}

// -----

void Camera_Set_Target(camstruc *c, int x, int y, int z)
{
 c->targetx = x;
 c->targety = y;
 c->targetz = z;
}

// -----

// Create camera look-at matrix in m...
void Camera_Create_Lookat(camstruc *c, int *m)
{
 float xx,yy,zz;
 float d1,d2;
 float len;
 int mtemp[16],mtemp2[16];

// lager vektor av (xpos,ypos,zpos) og (targetx,targety,targetz)
// kameraet er i pos (xpos,ypos,zpos) og peker mot ^^^^^^
 xx = (float)(c->targetx - c->xpos);
 yy = (float)(c->targety - c->ypos);
 zz = (float)(c->targetz - c->zpos);
 len = (float)sqrt(xx*xx + yy*yy + zz*zz);
// gjør om til normal vektor
 xx /= len;
 yy /= len;
 zz /= len;
 d1 = sqrt(yy*yy + zz*zz);
 d2 = sqrt(xx*xx + yy*yy);
 Matrix_Identity(mtemp);
// translate to origin
 Matrix_Translation(mtemp,-c->xpos,-c->ypos,-c->zpos);
// rotate around x to lie in xz plane
 Matrix_Identity(mtemp2);
 mtemp2[1*4+1] = ( zz/d1)*DIVD_NUM; mtemp2[1*4+2] = (yy/d1)*DIVD_NUM;
 mtemp2[2*4+1] = (-yy/d1)*DIVD_NUM; mtemp2[2*4+2] = (zz/d1)*DIVD_NUM;
 Matrix_Mult(mtemp,mtemp2);
// rotate around y to line up with z axis
 Matrix_Identity(mtemp2);
 mtemp2[0*4+0] = ( d1)*DIVD_NUM; mtemp2[0*4+2] = (xx)*DIVD_NUM;
 mtemp2[2*4+0] = (-xx)*DIVD_NUM; mtemp2[2*4+2] = (d1)*DIVD_NUM;
 Matrix_Mult(mtemp,mtemp2);
// rotate around z to line up with origo
//------
// litt feil med denne: Denne er cluet til at det bugger _litt_...
// Finner du ut akkurat hva dette skal være så gi meg et hint!
 Matrix_Identity(mtemp2);
 mtemp2[0*4+0] = (xx/d2)*DIVD_NUM; mtemp2[0*4+1] = (-yy/d2)*DIVD_NUM;
 mtemp2[1*4+0] = (yy/d2)*DIVD_NUM; mtemp2[1*4+1] = ( xx/d2)*DIVD_NUM;
 Matrix_Mult(mtemp,mtemp2);
// Så må vi rotere så mye som Bank sier at kameraet heller
 Matrix_Rotation(mtemp2,0,0,c->Bank);
 Matrix_Mult(mtemp,mtemp2);
 Matrix_Copy(m,mtemp);
}


