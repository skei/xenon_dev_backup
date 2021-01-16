typedef struct sVector
{
 float x,y,z;
} sVector;

// 컴컴컴컴컴

typedef struct sVertex
{
 sVector PosR;
 float u,v;
 float c;
} sVertex;

// 컴컴컴컴컴

typedef struct sPolygon
{
 sVertex *v1,*v2,*v3;
} sPolygon;

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

// (16.16 * 16.16) >> 16
// (16.16 *  8.8 ) >> 16
int fixedmul(int, int);
#pragma aux fixedmul=\
 "imul ebx",\
 "shrd eax,edx,16",\
 parm [eax] [ebx] value [eax] modify [edx];

// (16.16 << 16) / 16.16
// ( 8.8  << 16) / 16.16
int fixeddiv(int,int);
#pragma aux fixeddiv=\
 "cdq",\
 "shld edx,eax,16",\
 "idiv ebx",\
 parm [eax][ebx] value [eax] modify [edx];

// --------------------------------------------------------------------------

#define S 65536.0
#define MAGIC (((S*S*16)+(S*.5))*S)
#define MAGICfixed (MAGIC/65536.0)

// 컴컴컴컴컴

#pragma inline float2int;
static long float2int( float d )
{
 double dtemp = MAGIC+d;
 return (*(long *)&dtemp)-0x80000000;
}

// 컴컴컴컴컴

#pragma inline float2fixed;
static long float2fixed( float d )
{
 double dtemp = MAGICfixed+d;
 return (*(long *)&dtemp)-0x80000000;
}

// 컴컴컴컴컴

#pragma inline CEIL;
static int CEIL(float f)
{
 int i,j;

 i = float2fixed(f);
 j = i >> 16;
 if ((i&0xffff) != 0) j+=1;
 return j;
}

// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

int Lslope,Rslope;
int _outerUadder,_innerUadder;
int _outerVadder,_innerVadder;

// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

void Polygon ( sPolygon *p )
{
 sVertex *vv1,*vv2,*vv3,*t;
 float y2_y1, y3_y2;
 float inv_y2_y1, inv_y3_y2;
 float x2_x1, x3_x2;
 float inv_x2_x1, inv_x3_x2;
 float Slope12,Slope13;
 float prestep;

 #define SWAP(a,b) { t=a; a=b; b=t; }

 vv1 = p->v1;
 vv2 = p->v2;
 vv3 = p->v3;

 #define X1 (vv1->PosR.x)
 #define Y1 (vv1->PosR.y)
 #define U1 (vv1->u)
 #define V1 (vv1->v)


 #define X2 (vv2->PosR.x)
 #define Y2 (vv2->PosR.y)
 #define U2 (vv2->u)
 #define V2 (vv2->v)

 #define X3 (vv3->PosR.x)
 #define Y3 (vv3->PosR.y)
 #define U3 (vv3->u)
 #define V3 (vv3->v)

 if (Y1<Y2) SWAP (vv1,vv2);
 if (Y1<Y3) SWAP (vv1,vv3);
 if (Y2<Y3) SWAP (vv2,vv3);

 if (Y1==Y3) return;

 if (Y1==Y2) goto FlatTop;
 if (Y2==Y3) goto FlatBottom;
 Slope12 = (X2-X1)/(Y2-Y1);
 Slope13 = (X3-X1)/(Y3-Y1);
 if (Slope12 > Slope13) goto Flatleft;
 goto FlatRight;

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 FlatTop:
// 1 2     2 1
//  3       3
 {
  int i,j;

  if (X1<X2) SWAP (vv1,vv2);
  y2 = CEIL( Y2 );
  i = float2fixed(Y1);
  y1 = i >> 16;
  if ((i&0xffff) != 0)
  {
   prestep =
   y1+=1;
  }
  else
  {
  }


  x1 = float2fixed(X1 + (xslope*prestep)) + (1<<15);



  y2_y1 = Y2-Y1;
  inv_y2_y1 = 1/y2_y1;





  Lslope       = float2fixed((X2-X1)*inv_y2_y1);
  Rslope       = float2fixed((X3-X2)*inv_y2_y1);
  _outerUadder = float2fixed((U3-U1)*inv_y2_y1);
  _outerVadder = float2fixed((V3-V1)*inv_y2_y1);

  x2_x1 = X2-X1;
  inv_x2_x1 = 1/x2_x1;

  _innerUadder = float2fixed((U2-U1)*inv_x2_x1);
  _innerVadder = float2fixed((V2-V1)*inv_x2_x1);
  return;
 }
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 FlatBottom:
//  1      1
// 2 3    3 2
 {
  if (X2<X3) SWAP (vv2,vv3);
  return;
 }
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 FlatLeft:
//  1
//   2
// 3
 {
  return;
 }
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 FlatRight:
//  1
// 2
//   3
 {
  return;
 }
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

}

