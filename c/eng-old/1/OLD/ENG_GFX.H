#ifndef eng_gfx_h_included
#define eng_gfx_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

#pragma aux setrgb=\
 "mov dx,3c8h",\
 "out dx,al",\
 "inc dx",\
 "mov al,ah",\
 "out dx,al",\
 "mov al,bl",\
 "out dx,al",\
 "mov al,bh",\
 "out dx,al",\
 parm [al] [ah] [bl] [bh] modify [edx];

void waitvr (void);
#pragma aux waitvr =\
 " mov dx,3dah",\
 "wait1:",\
 " in al,dx",\
 " test al,8",\
 " jnz wait1",\
 "wait2:",\
 " in al,dx",\
 " test al,8",\
 " jz wait2",\
 modify [al dx];


// --------------------------------------------------------------------------

void Line(int x0, int y0, int x1, int y1, char color, char *_B);
void DrawChar( int x, int y, unsigned char c, short int color,char *_B);
void DrawString(int x1, int y1, char *s, short int color,char *_B);
void SetPalette( char *p );

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
