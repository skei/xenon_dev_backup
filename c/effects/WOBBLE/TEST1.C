#include <math.h>     // sin, cos
#include <stdio.h>    // file stuff
#include <stdlib.h>   // memory yucku
#include <conio.h>    // kbhit

#define PI 3.1415926535

char map[16384];
char pal[768];

char *screen;

int Sin[4096];
int Sinus[4096];
int Cos[4096];
int Cosinus[4096];

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

// set video mode
void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

// -----

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

// -----

// Vaits for Vertical retrace to START
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


// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void Init( int size, float amp)
{
 int i;
 float realangle;

 realangle = -PI;
 for (i=0;i<size;i++)
 {
  Sin[i] = (int)(sin(realangle) * amp);
  Cos[i] = (int)(cos(realangle*2) * amp);

//  Sin[i] = ( sin(((i+870)*1)/(163*4))*128 );
//  Cos[i] = ( cos(((i    )*2)/(163*4))*128 );

  realangle += (PI*2)/size;
 }
}

// 컴컴컴컴컴컴컴컴컴컴

void setup_sines(float factor, int size, float amp)
{
 int cnt;

 for (cnt=0;cnt<size;cnt++)
 {
  Sinus[cnt]   = (int)((float)((float)Sin[cnt] * factor) / amp);
  Cosinus[cnt] = (int)((float)((float)Cos[cnt] * factor) / amp);
 }
}

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

void main(void)
{
 FILE *fp;
 int i;
 int tmp = 0;
 int x,y;
 float sine_size;
 float sine_speed;

 Init(4096,127);

// -----

 fp = fopen("pat0035.pix","rb");
 fread(map,1,16384,fp);
 fclose(fp);
 fp = fopen("pat0035.pal","rb");
 fread(pal,1,768,fp);
 fclose(fp);

 SetMode(0x13);
 for (i=0;i<256;i++) setrgb(i,pal[i*3],pal[i*3+1],pal[i*3+2]);

 setup_sines(20,4096,31);

// -----

 sine_size = 0;
 sine_speed = 100.0;

 while (!kbhit())
 {
  tmp++;
  if (sine_size < 50) sine_size+=0.5;
//  sine_size = 45;
  setup_sines( sin(tmp/(sine_speed))*sine_size ,4096,127);
  screen = (char *)0xA0000;

  for (y=0;y<200;y++)
  {
   for (x=0;x<320;x++)
   {
    screen[y*320+x] = map[(((  Sinus[((x+tmp)<<4) & 4095]+y)<<7)
                         + ((Cosinus[((y+tmp)<<3) & 4095]+x)) ) & 16383];
   }
  }
 }

// -----

 SetMode(3);


}
