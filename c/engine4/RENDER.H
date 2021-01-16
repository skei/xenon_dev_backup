#ifndef render_h_included
#define render_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#include "types.h"

void Camera_CreateMatrix_Free(sCamera *c);
void Object_CreateMatrix(sObject *o, sCamera *c);
int Object_CheckBoundingSphere(sObject *o, sCamera *c);
void SortObject( sPolygon *polys );
void HandleGroup( sGroup *g);
void Object_Process(sObject *o);
void HandleObject( sObject *o);

void GTfiller( sPolygon *p );
void Render(void);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
