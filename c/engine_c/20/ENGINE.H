#ifndef engine_h_included
#define engine_h_included

#include "types.h"

void CopyVertexNormals(objstruc *o);
void ProjectVertexNormals (objstruc *o);
void ProjectCoords (objstruc *o);
void Object_DrawTexture(objstruc *o,int nv, int *table, int *tri, int *tex);
void Object_Project(objstruc *o, camstruc *c);
void Object_Draw(screen_S *vp,objstruc *o);
void Camera_Process_Aim(camstruc *c);
void Camera_Process_Free(camstruc *c);
void Check_BoundingBox(screen_S *vp,objstruc *o, camstruc *c);

void Anim_Init(objstruc *o1,objstruc *o2,objstruc *o3, morphstruc *os);
void Anim_Update(objstruc *anim, int objloc, morphstruc *os);

extern int lightx,lighty;

#endif

