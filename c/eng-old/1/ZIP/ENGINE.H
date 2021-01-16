#ifndef engine_h_included
#define engine_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#include "types.h"

sObject *Object_LoadBuffer( char *LoadedObject );
sObject *Object_LoadFile( char *filename );

void Object_Free(sObject *O);
void Object_SetHierarchy( sObject *O, sObject *parentO, sObject *childO, int inherit );

void Object_SetPos( sObject *o, float x, float y, float z);
void Object_AddPos( sObject *o, float x, float y, float z);
void Object_SetRot( sObject *o, float x, float y, float z);
void Object_AddRot( sObject *o, float x, float y, float z);

void Camera_SetPos( sCamera *c, float x, float y, float z);
void Camera_SetRot( sCamera *c, float x, float y, float z);
void Camera_SetTarget( sCamera *c, float x, float y, float z);
void Camera_AddTarget( sCamera *c, float x, float y, float z);
void Camera_AddRot( sCamera *c, float x, float y, float z);
void Camera_AddPos( sCamera *c, float x, float y, float z);

char ClosestColor(char *palette,char r, char g, char b);
void Calc_ShadeTable(char *palette, char *ShadeTable);
void Calc_ShadeHighLight(char *palette, char *ShadeTable,int diffuse,
                         float rr, float gg, float bb);
void Calc_PhongIntensity(float _L, float _F, float _D, float _S, float _G);
void Calc_PhongMap(void);
void Calc_GouraudMap(void);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
