#ifndef vars_h_included
#define vars_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#include "defines.h"
#include "types.h"
#include "vars.h"

// sin/cos tables.... allocated in init
extern float *sine;
extern float *cosine;

extern unsigned char *_Screen;
extern unsigned char *_Buffer;
extern unsigned char *_Texture;
extern unsigned char *_ShadeTable;

extern int ScreenWidth,ScreenHeight;

extern float viewdistance;
extern float halfscreenwidth_vd;
extern float halfscreenheight_vd;
extern float minclip_z;
extern float maxclip_z;

extern int MinX,MaxX;
extern int MinY,MaxY;


extern sWorld *World;

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
