#include "defines.h"
#include "types.h"
#include "vars.h"
#include "math3d.h"

// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
// ฑ Camera handling routines ฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑฑ
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ

void Camera_Set_Rot(sCamera *c, int xa, int ya, int za)
{
 c->xang = xa;
 c->yang = ya;
 c->zang = za;
}

// -----

void Camera_Set_Pos(sCamera *c, int x, int y, int z)
{
 c->xpos = x;
 c->ypos = y;
 c->zpos = z;
}

// -----

void Camera_Add_Rot(sCamera *c, int xa, int ya, int za)
{
 c->xang += xa;
 c->yang += ya;
 c->zang += za;
}

// -----

void Camera_Add_Pos(sCamera *c, int x, int y, int z)
{
 c->xpos += x;
 c->ypos += y;
 c->zpos += z;
}

// -----

void Camera_Set_Target(sCamera *c, int x, int y, int z)
{
 c->targetx = x;
 c->targety = y;
 c->targetz = z;
}

// -----

// Create camera look-at matrix in m...
void Camera_Create_Lookat(sCamera *c, int *m)
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
// gj๘r om til normal vektor
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
// Finner du ut akkurat hva dette skal vๆre sๅ gi meg et hint!
 Matrix_Identity(mtemp2);
 mtemp2[0*4+0] = (xx/d2)*DIVD_NUM; mtemp2[0*4+1] = (-yy/d2)*DIVD_NUM;
 mtemp2[1*4+0] = (yy/d2)*DIVD_NUM; mtemp2[1*4+1] = ( xx/d2)*DIVD_NUM;
 Matrix_Mult(mtemp,mtemp2);
// Sๅ mๅ vi rotere sๅ mye som Bank sier at kameraet heller
 Matrix_Rotation(mtemp2,0,0,c->Bank);
 Matrix_Mult(mtemp,mtemp2);
 Matrix_Copy(m,mtemp);
}

