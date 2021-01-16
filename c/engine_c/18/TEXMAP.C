// texmap.c

#include "defines.h"
#include "types.h"
#include "vars.h"

extern unsigned char PhongMap[];

int leftx[256];
int rightx[256];

int lefttx[256];
int leftty[256];

leftc[256];

// --------------------
// Texture Mapper v0.2
// --------------------

void TexLeftScan(int y1, int y2, int x, int xadd, int tx, int ty, int txadd, int tyadd)
{
 int i,skip;

 if (y1 > 255) return;
 if (y2 < 0) return;
 if (y1 < 0)
 {
	skip = (0-y1);
  x  += (xadd*skip);
  tx += (txadd*skip);
  ty += (tyadd*skip);
	y1 = 0;
 }
 if (y2 > 255) y2 = 255;
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

 if (y1 > 255) return;
 if (y2 < 0) return;
 if (y1 < 0)
 {
	skip = (0-y1);
	x  += (xadd*skip);
	y1 = 0;
 }
 if (y2 > 255) y2 = 255;
 for (i=y1; i<y2;i++)
 {
	rightx[i] = x >> 8;
	x+=xadd;
 }
}

// -----

void TexFillPoly(struct screen_S *vp,int y1,int y2,int txadd,int tyadd, unsigned char *UseTexture)
{
 int i,j,len,x1,x2,tx,ty,skip;
 unsigned short int *p;
// int linevisible;

 if (y1 > 255) return;
 if (y2 < 0) return;
 if (y1 < 0) y1 = 0;
 if (y2 > 255) y2 = 255;

 for (i=y1;i<y2;i++)
 {
  tx = lefttx[i];
  ty = leftty[i];

	x1 = leftx[i];
	x2 = rightx[i];

// 	linevisible = 1;
	
	if (x1 > 255) goto skipthisline;
	if (x2 < 0) goto skipthisline;
	if (x2 > 255) x2 = 255;
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
    *p++ = colortable[UseTexture[ ( (ty>>8)*256 + (tx>>8)) & 0xffff]];
    tx+=txadd;
    ty+=tyadd;
   }

	}
 skipthisline:;
 }
}

void TexMap(struct screen_S *vp,int c1, int c2, int c3, int *tric, int *texc, unsigned char *Texture)
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



// --------------------
// Gouraud Texture Mapper v0.1
// --------------------

void GTexLeftScan(int y1, int y2, int x, int xadd, int tx, int ty, int c, int txadd, int tyadd, int cadd)
{
 int i,skip;

 if (y1 > 255) return;
 if (y2 < 0) return;
 if (y1 < 0)
 {
	skip = (0-y1);
  x  += (xadd*skip);
  tx += (txadd*skip);
  ty += (tyadd*skip);
	c  += (cadd*skip);
	y1 = 0;
 }
 if (y2 > 255) y2 = 255;
 for (i=y1; i<y2;i++)
 {
	leftx[i] = x >> 8;
  lefttx[i] = tx;
	leftty[i] = ty;
	leftc[i] = c;
	x+=xadd;
	tx+=txadd;
	ty+=tyadd;
	c+=cadd;
 }
}

// -----

void GTexRightScan(int y1, int y2, int x, int xadd)
{
 int i,skip;

 if (y1 > 255) return;
 if (y2 < 0) return;
 if (y1 < 0)
 {
	skip = (0-y1);
	x  += (xadd*skip);
	y1 = 0;
 }
 if (y2 > 255) y2 = 255;
 for (i=y1; i<y2;i++)
 {
	rightx[i] = x >> 8;
	x+=xadd;
 }
}

// -----

