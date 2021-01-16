#ifndef light_h_included
#define light_h_included
// --------------------------------------------------------------------------

#include "defines.h"
#include "types.h"
#include "vars.h"

void Calc_ShadeHighLight(char *palette, char *ShadeTable,int diffuse,
                         float rr, float gg, float bb);

void Calc_PhongIntensity(float L, float F, float D, float S, float G);

void Calc_PhongMap(void);

void Calc_GouraudMap(void);

void CalcLight_Camera(sObject *o);

// --------------------------------------------------------------------------
#endif

