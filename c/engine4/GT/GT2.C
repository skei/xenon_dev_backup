// Gouraud shaded Texture Mapping...
// Axon / Xenon & Inf <ÄÍÍÍÄ> (Tor-Helge Skei)

// 10.49 = 10.00 = 95.328 25-pixel polys p/sek

// ÄÄÄÄÄ Testing functions & variables ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

#include <conio.h>      // getch,outp
#include <stdio.h>      // printf
#include <stdlib.h>     // malloc,abs
#include <string.h>     // memset

// #pragma inline GTfiller();   Funker noe s†nt, mon tro???

void SetMode(short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [eax];



// ÄÄÄÄÄ Typedef ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

typedef struct
{
 int x,y;
// int px,py,pz;  // position
// int nx,ny,nz;  // normal
 int u,v;
 int c;
} sRenderVertex;

// ÄÄÄÄÄ

typedef struct
{
 sRenderVertex *v1,*v2,*v3;
 char *texture;
} sPoly;

// ÄÄÄÄÄ

sPoly *RenderList;

// ÄÄÄÄÄ

/*

typedef struct
{
 int px,py,pz;  // position
 int nx,ny,nz;  // normal
 int tu,tv;
 int color;
} sVertex;

typedef struct
{
 int vCount;      // vertex count
 int fCount;      // face count
 int *fData;      // face data (array of 3*fCount vertex numbers)
 char *Texture;   // Group texture
 int RenderFlags  // rendering type (flat, gouraud, etc..)
} sGroup;

typedef struct
{
 int xpos,ypos,zpos;
 int xang,yang,zang;
 int NumGroups;
 sGroup *Groups;
} sObject;

*/

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

// ÄÄÄÄÄ The polyfiller innerloop ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

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

void GTfiller(int y1, int y2, int left, int right, int tex, int c)
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
  GTinnerloop(
               (char *)((int)_Buffer+mul960[i+200]+(left>>16)),
               tex,
               c,
//               (right >> 16) - (left>>16)
               (right - left) >> 16

              );
  tex   += _outerTexAdder;
  c     += _outerColorAdder;
  left  += _outerLeftAdder;
  right += _outerRightAdder;
 }
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ


