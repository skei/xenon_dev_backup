
#include "defines.h"
#include "types.h"
#include "vars.h"

char *_Screen;          // pointer to screen
char *_Buffer;          // ptr to background buffer

int MinX,MaxX,MinY,MaxY;
int ScreenWidth,ScreenHeight;

int minclip_z, maxclip_z;
int PROJ_PARAM;
int viewdistance;
int halfscreenwidth_vd;
int halfscreenheight_vd;
int DISTANCE;

float *sine;
float *cosine;
int MulWidth[768];

sWorld World;
sMaterial *Materials;

int engine_mode;
