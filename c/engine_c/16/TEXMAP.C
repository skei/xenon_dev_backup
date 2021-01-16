// texmap.c

#include "defines.h"
#include "types.h"

#ifdef VisCVersion
 #include "winmain.h"
#else
 #include "dosmain.h"
#endif


#ifdef VisCVersion
 extern unsigned short int colortable[256];
#endif

int leftx[SCREEN_HEIGHT];
int rightx[SCREEN_HEIGHT];

int lefttx[SCREEN_HEIGHT];
int leftty[SCREEN_HEIGHT];

leftc[SCREEN_HEIGHT];

// --------------------
// Texture Mapper v0.2
// --------------------

void TexLeftScan(int y1, int y2, int x, int xadd, int tx, int ty, int txadd, int tyadd)
{
 int i,skip;

 if (y1 > (SCREEN_HEIGHT-1)) return;
 if (y2 < 0) return;
 if (y1 < 0)
 {
	skip = (0-y1);
  x  += (xadd*skip);
  tx += (txadd*skip);
  ty += (tyadd*skip);
	y1 = 0;
 }
 if (y2 > (SCREEN_HEIGHT-1)) y2 = (SCREEN_HEIGHT-1);
 for (i=y1; i<y2;i++)
 {
	leftx[i] = x >> 8;
  lefttx[i] = tx;
	leftty[i] = ty;
	x+=xadd;
	tx+=txadd;
	ty+=tyadd;
 }
}

// -----

void TexRightScan(int y1, int y2, int x, int xadd)
{
 int i,skip;

 if (y1 > (SCREEN_HEIGHT-1)) return;
 if (y2 < 0) return;
 if (y1 < 0)
 {
	skip = (0-y1);
	x  += (xadd*skip);
	y1 = 0;
 }
 if (y2 > (SCREEN_HEIGHT-1)) y2 = (SCREEN_HEIGHT-1);
 for (i=y1; i<y2;i++)
 {
	rightx[i] = x >> 8;
	x+=xadd;
 }
}

// -----

void TexFillPoly(screen_S *vp,int y1,int y2,int txadd,int tyadd, unsigned char *UseTexture)
{
 int i,j,len,x1,x2,tx,ty,skip;
#ifdef VisCVersion
 unsigned short int *p;
#else
 unsigned char *p;
#endif

// int linevisible;

 if (y1 > (SCREEN_HEIGHT-1)) return;
 if (y2 < 0) return;
 if (y1 < 0) y1 = 0;
 if (y2 > (SCREEN_HEIGHT-1)) y2 = (SCREEN_HEIGHT-1);

 for (i=y1;i<y2;i++)
 {
  tx = lefttx[i];
  ty = leftty[i];

	x1 = leftx[i];
	x2 = rightx[i];

// 	linevisible = 1;

	if (x1 > (SCREEN_WIDTH-1)) goto skipthisline;
	if (x2 < 0) goto skipthisline;
	if (x2 > (SCREEN_WIDTH-1)) x2 = (SCREEN_WIDTH-1);
	if (x1 < 0)
	{
	 skip = (0-x1);
	 tx += (txadd*skip);
	 ty += (tyadd*skip);
	 x1 = 0;
	}
	len = x2-x1;
  if ((len > 0) /*&& linevisible)*/)
  {
   p = &(vp->screen[i*vp->width+x1]);

   for (j=len;j>0;j--)
   {
#ifdef VisCVersion
		*p++ = colortable[UseTexture[ ( (ty>>8)*256 + (tx>>8)) & 0xffff]];
#else
		*p++ = UseTexture[ ( (ty>>8)*256 + (tx>>8)) & 0xffff];
#endif

    tx+=txadd;
    ty+=tyadd;
   }

	}
 skipthisline:;
 }
}

// -----

