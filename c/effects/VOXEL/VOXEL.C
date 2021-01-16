#include <math.h>
#include <stdio.h>

#include "voxel.h"
#include "vbe2.h"

word *Vesa;
word Buffer[64000];
char temp[65536];
char pal[768];

 // set video mode
void SetMode(unsigned short int);
 #pragma aux SetMode =\
  "int 10h",\
  parm [ax] modify [ax];

 // --------------------------------------------------------------------------

 // Vaits for Vertical retrace to START
void WaitVR(void);
 #pragma aux WaitVR =\
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

// Reduces a value to 0..255 (used in height field computation)
int Clamp(int x)
{
 return (x<0 ? 0 : (x>255 ? 255 : x));
}

// --------------------------------------------------------------------------

// Heightfield and colormap computation
void VoxComputeMap(void)
{
 int p,i,j,k,k2,p2;
 int a,b,c,d;


 // Start from a plasma clouds fractal
 HMap[0]=128;
 for ( p=256; p>1; p=p2 )
 {
  p2=p>>1;
  k=p*8+20; k2=k>>1;
  for ( i=0; i<256; i+=p )
  {
   for ( j=0; j<256; j+=p )
   {
    a=HMap[(i<<8)+j];
    b=HMap[(((i+p)&255)<<8)+j];
    c=HMap[(i<<8)+((j+p)&255)];
    d=HMap[(((i+p)&255)<<8)+((j+p)&255)];
    HMap[(i<<8)+((j+p2)&255)]            = Clamp(((a+c)>>1)+(rand()%k-k2));
    HMap[(((i+p2)&255)<<8)+((j+p2)&255)] = Clamp(((a+b+c+d)>>2)+(rand()%k-k2));
    HMap[(((i+p2)&255)<<8)+j]            = Clamp(((a+b)>>1)+(rand()%k-k2));
   }
  }
 }
 // Smoothing
 for ( k=0; k<3; k++ )
 {
  for ( i=0; i<256*256; i+=256 )
  {
   for ( j=0; j<256; j++ )
   {
    HMap[i+j] =  (HMap[((i+256)&0xFF00)+j]+HMap[i+((j+1)&0xFF)]
                + HMap[((i-256)&0xFF00)+j]+HMap[i+((j-1)&0xFF)])>>2;
   }
  }
 }

 // Color computation (derivative of the height field)

 for ( i=0; i<256*256; i+=256 )
 {
  for ( j=0; j<256; j++ )
  {
   k = 128+(HMap[((i+256)&0xFF00)+((j+1)&255)]-HMap[i+j])*4;
   if ( k<0 ) k=0;
   if (k>255) k=255;
   RMap[i+j] = k;
   GMap[i+j] = k;
   BMap[i+j] = k;
  }
 }
}

// --------------------------------------------------------------------------

// Draw a "section" of the landscape; x0,y0 and x1,y1 and the xy coordinates
// on the height field, hy is the viewpoint height, s is the scaling factor
// for the distance. x0,y0,x1,y1 are 16.16 fixed point numbers and the
// scaling factor is a 16.8 fixed point value.
void VoxLine(word *toarea, int x0,int y0,int x1,int y1,int hy,int s, int color)
{
 int i,sx,sy;
 int c,y,h,u0,v0,u1,v1,a,b,h0,h1,h2,h3;
 int R,G,B;

 // Compute xy speed
 sx=(x1-x0)/320; sy=(y1-y0)/320;
 for ( i=0; i<320; i++ )
 {
  // Compute the xy coordinates; a and b will be the position inside the
  // single map cell (0..255).
  //
  u0=(x0>>16)&0xFF;    a=(x0>>8)&255;
  v0=((y0>>8)&0xFF00); b=(y0>>8)&255;
  u1=(u0+1)&0xFF;
  v1=(v0+256)&0xFF00;

  // Fetch the height at the four corners of the square the point is in
  h0=HMap[u0+v0]; h2=HMap[u0+v1];
  h1=HMap[u1+v0]; h3=HMap[u1+v1];

  // Compute the height using bilinear interpolation
  h0=(h0<<8)+a*(h1-h0);
  h2=(h2<<8)+a*(h3-h2);
  h=((h0<<8)+b*(h2-h0))>>16;

  // Fetch the color at the four corners of the square the point is in
  h0=RMap[u0+v0]; h2=RMap[u0+v1];
  h1=RMap[u1+v0]; h3=RMap[u1+v1];
  // Compute the color using bilinear interpolation (in 16.16)
  h0=(h0<<8)+a*(h1-h0);
  h2=(h2<<8)+a*(h3-h2);
  R=((h0<<8)+b*(h2-h0));
  R = (R * color) >> 6;       // hazing

  // Fetch the color at the four corners of the square the point is in
  h0=GMap[u0+v0]; h2=GMap[u0+v1];
  h1=GMap[u1+v0]; h3=GMap[u1+v1];
  // Compute the color using bilinear interpolation (in 16.16)
  h0=(h0<<8)+a*(h1-h0);
  h2=(h2<<8)+a*(h3-h2);
  G=((h0<<8)+b*(h2-h0));
  G = (G * color) >> 6;       // hazing

  // Fetch the color at the four corners of the square the point is in
  h0=BMap[u0+v0]; h2=BMap[u0+v1];
  h1=BMap[u1+v0]; h3=BMap[u1+v1];
  // Compute the color using bilinear interpolation (in 16.16)
  h0=(h0<<8)+a*(h1-h0);
  h2=(h2<<8)+a*(h3-h2);
  B=((h0<<8)+b*(h2-h0));
  B = (B * color) >> 6;       // hazing

  // Compute screen height using the scaling factor
  y=(((h-hy)*s)>>11)+100;
  // Draw the column
  if ( y<(a=lasty[i]) )
  {
   word *b=toarea+a*320+i;
   int sr,cr,Rcol;
   int sg,cg,Gcol;
   int sb,cb,Bcol;

   if ( lastr[i]==-1 ) lastr[i]=R;
   sr=(R-lastr[i])/(a-y);
   cr=lastr[i];
   if ( lastg[i]==-1 ) lastg[i]=G;
   sg=(G-lastg[i])/(a-y);
   cg=lastg[i];
   if ( lastb[i]==-1 ) lastb[i]=B;
   sb=(B-lastb[i])/(a-y);
   cb=lastb[i];

   if ( a>199 )
   {
    b -= (a-199)*320;
    cr+= (a-199)*sr;
    cg+= (a-199)*sg;
    cb+= (a-199)*sb;
    a=199;
   }

   if ( y<0 ) y=0;
   while ( y<a )
   {
    Rcol=cr>>19;
    Gcol=cg>>19;
    Bcol=cb>>19;
    *b=(Rcol<<10)+(Gcol<<5)+Bcol;
    cr+=sr;
    cg+=sg;
    cb+=sb;
    b-=320;
    a--;
   }
   lasty[i]=y;
  }
  lastr[i]=R;
  lastg[i]=G;
  lastb[i]=B;
  // Advance to next xy position
  x0+=sx; y0+=sy;
 }
}

