#ifndef engine_h_included
#define engine_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#include "defines.h"
#include "types.h"

// 께께� World 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

sWorld *World_Load( char *s );

// 께께� Object 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께�

sObject *Object_Load( char *s );
void Object_Free(sObject *o);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