void GTpoly(sPoly *p)
{
 sRenderVertex *v1,*v2,*v3,*temp;
 int y2_y1, y3_y1, y3_y2;
 int x2_x1, x3_x1, x3_x2;
 int slope12, slope13;

 _Texture = p->texture;

 #define SWAP(a,b) { temp=a; a=b; b=temp; }

 v1 = p->v1;
 v2 = p->v2;
 v3 = p->v3;

 if ( v1->y > v2->y ) SWAP(v1,v2);
 if ( v1->y > v3->y ) SWAP(v1,v3);
 if ( v2->y > v3->y ) SWAP(v2,v3);

 y3_y1 = (v3->y - v1->y);
 if (y3_y1 == 0) return;
 if ((v1->y == v2->y) && (v1->x > v2->x)) SWAP(v1,v2);
 if ((v2->y == v3->y) && (v2->x > v3->x)) SWAP(v2,v3);

// poly is one of the following:
// FlatLeft  FlatRight  FlatTop  FlatBottom
// ----------------------------------------
//  1         1
//   2       2          1 2       1
// 3           3         3       2 3

 y2_y1 = (v2->y - v1->y);
 if (y2_y1 == 0) goto FlatTop;
 if ((v3->y - v2->y) == 0) goto FlatBottom;
 slope12 = ((v2->x - v1->x)<<16) / y2_y1;
 slope13 = ((v3->x - v1->x)<<16) / y3_y1;
 if (slope12 == slope13) return;
 if ( slope12 > slope13 ) goto FlatLeft;
 goto FlatRight;

// ..........................................................................
 FlatTop:
 // 1 2
 //  3
 {
  x2_x1 = ( v2->x - v1->x );
  _innerColorAdder = ((v2->c - v1->c) << 8) / x2_x1;
  _innerTexAdder   = ((short int)(((v2->u - v1->u)<<8)/x2_x1) << 16)
                    + (short int)(((v2->v - v1->v)<<8)/x2_x1);

  _outerTexAdder   = ((short int)(((v3->u - v1->u)<<8)/y3_y1) << 16)
                    + (short int)(((v3->v - v1->v)<<8)/y3_y1);
  _outerColorAdder = ((v3->c - v1->c) << 8 ) / y3_y1;
  _outerLeftAdder  = ((v3->x - v1->x) << 16) / y3_y1;
  _outerRightAdder = ((v3->x - v2->x) << 16) / y3_y1;
  GTfiller(
           v1->y,
           v3->y,
           v1->x << 16,
           v2->x << 16,
           ((v1->u << 24) + (v1->v << 8)),
           v1->c << 8 );
  goto FinitoGT;
 }
// ..........................................................................
 FlatBottom:
 //  1
 // 2 3
 {
  x3_x2 = ( v3->x - v2->x );
  _innerColorAdder = ((v3->c - v2->c) << 8) / x3_x2;
  _innerTexAdder   = ((short int)(((v3->u - v2->u)<<8)/x3_x2) << 16)
                    + (short int)(((v3->v - v2->v)<<8)/x3_x2);

  _outerTexAdder   = ((short int)(((v2->u - v1->u)<<8)/y3_y1) << 16)
                    + (short int)(((v2->v - v1->v)<<8)/y3_y1);
  _outerColorAdder = ((v2->c - v1->c) << 8 ) / y3_y1;
  _outerLeftAdder  = ((v2->x - v1->x) << 16) / y3_y1;
  _outerRightAdder = ((v3->x - v1->x) << 16) / y3_y1;
  GTfiller(
           v1->y,
           v3->y,
           v1->x << 16,
           v1->x << 16,
           ((v1->u << 24) + (v1->v << 8)),
           v1->c << 8 );
  goto FinitoGT;
 }
// ..........................................................................
 FlatLeft:
 //  1
 //   2
 // 3 (4)
 {
  x4 = v1->x + (((v2->x - v1->x) * y3_y1) / y2_y1);
  u4 = v1->u + (((v2->u - v1->u) * y3_y1) / y2_y1);
  v4 = v1->v + (((v2->v - v1->v) * y3_y1) / y2_y1);
  c4 = v1->c + (((v2->c - v1->c) * y3_y1) / y2_y1);

  x4_x3 = ( x4 - v3->x );
  _innerColorAdder = ((c4 - v3->c) << 8) / x4_x3;
  _innerTexAdder   = ((short int)(((u4 - v3->u)<<8)/x4_x3) << 16)
                    + (short int)(((v4 - v3->v)<<8)/x4_x3);

  _outerTexAdder   = ((short int)(((v3->u - v1->u)<<8)/y3_y1) << 16)
                    + (short int)(((v3->v - v1->v)<<8)/y3_y1);
  _outerColorAdder = ((v3->c - v1->c) << 8 ) / y3_y1;
  _outerLeftAdder  = ((v3->x - v1->x) << 16) / y3_y1;
  _outerRightAdder = ((v2->x - v1->x) << 16) / y2_y1;
  GTfiller(
           v1->y,
           v2->y,
           v1->x << 16,
           v1->x << 16,
           ((v1->u << 24) + (v1->v << 8)),
           v1->c << 8 );
  _outerRightAdder = ((v3->x - v2->x) << 16) / (v3->y - v2->y);
  GTfiller(
           v2->y,
           v3->y,
           v1->x << 16,
           v1->x << 16,
           ((v1->u << 24) + (v1->v << 8)),
           v1->c << 8 );




  goto FinitoGT;
 }
// ..........................................................................
 FlatRight:
 //   1
 //  2
 //(4) 3
 {
  goto FinitoGT;
 }
// ..........................................................................
 FinitoGT:
 ;
}

// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ

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

// --------------------------------------------------------------------------
void visible(void)
{
 sRenderVertex v1,v2,v3;
 sPoly p;
 int i;
 FILE *fp;
 char *pp;
 int x,y;

 printf("\n");
 // init mul960 table
 printf("ş Calculating multiplication table\n");
 for (i=0;i<600;i++)
 {
  mul960[i] = ((i-200)*960 + 320);
 }
 printf("ş Loading Palette\n");
 palette = (char *)malloc(768);
 fp = fopen("palette.raw","rb");
 fread(palette,1,768,fp);
 fclose(fp);

 printf("ş Loading Texture\n");
 tex = (char *)malloc(65536);
 fp = fopen("texture.raw","rb");
 fread(tex,1,65536,fp);
 fclose(fp);

 printf("ş Calculating Shade table\n");
 _ShadeTable= (char *)malloc(65536);
 memset(_ShadeTable,1,65536);
 CalcShadeTable(palette,_ShadeTable);

 v1.x = 160;
 v1.y = 10;
 v1.u = 0;
 v1.v = 0;
 v1.c = 0;

 v2.x = 10;
 v2.y = 190;
 v2.u = 127;
 v2.v = 0;
 v2.c = 31;

 v3.x = 310;
 v3.y = 190;
 v3.u = 127;
 v3.v = 127;
 v3.c = 63;

 p.v1 = &v1;
 p.v2 = &v2;
 p.v3 = &v3;
 p.texture = tex;

 printf("ş Allocating background buffer\n");
 _Buffer = (char *)malloc(64000*9);
 memset(_Buffer,0,64000*9);

 SetMode(0x13);
 for (i=0;i<256;i++)
 {
  outp(0x3c8,i);
  outp(0x3c9,palette[i*3]);
  outp(0x3c9,palette[i*3+1]);
  outp(0x3c9,palette[i*3+2]);
 }
 GTpoly(&p);
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
}

// --------------------------------------------------------------------------

void main(void)
{
 visible();
}


