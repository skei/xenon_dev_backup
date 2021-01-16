#include <math.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define PI 3.1415926535

// variables

 #include "voxelpic.inc"
 #include "voxelpal.inc"

 unsigned char rng[321];
 int mul320_table[200];

 int lastzobs;

// --------------------------------------------------------------------------

 // Calculate Mul320 table
void MakeMulTable(void)
 {
  int i;
  for (i=0;i<200;i++) mul320_table[i] = i*320;
 }

// --------------------------------------------------------------------------
// ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²

#define RANDOM(a) (rand()/(RAND_MAX/(a)))         // a simple random function

#define MAPSHIFT   8                              // 2^MAPSHIFT = size of map
#define MAPSIZE    (1<<MAPSHIFT)                  // size of one side of the map
#define BYTE(a)    (a&(MAPSIZE-1))                // put the range within MAPSIZE
#define INDEX(x,y) ((BYTE(y)<<MAPSHIFT)+BYTE(x))  // index calculation

char newcolor(int mc, int n, int dvd)
{
 int loc;

 loc = (mc + n - RANDOM(n << 1)) / dvd - 1;
 if (loc > 255) loc = 255;
 if (loc < 10) loc = 10;
 return(loc);
}

// ---

void createfractalmap(char *heightmap, int x1, int y1, int x2, int y2)
{
 int p1, p2, p3, p4;
 int xn, yn, dxy;

 if (((x2-x1) < 2) && ((y2-y1) < 2)) return;
 p1 = heightmap[INDEX(x1,y1)];
 p2 = heightmap[INDEX(x1,y2)];
 p3 = heightmap[INDEX(x2,y1)];
 p4 = heightmap[INDEX(x2,y2)];
 xn = (x2+x1) >> 1;
 yn = (y2+y1) >> 1;
 dxy = 5*(x2 - x1 + y2 - y1) / 3;
 if (heightmap[INDEX(xn,y1)] == 0) heightmap[INDEX(xn,y1)] = newcolor(p1+p3, dxy, 2);
 if (heightmap[INDEX(x1,yn)] == 0) heightmap[INDEX(x1,yn)] = newcolor(p2+p4, dxy, 2);
 if (heightmap[INDEX(x2,yn)] == 0) heightmap[INDEX(x2,yn)] = newcolor(p3+p4, dxy, 2);
 if (heightmap[INDEX(xn,y2)] == 0) heightmap[INDEX(xn,y2)] = newcolor(p1+p2, dxy, 2);
 heightmap[INDEX(xn,yn)] = newcolor(p1+p2+p3+p4, dxy, 4);
 createfractalmap(heightmap,x1, y1, xn, yn);
 createfractalmap(heightmap,xn, y1, x2, yn);
 createfractalmap(heightmap,x1, yn, xn, y2);
 createfractalmap(heightmap,xn, yn, x2, y2);
}

// ---

void smoothmap( char *heightmap)
{
 int x,y;

 for (x = 0; x < MAPSIZE; x++)
 {
  for (y = 0; y < MAPSIZE; y++)
  {
   heightmap[INDEX(x,y)] = (heightmap[INDEX(x-1,y-1)] +
                            heightmap[INDEX(x-1,y+1)] +
                            heightmap[INDEX(x+1,y-1)] +
                            heightmap[INDEX(x+1,y+1)] +
                            heightmap[INDEX(x,  y-1)] +
                            heightmap[INDEX(x,  y+1)] +
                            heightmap[INDEX(x-1,y)] +
                            heightmap[INDEX(x+1,y)]) >> 3;
  }
 }
}


// ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
// --------------------------------------------------------------------------

// draw the voxel landscape to the background buffer
void DrawVoxel (int xp, int yp, /*int*/float dir,
                char *heightmap, char *colormap, char *buffer)
{
 int ixp,iyp,z;
 int zobs,iy1,y;
 int ix,iy,x,csf,snf,mpc,i,j;
 int s;
 char clr;
 int ps;

 memset(rng,200,sizeof(rng));
// zobs = 250;
 z = 50+heightmap[256*yp+xp];
 if (lastzobs > z) lastzobs-=1;
 if (lastzobs < z) lastzobs+=2;

 zobs = lastzobs;

// snf = (256*sin ((double)dir/180 * PI));
// csf = (256*cos ((double)dir/180 * PI));

 snf = 1024*sin(dir);
 csf = 1024*cos(dir);

 for (iy=yp;iy<=yp+64;iy++)   // yp+nn = antall linjer innover i skjerm † tegne
 {
  iy1 = 2 + 2*(iy-yp);    // avstand fra kamera til "current Y in map"

  s = 1+(360/iy1);  // Stolpe-bredde

  for (ix=xp+yp-iy;ix<=xp-yp+iy;ix++)
  {
   ixp = xp+((ix-xp)*csf+(iy-yp)*snf) / 1024;
   iyp = yp+((iy-yp)*csf-(ix-xp)*snf) / 1024;

   x = 160+360*(ix-xp) / iy1;

   if ((x>=0) && ((x+s)<319))
   {
    z = heightmap[ (256*iyp+ixp)&0xFFFF];
    clr = colormap[(256*iyp+ixp)&0xFFFF];

    y = 100+((zobs-z)*32) / iy1;
    if ((y>=0) && (y<=199))
    {
     for (j=x;j<=x+s;j++)
     {
      ps = mul320_table[y];
      ps += j;
      for (i=y;i<rng[j];i++)
      {
       buffer[ps] = clr;
       ps+=320;
      }
      if (y<rng[j]) rng[j]=y;
     }
    }
   }
  }
 }
}

// --------------------------------------------------------------------------

/*

void main(void)
{
 FILE *fp;
 int i,j;
 int xx,yy;
 float ddir;

 fp = fopen("h.raw","rb");
 fread(heightmap,1,65536,fp);
 fclose(fp);

 fp = fopen("p.raw","rb");
 fread(pal,1,768,fp);
 fclose(fp);

 fp = fopen("t.raw","rb");
 fread(colormap,1,65536,fp);
 fclose(fp);

 MakeTables();

 createfractalmap(0,0,255,255);
 for (i=0;i<15;i++) smoothmap();

 SetMode(0x13);

// outp(0x3C8,0);
 for (i=0;i<256;i++)
 {
//  pal[i*3  ] = (i/4);
//  pal[i*3+1] = (i/4);
//  pal[i*3+2] = 0;
 }

 Set_Vesa(VesaMode);
 Vesa = (unsigned short int *)VesaPtr;

 xx = 128;
 yy = 128;
 ddir = 0;

 lastzobs = 255;

 while(!kbhit())
 {
  memset(buffer,0,64000*2);
  DrawVoxel(xx,yy,ddir);
  ddir+=0.005;
  if (ddir>359) ddir-=360;
  if (ddir<0) ddir+=360;
  xx+=1;
  xx&=255;
  yy+=1;
  yy&=255;
  memcpy(Vesa,buffer,64000*2);
 }
 getch();
 SetMode(3);
 Set_Vesa(3);
 VesaUnInit();
}

*/
       
