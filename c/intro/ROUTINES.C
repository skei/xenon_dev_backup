 #include "std.h"
 #include "fmath.h"

 // plasma & fractal map stuff ---

 #define RANDOM(a) (rand()/(RAND_MAX/(a)))         // a simple random function
 #define MAPSHIFT   8                              // 2^MAPSHIFT = size of map
 #define MAPSIZE    (1<<MAPSHIFT)                  // size of one side of the map
 #define BYTE(a)    (a&(MAPSIZE-1))                // put the range within MAPSIZE
 #define INDEX(x,y) ((BYTE(y)<<MAPSHIFT)+BYTE(x))  // index calculation

 #define SkurrNum 20
 #define SkurrMax 20

 // ±±± VARIABLES ±±±

 int Rseed;         // Random seed
 int Mul320[200];
 int Skurr_Array[200][SkurrNum];
 unsigned char rng[321];
 int lastzobs;

//
// ±±± Misc ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

 // ---

float Random_Float( float x )
 {
  Rseed = (Rseed*134775813)+1;
  if (Rseed<0) return ((Rseed/4294967296.0) + 1.0);
  else return (Rseed/4294967296.0);
 }

 // ---

int Random_Int( int x )
 {
  Rseed = (Rseed*134775813)+1;
  return (Rseed >> 16) % x;
 }

 // ---

void Init_Mul320Table(void)
 {
  int i;
  for (i=0; i < 200; i++) Mul320[i] = i * 320;
 }

//
// ±±± Map/Texture/Gfx calculation ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

unsigned char newcolor(int mc, int n, int dvd)
 {
  int loc;

  loc = (mc + n - RANDOM(n << 1)) / dvd - 1;
  if (loc > 255) loc = 255;
  if (loc < 10) loc = 10;
  return(loc);
 }

 // ---

void createfractalmap(unsigned char *map,int x1, int y1, int x2, int y2)
 {
  int p1, p2, p3, p4;
  int xn, yn, dxy;

  if (((x2-x1) < 2) && ((y2-y1) < 2)) return;
  p1 = map[INDEX(x1,y1)];
  p2 = map[INDEX(x1,y2)];
  p3 = map[INDEX(x2,y1)];
  p4 = map[INDEX(x2,y2)];
  xn = (x2+x1) >> 1;
  yn = (y2+y1) >> 1;
  dxy = 5*(x2 - x1 + y2 - y1) / 3;
  if (map[INDEX(xn,y1)] == 0) map[INDEX(xn,y1)] = newcolor(p1+p3, dxy, 2);
  if (map[INDEX(x1,yn)] == 0) map[INDEX(x1,yn)] = newcolor(p2+p4, dxy, 2);
  if (map[INDEX(x2,yn)] == 0) map[INDEX(x2,yn)] = newcolor(p3+p4, dxy, 2);
  if (map[INDEX(xn,y2)] == 0) map[INDEX(xn,y2)] = newcolor(p1+p2, dxy, 2);
  map[INDEX(xn,yn)] = newcolor(p1+p2+p3+p4, dxy, 4);
  createfractalmap(map,x1, y1, xn, yn);
  createfractalmap(map,xn, y1, x2, yn);
  createfractalmap(map,x1, yn, xn, y2);
  createfractalmap(map,xn, yn, x2, y2);
 }

 // ---

 // ---

 // Marble Generator
void marble (unsigned char *source, unsigned char *dest, float scale,
             float turbulence, unsigned char *xrpage, unsigned char *yrpage)
 {
  int i,x,y;
  unsigned char xx,yy;

  int cval[256];
  int sval[256];
  for (i=0; i<256; i++)
  {
   sval[i]=-scale*fsin((float)i/(float)turbulence);
   cval[i]= scale*fcos((float)i/(float)turbulence);
  }
  for (x=0; x<256; x++)
   for (y=0; y<256; y++)
   {
    xx=(x+sval[xrpage[x+(y<<8)]]);
    yy=(y+cval[yrpage[x+(y<<8)]]);
    dest[x+(y<<8)]=source[xx+(yy<<8)];
   }
 }

 /* eaxmple of usage:

 unsigned char *xrpage = (unsigned char *)malloc(0x10000);
 unsigned char *yrpage = (unsigned char *)malloc(0x10000);
 memset(xrpage,0,65536);
 memset(yrpage,0,65536);
 memset(source,0,65536);
 // Generate Source Images (we use a different plasma for x and y
 // displacement. That makes the marbles more weired!
 xrpage[0] = 127;
 createfractalmap(xrpage,0,0,255,255);
 yrpage[0] = 85;
 createfractalmap(yrpage,0,0,255,255);
 source[0] = 170;
 createfractalmap(source,0,0,255,255);
 Smooth(xrpage);
 Smooth(xrpage);
 Smooth(xrpage);
 Smooth(yrpage);
 Smooth(yrpage);
 Smooth(yrpage);
 Smooth(source);
 Smooth(source);
 Smooth(source);
 scale = 14;
 turbulence = 30;
 marble ((unsigned char *) source,
         (unsigned char *) test,
         scale, turbulence,
         (unsigned char *) xrpage,
         (unsigned char *) yrpage);
 */

