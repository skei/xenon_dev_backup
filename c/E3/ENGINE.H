#ifndef engine_h_included
#define engine_h_included
// ��������������������������������������������������������������������������

#include "defines.h"
#include "types.h"
#include "vars.h"

void Engine_Init( int mode, int maxpolys, int maxmaterials, int w, int h,
                  int projparam, int minz, int maxz);
void Engine_UnInit(void);

sObject *Object_Load( char *s );
void Object_Free( sObject *o);

// ��������������������������������������������������������������������������
#endif