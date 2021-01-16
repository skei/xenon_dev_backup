// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
// ± Texture mapping routines ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ

int leftx[200];
int rightx[200];
int lefttx[200];
int leftty[200];
int leftc[200];

extern unsigned char *ShadeTable;

// --------------------
// Texture Mapper v0.2
// --------------------

void TexLeftScan(int y1, int y2, int x, int xadd, int tx, int ty, int txadd, int tyadd)
{
 int i,skip;
 if (y1 > 199) return;
 if (y2 < 0) return;
 if (y1 < 0)
 {
	skip = (0-y1);
  x  += (xadd*skip);
  tx += (txadd*skip);
  ty += (tyadd*skip);
	y1 = 0;
 }
 if (y2 > 199) y2 = 199;
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

 if (y1 > 199) return;
 if (y2 < 0) return;
 if (y1 < 0)
 {
	skip = (0-y1);
	x  += (xadd*skip);
	y1 = 0;
 }
 if (y2 > 199) y2 = 199;
 for (i=y1; i<y2;i++)
 {
	rightx[i] = x >> 8;
	x+=xadd;
 }
}

// -----

// eax = temp
// ebx =
// ecx = xint.xfrac : yint.yfrac
// edx = UseTexture
// esi = adder --"--
// edi = right X (screenpos)
// ebp = negative line length
void TexInner(int neglen, unsigned int tex, unsigned int texadd,
              unsigned char *scrposright, char *texture);
#pragma aux TexInner=\
 " push ebp",\
 " mov ebp,eax",\
 "texinner1:",\
 " xor eax,eax",\
 " shld eax,ecx,8",\
 " mov ah,ch",\
 " add eax,edx",\
 " mov al,[eax]",\
 " mov [edi+ebp],al",\
 " add ecx,esi",\
 " inc ebp",\
 " jnz texinner1",\
 " pop ebp",\
 parm [eax] [ecx] [esi] [edi] [edx];

// -----

void TexFillPoly(unsigned char *buf,int y1,int y2,int txadd,int tyadd, unsigned char *UseTexture)
{
 int i,j,len,x1,x2,tx,ty,skip;
 unsigned char *p;

// int linevisible;

 if (y1 > 199) return;
 if (y2 < 0) return;
 if (y1 < 0) y1 = 0;
 if (y2 > 199) y2 = 199;

 for (i=y1;i<y2;i++)
 {
  tx = lefttx[i];
  ty = leftty[i];

	x1 = leftx[i];
	x2 = rightx[i];

// 	linevisible = 1;

  if (x1 > 319) goto skipthisline;
	if (x2 < 0) goto skipthisline;
  if (x2 > 319) x2 = 319;
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
//   p = &(buf[i*320+x1]);
//   for (j=len;j>0;j--)
//   {
//    *p++ = UseTexture[((ty & 0xff00) + (tx>>8))/* & 0xffff*/];
//    tx+=txadd;
//    ty+=tyadd;
//   }
   TexInner(-len,
            ((tx<<16)+ty),
            ((txadd<<16)+tyadd),
            &(buf[i*320+x2]),
            UseTexture);
  }
 skipthisline:;
 }
}

// -----

void TexMap(unsigned char *buf,int c1, int c2, int c3, int *tric, int *texc, unsigned char *Texture)
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
  TexFillPoly(buf,y1,y3,TxAdder,TyAdder,Texture);
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
  TexFillPoly(buf,y1,y3,TxAdder,TyAdder,Texture);
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
  TexFillPoly(buf,y1,y3,TxAdder,TyAdder,Texture);
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
  TexFillPoly(buf,y1,y3,TxAdder,TyAdder,Texture);
	goto FinitoFiller;
 }
FinitoFiller:;
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

// ---------------------------
// Gouraud Texture Mapper v0.1
// ---------------------------

void GTexLeftScan(int y1, int y2, int x, int xadd, int tx, int ty, int c, int txadd, int tyadd, int cadd)
{
 int i,skip;
 if (y1 > 199) return;
 if (y2 < 0) return;
 if (y1 < 0)
 {
	skip = (0-y1);
  x  += (xadd*skip);
  tx += (txadd*skip);
  ty += (tyadd*skip);
  c += (cadd*skip);
	y1 = 0;
 }
 if (y2 > 199) y2 = 199;
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

 if (y1 > 199) return;
 if (y2 < 0) return;
 if (y1 < 0)
 {
	skip = (0-y1);
	x  += (xadd*skip);
	y1 = 0;
 }
 if (y2 > 199) y2 = 199;
 for (i=y1; i<y2;i++)
 {
	rightx[i] = x >> 8;
	x+=xadd;
 }
}

