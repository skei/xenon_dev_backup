#ifndef engine_h_included
#define engine_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#include "defines.h"
#include "types.h"
#include "vars.h"

void Engine_Init(int maxpolys);
void Engine_Exit(void);
sObject *Object_Load( char *s );

void Object_SetPos( sObject *o, float x, float y, float z);
void Object_SetRot( sObject *o, float x, float y, float z);
void Object_AddRot( sObject *o, float x, float y, float z);

void Camera_SetPos( sCamera *c, float x, float y, float z);
void Camera_SetRot( sCamera *c, float x, float y, float z);
void Camera_AddRot( sCamera *c, float x, float y, float z);
void Camera_AddPos( sCamera *c, float x, float y, float z);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
