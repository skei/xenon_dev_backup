 #ifndef routines_h_included
 #define routines_h_included
 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

 // --- variables ---

 extern int Rseed;
 extern int Mul320[200];

 // --- prototypes ---

void  Randomize(void);
 #pragma aux Randomize=\
  " xor edx,edx"      ,\
  " xor ecx,ecx"      ,\
  " mov ah,0x2c"      ,\
  " int 21h"          ,\
  " mov [Rseed],edx"  ,\
  " shl [Rseed],16"   ,\
  " or  [Rseed], ecx" ,\
 modify [eax ecx ecx];

float Random_Float( float x );
int   Random_Int( int x );
void  Init_Mul320Table(void);

void  createfractalmap(unsigned char *map,int x1, int y1, int x2, int y2);
void  marble (unsigned char *source, unsigned char *dest, float scale,
              float turbulence, unsigned char *xrpage, unsigned char *yrpage);
void  HexagonMap(unsigned char *dest, int xn, int yn, int c1, int c2, int c3);
void  CalcBackground(char *buffer, int iter1, int dir, int addcolor);
void  Smooth(unsigned char *map);

void  SphMap_Init( char *p );
void  SphMap_Draw( char *map, char *dest, char uadd, char vadd,char *tex );

void  RotoZoom(char *screen,char *image, float angle, int scale);

void  Skurr_Init(void);
void  Skurr_Draw(char *dst, char *src, int num);

void  DrawVoxel (int xp, int yp, /*int*/float dir,
                 char *heightmap, char *colormap, char *buffer);







 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
 #endif
