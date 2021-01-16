#ifndef vars_h_included
#define vars_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

extern char *_Screen;          // pointer to screen
extern char *_Buffer;          // ptr to background buffer

extern int MinX,MaxX,MinY,MaxY;
extern int ScreenWidth,ScreenHeight;

extern int minclip_z, maxclip_z;
extern int PROJ_PARAM;
extern int viewdistance;
extern int halfscreenwidth_vd;
extern int halfscreenheight_vd;
extern int DISTANCE;

extern float *sine;
extern float *cosine;
extern int MulWidth[768];

extern sMaterial *Materials;
extern sWorld World;

extern int engine_mode;

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
