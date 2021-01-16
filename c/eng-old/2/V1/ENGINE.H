#ifndef engine_h_included
#define engine_h_included
// ��������������������������������������������������������������������������

#include "defines.h"
#include "types.h"

// ����� World ��������������������������������������������������������������

sWorld *World_Load( char *s );

// ����� Object �������������������������������������������������������������

sObject *Object_Load( char *s );
void Object_Free(sObject *o);

// ��������������������������������������������������������������������������
#endif