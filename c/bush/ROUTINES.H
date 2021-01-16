#ifndef routines_h_included
#define routines_h_included
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#define SkurrNum 20
#define SkurrMax 20

// ---

extern int Rseed;
extern int Mul320[200];
extern int Skurr_Array[200][SkurrNum];

// --


void SetRGB(unsigned char color,char r,char g,char b);
void WaitVR(void);
void flash (char *pal, int num);
void BlackPalette(void);
void SetPalette( int from, int to, char *Pal);

 // Calculate background
 // buffer = 64k buffer
 // addcolor = lowest color to use (added to image after creation)
void CalcBackground(char *buffer, int addcolor);

 // Smooth buffer
void smooth(char *buffer);

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
void Init_Mul320Table(void);
void DrawRotoZoom(char *screen,char *image, float angle, int scale);
void Init_SphereMap( char *p );
void DrawSphere( char *map, char *dest, unsigned char u, unsigned char v, char *tex );
void Skurr_Init(void);
void Skurr_Draw(char *dst, char *src, int num);

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
#endif
