#ifndef render_h_included
#define render_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#include "defines.h"
#include "types.h"

void Camera_CreateMatrix_Free(sCamera *c);
void Object_CreateMatrix(sObject *o, sCamera *c);
int Object_CheckBoundingSphere(sObject *o, sCamera *c);
void SortWorldObject( void );
void HandleGroup( sObject *o,sGroup *g);
void HandleObject( sObject *o);
void Render(void);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