// -----

void GTexFillPoly(unsigned char *buf,int y1,int y2,int txadd,int tyadd, int cadd, unsigned char *UseTexture, unsigned char *ShadeTable)
{
 int i,j,len,x1,x2,tx,ty,c,skip;
 unsigned char *p;

// int linevisible;

 if (y1 > 199) return;
 if (y2 < 0) return;
 if (y1 < 0) y1 = 0;
 if (y2 > 199) y2 = 199;

 for (i=y1;i<y2;i++)
 {
  tx = lefttx[i];
  ty = leftty[i];
  c  = leftc[i];

	x1 = leftx[i];
	x2 = rightx[i];

// 	linevisible = 1;

  if (x1 > 319) goto skipthisline;
	if (x2 < 0) goto skipthisline;
  if (x2 > 319) x2 = 319;
	if (x1 < 0)
	{
	 skip = (0-x1);
	 tx += (txadd*skip);
	 ty += (tyadd*skip);
   c  += (cadd*skip);
	 x1 = 0;
	}
	len = x2-x1;
  if ((len > 0) /*&& linevisible)*/)
  {
   p = &(buf[i*320+x1]);
   for (j=len;j>0;j--)
   {
    *p++ = ShadeTable[(c & 0xff00) + UseTexture[((ty & 0xff00)+(tx>>8))] ];
    tx+=txadd;
    ty+=tyadd;
    c+=cadd;
   }
  }
 skipthisline:;
 }
}

// -----

void GTexMap(unsigned char *buf,int v1, int v2, int v3, int *tric, int *texc, int *cc,unsigned char *Texture, unsigned char *ShadeTable)
{
 int t;
 int x1,y1,x2,y2,x3,y3,x4;
 int tx1,ty1,tx2,ty2,tx3,ty3,tx4,ty4;
 int c1,c2,c3,c4;
 int Slope,TxAdder,TyAdder,CAdder;
 int slope12, slope13;

 x1 = tric[v1*2];
 y1 = tric[v1*2+1];
 x2 = tric[v2*2];
 y2 = tric[v2*2+1];
 x3 = tric[v3*2];
 y3 = tric[v3*2+1];


 // Perhaps I should use 3 poiners instead... :-/

 if (y1 > y2)
 {
	t = x1; x1 = x2; x2 = t;
	t = y1; y1 = y2; y2 = t;
  t = v1; v1 = v2; v2 = t;
 }
 if (y1 > y3)
 {
	t = x1; x1 = x3; x3 = t;
	t = y1; y1 = y3; y3 = t;
  t = v1; v1 = v3; v3 = t;
 }
 if (y2 > y3)
 {
	t = x2; x2 = x3; x3 = t;
	t = y2; y2 = y3; y3 = t;
  t = v2; v2 = v3; v3 = t;
 }
 if (y1==y3) return;
 if ((y1==y2) && (x1>x2))
 {
	t = x1; x1 = x2; x2 = t;
	t = y1; y1 = y2; y2 = t;
  t = v1; v1 = v2; v2 = t;
 }
 if ((y2==y3) && (x2>x3))
 {
	t = x2; x2 = x3; x3 = t;
	t = y2; y2 = y3; y3 = t;
  t = v2; v2 = v3; v3 = t;
 }
/*
   a     b     c     d

	 1     1    1 2    1
	2       2    3    2 3
    3   3
*/
 tx1 = texc[v1*2];
 ty1 = texc[v1*2+1];
 tx2 = texc[v2*2];
 ty2 = texc[v2*2+1] ;
 tx3 = texc[v3*2];
 ty3 = texc[v3*2+1];

 c1 = cc[v1];
 c2 = cc[v2];
 c3 = cc[v3];

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
  CAdder  = (( c3- c1)<<8) / (y3-y1);

	Slope = ((x3-x1)<<8) / (y3-y1);
  GTexLeftScan(y1,y3,(x1<<8),Slope,(tx1<<8),(ty1<<8),(c1<<8),TxAdder,TyAdder,CAdder);
	Slope = ((x3-x2)<<8) / (y3-y2);
  GTexRightScan(y2,y3,(x2<<8),Slope);
	if (x1 == x2) x2+=1;
  TxAdder = ((tx2-tx1)<<8) / (x2-x1);
	TyAdder = ((ty2-ty1)<<8) / (x2-x1);
  CAdder  = (( c2- c1)<<8) / (x2-x1);
  GTexFillPoly(buf,y1,y3,TxAdder,TyAdder,CAdder,Texture,ShadeTable);
	goto FinitoFiller;
 }