void TexMap(screen_S *vp,int c1, int c2, int c3, int *tric, int *texc, unsigned char *Texture)
{
 int t;
 int x1,y1,x2,y2,x3,y3,x4;
 int tx1,ty1,tx2,ty2,tx3,ty3,tx4,ty4;
 int Slope,TxAdder,TyAdder;
 int slope12, slope13;

 x1 = tric[c1*2];
 y1 = tric[c1*2+1];
 x2 = tric[c2*2];
 y2 = tric[c2*2+1];
 x3 = tric[c3*2];
 y3 = tric[c3*2+1];

 // Perhaps I should use 3 poiners instead... :-/

 if (y1 > y2)
 {
	t = x1; x1 = x2; x2 = t;
	t = y1; y1 = y2; y2 = t;
	t = c1; c1 = c2; c2 = t;
 }
 if (y1 > y3)
 {
	t = x1; x1 = x3; x3 = t;
	t = y1; y1 = y3; y3 = t;
	t = c1; c1 = c3; c3 = t;
 }
 if (y2 > y3)
 {
	t = x2; x2 = x3; x3 = t;
	t = y2; y2 = y3; y3 = t;
	t = c2; c2 = c3; c3 = t;
 }
 if (y1==y3) return;
 if ((y1==y2) && (x1>x2))
 {
	t = x1; x1 = x2; x2 = t;
	t = y1; y1 = y2; y2 = t;
	t = c1; c1 = c2; c2 = t;
 }
 if ((y2==y3) && (x2>x3))
 {
	t = x2; x2 = x3; x3 = t;
	t = y2; y2 = y3; y3 = t;
	t = c2; c2 = c3; c3 = t;
 }
/*
   a     b     c     d

	 1     1    1 2    1
	2       2    3    2 3
    3   3
*/
 tx1 = texc[c1*2];
 ty1 = texc[c1*2+1];
 tx2 = texc[c2*2];
 ty2 = texc[c2*2+1] ;
 tx3 = texc[c3*2];
 ty3 = texc[c3*2+1];

 if ( y1 == y2 ) goto FlatTop;
 if ( y2 == y3 ) goto FlatBottom;
 slope12 = (((x2-x1)<<8) / (y2-y1));
 slope13 = (((x3-x1)<<8) / (y3-y1));
// slope23 = (((x3-x2)<<8) / (y3-y2));
 if ( slope12 == slope13 ) return;
 if ( slope12 > slope13 ) goto FlatLeft;
 if ( slope12 < slope13 ) goto FlatRight;
 return;
 
// PolyType c
// 1 2
//  3
FlatTop:
 {
	TxAdder = ((tx3-tx1)<<8) / (y3-y1);
	TyAdder = ((ty3-ty1)<<8) / (y3-y1);
	Slope = ((x3-x1)<<8) / (y3-y1);
	TexLeftScan(y1,y3,(x1<<8),Slope,(tx1<<8),(ty1<<8),TxAdder,TyAdder);
	Slope = ((x3-x2)<<8) / (y3-y2);
	TexRightScan(y2,y3,(x2<<8),Slope);
	if (x1 == x2) x2+=1;
  TxAdder = ((tx2-tx1)<<8) / (x2-x1);
	TyAdder = ((ty2-ty1)<<8) / (x2-x1);
	TexFillPoly(vp,y1,y3,TxAdder,TyAdder,Texture);
	goto FinitoFiller;
 }

FlatBottom: 
// PolyType d 
//  1
// 2 3
 {
  TxAdder = ((tx2-tx1)<<8) / (y2-y1);
	TyAdder = ((ty2-ty1)<<8) / (y2-y1);
	Slope = ((x2-x1)<<8) / (y2-y1);
	TexLeftScan(y1,y2,(x1<<8),Slope,(tx1<<8),(ty1<<8),TxAdder,TyAdder);
	Slope = ((x3-x1)<<8) / (y3-y1);
	TexRightScan(y1,y3,(x1<<8),Slope);
	if (x2 == x3) x3+=1;
  TxAdder = ((tx3-tx2)<<8) / (x3-x2);
	TyAdder = ((ty3-ty2)<<8) / (x3-x2);
	TexFillPoly(vp,y1,y3,TxAdder,TyAdder,Texture);
	goto FinitoFiller;
 }

FlatLeft:
// PolyType b
//  1
//   2
// 3
 {
  TxAdder = ((tx3-tx1)<<8) / (y3-y1);
  TyAdder = ((ty3-ty1)<<8) / (y3-y1);
  Slope = ((x3-x1)<<8) / (y3-y1);
  TexLeftScan(y1,y3,(x1<<8),Slope,(tx1<<8),(ty1<<8),TxAdder,TyAdder);
  Slope = ((x2-x1)<<8) / (y2-y1);
  TexRightScan(y1,y2,(x1<<8),Slope);
  Slope = ((x3-x2)<<8) / (y3-y2);
  TexRightScan(y2,y3,(x2<<8),Slope);
 
	x4 = x1+((x2-x1)*(y3-y1))/(y2-y1);
  tx4 = tx1+((tx2-tx1)*(y3-y1))/(y2-y1);
  ty4 = ty1+((ty2-ty1)*(y3-y1))/(y2-y1);
	if (x3 == x4) x4+=1;
  TxAdder = ((tx4-tx3)<<8) / (x4-x3);
	TyAdder = ((ty4-ty3)<<8) / (x4-x3);
	TexFillPoly(vp,y1,y3,TxAdder,TyAdder,Texture);
	goto FinitoFiller;
 }

FlatRight:
// PolyType a
//  1
// 2
//   3
 {
  TxAdder = ((tx2-tx1)<<8) / (y2-y1);
  TyAdder = ((ty2-ty1)<<8) / (y2-y1);
  Slope = ((x2-x1)<<8) / (y2-y1);
  TexLeftScan(y1,y2,(x1<<8),Slope,(tx1<<8),(ty1<<8),TxAdder,TyAdder);
  TxAdder = ((tx3-tx2)<<8) / (y3-y2);
  TyAdder = ((ty3-ty2)<<8) / (y3-y2);
  Slope = ((x3-x2)<<8) / (y3-y2);
  TexLeftScan(y2,y3,(x2<<8),Slope,(tx2<<8),(ty2<<8),TxAdder,TyAdder);
  Slope = ((x3-x1)<<8) / (y3-y1);
  TexRightScan(y1,y3,(x1<<8),Slope);

	x4 = x1+((x2-x1)*(y3-y1))/(y2-y1);
  tx4 = tx1+((tx2-tx1)*(y3-y1))/(y2-y1);
  ty4 = ty1+((ty2-ty1)*(y3-y1))/(y2-y1);
	if (x3 == x4) x4-=1;
  TxAdder = ((tx3-tx4)<<8) / (x3-x4);
	TyAdder = ((ty3-ty4)<<8) / (x3-x4);
	TexFillPoly(vp,y1,y3,TxAdder,TyAdder,Texture);
	goto FinitoFiller;

 }

FinitoFiller:;
}

// -----------------------------------------------
