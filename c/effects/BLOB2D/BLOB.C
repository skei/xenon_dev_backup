#include <stdio.h>
#include <dos.h>
#include <math.h>

#include <conio.h>
#include <dos.h>

int X_RES = 320;
int Y_RES = 200;

unsigned char *screen;
unsigned char *bob;                  // Map for blob shadebob
char pal[768];

typedef struct sBlob
{
 float x,y;                 // x-,y-position
 float dx,dy;               // x-,y-translation
} sBlob;

static int NumBlobs = 0;    // number of blobs
sBlob blob[80];

#define xMax (320-40)
#define xMin (40 )
#define yMax (200-40)
#define yMin (40)

#define Number_Of_Blobs 40

unsigned char vscreen[320*200];
unsigned char *scrpointer;

// --------------------------------------------------------------------------

// calc shadeblob with blobfunction f. gives you intensity at postion x,y
int BlobFunc (int x,int y)
{
 int distX,distY;
 int r,temp,field;
 int r2;
 int i;

 field = 0;
 distX = x-32 ;
 distY = y-32 ;
 r  = distX*distX+distY*distY;
 r2 = 32*32;
 if (r < r2)
 {
  temp = (1023 - (1023*r/r2));
  field +=temp*temp;
 }
 field>>=13;
 return field;
}

// --------------------------------------------------------------------------

#define FKT_MIN(x, y)           ( ((x) < (y)) ? (x) : (y) )
#define FKT_MAX(x, y)           ( ((x) < (y)) ? (y) : (x) )

// --------------------------------------------------------------------------

// Draw positive 64x64 blob
void PosBob(unsigned char *scr,int x,int y)
{
 int a,c;
 unsigned char *b;
 unsigned char *s;
 int h;

 s = scr;
 b=bob;
 s+=(x-32)+(y-32)*X_RES;
 for(a=0;a<64;a++)
 {
  for(c=0;c<64;c++)
  {
   h = *s + (*b++);
   *s++ = FKT_MIN(255,h );
  }
  s+=320-64;
 }
}

// --------------------------------------------------------------------------

// Draw negative 64x64 blob
void NegBob(char *screen,int x,int y)
{
 int a,c;
 char *b;
 int h;

 b=bob;
 screen+=(x-32)+(y-32)*X_RES;
 for(a=0;a<64;a++)
 {
  for(c=0;c<64;c++)
  {
   h=*screen- (*b++);
   *screen++ =FKT_MAX(1,h );
  }
  screen+=320-64;
 }
}

// --------------------------------------------------------------------------

void blob2d(unsigned char *screen)
{
 int i,x,y,pos;
 float off;
 int b;

 for(i=0;i<NumBlobs;i++)
 {
  blob[i].x += blob[i].dx;
  blob[i].y += blob[i].dy;

  if (blob[i].x >= xMax)
  {
   blob[i].x  = xMax-(blob[i].x-xMax);
   blob[i].dx = -blob[i].dx;
  }

  if (blob[i].x < xMin)
  {
   blob[i].x  = xMin-(blob[i].x-xMin);
   blob[i].dx = -blob[i].dx;
  }

  if (blob[i].y >= yMax)
  {
   blob[i].y  = yMax-(blob[i].y-yMax);
   blob[i].dy = -blob[i].dy;
  }

  if (blob[i].y < yMin)
  {
   blob[i].y  = yMin-(blob[i].y-yMin);
   blob[i].dy = -blob[i].dy;
  }
  PosBob(screen,(int)blob[i].x,(int)blob[i].y);
 }
}


// --------------------------------------------------------------------------

void initBlob2D(void)
{
 int i,f,q,x,y;
 bob=(unsigned char *)malloc(64*64);


 // calc blob shadebob with blob function
 for (x=0;x<64;x++)
  for (y=0;y<64;y++)
   bob[y*64+x]=BlobFunc(x,y)*2;
 // init blob positions
 NumBlobs=0;
 for (i=0;i<Number_Of_Blobs;i++)
 {
  blob[i].x=40+(rand()%240);
  blob[i].y=40+(rand()%120);
  blob[i].dx=-1.01+2*(float)(rand()%10)/10;
  blob[i].dy=-1.01+2*(float)(rand()%10)/10;
  NumBlobs++;
 }
 X_RES=320;Y_RES=200;
}

// --------------------------------------------------------------------------

int main(void)
{
 FILE *fp;
 union REGS r;
 int i;

 srand();
 scrpointer = &vscreen;
 initBlob2D();

 r.w.ax=0x13; int386(0x10,&r,&r);

 fp = fopen("blob.pal","rb");
 fread(pal,1,768,fp);
 fclose(fp);

 for ( i=0; i<256; i++ )
 {
  outp(0x3C8,i);
  outp(0x3C9,pal[384+(i/2)*3  ]);
  outp(0x3C9,pal[384+(i/2)*3+1]);
  outp(0x3C9,pal[384+(i/2)*3+2]);
 }

 while (!kbhit())
 {
  memset(vscreen,0,64000);
  blob2d(scrpointer);
  memcpy((char *)0xA0000,vscreen,64000);
 }

 r.w.ax=0x3; int386(0x10,&r,&r);

 free(bob);
 return(0);
}
