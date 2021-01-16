 #include <stdio.h>
 #include <i86.h>
 #include <conio.h>
 #include <stdlib.h>
 #include <math.h>
 #include <string.h>

 #include "vbe2.h"

 typedef unsigned char      byte;
 typedef unsigned short int word;

 byte HMap[256*256];      // Height field
 byte RMap[256*256];      // Color map
 byte GMap[256*256];      // Color map
 byte BMap[256*256];      // Color map

 word *Vesa;
 word Buffer[64000];
 byte temp[65536];
 byte pal[768];

 int lasty[320],lastr[320],lastg[320],lastb[320];

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 // set video mode
void SetMode(unsigned short int);
 #pragma aux SetMode =\
  "int 10h",\
  parm [ax] modify [ax];

 // ---

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

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 //
 // Reduces a value to 0..255 (used in height field computation)
 //
int  Clamp(int x)
 {
  return (x<0 ? 0 : (x>255 ? 255 : x));
 }

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 //
 // Heightfield and colormap computation
 //
void ComputeMap(byte *HMap, int smooth)
 {
  int p,i,j,k,k2,p2;

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
     int a,b,c,d;
     a=HMap[(   i          <<8 ) +   j         ];
     b=HMap[( ((i+p) &255) <<8 ) +   j         ];
     c=HMap[(   i          <<8 ) + ((j+p) &255)];
     d=HMap[( ((i+p) &255) <<8 ) + ((j+p) &255)];
       HMap[(   i          <<8 ) + ((j+p2)&255)] = Clamp(((a  +c  )>>1)+(rand()%k-k2));
       HMap[( ((i+p2)&255) <<8 ) + ((j+p2)&255)] = Clamp(((a+b+c+d)>>2)+(rand()%k-k2));
       HMap[( ((i+p2)&255) <<8 ) +   j         ] = Clamp(((a+b    )>>1)+(rand()%k-k2));
    }
   }
  }
  // Smoothing
  for ( k=0; k<smooth; k++ )
   for ( i=0; i<256*256; i+=256 )
    for ( j=0; j<256; j++ )
    {
     HMap[i+j]=(   HMap[((i+256)&0xFF00)+j]+HMap[i+((j+1)&0xFF)]
                 + HMap[((i-256)&0xFF00)+j]+HMap[i+((j-1)&0xFF)] ) >>2;
    }

  //// Color computation (derivative of the height field)
  //for ( i=0; i<256*256; i+=256 )
  // for ( j=0; j<256; j++ )
  // {
  //  k=128+(HMap[((i+256)&0xFF00)+((j+1)&255)]-HMap[i+j])*4;
  //  if ( k<0 ) k=0; if (k>255) k=255;
  //  RMap[i+j]=k;
  //  GMap[i+j]=k;
  //  BMap[i+j]=k;
  // }
 }

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 //
 // Draw a "section" of the landscape; x0,y0 and x1,y1 and the xy coordinates
 // on the height field, hy is the viewpoint height, s is the scaling factor
 // for the distance. x0,y0,x1,y1 are 16.16 fixed point numbers and the
 // scaling factor is a 16.8 fixed point value.
 //
