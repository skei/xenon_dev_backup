#pragma warning (once:4244)

#define CAMERA_ROLL_PREC 

// engine.c

#include <malloc.h>
#include <memory.h>
#include <math.h>
#include "defines.h"			 // all #define's
#include "vars.h"					 // global variables.... ugh! :-/
#include "types.h"				 // mainly structures
#include "matrix.h"
#include "vector.h"

// -------
// Objects
// -------

struct objstruc* Object_init(void)
{
 struct objstruc *newo;
 
 newo = malloc(sizeof(struct objstruc));
 if (newo!=0)
 {
	memset(newo,0,sizeof(struct objstruc));
 }
 return newo;
}

struct objstruc* Object_copy(struct objstruc *o)
{
 struct objstruc *newo;

 newo = malloc(sizeof(struct objstruc));
 if (newo!=0 )
 {
	memcpy(newo,o,sizeof(struct objstruc));
 }
 return newo;
}

void Object_set_rot(struct objstruc *o, int xa, int ya, int za)
{
 o->xang = xa;
 o->yang = ya;
 o->zang = za;
}

void Object_set_pos(struct objstruc *o, int x, int y, int z)
{
 o->xpos = x;
 o->ypos = y;
 o->zpos = z;
}

void Object_add_rot(struct objstruc *o, int xa, int ya, int za)
{
 o->xang += xa;
 o->yang += ya;
 o->zang += za;
}

void Object_add_pos(struct objstruc *o, int x, int y, int z)
{
 o->xpos += x;
 o->ypos += y;
 o->zpos += z;
}

void Object_set_mapping(struct objstruc *o, int t)
{
 switch(t)
 {
// Environment mapping
	case 0 : o->rottype = 0;
	         o-> shadetype = 0;
	         break;
// Standard texture mapping
	case 1 : o->rottype = 1;
	         o-> shadetype = 1;
	         break;
// Gouraud shaded texture mapping
	case 2 : o->rottype = 2;
	         o-> shadetype = 2;
	         break;
 }
}


// ------
// Camera
// ------

void Camera_set_rot(struct camstruc *c, int xa, int ya, int za)
{
 c->xang = xa;
 c->yang = ya;
 c->zang = za;
}

void Camera_set_pos(struct camstruc *c, int x, int y, int z)
{
 c->xpos = x;
 c->ypos = y;
 c->zpos = z;
}

void Camera_add_rot(struct camstruc *c, int xa, int ya, int za)
{
 c->xang += xa;
 c->yang += ya;
 c->zang += za;
}

void Camera_add_pos(struct camstruc *c, int x, int y, int z)
{
 c->xpos += x;
 c->ypos += y;
 c->zpos += z;
}

void Camera_set_target(struct camstruc *c, int x, int y, int z)
{
 c->targetx = x;
 c->targety = y;
 c->targetz = z;
}

// -----

// Create camera look-at vector
void camera_create_lookat(struct camstruc *c, float *m)
{
 float xx,yy,zz;
 float d1,d2;
 float len;
 float mtemp[16],mtemp2[16];  

// lager vektor av (xpos,ypos,zpos) og (targetx,targety,targetz)
// kameraet er i pos (xpos,ypos,zpos) og peker mot ^^^^^^
 xx = c->targetx - c->xpos;
 yy = c->targety - c->ypos;
 zz = c->targetz - c->zpos;

 len = sqrt(xx*xx + yy*yy + zz*zz);

// gjør om til normal vektor
 xx /= len;                                 
 yy /= len;                                 
 zz /= len;                             
 
 d1 = sqrt(yy*yy + zz*zz);
 d2 = sqrt(xx*xx + yy*yy);

 init_identity(mtemp);                 
// translate to origin          
 init_translation(mtemp,-c->xpos,-c->ypos,-c->zpos);
// rotate around x to lie in xz plane        
 init_identity(mtemp2);                         
 mtemp2[1*4+1] =  zz/d1; mtemp2[1*4+2] = yy/d1;
 mtemp2[2*4+1] = -yy/d1; mtemp2[2*4+2] = zz/d1;
 mult_matrix(mtemp,mtemp2);
// rotate around y to line up with z axis       
 init_identity(mtemp2);                             
 mtemp2[0*4+0] =  d1; mtemp2[0*4+2] = xx;
 mtemp2[2*4+0] = -xx; mtemp2[2*4+2] = d1;
 mult_matrix(mtemp,mtemp2);
// rotate around z to line up with origo 
//------
// litt feil med denne: Denne er cluet til at det bugger _litt_... 
// Finner du ut akkurat hva dette skal være så gi meg et hint!
 init_identity(mtemp2);
 mtemp2[0*4+0] = xx/d2; mtemp2[0*4+1] = -yy/d2;
 mtemp2[1*4+0] = yy/d2; mtemp2[1*4+1] =  xx/d2;
 mult_matrix(mtemp,mtemp2);
// Så må vi rotere så mye som Bank sier at kameraet heller
 init_rotation(mtemp2,0,0,c->Bank);
 mult_matrix(mtemp,mtemp2);                                           
 copy_matrix(m,mtemp);
}                                             

// -----

void create_camera_matrix(struct camstruc *c, float *m)
{
 float RotX[16],RotY[16],RotZ[16];
 vector d,dp,d2;
 float sinX,cosX,sinY,cosY,sinZ,cosZ;

 init_identity(c->m);
 init_identity(RotX);
 init_identity(RotY);
 init_identity(RotZ);

 //d = Vector_Sub(c->target, c->pos);	// get camera-direction
 d.x = c->targetx - c->xpos;
 d.y = c->targety - c->ypos;
 d.z = c->targetz - c->zpos;

 dp.x = d.x;                        // project d into (x,z)-plane
 dp.y = 0;
 dp.z = d.z;

 Vector_Norm(d);
 Vector_Norm(dp);

 if ( (dp.x==0) && (dp.z==0) )
 {
	sinY=0.0;
  cosY=1.0;
 }
 else
 {
	sinY=-dp.x;
  cosY=dp.z;
 }
 RotY[2*4+0]= sinY;
 RotY[0*4+2]=-sinY;
 RotY[0*4+0]= cosY;
 RotY[2*4+2]= cosY;

 d2 = Vector_Trans(d,RotY);      //Rotate d into (y,z)-plane

 if ((d2.y==0)&&(d2.z==0))
 {
	sinX=0.0;
  cosX=1.0;
 }
 else
 {
	sinX=d2.y;
  cosX=d2.z;
 }
 RotX[1*4+2]= sinX;
 RotX[2*4+1]=-sinX;
 RotX[1*4+1]= cosX;
 RotX[2*4+2]= cosX;

 sinZ=sin(2*PI*(c->Bank/DIVD_NUM));
 cosZ=cos(2*PI*(c->Bank/DIVD_NUM));

 RotZ[0*4+1]= sinZ;
 RotZ[1*4+0]=-sinZ;
 RotZ[0*4+0]= cosZ;
 RotZ[1*4+1]= cosZ;

 mult_matrix(c->m,RotY);
 mult_matrix(c->m,RotZ);
 mult_matrix(c->m,RotX);

 c->m[3*4+0] = c->xpos;
 c->m[3*4+1] = c->ypos;
 c->m[3*4+2] = c->zpos;
}