FlatBottom:
// PolyType d
//  1
// 2 3
 {
  TxAdder = ((tx2-tx1)<<8) / (y2-y1);
	TyAdder = ((ty2-ty1)<<8) / (y2-y1);
  CAdder  = (( c2- c1)<<8) / (y2-y1);
  Slope = ((x2-x1)<<8) / (y2-y1);
  GTexLeftScan(y1,y2,(x1<<8),Slope,(tx1<<8),(ty1<<8),(c1<<8),TxAdder,TyAdder,CAdder);
	Slope = ((x3-x1)<<8) / (y3-y1);
  GTexRightScan(y1,y3,(x1<<8),Slope);
	if (x2 == x3) x3+=1;
  TxAdder = ((tx3-tx2)<<8) / (x3-x2);
  TyAdder = ((ty3-ty2)<<8) / (x3-x2);
  CAdder  = (( c3- c2)<<8) / (x3-x2);
  GTexFillPoly(buf,y1,y3,TxAdder,TyAdder,CAdder,Texture,ShadeTable);
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
  CAdder  = (( c3- c1)<<8) / (y3-y1);
  Slope = ((x3-x1)<<8) / (y3-y1);
  GTexLeftScan(y1,y3,(x1<<8),Slope,(tx1<<8),(ty1<<8),(c1<<8),TxAdder,TyAdder,CAdder);
  Slope = ((x2-x1)<<8) / (y2-y1);
  GTexRightScan(y1,y2,(x1<<8),Slope);
  Slope = ((x3-x2)<<8) / (y3-y2);
  GTexRightScan(y2,y3,(x2<<8),Slope);

	x4 = x1+((x2-x1)*(y3-y1))/(y2-y1);
  tx4 = tx1+((tx2-tx1)*(y3-y1))/(y2-y1);
  ty4 = ty1+((ty2-ty1)*(y3-y1))/(y2-y1);
  c4  =  c1+(( c2- c1)*(y3-y1))/(y2-y1);
  if (x3 == x4) x4+=1;
  TxAdder = ((tx4-tx3)<<8) / (x4-x3);
	TyAdder = ((ty4-ty3)<<8) / (x4-x3);
  CAdder  = (( c4- c3)<<8) / (x4-x3);
  GTexFillPoly(buf,y1,y3,TxAdder,TyAdder,CAdder,Texture,ShadeTable);
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
  CAdder  = (( c2- c1)<<8) / (y2-y1);
  Slope = ((x2-x1)<<8) / (y2-y1);
  GTexLeftScan(y1,y2,(x1<<8),Slope,(tx1<<8),(ty1<<8),(c1<<8),TxAdder,TyAdder,CAdder);
  TxAdder = ((tx3-tx2)<<8) / (y3-y2);
  TyAdder = ((ty3-ty2)<<8) / (y3-y2);
  CAdder  = (( c3- c2)<<8) / (y3-y2);
  Slope = ((x3-x2)<<8) / (y3-y2);
  GTexLeftScan(y2,y3,(x2<<8),Slope,(tx2<<8),(ty2<<8),(c2<<8),TxAdder,TyAdder,CAdder);
  Slope = ((x3-x1)<<8) / (y3-y1);
  GTexRightScan(y1,y3,(x1<<8),Slope);

	x4 = x1+((x2-x1)*(y3-y1))/(y2-y1);
  tx4 = tx1+((tx2-tx1)*(y3-y1))/(y2-y1);
  ty4 = ty1+((ty2-ty1)*(y3-y1))/(y2-y1);
  c4  =  c1+(( c2- c1)*(y3-y1))/(y2-y1);
  if (x3 == x4) x4-=1;
  TxAdder = ((tx3-tx4)<<8) / (x3-x4);
	TyAdder = ((ty3-ty4)<<8) / (x3-x4);
  CAdder  = (( c3- c4)<<8) / (x3-x4);
  GTexFillPoly(buf,y1,y3,TxAdder,TyAdder,CAdder,Texture,ShadeTable);
	goto FinitoFiller;
 }
FinitoFiller:;
}