void Line(word *toarea,int x0,int y0,int x1,int y1,int hy,int s, int color)
 {
  int i,sx,sy;

  // Compute xy speed
  sx=(x1-x0)/320;
  sy=(y1-y0)/320;
  for (i=0;i<320;i++ )
  {
   int c,y,h,u0,v0,u1,v1,a,b,h0,h1,h2,h3;
   int rr,gg,bb;
   // Compute the xy coordinates; a and b will be the position inside the
   // single map cell (0..255).

   a=(x0>>8)&255;
   b=(y0>>8)&255;
   u0=(x0>>16)&0xFF;
   v0=((y0>>8)&0xFF00);
   u1=(u0+1)&0xFF;
   v1=(v0+256)&0xFF00;

   // Fetch the height at the four corners of the square the point is in
   h0=HMap[u0+v0]; h2=HMap[u0+v1];
   h1=HMap[u1+v0]; h3=HMap[u1+v1];
   // Compute the height using bilinear interpolation
   h0=(h0<<8)+a*(h1-h0);
   h2=(h2<<8)+a*(h3-h2);
   h=((h0<<8)+b*(h2-h0))>>16;

   // AXON: f›lgende kan repeteres for RGB, istedenfor bare _en_ farge...

   // Red

   // Fetch the color at the four corners of the square the point is in
   h0=RMap[u0+v0]; h2=RMap[u0+v1];
   h1=RMap[u1+v0]; h3=RMap[u1+v1];
   // Compute the color using bilinear interpolation (in 16.16)
   h0=(h0<<8)+a*(h1-h0);
   h2=(h2<<8)+a*(h3-h2);
   c=((h0<<8)+b*(h2-h0));
   rr = (c*color)>>6;

   // Green

   // Fetch the color at the four corners of the square the point is in
   h0=GMap[u0+v0]; h2=GMap[u0+v1];
   h1=GMap[u1+v0]; h3=GMap[u1+v1];
   // Compute the color using bilinear interpolation (in 16.16)
   h0=(h0<<8)+a*(h1-h0);
   h2=(h2<<8)+a*(h3-h2);
   c=((h0<<8)+b*(h2-h0));
   gg = (c*color)>>6;

   // Blue

   // Fetch the color at the four corners of the square the point is in
   h0=BMap[u0+v0]; h2=BMap[u0+v1];
   h1=BMap[u1+v0]; h3=BMap[u1+v1];
   // Compute the color using bilinear interpolation (in 16.16)
   h0=(h0<<8)+a*(h1-h0);
   h2=(h2<<8)+a*(h3-h2);
   c=((h0<<8)+b*(h2-h0));
   bb = (c*color)>>6;

   // Compute screen height using the scaling factor
   y=(((h-hy)*s)>>11) + 50;    // 100 = top-y, 11 = y-scale
   // Draw the column
   if ( y<(a=lasty[i]) )
   {
    word *b=toarea+a*320+i;
    //unsigned char *b=Video+a*320+i;
    int sr,cr,Rcol;
    int sg,cg,Gcol;
    int sb,cb,Bcol;

    if ( lastr[i]==-1 ) lastr[i]=rr;
    sr=(rr-lastr[i])/(a-y);
    cr=lastr[i];

    if ( lastg[i]==-1 ) lastg[i]=gg;
    sg=(gg-lastg[i])/(a-y);
    cg=lastg[i];

    if ( lastb[i]==-1 ) lastb[i]=bb;
    sb=(bb-lastb[i])/(a-y);
    cb=lastb[i];

    if ( a>199 )
    {
     b  -= (a-199)*320;
     cr += (a-199)*sr;
     cg += (a-199)*sg;
     cb += (a-199)*sb;
     a   = 199;
    }
    if (y<0) y=0;
    while (y<a)
    {
     Rcol=cr>>19;
     Gcol=cg>>19;
     Bcol=cb>>19;
     *b=(Bcol<<10)+(Gcol<<5)+Rcol;
     // *b=cc>>18;
     cr+=sr;
     cg+=sg;
     cb+=sb;
     b-=320;
     a--;
    }
    lasty[i]=y;
   }
   lastr[i]=rr;
   lastg[i]=gg;
   lastb[i]=bb;
   // Advance to next xy position
   x0+=sx; y0+=sy;
  }
 }

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 float FOV=3.141592654/4;   // half of the xy field of view

 //
 // Draw the view from the point x0,y0 (16.16) looking at angle a
 //
