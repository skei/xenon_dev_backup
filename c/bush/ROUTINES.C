#include "routines.h"
#include "std.h"
#include "fmath.h"

//
// ÄÄÄÄÄ Variables ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

int Rseed;
int Mul320[200];
int Skurr_Array[200][SkurrNum];

//
// ÄÄÄÄÄ Functions ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

void SetRGB(unsigned char color,char r,char g,char b)
 {
  outp(0x3c8,color);
  outp(0x3c9,r);
  outp(0x3c9,g);
  outp(0x3c9,b);
 }

 // ---

void WaitVR(void)
 {
  while ((inp(0x3da)&8)!=0);
  while ((inp(0x3da)&8)==0);
 }

 // ---

void flash (char *pal, int num)
 {
  int i;

  for (i=0;i<256;i++) SetRGB(i,63,63,63);
  for (i=0;i<num;i++) WaitVR();
  for (i=0;i<256;i++) SetRGB(i,pal[i*3],pal[i*3+1],pal[i*3+2]);
 }

 // ---

void BlackPalette(void)
 {
  int i;

  for (i=0;i<256;i++) SetRGB(i,0,0,0);
 }

 // ---

void SetPalette( int from, int to, char *Pal)
 {
  int i;

  for (i=from;i<=to;i++) SetRGB(i,Pal[i*3],Pal[i*3+1],Pal[i*3+2]);
 }




 // Calculate background
 // buffer = 64k buffer
 // addcolor = lowest color to use (added to image after creation

void CalcBackground(char *buffer, int addcolor)
 {
  unsigned short int from,to;
  int counter1,counter2;
  char data;

  from=0;
  to=0;
  for (counter1=0;counter1<10;counter1++)
  {
   for (counter2=0;counter2<65536;counter2++)
   {
    data = buffer[from+=2];
    if ((data&1)==1) (to+=321); else (to+=320);
    buffer[to]++;
   }
  }
  for (counter1=0;counter1<65536;counter1++)
  {
   buffer[counter1] += addcolor;
  }
 }

 // -------------------------------------------------------------------------

 // Smooth buffer
void smooth(char *buffer)
 {
  unsigned short int from;
  char data;

  for (from=1;from<318;from++)
  {
   buffer[from] = (buffer[from-1]+buffer[from+1]) >> 1;
  }
  for (from=320;from<64000;from++)
  {
   buffer[from] = (buffer[from-320]+buffer[from-1]+buffer[from+1]+buffer[from+320]) >> 2;
  }
 }

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ


float FloatRandom( float x )
 {
  Rseed = (Rseed*134775813)+1;
  if (Rseed<0) return ((Rseed/4294967296.0) + 1.0);
  else return (Rseed/4294967296.0);
 }

 // -------------------------------------------------------------------------

int IntRand1( int x )
 {
  Rseed = (Rseed*134775813)+1;
  return (Rseed >> 16) % x;
 }

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void Init_Mul320Table(void)
 {
  int i;
  for (i=0; i < 200; i++) Mul320[i] = i * 320;
 }

 // -------------------------------------------------------------------------

 // scale = 1024 = 1
void DrawRotoZoom(char *screen,char *image, float angle, int scale)
 {
  register int rowU, rowV, dU, dV;
  int y, x, u, v, tx, ty;
  char c;
  float realangle;

  // angle &= 4095;
  realangle = angle;//*((PI*2)/4096);
  dU = (int)(scale*fsin(realangle));
  dV = (int)(scale*fcos(realangle));
  rowU = ((-160 * dV) + (100 * dU)) + (160 << 10);
  rowV = ((-160 * dU) - (100 * dV)) + (100 << 10);


  y=200;
  while (y--)
  {
   u = rowU;
   v = rowV;
   x = 320;
   while (x--)
   {
    tx = (u >> 10);
    ty = (v >> 10);
    while (ty>199) ty-=200;
    while (ty<0) ty+=200;
    while (tx>319) tx-=320;
    while (tx<0) tx+=320;
    // check for disabling texture tiling
 //   if (ty<=199 && tx<=319)
    {
     c = *(image + (Mul320[ty]+tx));
     /*if (c!=0)*/ *screen = c; //else *screen = *screen -4;
    }
    screen++;
    u+=dV;
    v+=dU;
   }
   rowU -= dU;
   rowV += dV;
  }
 }

 // -------------------------------------------------------------------------

void Init_SphereMap( char *p )
 {
  #define ScreenW  320
  #define ScreenH  200

  #define umin -128.0
  #define vmin -128.0
  #define umid  128.0
  #define vmid  128.0

  float n;
  float nnn;
  int x,y;
  float du,dv;
  float z = 256.0;


  z = (ScreenW/320)*200.0;   // *z
  // calc_coords;
  x = -160;
  y = -100;
  nnn = (x*x)+(y*y)+(z*z);
  du = (fsqrt(nnn)*umin)/x;
  dv = (fsqrt(nnn)*vmin)/y;
  for (y=-100;y<100;y++)
  {
   for (x=-160;x<160;x++)
   {
    nnn = ((x*x)+(y*y)+(z*z));
    if (nnn==0)    n = 0;   else n = 1/fsqrt(nnn);
    *p++ = ((n*x)*du)+umid;
    *p++ = ((n*y)*dv)+vmid;
   }
  }
 }

 // -------------------------------------------------------------------------

void DrawSphere( char *map, char *dest, unsigned char u, unsigned char v, char *tex )
 {
  int i;
  unsigned char *m;

  m = (unsigned char *)map;
  for (i=0;i<64000;i++)
  {
   *dest++ = tex[(((v + *m++)<<8) + (u + *m++))&0xffff];
  }
 }

 // -------------------------------------------------------------------------

void Skurr_Init(void)
 {
  int i,j;
  float displace;
  float delta;

  Randomize();

  for (i=0;i<200;i++)
  {
   do { displace = (IntRand1(SkurrMax*2)-SkurrMax); } while (displace == 0);

   delta = displace/SkurrNum;
   for (j=SkurrNum;j>=0;j--)
   {
    Skurr_Array[i][j] = (int)displace;
    displace-=delta;
   }
  }
 }

 // -----

void Skurr_Draw(char *dst, char *src, int num)
 {
  int i;
  int displace;
  char *d,*s;

  d = dst;
  s = src;
  if (num <= 0)
  {
   memcpy (d,s,64000);
   return;
  }
  for (i=0;i<200;i++)
  {
   displace = Skurr_Array[i][num];


   if (displace > 0)
   {
    memset(d,0,displace);
    memcpy(d+displace,s,(320-displace));
   }

   else
   if (displace < 0)
   {
    memcpy(d,s-displace,(320+displace));
    memset(d+(320+displace),0,-(displace));
   }
   else if (displace==0) memcpy(d,s,320);

   d+=320;
   s+=320;
  }
 }

