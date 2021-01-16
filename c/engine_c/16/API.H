// engine.h

#ifndef api_h_included
#define api_h_included

#include "types.h"				 // mainly structures

objstruc* Object_Init(void);
objstruc* Object_Copy(objstruc *o);

void Object_Set_Rot(objstruc *o, int xa, int ya, int za);
void Object_Set_Pos(objstruc *o, int x, int y, int z);
void Object_Add_Rot(objstruc *o, int xa, int ya, int za);
void Object_Add_Pos(objstruc *o, int x, int y, int z);
void Object_Set_Mapping(objstruc *o, int t, char *texture);

char* Load_Texture(char *f);
char* Load_Palette(char *f);


void Camera_Set_Rot(camstruc *c, int xa, int ya, int za);
void Camera_Set_Pos(camstruc *c, int x, int y, int z);
void Camera_Add_Rot(camstruc *c, int xa, int ya, int za);
void Camera_Add_Pos(camstruc *c, int x, int y, int z);
void Camera_Set_Target(camstruc *c, int x, int y, int z);
void Camera_Create_Lookat(camstruc *c, float *m);

#endif