void View(word *toarea,int x0,int y0,float aa, int h)
 {
  int d;
  int a,b,u0,v0,u1,v1,h0,h1,h2,h3;

  memset(toarea,0,320*200*2);
  for(d=0;d<320;d++)
  {
   lasty[d] = 200;
   lastr[d] = -1;
   lastg[d] = -1;
   lastb[d] = -1;
  }

  // --- kalkulering av h ---

  // Compute viewpoint height value
  // Compute the xy coordinates; a and b will be the position inside the
  // single map cell (0..255).
  //a  = (x0>>8)&255;
  //b  = (y0>>8)&255;
  //u0 = ( x0>>16) & 0xFF;
  //v0 = ( y0>>8 ) & 0xFF00;
  //u1 = ( u0+1  ) & 0xFF;
  //v1 = ( v0+256) & 0xFF00;
  //// Fetch the height at the four corners of the square the point is in
  //h0=HMap[u0+v0]; h2=HMap[u0+v1];
  //h1=HMap[u1+v0]; h3=HMap[u1+v1];
  //// Compute the height using bilinear interpolation
  //h0=(h0<<8)+a*(h1-h0);
  //h2=(h2<<8)+a*(h3-h2);
  //h=((h0<<8)+b*(h2-h0))>>16;

  // Draw the landscape from near to far without overdraw
  for ( d=0; d<63; d+=1+(d>>6) )
  {
   Line(toarea,
        x0+d*65536*cos(aa-FOV),y0+d*65536*sin(aa-FOV),
        x0+d*65536*cos(aa+FOV),y0+d*65536*sin(aa+FOV),
        h,                // height
        100*256/(d+1),    // scale
        63-d);            // brightness (0..63)
  }
 }

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

void main(void)
 {
  FILE *fp;
  union REGS r;
  int i,k;
  float ss,sa,a,s;
  int x0,y0;
  // AXON
  int h;

  Vesa_Init(320,200,15);
  Vesa = (word *)Vesa_Ptr;

  // Enter 320x200x256 mode
  //r.w.ax=0x13; int386(0x10,&r,&r);
  // Set up the first 64 colors to a grayscale
  //for ( i=0; i<64; i++ )
  //{
  // outp(0x3C8,i);
  // outp(0x3C9,i);
  // outp(0x3C9,i);
  // outp(0x3C9,i);
  //}
  // Compute the height map
  ComputeMap(HMap,3);
  ComputeMap(RMap,0);
  ComputeMap(GMap,0);
  ComputeMap(BMap,0);

  //fp = fopen("stalag.pix","rb");
  //fread(temp,1,65536,fp);
  //fclose(fp);
  //fp = fopen("stalag.pal","rb");
  //fread(pal,1,768,fp);
  //fclose(fp);
  //for (i=0;i<65536;i++)
  //{
  // RMap[i] = pal[ (temp[i] * 3)    ] << 2;
  // GMap[i] = pal[ (temp[i] * 3) +1 ] << 2;
  // BMap[i] = pal[ (temp[i] * 3) +2 ] << 2;
  //}

  Vesa_SetMode(Vesa_Mode);

  //
  // Main loop
  //
  //   a     = angle
  //   x0,y0 = current position
  //   s     = speed constant
  //   ss    = current forward/backward speed
  //   sa    = angular speed
  //
  a  = 0;
  k  = 0;
  x0 = 0;
  y0 = 0;
  s  = 4096;
  ss = 0;
  sa = 0;

  // AXON

  h  = 0;

  while(k!=27)
  {
   // Draw the frame
   View(Buffer,x0,y0,a,h);
   // Blit the final image to the screen
   memcpy((unsigned char *)Vesa_Ptr,Buffer,320*200*2);

   // Update position/angle
   x0+=ss*cos(a);
   y0+=ss*sin(a);
   a+=sa;
   // User input
   if ( kbhit() )
   {
    if ( (k=getch())==0 ) k=-getch();
    switch(k)
    {
     case -75: sa-=0.005; break;
     case -77: sa+=0.005; break;
     case -72: ss+=s; break;
     case -80: ss-=s; break;
    }
   }
  }
  // Exit to text mode
  r.w.ax=0x03; int386(0x10,&r,&r);
 }
