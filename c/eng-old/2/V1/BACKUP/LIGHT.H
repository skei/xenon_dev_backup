#ifndef light_h_included
#define light_h_included
// ��������������������������������������������������������������������������

#include "types.h"

// L = Intensity of light that sihines on surface
// F = Intensity of surface itself
// D = Diffuse reflection level (0-1)
// S = Specular reflection level (0-1)
// G = Glossiness factor (1-400)
void Calc_PhongIntensity(float L, float F, float D, float S, float G);
void Calc_PhongMap(void);
void Calc_GouraudMap(void);


void CalcLight_Camera(sObject *o);
void CalcLight_Directional(sObject *o, sGroup *g);
void CalcLight_EnvMap(sObject *o, sGroup *g);

// ��������������������������������������������������������������������������
#endif