void GTexFillPoly(struct screen_S *vp,int y1,int y2,int txadd,int tyadd, int cadd, unsigned char *UseTexture)
{
 int i,j,len,x1,x2,tx,ty,c,skip;
 unsigned short int *p;
// int linevisible;

 if (y1 > 255) return;
 if (y2 < 0) return;
 if (y1 < 0) y1 = 0;
 if (y2 > 255) y2 = 255;

 for (i=y1;i<y2;i++)
 {
  tx = lefttx[i];
  ty = leftty[i];
	c = leftc[i];

	x1 = leftx[i];
	x2 = rightx[i];

// 	linevisible = 1;
	
	if (x1 > 255) goto skipthisline;
	if (x2 < 0) goto skipthisline;
	if (x2 > 255) x2 = 255;
	if (x1 < 0)
	{
	 skip = (0-x1);
	 tx += (txadd*skip);
	 ty += (tyadd*skip);
	 c += (cadd*skip);
	 x1 = 0;
	}
	len = x2-x1;
  if (len > 0)
  {
   p = &(vp->screen[i*vp->width+x1]);

   for (j=len;j>0;j--)
   {
		*p++ = ShadeMap[c&0x3f00+(UseTexture[((ty>>8)*256+(tx>>8))&0xffff])];

    tx+=txadd;
    ty+=tyadd;
		c+=cadd;
   }

	}
 skipthisline:;
 }
}

void GTexMap(struct screen_S *vp,int cc1, int cc2, int cc3, int *tric, int *texc, char *colorc, unsigned char *Texture)
{
 int t;
 int x1,y1,x2,y2,x3,y3,x4,c1,c2,c3,c4;
 int tx1,ty1,tx2,ty2,tx3,ty3,tx4,ty4;
 int Slope,TxAdder,TyAdder,CAdder;
 int slope12, slope13;

 x1 = tric[cc1*2];
 y1 = tric[cc1*2+1];
 x2 = tric[cc2*2];
 y2 = tric[cc2*2+1];
 x3 = tric[cc3*2];
 y3 = tric[cc3*2+1];

 // Perhaps I should use 3 poiners instead... :-/
 
 if (y1 > y2)
 {
	t = x1; x1 = x2; x2 = t;
	t = y1; y1 = y2; y2 = t;
	t = cc1; cc1 = cc2; cc2 = t;
 }
 if (y1 > y3)
 {
	t = x1; x1 = x3; x3 = t;
	t = y1; y1 = y3; y3 = t;
	t = cc1; cc1 = cc3; cc3 = t;
 }
 if (y2 > y3)
 {
	t = x2; x2 = x3; x3 = t;
	t = y2; y2 = y3; y3 = t;
	t = cc2; cc2 = cc3; cc3 = t;
 }
 if (y1==y3) return;
 if ((y1==y2) && (x1>x2))
 {
	t = x1; x1 = x2; x2 = t;
	t = y1; y1 = y2; y2 = t;
	t = cc1; cc1 = cc2; cc2 = t;
 }
 if ((y2==y3) && (x2>x3))
 {
	t = x2; x2 = x3; x3 = t;
	t = y2; y2 = y3; y3 = t;
	t = cc2; cc2 = cc3; cc3 = t;
 }

//   a     b     c     d

//	 1     1    1 2    1
//	2       2    3    2 3
//    3   3

 tx1 = texc[cc1*2];
 ty1 = texc[cc1*2+1];
 tx2 = texc[cc2*2];
 ty2 = texc[cc2*2+1] ;
 tx3 = texc[cc3*2];
 ty3 = texc[cc3*2+1];

 c1 = colorc[cc1];
 c2 = colorc[cc2];
 c3 = colorc[cc3];

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
	CAdder = ((c3-c1)<<8) / (y3-y1);
	Slope = ((x3-x1)<<8) / (y3-y1);
	GTexLeftScan(y1,y3,(x1<<8),Slope,(tx1<<8),(ty1<<8),(c1<<8),TxAdder,TyAdder,CAdder);
	Slope = ((x3-x2)<<8) / (y3-y2);
	GTexRightScan(y2,y3,(x2<<8),Slope);
  TxAdder = ((tx2-tx1)<<8) / (x2-x1);
	TyAdder = ((ty2-ty1)<<8) / (x2-x1);
	CAdder = ((c2-c1)<<8) / (x2-x1);
	GTexFillPoly(vp,y1,y3,TxAdder,TyAdder,CAdder,Texture);
	goto FinitoFiller;
 }

