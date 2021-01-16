 #include <math.h>
 #include <string.h>
 #include <stdio.h>

 // -------------------------------------------------------------------------

 #define PI 3.1415926535

 unsigned char *Xtable;
 unsigned char *Ytable;
 unsigned short int *Map;
 unsigned short int *AddMap;

 unsigned char *tex;
 unsigned char pal[768];



 // -------------------------------------------------------------------------

void SetMode(unsigned short int);
 #pragma aux SetMode =\
  "int 10h",\
 parm [ax] modify [ax];

void SetRGB(char color, char r,char g, char b);
 #pragma aux SetRGB=\
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

void WaitVR (void);
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

void CalcTunnelMap( unsigned short int *Map, int xsize, int ysize, float r,float sd)
 {
  float x,y;
  int blah;
  float yp;
  float xy2,l,z,t;
  int zc;
  unsigned char col;
  unsigned short int MapValue;
  unsigned char xx,yy;

  xsize >>= 1;
  ysize >>= 1;
  blah = 0;
  for ( y=-ysize; y<ysize; y++ )
  {
   yp = y;
   for ( x=-xsize; x<xsize; x++ )
   {
    xy2 = fabs(( x * x ) + ( y * y )/0.8);
    xy2 = sqrt( xy2 );
    if (xy2 != 0) l = r/xy2;
    z = l * sd;
    if ((x != 0) && (y != 0))
    {
     t = atan( y / x );
     t = 256 * t / ( 2 * PI );
     if ((x <= 0) && (y <= 0)) t++;
     if ((x > 0) && (y > 0)) t++;
     if (x >= 0) t = t+128;
    }
    zc = ceil( z / 4 );
    zc = zc % 256;
    col = zc;
    xx = col;
    // Xtable[blah] = col;
    xx = col;
    col = t;
    if ((y == 0) && (x < 0)) col = 1;
    // Ytable[blah] = col;
    yy = col;
    Map[blah] = (yy<<8)+xx;
    blah++;
   }
  }
 }


 // -------------------------------------------------------------------------

// eax   0      : temp
// ebx          : Y-add : X-add
// ecx counter
// edx                  : temp
// esi TunnelMap..............
// edi Screen/Buffer..........
// ebp

// eax = BitMap
// esi = TunnelMap
// edi = screen
// bh = yadd
// bl = xadd

void AsmDrawTunnel(unsigned char *screen, unsigned char *bitmap,
                   unsigned short int *tunnelmap,
                   unsigned char xadd, unsigned char yadd);
#pragma aux AsmDrawTunnel=\
 " push ebp",\
 " mov ebp,eax",\
 " mov ecx,64000",\
 "L1:",\
 " movzx eax,word ptr [esi]",\
 " add esi,2",\
 " add ah,bh",\
 " add al,bl",\
 " mov dl,[ebp+eax]",\
 " mov [edi],dl",\
 " inc edi",\
 " dec ecx",\
 " jnz L1",\
 " pop ebp",\
 parm [edi][eax][esi][bl][bh] modify [eax ecx edx esi edi];

 // ---

// eax   0      : temp
// ebx          : Y-add : X-add
// ecx counter
// edx                  : temp
// esi TunnelMap..............
// edi Screen/Buffer..........
// ebp

void AsmDrawTunnel640(unsigned char *screen, unsigned char *bitmap,
                      unsigned short int *tunnelmap,
                      unsigned char xadd, unsigned char yadd,
                      unsigned char xpos, unsigned char ypos);
#pragma aux AsmDrawTunnel640=\
 " push ebp",\
 " mov ebp,eax",\
 " xor edx,edx",\
 " mov dl,ch",\
 " add esi,edx",\
 " add esi,edx",\
 " and ecx,0xff",\
 " imul ecx,1280",\
 " add esi,ecx",\
 " mov dh,200",\
 "L1:",\
 " mov ecx,320",\
 "L2:",\
 " movzx eax,word ptr [esi]",\
 " add esi,2",\
 " add ah,bh",\
 " add al,bl",\
 " mov dl,[ebp+eax]",\
 " mov [edi],dl",\
 " inc edi",\
 " dec ecx",\
 " jnz L2",\
 " add esi,640",\
 " dec dh",\
 " jnz L1",\
 " pop ebp",\
 parm [edi][eax][esi][bl][bh][ch][cl] modify [eax ecx edx esi edi];


 // ---

