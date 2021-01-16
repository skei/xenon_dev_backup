#include <math.h>
#include <conio.h>
#include <string.h>

#define iterations 127

char pix[256*256];     // used to store the julia
int hw[256];          // start on which pixel on row
int re;
int im;
int sqrs[256];
int ingvar;           // counter for re and im
char redv[256];
char greenv[256];
char bluev[256];

// --------------------------------------------------------------------------

void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

void setrgb (char,char,char,char);
#pragma aux setrgb=\
 "mov dx,3c8h",\
 "out dx,al",\
 "inc dx",\
 "mov al,ah",\
 "out dx,al",\
 "mov al,bl",\
 "out dx,al",\
 "mov al,bh",\
 "out dx,al",\
 parm [al] [ah] [bl] [bh] modify [edx];

void waitvr (void);
#pragma aux waitvr =\
 " mov dx,3dah",\
 "wait1:",\
 " in al,dx",\
 " test al,8",\
 " jnz wait1",\
 "wait2:",\
 " in al,dx",\
 " test al,8",\
 " jz wait2",\
 modify [al dx];

// --------------------------------------------------------------------------

void buildpalette (int r2,int g2,int b2,   // to these values
                   int r, int g, int b,    // from these
                   int n, int start)       // how many, starting from
{
 int i;
 int m=n;

 for (i = 0; i < n; i++,m--)
 {
  redv  [i + start] =(char)(r2*i/n + r*m/n);
  greenv[i + start] = (char)(g2*i/n + g*m/n);
  bluev [i + start] = (char)(b2*i/n + b*m/n);
 }
}


// --------------------------------------------------------------------------

// magic function of recursion
// really fast but looses some precision due to roundofs in integer division
int iterate(int x, int y, int iter)
{
 int it;
 int nx, ny;

 if ((it = pix[x-(y*256) + 127*257]) == 0)  // do we have a previous value
 {
  //  nop we dont
  if (iter < iterations)
  {
   ny = (x * y + im) >> 5;
   // is y within limit?
   if (ny > 127 || ny < -127) it = 1; // no then dont calculate x
   else
   {      // yes then check x to
    nx = (sqrs[x + 127] - sqrs[y + 127] + re) >> 6; // here we get errors in division
    if ( nx > hw[ny + 127] || nx < -hw[ny + 127]) it = 1;
    else it = 1 + iterate (nx, ny, iter + 1) ; // inside so recurse to find the answear
   }
  }
  else it = iterations;
  pix[x - y * 256 + 127 * 257] = pix[-x + ((y+1) << 8) + 127 * 257] = it;
 }
 return it == iterations ? it - 1 : it;
}

// --------------------------------------------------------------------------

void main(void)
{
 int i;
 int x,y;

// --------------------
 re = 2;
 im = 4;
 ingvar = 0;
 // calculate outer bounds of fractal (need only be done once)
 // and build sqaure vector.
 for ( i=0; i<128; i++ )
 {
  hw[i] = hw[255 - i] = (int) (128 * sin( acos((128-i)/128.0)));
  sqrs[127 - i] = sqrs [128 + i] = i * i;
 }
// --------------------
 SetMode(0x13);

 while (inp(0x60) != 1)
 {
  //update new re and new im part....
  // change these to other functions to get diffrent julias

 re=(int)(2048*cos(0.05*ingvar));
 im=(int)(2048*sin(0.05*ingvar /* + 0.4711*/));
 ingvar++;

 memset(pix,0,65536);

  // get value
  // 128 represents 2.0 on the axis
  // x,y
  // -127,128---------128,128
  //     |    screen     |
  //     |  cordinates   |
  //-127,-127---------128,-127
  // by starting from the inner parts I have a bigger chans to fill in
  // more of the julia in one row.
  for (y = 0; y < 128; y++)
  {
   for (x = -hw[y + 127]; x < hw[y+127] + 1; x++)
   {
    iterate (x, y, 0);
   }
  }

  for (y=0;y<200;y++)
  {
   for (x=0;x<256;x++)
   {
    *(char *)(0xA0000+y*320+x) = pix[y*256+x];
   }
  }
  *(char *)(0xA0000) = ingvar;
 }

 SetMode(3);

}