FlatBottom: 
// PolyType d 
//  1
// 2 3
 {
  TxAdder = ((tx2-tx1)<<8) / (y2-y1);
	TyAdder = ((ty2-ty1)<<8) / (y2-y1);
	CAdder = ((c2-c1)<<8) / (y2-y1);
	Slope = ((x2-x1)<<8) / (y2-y1);
	GTexLeftScan(y1,y2,(x1<<8),Slope,(tx1<<8),(ty1<<8),(c1<<8),TxAdder,TyAdder,CAdder);
	Slope = ((x3-x1)<<8) / (y3-y1);
	GTexRightScan(y1,y3,(x1<<8),Slope);
  TxAdder = ((tx3-tx2)<<8) / (x3-x2);
	TyAdder = ((ty3-ty2)<<8) / (x3-x2);
	CAdder = ((c3-c2)<<8) / (x3-x2);
	GTexFillPoly(vp,y1,y3,TxAdder,TyAdder,CAdder,Texture);
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
  CAdder = ((c3-c1)<<8) / (y3-y1);
  Slope = ((x3-x1)<<8) / (y3-y1);
  GTexLeftScan(y1,y3,(x1<<8),Slope,(tx1<<8),(ty1<<8),(c1<<8),TxAdder,TyAdder,CAdder);
  Slope = ((x2-x1)<<8) / (y2-y1);
  GTexRightScan(y1,y2,(x1<<8),Slope);
  Slope = ((x3-x2)<<8) / (y3-y2);
  GTexRightScan(y2,y3,(x2<<8),Slope);
 
	x4 = x1+((x2-x1)*(y3-y1))/(y2-y1);
  tx4 = tx1+((tx2-tx1)*(y3-y1))/(y2-y1);
  ty4 = ty1+((ty2-ty1)*(y3-y1))/(y2-y1);
  c4 = c1+((c2-c1)*(y3-y1))/(y2-y1);
	if (x3 == x4) x4+=1;
  TxAdder = ((tx4-tx3)<<8) / (x4-x3);
	TyAdder = ((ty4-ty3)<<8) / (x4-x3);
	CAdder = ((c4-c3)<<8) / (x4-x3);
	GTexFillPoly(vp,y1,y3,TxAdder,TyAdder,CAdder,Texture);
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
  CAdder = ((c2-c1)<<8) / (y2-y1);
  Slope = ((x2-x1)<<8) / (y2-y1);
  GTexLeftScan(y1,y2,(x1<<8),Slope,(tx1<<8),(ty1<<8),(c1<<8),TxAdder,TyAdder,CAdder);
  TxAdder = ((tx3-tx2)<<8) / (y3-y2);
  TyAdder = ((ty3-ty2)<<8) / (y3-y2);
  CAdder = ((c3-c2)<<8) / (y3-y2);
  Slope = ((x3-x2)<<8) / (y3-y2);
  GTexLeftScan(y2,y3,(x2<<8),Slope,(tx2<<8),(ty2<<8),(c2<<8),TxAdder,TyAdder,CAdder);
  Slope = ((x3-x1)<<8) / (y3-y1);
  TexRightScan(y1,y3,(x1<<8),Slope);

	x4 = x1+((x2-x1)*(y3-y1))/(y2-y1);
  tx4 = tx1+((tx2-tx1)*(y3-y1))/(y2-y1);
  ty4 = ty1+((ty2-ty1)*(y3-y1))/(y2-y1);
  c4 = c1+((c2-c1)*(y3-y1))/(y2-y1);
	if (x3 == x4) x4-=1;
  TxAdder = ((tx3-tx4)<<8) / (x3-x4);
	TyAdder = ((ty3-ty4)<<8) / (x3-x4);
	CAdder = ((c3-c4)<<8) / (x3-x4);
	GTexFillPoly(vp,y1,y3,TxAdder,TyAdder,CAdder,Texture);
	goto FinitoFiller;

 }

FinitoFiller:;
}