void AsmAddTunnel(unsigned short int *screen,
                  unsigned short int *tunnelmap,
                  unsigned char xadd, unsigned char yadd);
#pragma aux AsmAddTunnel=\
 " mov ecx,64000",\
 "L1:",\
 " movzx eax,word ptr [esi]",\
 " add esi,2",\
 " add ah,bh",\
 " add al,bl",\
 " add [edi],ax",\
 " add edi,2",\
 " dec ecx",\
 " jnz L1",\
 parm [edi][esi][bl][bh] modify [eax ecx edx esi edi];

 // ---

void AsmAddTunnel640(unsigned short int *screen,
                     unsigned short int *tunnelmap,
                     unsigned char xadd, unsigned char yadd,
                     unsigned int xpos, unsigned int ypos);
#pragma aux AsmAddTunnel640=\
 " xor edx,edx",\
 " mov dl,ch",\
 " add esi,eax",\
 " add esi,eax",\
 " imul ecx,1280",\
 " add esi,ecx",\
 " mov dh,200",\
 "L1:",\
 " mov ecx,320",\
 "L2:",\
 " movzx eax,word ptr [esi]",\
 " add esi,2",\
 " add ah,bh",\
 " add al,bl",\
 " add [edi],ax",\
 " add edi,2",\
 " dec ecx",\
 " jnz L2",\
 " add esi,640",\
 " dec dh",\
 " jnz L1",\
 parm [edi][esi][bl][bh][eax][ecx] modify [eax ecx edx esi edi];


 // ---

void BlitAddedTunnel(unsigned char *screen, unsigned short int *map,
                     unsigned char *BitMap)
 {
  int i;

  for (i=0;i<64000;i++) screen[i] = BitMap[map[i]];
 }


 // -------------------------------------------------------------------------

void main(void)
 {
  FILE *fp;
  unsigned char x1,y1;
  unsigned char x2,y2;
  float x1pos,y1pos;
  float x2pos,y2pos;
  float x3pos,y3pos;
  int x,y;
  unsigned char *b;
  int i;

  Map = (unsigned short int *)malloc(320*200*2*4);
  AddMap = (unsigned short int *)malloc(320*200*2*4);

  tex = (unsigned char *)malloc(65536);

  fp = fopen("tex.pix","rb");
  fread(tex,1,65536,fp);
  fclose(fp);

  fp = fopen("tex.pal","rb");
  fread(pal,1,768,fp);
  fclose(fp);

  // radius, dist between viewer & screen
  // CalcTables(200,200);
  CalcTunnelMap(Map,640,400,200,200);

  SetMode(0x13);
  for (i=0;i<256;i++) SetRGB(i,pal[i*3],pal[i*3+1],pal[i*3+2]);

  x1 = 0;
  y1 = 0;
  x1pos = 0;
  y1pos = 0;
  x2pos = 1;
  y2pos = 1;
  x3pos = 2;
  y3pos = 2;
  while (!kbhit())
  {
   memset(AddMap,0,128000);
   WaitVR();
   AsmAddTunnel640(AddMap,Map,x1,y1,(int)(sin(x1pos)*159)+160,
                                    (int)(cos(y1pos)*99)+100);
   AsmAddTunnel640(AddMap,Map,x1,y1,(int)(sin(x2pos)*159)+160,
                                    (int)(cos(y2pos)*99)+100);
   AsmAddTunnel640(AddMap,Map,x1,y1,(int)(sin(x3pos)*159)+160,
                                    (int)(cos(y3pos)*99)+100);
   BlitAddedTunnel((unsigned char *)0xA0000,AddMap,tex);

   x1+=1;
   y1+=1;
    x1pos += 0.03;
    y1pos += 0.02;
    x2pos -= 0.04;
    y2pos -= 0.01;
    x3pos += 0.02;
    y3pos -= 0.03;

  }

  SetMode(3);

  free(tex);
  free(Map);
  free(AddMap);
 }



















