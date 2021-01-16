// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
// ± Texture mapping routines ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ

int leftx[200];
int rightx[200];
int leftc[200];
int leftc[200];

// --------------------
// Texture Mapper v0.2
// --------------------

void GLeftScan(int y1, int y2, int x, int xadd, int c, int cadd)
{
 int i,skip;
 if (y1 > 199) return;
 if (y2 < 0) return;
 if (y1 < 0)
 {
	skip = (0-y1);
  x  += (xadd*skip);
  c += (cadd*skip);
  y1 = 0;
 }
 if (y2 > 199) y2 = 199;
 for (i=y1; i<y2;i++)
 {
	leftx[i] = x >> 8;
  leftc[i] = c;
  x+=xadd;
  c+=cadd;
 }
}

// -----

void GRightScan(int y1, int y2, int x, int xadd)
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

void GFillPoly(unsigned char *buf,int y1,int y2,int cadd)
{
 int i,j,len,x1,x2,c,skip;
 unsigned char *p;

// int linevisible;

 if (y1 > 199) return;
 if (y2 < 0) return;
 if (y1 < 0) y1 = 0;
 if (y2 > 199) y2 = 199;

 for (i=y1;i<y2;i++)
 {
  c = leftc[i];
  x1 = leftx[i];
	x2 = rightx[i];

// 	linevisible = 1;

  if (x1 > 319) goto skipthisline;
	if (x2 < 0) goto skipthisline;
  if (x2 > 319) x2 = 319;
	if (x1 < 0)
	{
	 skip = (0-x1);
   c += (cadd*skip);
   x1 = 0;
	}
	len = x2-x1;
  if ((len > 0) /*&& linevisible)*/)
  {
   p = &(buf[i*320+x1]);
   for (j=len;j>0;j--)
   {
    *p++ = c >> 8;
    c+=cadd;
   }
  }
 skipthisline:;
 }
}

// -----

void Gouraud(unsigned char *buf,int v1, int v2, int v3, int *tric, int *cc)
{
 int t;
 int x1,y1,x2,y2,x3,y3,x4;
 int c1,c2,c3,c4;
 int Slope,CAdder;
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
  CAdder = ((c3-c1)<<8) / (y3-y1);
  Slope = ((x3-x1)<<8) / (y3-y1);
  GLeftScan(y1,y3,(x1<<8),Slope,(c1<<8),CAdder);
	Slope = ((x3-x2)<<8) / (y3-y2);
  GRightScan(y2,y3,(x2<<8),Slope);
	if (x1 == x2) x2+=1;
  CAdder = ((c2-c1)<<8) / (x2-x1);
  GFillPoly(buf,y1,y3,CAdder);
	goto FinitoFiller;
 }

FlatBottom:
// PolyType d
//  1
// 2 3
 {
  CAdder = ((c2-c1)<<8) / (y2-y1);
  Slope = ((x2-x1)<<8) / (y2-y1);
  GLeftScan(y1,y2,(x1<<8),Slope,(c1<<8),CAdder);
	Slope = ((x3-x1)<<8) / (y3-y1);
  GRightScan(y1,y3,(x1<<8),Slope);
	if (x2 == x3) x3+=1;
  CAdder = ((c3-c2)<<8) / (x3-x2);
  GFillPoly(buf,y1,y3,CAdder);
	goto FinitoFiller;
 }

FlatLeft:
// PolyType b
//  1
//   2
// 3
 {
  CAdder = ((c3-c1)<<8) / (y3-y1);
  Slope = ((x3-x1)<<8) / (y3-y1);
  GLeftScan(y1,y3,(x1<<8),Slope,(c1<<8),CAdder);
  Slope = ((x2-x1)<<8) / (y2-y1);
  GRightScan(y1,y2,(x1<<8),Slope);
  Slope = ((x3-x2)<<8) / (y3-y2);
  GRightScan(y2,y3,(x2<<8),Slope);

	x4 = x1+((x2-x1)*(y3-y1))/(y2-y1);
  c4 = c1+((c2-c1)*(y3-y1))/(y2-y1);
  if (x3 == x4) x4+=1;
  CAdder = ((c4-c3)<<8) / (x4-x3);
  GFillPoly(buf,y1,y3,CAdder);
	goto FinitoFiller;
 }

FlatRight:
// PolyType a
//  1
// 2
//   3
 {
  CAdder = ((c2-c1)<<8) / (y2-y1);
  Slope = ((x2-x1)<<8) / (y2-y1);
  GLeftScan(y1,y2,(x1<<8),Slope,(c1<<8),CAdder);
  CAdder = ((c3-c2)<<8) / (y3-y2);
  Slope = ((x3-x2)<<8) / (y3-y2);
  GLeftScan(y2,y3,(x2<<8),Slope,(c2<<8),CAdder);
  Slope = ((x3-x1)<<8) / (y3-y1);
  GRightScan(y1,y3,(x1<<8),Slope);

	x4 = x1+((x2-x1)*(y3-y1))/(y2-y1);
  c4 = c1+((c2-c1)*(y3-y1))/(y2-y1);
  if (x3 == x4) x4-=1;
  CAdder = ((c3-c4)<<8) / (x3-x4);
  GFillPoly(buf,y1,y3,CAdder);
	goto FinitoFiller;
 }
FinitoFiller:;
}

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ


