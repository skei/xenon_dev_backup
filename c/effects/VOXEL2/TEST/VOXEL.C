// test fors쌽... Send 320 rays, hver finner 64 Y-verdier....
// Ray-casting

// --------------------------------------------------------------------------

#include <math.h>     // sin/cos
#include <string.h>   // memset
#include <stdio.h>    // file

// --------------------------------------------------------------------------

unsigned char heightmap[65536];
unsigned char *ScreenPtr;

// --------------------------------------------------------------------------

 void SetMode(unsigned short int);
#pragma aux SetMode =\
 "int 10h",\
 parm [ax] modify [ax];

 void setrgb(char color, char r,char g, char b);
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


// float -> int

#define S 65536.0
#define MAGIC (((S*S*16)+(S*.5))*S)
#define MAGICfixed (MAGIC/65536.0)
#define MAGICfixed12_4 (MAGIC/16.0)

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

#pragma inline float2fixed12_4;
 static long float2fixed12_4( float d )
 {
  double dtemp = MAGICfixed12_4+d;
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

// --------------------------------------------------------------------------



// Send ray from x/y-start thought x/y-dir (screen/y in map)
void Ray( int screenX, float xpos, float ypos, float dir )
{
 float xstart,ystart;
 float xend,yend;
 float xslope,yslope;
 int i,j;
 unsigned char x,y,cc;


 xstart = xpos + sin(dir)*16;
 ystart = ypos + cos(dir)*16;

 xend = xpos + sin(dir)*48;
 yend = ypos + cos(dir)*48;

 xslope = (xend-xstart)/64;
 yslope = (yend-ystart)/64;

 for (i=0;i<64;i++)
 {
//  x = (float2int(xpos))&255;
//  y = (float2int(ypos))&255;
  x = ((int)xpos)&255;
  x = ((int)ypos)&255;
  cc = heightmap[y<<8+x];
  ScreenPtr[(y/2)<<8+(x/2)] = 1;
  while (cc>0)
  {
   *(char *)(0xA0000+screenX+((199-cc)*320)) = cc;
   cc--;
  }

  xpos += xslope;
  ypos += yslope;
 }
}

// --------------------------------------------------------------------------

void main(void)
{
 FILE *fp;
 int i;
 float angle,dir,deltaangle,direction;
 int x,y;

 fp = fopen("t.raw","rb");
 fread(heightmap,1,65536,fp);
 fclose(fp);

 SetMode(0x13);
 ScreenPtr = (char *)0xA0000;

 direction = 2.0;

 dir = direction-1;
 deltaangle = ((dir-1)-(dir+1))/320;

// Draw heightmap
 for (y=0;y<128;y++)
 {
  for (x=0;x<128;x++)
  {
   ScreenPtr[y*320+x] = heightmap[(y*2*256)+(x*2)];
  }
 }

 for (i=0;i<320;i++)
 {
  Ray(i,128,128,dir);
  dir+=deltaangle;
 }
 getch();
 SetMode(3);

}
