#ifndef eng_rend_h_included
#define eng_rend_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#include "eng_type.h"

void CalcLight_Directional(sObject *o,sWorld *w);

void Camera_CreateMatrix_Free(sCamera *c);
void Camera_CreateMatrix_Aim(sCamera *c);

void Object_CreateMatrix(sObject *o, sCamera *c);
int Object_Visible(sObject *o, sCamera *c);
void SortWorldObject( sWorld *World );
void Handle_Object( sObject *O, sCamera *C );

void Render( sWorld *World);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
