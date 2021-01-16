#include "defines.h"
#include "types.h"
#include "vars.h"

//--------------------------------------------------------------------------

float *sine;
float *cosine;

unsigned char *_Screen;
unsigned char *_Buffer;
unsigned char *_Texture;
unsigned char *_ShadeTable;

int ScreenWidth,ScreenHeight;

float viewdistance;
float halfscreenwidth_vd;
float halfscreenheight_vd;
float minclip_z;
float maxclip_z;

int MinX,MaxX;
int MinY,MaxY;

sWorld *World;

