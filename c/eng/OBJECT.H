#ifndef object_h_included
#define object_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#include "types.h"

sObject* Object_Init(void);
sObject* Object_Copy(sObject *o);
void Object_Set_Rot(sObject *o, int xa, int ya, int za);
void Object_Set_Pos(sObject *o, int x, int y, int z);
void Object_Add_Rot(sObject *o, int xa, int ya, int za);
void Object_Add_Pos(sObject *o, int x, int y, int z);
void Object_Set_Mapping(sObject *o, int t, char *texture);
char* Load_Texture(char *f);
char* Load_Palette(char *f);
int Object_Visible(sObject *o, sCamera *c);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
