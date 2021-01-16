#ifndef engine_h_included
#define engine_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#include "types.h"

// project vertex normals
// r = array of rotated vertex normals (x,y,z)
// t = array of texture coordinates (u,v)
void EnvProjectVertexNormals (sObject *o, int *r, int *t);
// project coordinates 3D (rotated) -> 2D (tricoords)
// r = array of rotated coordinates (x,y,z)
// t = array of projected coordinates (x,y)
// z = array of Z-values
void ProjectCoords (sObject *o, int *r, int *t, int *z);
// nv = number of visible polygons
// table = sorted tablr (from bytesort)
// tri -> triangle coordinates
// tex -> texture coordinates
void Object_DrawTexture(unsigned char *b,sObject *o,int nv, int *table, int *tri, int *tex);
void Object_Project(sObject *o, sCamera *c);
void Object_Draw(unsigned char *b,sObject *o);
void Camera_Process_Aim(sCamera *c);
void Camera_Process_Free(sCamera *c);
void Anim_Init(sObject *o1,sObject *o2,sObject *o3, sMorph *os);
void Anim_Update(sObject *anim, int objloc, sMorph *os);

void Init_Engine(void);
void Exit_Engine(void);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
