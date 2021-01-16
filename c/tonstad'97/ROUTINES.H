 #ifndef routines_h_included
 #define routines_h_included
 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

 extern int Rseed;

 #define SkurrNum 20
 #define SkurrMax 20

 // --------------------------------------------------------------------------

void SetMode(unsigned short int);
 #pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

 // -----

void SetRGB(char color, char r,char g, char b);
 #pragma aux SetRGB=\
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

 // -----

void WaitVR (void);
 #pragma aux WaitVR =\
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

void SetPalette( char *palette );

// --------------------------------------------------------------------------

void Randomize(void);
 #pragma aux Randomize=\
  " xor edx,edx",\
  " xor ecx,ecx",\
  " mov ah,0x2c",\
  " int 21h",\
  " mov [Rseed],edx",\
  " shl [Rseed],16",\
  " or [Rseed], ecx",\
 modify [eax ecx ecx];

float FloatRandom( float x );
int IntRand1( int x );

void CalcBackground(char *buffer, int addcolor);
void smooth(char *buffer);

void Init_Mul320Table(void);
void DrawRotoZoom(char *screen,char *image, float angle, int scale);

void Init_SphereMap( char *p );
void DrawSphere( char *map, char *dest, unsigned char u, unsigned char v, char *tex );

void Skurr_Init(void);
void Skurr_draw(char *dst, char *src, int num);

 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
 #endif
