/* INF Development May 1997
   Written by bitbrat/inf

   routine: light rays
*/
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>

unsigned char plotto[64000+320];
unsigned char vrtscr[64000+320];
unsigned char lady[64000];
int      ray[314][128];
float p;

#define S 65536.0
#define MAGIC (((S*S*16)+(S*.5))*S)
#define MAGICfixed (MAGIC/65536.0)

#pragma inline float2fixed;
static long float2fixed( float d )
{
 double dtemp = MAGICfixed+d;
 return (*(long *)&dtemp)-0x80000000;
}

// --------------------------------------------------------------------------


void lightray(float x1, float y1, double degr, unsigned char *dst, unsigned char *plt)
{

 int   x, y;
 int   xi, yi;
 float xslope, yslope;
 float deltax, deltay;
 float i;
 float direct;
 float x2, y2;
 int   o;
 int   length;
 int   draw;
 int   tmp;

 x2 = x1 + (240*sin(degr));
 y2 = y1 + (200*cos(degr));

 x = float2fixed(x1);
 y = float2fixed(y1);
 length = 255;
 draw = 0;
 deltax = float2fixed((x2-x1)/256);
 deltay = float2fixed((y2-y1)/256);
 while (length>0)
 {
  if ( ((unsigned int)x < (320<<16)) && ((unsigned int)y < (200<<16)) )
  {
   xi = x>>16;
   yi = y>>16;
   o = (yi << 8) + (yi << 6) + xi;
   if (draw==0) if (dst[o]==0) draw = 1;
   if (draw==1) plt[o] = length;
  }

  x += deltax;
  y += deltay;
  length-=2;
 }
}

// --------------------------------------------------------------------------

void addscr(unsigned char *c, unsigned char *a, unsigned char *b)
{
 int i,q,clr;

 // Smooth light a bit
 for(q=0;q<4;q++)
 {
  for(i=320;i<63999-320;i++)
  {
   a[i] = (a[i]+a[i+1]+a[i-320]+a[i+320])>>2;
  }
 }

 // Add screens together
 for(i=0;i<64000;i++)
 {
  clr = a[i] + b[i];
  if (clr>255) c[i] = 255; else c[i] = clr;

 }
}

// --------------------------------------------------------------------------

void star(int x, int y, unsigned char *dst, unsigned char *plt)
{
 float i;
// p+=0.01;
 #define p 0

 for(i=p;i<p+6.28;i+=0.05) lightray(x,y,i,dst,plt);
 #undef p
}

// --------------------------------------------------------------------------

void setmode(unsigned short int m);
#pragma aux setmode = \
" int 10h " \
parm [ax];

// --------------------------------------------------------------------------

void init()
{
 int i;
 FILE *fp;
 int  r,g,b;
 char pal[768];
 setmode(0x13);

 fp = fopen("morocco3.pix", "rb");
 fread(lady, 1, 64000, fp);
 fclose(fp);
 fp = fopen("morocco3.pal", "rb");
 fread(pal, 1, 768, fp);
 fclose(fp);

 outp(0x3c8,0);
 for(i=0;i<255;i++)
 {
  outp(0x3c9,(pal[(i*3)+0]+pal[(i*3)+1]+pal[(i*3)+2])/3);
  outp(0x3c9,(pal[(i*3)+0]+pal[(i*3)+1]+pal[(i*3)+2])/3);
  outp(0x3c9,(pal[(i*3)+0]+pal[(i*3)+1]+pal[(i*3)+2])/3);
 }
 outp(0x3c8,255);
 outp(0x3c9,63);
 outp(0x3c9,63);
 outp(0x3c9,63);
}

// --------------------------------------------------------------------------

int  main()
{
 int i;
 double j;

 init();
 p=0;

 for(;;)
 {
  star(160+(sin(j)*110), 75/*+(sin(j*1.523)*75)*/, lady, vrtscr);
  addscr(plotto,vrtscr, lady);
  memcpy((unsigned char *)0xa0000+320, plotto+320, 64000-(320*2));

//  for (i=0;i<64000;i++)
//  if (vrtscr[i] >4 ) vrtscr[i]-=5; else vrtscr[i] = 0;
  memset(vrtscr,0,64000);

  j+=0.05;
  if (kbhit()) break;
 }

 getch();
 setmode(0x3);
 return 0;
}