// --------------------------------------------------------------------------

// Draw the view from the point x0,y0 (16.16) looking at angle a
void VoxView(word *toarea,int x0,int y0,float aa)
{
 int d;
 int a,b,h,u0,v0,u1,v1,h0,h1,h2,h3;

 // Clear offscreen buffer
 memset(toarea,0,320*200*2);
 // Initialize last-y and last-color arrays
 for ( d=0; d<320; d++ )
 {
  lasty[d]=200;
  lastc[d]=-1;
 }
 // Compute viewpoint height value
 // Compute the xy coordinates; a and b will be the position inside the
 // single map cell (0..255).
 u0=(x0>>16)&0xFF;    a=(x0>>8)&255;
 v0=((y0>>8)&0xFF00); b=(y0>>8)&255;
 u1=(u0+1)&0xFF;
 v1=(v0+256)&0xFF00;
/*
 // Fetch the height at the four corners of the square the point is in
 h0=HMap[u0+v0]; h2=HMap[u0+v1];
 h1=HMap[u1+v0]; h3=HMap[u1+v1];
 // Compute the height using bilinear interpolation
 h0=(h0<<8)+a*(h1-h0);
 h2=(h2<<8)+a*(h3-h2);
 h=((h0<<8)+b*(h2-h0))>>16;
*/

 // Draw the landscape from near to far without overdraw
 for ( d=0; d<63; d+=1+(d>>6) )
 {
  VoxLine(toarea, x0+d*65536*cos(aa-FOV),y0+d*65536*sin(aa-FOV),
                  x0+d*65536*cos(aa+FOV),y0+d*65536*sin(aa+FOV),
                  /*h-30*/ 0,
                  100*256/(d+1),
                  63-d);
 }
 // Blit the final image to the screen
}

// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

void main(void)
{
 FILE *fp;
 int x,y;
 float a;

 VesaInit(320,200,15);
 Vesa = (word *)VesaPtr;
 VoxComputeMap();
 x = 128;
 y = 128;
 a = 0;

 fp = fopen("t.pix","rb");
 fread(temp,1,65536,fp);
 fclose(fp);

 fp = fopen("t.pal","rb");
 fread(pal,1,768,fp);
 fclose(fp);

 for (x=0;x<65536;x++)
 {
  RMap[x] = pal[ temp[x] * 3    ] << 2;
  GMap[x] = pal[ temp[x] * 3 +1 ] << 2;
  BMap[x] = pal[ temp[x] * 3 +2 ] << 2;
 }

 Set_Vesa(VesaMode);

 while (!kbhit())
 {
  // Dest buffer
  // x,y : 16.16
  // angle
  VoxView(Buffer,x,y,a);
  WaitVR();
  memcpy(Vesa,Buffer,128000);
  x+=5 << 12;
  y+=3 << 12;
  a+=0.005;
 }
 getch();
 Set_Vesa(3);
 VesaUnInit();
}
