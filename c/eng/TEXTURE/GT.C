#include <string.h>
#include <stdlib.h>

extern short int outerTX;
extern short int outerTY;
extern short int outerC;

extern short int outerTXadder;
extern short int outerTYadder;
extern short int outerCadder;

extern short int innerTXadder;
extern short int innerTYadder;
extern short int innerCadder;

extern int LeftX;
extern int RightX;
extern int LeftXadder;
extern int RightXadder;

// -----

// Both the following must be 64k aligned :-/
char *shadetable;
char *texture;

void GTfiller(unsigned char *texture, unsigned char *stable);
#pragma aux GTfiller parm [esi][edi] modify [eax ebx ecx edx];

// --------------------------------------------------------------------------

typedef struct
{
 int x,y;
 int u,v;
 int c;
} sVertex;

// -----

typedef struct
{
 sVertex *v1, *v2, *v3;
} sPoly;

// --------------------------------------------------------------------------

void GouraudTexture(sPoly *p)
{
 sVertex *v1, *v2, *v3, *vtemp;
 int slope12, slope13, slope23;

#define SWAP(a,b) { vtemp = a; a = b; b = vtemp; }

 v1 = p->v1;
 v2 = p->v2;
 v3 = p->v3;

#define x1 v1->x
#define y1 v1->y
#define x2 v2->x
#define y2 v2->y
#define x3 v3->x
#define y3 v3->y

 if (y1>y2) SWAP(v1,v2);
 if (y1>y3) SWAP(v1,v3);
 if (y2>y3) SWAP(v2,v3);
 if (y1==y3) return;
 if ((y1==y2) && (x1>x2)) SWAP(v1,v2);
 if ((y2==y3) && (x2>x3)) SWAP(v2,v3);

 if ( y1 == y2 ) goto FlatTop;
 if ( y2 == y3 ) goto FlatBottom;
 slope12 = (((x2-x1)<<8) / (y2-y1));
 slope13 = (((x3-x1)<<8) / (y3-y1));
// slope23 = (((x3-x2)<<8) / (y3-y2));
 if ( slope12 == slope13 ) return;
 if ( slope12 > slope13 ) goto FlatLeft;
 if ( slope12 < slope13 ) goto FlatRight;
 return;
// ..........................................................................
#define tx1 v1->u
#define ty1 v1->v
#define c1  v1->c
#define tx2 v2->u
#define ty2 v2->v
#define c2  v2->c
#define tx3 v3->u
#define ty3 v3->v
#define c3  v3->c

// ..........................................................................
 FlatTop:
 {
  // 1 2
  //  3
  outerTX = (tx1 << 8);
  outerTXadder = ((tx3-tx1)<<8)/(y3-y1);
  innerTXadder = ((tx2-tx1)<<8)/(x2-x1);
  outerTY = (ty1 << 8);
  outerTYadder = ((ty3-ty1)<<8)/(y3-y1);
  innerTYadder = ((ty2-ty1)<<8)/(x2-x1);
  outerC  = (c1 << 8);
  outerCadder  = (( c3- c1)<<8)/(y3-y1);
  innerCadder  = (( c2- c1)<<8)/(x2-x1);
  LeftX  = ((320+x1)<<16) + (y1*960);
  LeftXadder  = slope13;
  RightX = ((320+x2)<<16) + (y1*960);
  RightXadder = (((x3-x2)<<8) / (y3-y2));
  GTfiller(texture, shadetable);
  return;
 }
// ..........................................................................
 FlatBottom:
 {
  //  1
  // 2 3
  return;
 }
// ..........................................................................
 FlatLeft:
 {
  //  1
  //   2
  // 3
  return;
 }
// ..........................................................................
 FlatRight:
 {
  //  1
  // 2
  //   3
  return;
 }
// ..........................................................................
// FinitoFiller:;
}

// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様

void main(void)
{
 sVertex v1,v2,v3;
 sPoly p;

 texture = (char *)malloc(65536*2);
 texture += ( (int)texture & 0xffff );
 shadetable = (char *)malloc(65536*2);
 shadetable += ( (int)shadetable & 0xffff );
 memset(shadetable,32768,15);

 v1.x = 10;
 v1.y = 10;
 v1.u = 0;
 v1.v = 0;
 v1.c = 0;
 p.v1 = &v1;
 v2.x = 310;
 v2.y = 10;
 v2.u = 127;
 v2.v = 0;
 v2.c = 31;
 p.v2 = &v2;
 v3.x = 160;
 v3.y = 190;
 v3.u = 127;
 v3.v = 127;
 v3.c = 63;
 p.v3 = &v3;

 SetMode(0x13);
 GouraudTexture(&p);

}



