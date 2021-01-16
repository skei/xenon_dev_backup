// poly.c

// Gouraud          7.14 (under Win95 + netscape open/idle)
// Gouraud texture 14.33 (under Win95 + netscape open/idle)
// ÄÄÄÄÄ Testing functions & variables ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

#include <conio.h>      // getch,outp
#include <stdio.h>      // printf
#include <stdlib.h>     // malloc,exit,abs
#include <string.h>     // memset

void SetMode(short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [eax];

// ÄÄÄÄÄ Typedef ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

typedef struct sVertex
{
 int x,y,z;
 int u,v;
 int c;
} sVertex;

// ÄÄÄÄÄ

typedef struct sPolygon
{
 sVertex *vertex1, *vertex2, *vertex3;
 char *texture;
} sPolygon;

// ÄÄÄÄÄ

// ÄÄÄÄÄ Global variables ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

int mul960[600];

char *palette;
char *tex;

// GTinnerloop
char *_Buffer;
char *_Texture;           // constant for object (group)
char *_ShadeTable;        // constant for palette/texture
int _innerTexAdder;
int _innerColorAdder;
// GTfiller
int _outerTexAdder;
int _outerColorAdder;
int _outerLeftAdder;
int _outerRightAdder;

int _savedTex;
int _savedColor;
int _savedLeft;
int _savedRight;

// ÄÄÄÄÄ The polyfiller innerloop ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

// ebx =            ?  - Cint:Cfrac
// ecx = linelength
// edi = screenpos
// ebp =
void Ginnerloop(char *screenpos, int color, int linelength);
#pragma aux Ginnerloop=\
 " cmp ecx,0",\
 " jle skipline",\
 "inner:",\
 " mov [edi],bh",\
 " add ebx,[_innerColorAdder]",\
 " inc edi",\
 " dec ecx",\
 " jnz inner",\
 "skipline:",\
 parm [edi][ebx][ecx];

// eax = temp
// ebx = temp
// ecx = linelength
// edx = TXint:TXfrac - TYint:TYfrac
// esi =           ?  - Cint:Cfrac
// edi = screenpos
// ebp =
void GTinnerloop(char *screenpos, int texpos, int color, int linelength);
#pragma aux GTinnerloop=\
 " cmp ecx,0",\
 " jle skipline",\
 "inner:",\
 " xor eax,eax",\
 " shld eax,edx,8",\
 " mov ah,dh",\
 " add eax,[_Texture]",\
 " movzx ebx,si",\
 " mov bl,[eax]",\
 " add edx,[_innerTexAdder]",\
 " add ebx,[_ShadeTable]",\
 " add esi,[_innerColorAdder]",\
 " mov al,[ebx]",\
 " mov [edi],al",\
 " inc edi",\
 " dec ecx",\
 " jnz inner",\
 "skipline:",\
 parm [edi][edx][esi][ecx] modify [eax ebx];

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void Gfiller1(int y1, int y2, int left, int right, int c)
{
 int i;

 if (y2 < 0)   return;
 if (y1 > 199) return;
 if (left  > (319<<16)) return;
 if (right < 0) return;

// maybe remove these
 if (y1 < -199 ) return;
 if (y2 >  399 ) return;

 for (i=y1;i<y2;i++)
 {
  Ginnerloop( (char *)((int)_Buffer+mul960[i+200]+(left>>16)),
              c,
              (right - left) >> 16 );
  c     += _outerColorAdder;
  left  += _outerLeftAdder;
  right += _outerRightAdder;
 }
 _savedColor = c;
 _savedLeft = left;
 _savedRight = right;
}

// -----

void Gfiller2(int y1, int y2)
{
 int i,left,right,c;

 if (y2 < 0)   return;
 if (y1 > 199) return;
 if (left  > (319<<16)) return;
 if (right < 0) return;

// maybe remove these
 if (y1 < -199 ) return;
 if (y2 >  399 ) return;

 c = _savedColor;
 left = _savedLeft;
 right = _savedRight;

 for (i=y1;i<y2;i++)
 {
  Ginnerloop( (char *)((int)_Buffer+mul960[i+200]+(left>>16)),
              c,
              (right - left) >> 16 );
  c     += _outerColorAdder;
  left  += _outerLeftAdder;
  right += _outerRightAdder;
 }
}


// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void GTfiller1(int y1, int y2, int left, int right, int tex, int c)
{
 int i;

 if (y2 < 0)   return;
 if (y1 > 199) return;
 if (left  > (319<<16)) return;
 if (right < 0) return;

// maybe remove these
 if (y1 < -199 ) return;
 if (y2 >  399 ) return;

 for (i=y1;i<y2;i++)
 {
  GTinnerloop( (char *)((int)_Buffer+mul960[i+200]+(left>>16)),
               tex,
               c,
               (right - left) >> 16 );
  tex   += _outerTexAdder;
  c     += _outerColorAdder;
  left  += _outerLeftAdder;
  right += _outerRightAdder;
 }
 _savedColor = c;
 _savedTex = tex;
 _savedLeft = left;
 _savedRight = right;

}

// -----

void GTfiller2(int y1, int y2)
{
 int i,c,tex,left,right;

 if (y2 < 0)   return;
 if (y1 > 199) return;
 if (left  > (319<<16)) return;
 if (right < 0) return;

// maybe remove these
 if (y1 < -199 ) return;
 if (y2 >  399 ) return;

 c = _savedColor;
 tex = _savedTex;
 left = _savedLeft;
 right = _savedRight;


 for (i=y1;i<y2;i++)
 {
  GTinnerloop( (char *)((int)_Buffer+mul960[i+200]+(left>>16)),
               tex,
               c,
               (right - left) >> 16 );
  tex   += _outerTexAdder;
  c     += _outerColorAdder;
  left  += _outerLeftAdder;
  right += _outerRightAdder;
 }

}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ


 #define SWAP(a,b) { temp=a; a=b; b=temp; }

 #define X1 v1->x
 #define Y1 v1->y
 #define U1 v1->u
 #define V1 v1->v
 #define C1 v1->c

 #define X2 v2->x
 #define Y2 v2->y
 #define U2 v2->u
 #define V2 v2->v
 #define C2 v2->c

 #define X3 v3->x
 #define Y3 v3->y
 #define U3 v3->u
 #define V3 v3->v
 #define C3 v3->c

// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ

void DrawPoly_Gouraud(sPolygon *p)
{
 sVertex *v1, *v2, *v3, *temp;
 int delta,CA,CB;
 int slope12, slope13;
 int x2_x1,x3_x1;
 int y2_y1,y3_y1;
 int c2_c1,c3_c1;

 v1 = p->vertex1;
 v2 = p->vertex2;
 v3 = p->vertex3;

 y2_y1 = Y2-Y1 + 1;
 y3_y1 = Y3-Y1 + 1;
 x2_x1 = X2-X1 + 1;
 x3_x1 = X3-X1 + 1;

 delta = (x2_x1)*(y3_y1) - (x3_x1)*(y2_y1);
 if (delta == 0) return;

 c2_c1 = C2-C1 + 1;
 c3_c1 = C3-C1 + 1;


 CA = ( ((c2_c1)*(y3_y1)-(c3_c1)*(y2_y1)) << 8 ) / delta;
 CB = ( ((x2_x1)*(c3_c1)-(x3_x1)*(c2_c1)) << 8 ) / delta;

 if (Y1 > Y2) SWAP(v1,v2);
 if (Y1 > Y3) SWAP(v1,v3);
 if (Y2 > Y3) SWAP(v2,v3);

 if (Y1 == Y2) goto FlatTop;
 if (Y2 == Y3) goto FlatBottom;

 slope12 = ((x2_x1)<<16) / (y2_y1);
 slope13 = ((x3_x1)<<16) / (y3_y1);

 if (slope12 > slope13) goto FlatLeft;
 /*if (slope12 < slope13)*/ goto FlatRight;

// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
// 1 2
//  3
 FlatTop:
 {
  if (X1 > X2) SWAP(v1,v2);
  _outerLeftAdder = ((x3_x1)<<16) / (y3_y1);
  _outerRightAdder = (((X3-X2)+1)<<16) / (y3_y1);
  _innerColorAdder = CA;
  _outerColorAdder = CA + ((_outerLeftAdder * CB) >> 16);
  Gfiller1(v1->y,
           v3->y,
           v1->x << 16,
           v2->x << 16,
           v1->c << 8 );
  goto FinitoFiller;
 }
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
//  1
// 2 3
 FlatBottom:
 {
  if (X2 > X3) SWAP(v2,v3);
  goto FinitoFiller;
 }
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
//  1
//   2
// 3
 FlatLeft:
 {
  goto FinitoFiller;
 }
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
//  1
// 2
//   3
 FlatRight:
 {
  goto FinitoFiller;
 }
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
 FinitoFiller:
 ;
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void DrawPoly_GouraudTexture(sPolygon *p)
{
 sVertex *v1, *v2, *v3, *temp;
 int delta,CA,CB,UA,UB,VA,VB;
 int slope12, slope13;
 int x2_x1,x3_x1;
 int y2_y1,y3_y1;
 int c2_c1,c3_c1;
 int u2_u1,u3_u1;
 int v2_v1,v3_v1;

 v1 = p->vertex1;
 v2 = p->vertex2;
 v3 = p->vertex3;

 y2_y1 = Y2-Y1 + 1;
 y3_y1 = Y3-Y1 + 1;
 x2_x1 = X2-X1 + 1;
 x3_x1 = X3-X1 + 1;

 delta = (x2_x1)*(y3_y1) - (x3_x1)*(y2_y1);
 if (delta == 0) return;

 _Texture = p->texture;

 c2_c1 = C2-C1 + 1;
 c3_c1 = C3-C1 + 1;
 u2_u1 = U2-U1 + 1;
 u3_u1 = U3-U1 + 1;
 v2_v1 = V2-V1 + 1;
 v3_v1 = V3-V1 + 1;

 CA = ( ((c2_c1)*(y3_y1)-(c3_c1)*(y2_y1)) << 8 ) / delta;
 CB = ( ((x2_x1)*(c3_c1)-(x3_x1)*(c2_c1)) << 8 ) / delta;

 UA = ( ((u2_u1)*(y3_y1)-(u3_u1)*(y2_y1)) << 8 ) / delta;
 UB = ( ((x2_x1)*(u3_u1)-(x3_x1)*(u2_u1)) << 8 ) / delta;

 VA = ( ((v2_v1)*(y3_y1)-(v3_v1)*(y2_y1)) << 8 ) / delta;
 VB = ( ((x2_x1)*(v3_v1)-(x3_x1)*(v2_v1)) << 8 ) / delta;

 if (Y1 > Y2) SWAP(v1,v2);
 if (Y1 > Y3) SWAP(v1,v3);
 if (Y2 > Y3) SWAP(v2,v3);

 if (Y1 == Y2) goto FlatTop;
 if (Y2 == Y3) goto FlatBottom;

 slope12 = ((x2_x1)<<16) / (y2_y1);
 slope13 = ((x3_x1)<<16) / (y3_y1);

 if (slope12 > slope13) goto FlatLeft;
 /*if (slope12 < slope13)*/ goto FlatRight;

// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
// 1 2
//  3
 FlatTop:
 {
  if (X1 > X2) SWAP(v1,v2);
  _outerLeftAdder = ((x3_x1)<<16) / (y3_y1);
  _outerRightAdder = (((X3-X2)+1)<<16) / (y3_y1);
  _innerColorAdder = CA;
  _innerTexAdder   = (((short int)UA << 16) + (short int)(VA));
  _outerColorAdder = CA + ((_outerLeftAdder * CB) >> 16);
  _outerTexAdder   = (short int)((UA+((_outerLeftAdder * UB) >> 16)) << 16)
                   + (short int) (VA+((_outerLeftAdder * VB) >> 16));
  GTfiller1( Y1,Y3,(X1<<16),(X2<<16),((U1<<24)+(V1 << 8)),(C1<<8));
  goto FinitoFiller;
 }
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
//  1
// 2 3
 FlatBottom:
 {
  if (X2 > X3) SWAP(v2,v3);
  _outerLeftAdder = ((x2_x1)<<16) / (y3_y1);
  _outerRightAdder = (((X3-X1)+1)<<16) / (y3_y1);
  _innerColorAdder = CA;
  _innerTexAdder   = (((short int)UA << 16) + (short int)(VA));
  _outerColorAdder = CA + ((_outerLeftAdder * CB) >> 16);
  _outerTexAdder   = (short int)((UA+((_outerLeftAdder * UB) >> 16)) << 16)
                   + (short int) (VA+((_outerLeftAdder * VB) >> 16));
  GTfiller1( Y1,Y3,(X1<<16),(X1<<16),((U1<<24)+(V1 << 8)),(C1<<8));
  goto FinitoFiller;
 }
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
//  1
//   2
// 3
 FlatLeft:
 {
  _outerLeftAdder = ((x3_x1)<<16) / (y3_y1);
  _outerRightAdder = (((X2-X1)+1)<<16) / (y2_y1);
  _innerColorAdder = CA;
  _innerTexAdder   = (((short int)UA << 16) + (short int)(VA));
  _outerColorAdder = CA + ((_outerLeftAdder * CB) >> 16);
  _outerTexAdder   = (short int)((UA+((_outerLeftAdder * UB) >> 16)) << 16)
                   + (short int) (VA+((_outerLeftAdder * VB) >> 16));
  GTfiller1( Y1,Y2,(X1<<16),(X1<<16),((U1<<24)+(V1 << 8)),(C1<<8));
  _outerRightAdder = (((X3-X2)+1)<<16) / (Y3-Y2);
  GTfiller2( Y2,Y3);
  goto FinitoFiller;
 }
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
//  1
// 2
//   3
 FlatRight:
 {
  _outerLeftAdder = ((x2_x1)<<16) / (y2_y1);
  _outerRightAdder = (((X3-X1)+1)<<16) / (y3_y1);
  _innerColorAdder = CA;
  _innerTexAdder   = (((short int)UA << 16) + (short int)(VA));
  _outerColorAdder = CA + ((_outerLeftAdder * CB) >> 16);
  _outerTexAdder   = (short int)((UA+((_outerLeftAdder * UB) >> 16)) << 16)
                   + (short int) (VA+((_outerLeftAdder * VB) >> 16));
  GTfiller1( Y1,Y2,(X1<<16),(X1<<16),((U1<<24)+(V1 << 8)),(C1<<8));
  _outerLeftAdder = (((X3-X2)+1)<<16) / (Y3-Y2);
  _outerColorAdder = CA + ((_outerLeftAdder * CB) >> 16);
  _outerTexAdder   = (short int)((UA+((_outerLeftAdder * UB) >> 16)) << 16)
                   + (short int) (VA+((_outerLeftAdder * VB) >> 16));
  GTfiller2( Y2,Y3);
  goto FinitoFiller;
 }
// ùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùùù
 FinitoFiller:
 ;
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void CalcShadeTable(char *palette, char *ShadeTable)
{
 int closest,closestdistance,distance;
 int or,og,ob;
 int cr,cg,cb;
 int Shade, OrigColor, CheckColor;

// memset(ShadeTable,0,64*256);
 for (Shade=1;Shade<64;Shade++)
 {
  for (OrigColor=0;OrigColor<256;OrigColor++)
  {
   or = ( palette[OrigColor*3+0] * 64 ) / Shade;
   og = ( palette[OrigColor*3+1] * 64 ) / Shade;
   ob = ( palette[OrigColor*3+2] * 64 ) / Shade;
   closest = 0;
   closestdistance= 65536;
   for (CheckColor=0;CheckColor<256;CheckColor++)
   {
    cr = palette[CheckColor*3+0];
    cg = palette[CheckColor*3+1];
    cb = palette[CheckColor*3+2];

    distance = (or-cr)*(or-cr) + (og-cg)*(og-cg) + (og-cg)*(og-cg);
    if (distance < closestdistance)
    {
     closestdistance = distance;
     closest = CheckColor;
    }
   }
   ShadeTable[Shade*256+OrigColor] = OrigColor/*closest*/;
  }
 }
}


// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
// ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void *memalloc( size_t size )
{
 void *m;

 m = malloc(size);
 if (m == NULL)
 {
  printf("Error during memory allocation!!!\n");
  printf("Requested bytes: %i\n",size);
  exit(1);
 }
 return m;
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void bench(void)
{
 sVertex v1,v2,v3;
 sPolygon p;
 int i;
 char *pp;
 int x,y;

 for (i=0;i<600;i++)
 {
  mul960[i] = ((i-200)*960 + 320);
 }

 tex = (char *)memalloc(65536);
 _ShadeTable= (char *)memalloc(65536);

 v1.x = 10;
 v1.y = 10;
 v1.u = 0;
 v1.v = 0;
 v1.c = 0;

 v2.x = 15;
 v2.y = 10;
 v2.u = 3;
 v2.v = 0;
 v2.c = 8;

 v3.x = 10;
 v3.y = 15;
 v3.u = 0;
 v3.v = 3;
 v3.c = 16;

 p.vertex1 = &v1;
 p.vertex2 = &v2;
 p.vertex3 = &v3;
 p.texture = tex;

 _Buffer = (char *)memalloc(64000*9);

 for (i=0;i<1000000;i++)
 {
//  DrawPoly_Gouraud(&p);
  DrawPoly_GouraudTexture(&p);
 }
 free(tex);
 free(_ShadeTable);
 free(_Buffer);

}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ


void visible(void)
{
 sVertex v1,v2,v3;
 sPolygon p;
 int i;
 FILE *fp;
 char *pp;
 int x,y;

 printf("\n");
 printf("ş Calculating multiplication table\n");
 for (i=0;i<600;i++)
 {
  mul960[i] = ((i-200)*960 + 320);
 }
 printf("ş allocating memory for Palette\n");
 palette = (char *)memalloc(768);
 fp = fopen("palette.raw","rb");
 printf("ş Loading Palette\n");
 fread(palette,1,768,fp);
 fclose(fp);

 printf("ş allocating memory for Texture\n");
 tex = (char *)memalloc(65536);
 fp = fopen("texture.raw","rb");
 printf("ş Loading Texture\n");
 fread(tex,1,65536,fp);
 fclose(fp);

 printf("ş allocating memory for ShadeTable\n");
 _ShadeTable= (char *)memalloc(65536);
 printf("ş Calculating Shade table\n");
 memset(_ShadeTable,1,65536);
 CalcShadeTable(palette,_ShadeTable);

 v1.x = 10;
 v1.y = 10;
 v1.u = 0;
 v1.v = 0;
 v1.c = 0;

 v2.x = 310;
 v2.y = 100;
 v2.u = 127;
 v2.v = 0;
 v2.c = 31;

 v3.x = 160;
 v3.y = 190;
 v3.u = 0;
 v3.v = 127;
 v3.c = 63;

 p.vertex1 = &v1;
 p.vertex2 = &v2;
 p.vertex3 = &v3;
 p.texture = tex;

 printf("ş Allocating background buffer\n");
 _Buffer = (char *)memalloc(64000*9);
 printf("ş Clearing background buffer\n");
 memset(_Buffer,63,64000*9);

 SetMode(0x13);
/*
 for (i=0;i<64;i++)
 {
  outp(0x3c8,i);
  outp(0x3c9,i);
  outp(0x3c9,i);
  outp(0x3c9,i);
 }
*/
 for (i=0;i<256;i++)
 {
  outp(0x3c8,i);
  outp(0x3c9,palette[i*3]);
  outp(0x3c9,palette[i*3+1]);
  outp(0x3c9,palette[i*3+2]);
 }
 {
//  DrawPoly_Gouraud(&p);
  DrawPoly_GouraudTexture(&p);

 }
 pp = (char *)0xA0000;
 for (y=0;y<200;y++)
 {
  for (x=0;x<320;x++)
  {
   pp[y*320+x] = _Buffer[mul960[y+200]+x];
  }
 }
 getch();
 SetMode(3);

 free(palette);
 free(tex);
 free(_ShadeTable);
 free(_Buffer);
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

// **************************************************************************

void main(void)
{
// bench();
 visible();
}