void HexagonMap(unsigned char *dest, int xn, int yn, int c1, int c2, int c3)
 {
  int x,y,i,j,c;

  for (j=0; j<256; j++)
  {
   for (i=0; i<256; i++)
   {
    x=i*xn;
    y=j*yn;
    y&=255;
    if (y&128) x+=128;
    y&=127;
    y*=3;
    x*=3;
    c=x>>8;
    x&=255;
    if ((x+y)<128) c++;
    if ((x-y)>=128) c+=2;
    c%=3;
    *dest++=(c==0)?c1:(c==1)?c2:c3;
   }
  }
 }

 // iter1 = 10
 // dir   = 1
void CalcBackground(char *buffer, int iter1, int dir, int addcolor)
 {
  unsigned short int from,to;
  int counter1,counter2;
  char data;

  from=0;
  to=0;
  for (counter1=0;counter1<iter1;counter1++)
  {
   for (counter2=0;counter2<65536;counter2++)
   {
    data = buffer[from+=2];      /* screen */
    if ((data&1)==1) (to+=(320+dir)); else (to+=320);
    buffer[to]++;                /* screen */
   }
  }
  for (counter1=0;counter1<65536;counter1++)
  {
   buffer[counter1] += addcolor;
  }
 }

 // ---

void Smooth(unsigned char *map)
 {
  int x,y;

  for (x = 1; x < (MAPSIZE-1); x++)
  {
   for (y = 1; y < (MAPSIZE-1); y++)
   {
    map[INDEX(x,y)] = (map[INDEX(x-1,y-1)] +
                       map[INDEX(x-1,y+1)] +
                       map[INDEX(x+1,y-1)] +
                       map[INDEX(x+1,y+1)] +
                       map[INDEX(x,  y-1)] +
                       map[INDEX(x,  y+1)] +
                       map[INDEX(x-1,y  )] +
                       map[INDEX(x+1,y  )]) >> 3;
   }
  }
 }

//
// ±±± Spherical Mapping ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void SphMap_Init( char *p )
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

 // ---

 //void SphMap_Draw( char *map, char *dest, unsigned char u, unsigned char v, char *tex )
 //{
 // int i;
 // unsigned char *m;
 //
 // m = (unsigned char *)map;
 // for (i=0;i<64000;i++)
 // {
 //  *dest++ = tex[(((v + *m++)<<8) + (u + *m++))&0xffff];
 // }
 //}

void SphMap_Draw( char *map, char *dest, char uadd, char vadd,char *tex )
 {
  int i;
  unsigned short int uv_add;
  unsigned short int *m;
  unsigned char u,v;

  for (i=0;i<64000;i++)
  {
   v = (*map++)+vadd;
   u = (*map++)+uadd;
   *dest++ = tex[((v<<8)+u)&0xffff];
  }
 }

//
// ±±± RotoZoom ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

 // scale = 1024 = 1
void RotoZoom(char *screen,char *image, float angle, int scale)
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
    // if (ty<=199 && tx<=319)
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

//
// ±±± "skurr" ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

void Skurr_Init(void)
 {
  int i,j;
  float displace;
  float delta;

  for (i=0;i<200;i++)
  {
   // !!! skift ut rand med Random_Float eller noe slikt... !!!
   // do { displace = (rand()%(SkurrMax*2))-SkurrMax; } while (displace == 0);
   delta = displace/SkurrNum;
   for (j=SkurrNum;j>=0;j--)
   {
    Skurr_Array[i][j] = (int)displace;
    displace-=delta;
   }
  }
 }

 // ---

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

//
// ±±± Voxel ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//

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

  snf = 1024*fsin(dir);
  csf = 1024*fcos(dir);

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
       ps = Mul320[y];
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


