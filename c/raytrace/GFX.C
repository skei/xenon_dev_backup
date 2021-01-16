//
// 컴� VBE2 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
//

 #include  <dos.h>
 #include  <i86.h>

 #define CUL(a) ((unsigned int) (a))

 //
 // --- Structures ---
 //

 #pragma pack(push);
 #pragma pack(1);

struct RMREGS
 {
  int EDI;
  int ESI;
  int EBP;
  int reserved_by_system;
  int EBX;
  int EDX;
  int ECX;
  int EAX;
  unsigned short int flags;
  unsigned short int ES,DS,FS,GS,IP,CS,SP,SS;
 };

 // ---

struct VESA_Mode_Info
 {
  unsigned short int ModeAttributes;
  unsigned char      WinAAttributes;
  unsigned char      WinBAttributes;
  unsigned short int WinGranularity;
  unsigned short int WinSize;
  unsigned short int WinASegment;
  unsigned short int WinBSegment;
  int                WinFuncPtr;
  unsigned short int BytesPerScanLine;

  unsigned short int XResolution;
  unsigned short int YResolution;
  unsigned char      XCharSize;
  unsigned char      YCharSize;
  unsigned char      NumberOfPlanes;
  unsigned char      BitsPerPixel;
  unsigned char      NumberOfBanks;
  unsigned char      MemoryModel;
  unsigned char      BankSize;
  unsigned char      NumberOfImagePages;
  unsigned char      Reserved;

  unsigned char      RedMaskSize;
  unsigned char      RedFieldPosition;
  unsigned char      GreenMaskSize;
  unsigned char      GreenFieldPosition;
  unsigned char      BlueMaskSize;
  unsigned char      BlueFieldPosition;
  unsigned char      RsvdMaskSize;
  unsigned char      RsvdFieldPosition;
  unsigned char      DirectColorModeInfo;
  int                PhysBasePtr;          /* VBE 2.0 Info */
  int                OffScreenMemOffset;   /* VBE 2.0 Info */
  short int          OffScreenMemSize;     /* VBE 2.0 Info */
  unsigned char      UnUsed[206];          /* VBE 2.0 Info */
 };

 // ---

struct Vesa_Info
 {
  unsigned char VesaSignature[4];
  short int     VesaVersion;
  char         *OEMStringPtr;
  int           Capabilities;
  char         *VideoModePtr;
  short int     TotalMemory;
  short int     OEMSoftwareRev;       /* VBE 2.0 Extensions */
  char         *OEMVendorPtr;         /* VBE 2.0 Extensions */
  char         *OEMProductNamePtr;    /* VBE 2.0 Extensions */
  char         *OEMProductRevPtr;     /* VBE 2.0 Extensions */
  unsigned char RESERVED[222];        /* VBE 2.0 Extensions */
  unsigned char OEM_DATA[256];        /* VBE 2.0 Extensions */
 };

 #pragma pack(pop);

// ---

int   Vesa_Mode;
int   Vesa_RedMaskSize;
int   Vesa_RedFieldPosition;
int   Vesa_GreenMaskSize;
int   Vesa_GreenFieldPosition;
int   Vesa_BlueMaskSize;
int   Vesa_BlueFieldPosition;
int   Vesa_BytesPerScanLine;
char *Vesa_Ptr;

// ---

void  Vesa_SetMode(unsigned int mm)
 {
  union REGS r;

  if (mm < 0x100 )
  {
   r.x.eax = mm;
   int386(0x10,&r,&r);
  }
  else
  {
   r.x.eax = 0x4F02;
   r.x.ebx = mm;
   int386(0x10,&r,&r);
  }
 }

 // ---

void *AllocDosMem(unsigned int RequestSize)
 {
  unsigned int MemSize;
  union REGS r;
  unsigned int *MemAdr;

  r.x.eax = 0x0100;                   // DPMI allocate DOS memory
  r.x.ebx = (RequestSize+15+16) >> 4; // Number of paragraphs requested+1
  int386 (0x31, &r, &r);
  if (r.x.cflag)
  {
   return (0L);
  }
  else
  {
   MemAdr=(unsigned int *) CUL((r.x.eax&0xFFFF) << 4);
   MemAdr[0]=r.x.edx;
   return(&MemAdr[4]);
  }
 }

 // ---

void  FreeDosMem(void *MemBlock)
 {
  union REGS r;

  unsigned int *MemAdr=(unsigned int*) CUL(CUL(MemBlock)-16);
  r.x.eax = 0x0101;     // DPMI free DOS memory
  r.x.edx = MemAdr[0];  // Fetch selector.
  int386 (0x31, &r, &r);
 }

 // ---

void *MapPhysicalToLinear(void *PhysAddr, unsigned int Length)
 {
  union REGS  r;

  r.w.ax  = 0x800;
  r.w.bx  = CUL(PhysAddr)>>16;
  r.w.cx  = CUL(PhysAddr) & 0xFFFF;
  r.w.si  = Length>>16;
  r.w.di  = Length & 0xFFFF;
  int386(0x31,&r,&r);
  if (r.x.cflag) return (0/*NULL_*/);
  else return((void *)(CUL(r.w.bx<<16)+CUL(r.w.cx)));
 }

 // ---

void  UnmapPhysicalToLinear( void *LinAddr)
 {
  union REGS  r;

  r.w.ax  = 0x801;
  r.w.bx  = CUL(LinAddr)>>16;
  r.w.cx  = CUL(LinAddr) & 0xFFFF;
  int386(0x31,&r,&r);
 }

 // ---

void  VBE2_GetModeInfo(struct VESA_Mode_Info *VMI,unsigned int ModeNr)
 {
  union REGS r;
  struct SREGS s;
  struct RMREGS rm;

  memset(&s,0,sizeof(struct SREGS));
  memset(&rm,0,sizeof(struct RMREGS));

  rm.EAX  = 0x4f01;
  rm.ECX  = ModeNr;
  rm.EDI  = 0;
  rm.ES   = CUL(VMI) >> 4;
  r.x.eax = 0x0300;
  r.x.ebx = 0x0010;
  r.x.ecx = 0x0000;
  s.ds    = FP_SEG(&r);
  s.es    = FP_SEG(&rm);
  r.x.edi = FP_OFF(&rm);
  int386x(0x31,&r,&r,&s);
 }

 // ---

void  VBE2_GetInfo(struct Vesa_Info *VesaBlk)
 {
  union   REGS    r;
  struct  SREGS   s;
  struct  RMREGS  rm;

  memset(&s,0,sizeof(struct SREGS));
  memset(&rm,0,sizeof(struct RMREGS));
  rm.EAX  = 0x4f00;
  rm.ECX  = 0;
  rm.EDI  = 0;
  rm.ES   = CUL(VesaBlk) >> 4;
  r.x.eax = 0x0300;
  r.x.ebx = 0x0010;
  r.x.ecx = 0x0000;
  s.ds    = FP_SEG(&r);
  s.es    = FP_SEG(&rm);
  r.x.edi = FP_OFF(&rm);
  int386x(0x31,&r,&r,&s);
 }

 // ---

int   Vesa_Init(unsigned short int X_Res,unsigned short int Y_Res,unsigned short int Depth)
 {
  void *DispAddr = 0;
  unsigned int ScrModeNr;
  struct Vesa_Info *VesaInfo;
  struct VESA_Mode_Info *VMI;
  struct VESA_Mode_Info *VMIMem;
  unsigned short int *ModePtr;
  struct ScreenMode *ScrMode;
  unsigned short int i=0;

  if(VesaInfo = AllocDosMem(sizeof(struct Vesa_Info)))
  {
   memset(VesaInfo,0,sizeof(struct Vesa_Info));
   strncpy(VesaInfo->VesaSignature,"VBE2",4);
   VBE2_GetInfo(VesaInfo);
   if( VMIMem = AllocDosMem(sizeof(struct VESA_Mode_Info)+0x8000))
   {
    VMI = (struct VESA_Mode_Info *)(CUL(VMIMem) + 4095 & ~4095);
    ModePtr = (unsigned short int *)(((CUL(VesaInfo->VideoModePtr) & 0xffff0000) >> 12) + (CUL(VesaInfo->VideoModePtr) & 0x0ffff));
    while(ModePtr[i] != 0xffff)
    {
     memset(VMI,0,sizeof(struct VESA_Mode_Info));
     VBE2_GetModeInfo(VMI,ModePtr[i]);
     if(VMI->PhysBasePtr) // We Have FlatMem
     {
      if(VMI->BitsPerPixel == Depth)
      {
       if(VMI->NumberOfPlanes == 1)
       {
        if ((VMI->XResolution==X_Res) && (VMI->YResolution==Y_Res))
        {
         ScrModeNr = (unsigned int)(ModePtr[i] | 0x4000);
         // printf("Found screen mode %x\n",ScrModeNr);
         DispAddr=(void *)MapPhysicalToLinear((void *)VMI->PhysBasePtr,0x400000);
         // printf("with DispAddr = %x\n",(int)DispAddr);
         Vesa_RedMaskSize = VMI->RedMaskSize;
         Vesa_RedFieldPosition = VMI->RedFieldPosition;
         Vesa_GreenMaskSize = VMI->GreenMaskSize;
         Vesa_GreenFieldPosition = VMI->GreenFieldPosition;
         Vesa_BlueMaskSize = VMI->BlueMaskSize;
         Vesa_BlueFieldPosition = VMI->BlueFieldPosition;
         Vesa_BytesPerScanLine = VMI->BytesPerScanLine;
         Vesa_Ptr = DispAddr;
         break;
        }
       }
      }
     }
     ++i;
    }
    FreeDosMem(VMIMem);
   }
   FreeDosMem(VesaInfo);
  }
  if (DispAddr)
  {
   Vesa_Mode = ScrModeNr;
   // Set_Vesa(ScrModeNr);
   return 1;
  }
  return 0;
 }

 // ---

void  Vesa_UnInit(void)
 {
  // Set_Vesa(3);
  UnmapPhysicalToLinear(Vesa_Ptr);
 }

 // ---

void  Vesa_GetStart(unsigned short int *x, unsigned short *y)
 {
  union REGS r;

  r.x.eax = 0x4F07;
  r.w.bx = 1;
  int386(0x10,&r,&r);
  *x = r.w.cx;
  *y = r.w.dx;
 }

 // ---

void  Vesa_SetStart(unsigned short int x, unsigned short y)
 {
  union REGS r;

  r.x.eax = 0x4F07;
  r.w.bx = 0;
  r.w.cx = x;
  r.w.dx = y;
  int386(0x10,&r,&r);
 }

 // ---

void  Vesa_GetScanlines(unsigned short int *bytes,
                       unsigned short int *pixels,
                       unsigned short int *numscanlines)
 {
  union REGS r;

  r.x.eax = 0x4F06;
  r.w.bx = 3;
  int386(0x10,&r,&r);
  *bytes = r.w.bx;
  *pixels = r.w.cx;
  *numscanlines = r.w.dx;
 }

 // ---

void  Vesa_SetScanlines(unsigned short int bytes)
 {
  union REGS r;

  r.x.eax = 0x4F06;
  r.w.bx = 2;
  r.w.cx = bytes;
  int386(0x10,&r,&r);
 }


//
// 컴� GFX 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
//

 #include <math.h>    // sqrt in spline

 /* 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

 TODO:

 - putimage w/clipping
 - putimage where the uppermost byte indicates transparency (0=draw, other=clear)
 - putimage w/alpha mapping, adding, etc... Upper byte...

 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴� */

#define NULL 0
#define RGB(r,g,b) ( (r) + (g<<8) + (b<<16) )

// --------------------

typedef unsigned int  Color;
typedef unsigned char byte;

typedef struct        tBitmap
 {
  int   width;
  int   height;
  int   xmin,xmax,ymin,ymax;
  int   bytesperline;
  int   datasize;
  byte *data;
 } tBitmap;

// --------------------

tBitmap  ScreenBMP;
tBitmap *Screen = &ScreenBMP;

// --------------------

void     GFX_init(int x, int y)
 {
  if (!Vesa_Init(x,y,24))
  {
   printf("Unable to initialize %ix%i 24-bit LFB mode\n",x,y);
   exit(1);
  }
  ScreenBMP.width        = x;
  ScreenBMP.height       = y;
  ScreenBMP.bytesperline = Vesa_BytesPerScanLine;
  ScreenBMP.datasize     = Vesa_BytesPerScanLine * y;
  ScreenBMP.data         = (byte *)Vesa_Ptr;
  ScreenBMP.xmin = 0;
  ScreenBMP.ymin = 0;
  ScreenBMP.xmax = x-1;
  ScreenBMP.ymax = y-1;
  Vesa_SetMode(Vesa_Mode);
 }

 // ---

void     GFX_exit(void)
 {
  Vesa_UnInit();
  Vesa_SetMode(3);
 }

 // ---

tBitmap *BMP_create(int x, int y)
 {
  tBitmap *B;
  B = (tBitmap *)malloc(sizeof(tBitmap));
  B->width        = x;
  B->height       = y;
  B->bytesperline = x*3;
  B->datasize     = B->bytesperline * y;
  B->data         = (byte *)malloc(B->datasize);
  B->xmin = 0;
  B->ymin = 0;
  B->xmax = x-1;
  B->ymax = y-1;

  return B;
 }

 // ---

void     BMP_delete(tBitmap *B)
 {
  free(B->data);
  free(B);
 }

 // ---

byte    *BMP_getptr(tBitmap *BMP, int x, int y)
 {
  return BMP->data + (y*BMP->bytesperline) + (x*3);
 }

 // ---

void     BMP_putpixel(tBitmap *BMP, int x, int y, Color c)
 {
  byte *ptr;
  ptr = BMP->data + (y*BMP->bytesperline) + (x*3);
  *ptr++ = (c & 0xff0000) >> 16;
  *ptr++ = (c & 0x00ff00) >> 8;
  *ptr++ = (c & 0x0000ff);
 }

 // ---

void     BMP_putpixelRGB(tBitmap *BMP, int x, int y, byte r, byte g, byte b)
 {
  byte *ptr;
  ptr = BMP->data + (y*BMP->bytesperline) + (x*3);
  *ptr++ = b;
  *ptr++ = g;
  *ptr++ = r;
 }


 // ---

Color    BMP_getpixel(tBitmap *BMP, int x, int y)
 {
  byte *ptr;
  ptr = BMP->data + (y*BMP->bytesperline) + (x*3);
  return ( (ptr[0]<<16) + (ptr[1]<<8) + ptr[2] );
 }

 // ---

void     BMP_horizline(tBitmap *BMP, int x, int y, int x2, Color c)
 {
  byte *ptr;
  int   i,r,g,b;
  ptr = BMP->data + (y*BMP->bytesperline) + (x*3);
  r = (c & 0x0000ff);
  g = (c & 0x00ff00) >> 8;
  b = (c & 0xff0000) >> 16;
  for (i=0;i<(x2-x+1);i++)
  {
   *ptr++ = b;
   *ptr++ = g;
   *ptr++ = r;
  }
 }

 // ---

void     BMP_horizlineInterp(tBitmap *BMP, int x, int y, int x2, Color c1, Color c2)
 {
  byte *ptr;
  int   i,r1,g1,b1,r2,g2,b2,ra,ga,ba;
  ptr = BMP->data + (y*BMP->bytesperline) + (x*3);
  r1 = (c1 & 0x0000ff) << 8;
  g1 = (c1 & 0x00ff00);
  b1 = (c1 & 0xff0000) >> 8;
  r2 = (c2 & 0x0000ff) << 8;
  g2 = (c2 & 0x00ff00);
  b2 = (c2 & 0xff0000) >> 8;
  ra = (r2-r1) / (x2-x);
  ga = (g2-g1) / (x2-x);
  ba = (b2-b1) / (x2-x);
  for (i=0;i<(x2-x+1);i++)
  {
   *ptr++ = (r1 >> 8) & 0xff;
   *ptr++ = (g1 >> 8) & 0xff;
   *ptr++ = (b1 >> 8) & 0xff;
   r1 += ra;
   g1 += ga;
   b1 += ba;
  }
 }

 // ---

void     BMP_vertline(tBitmap *BMP, int x, int y, int y2, Color c)
 {
  byte *ptr;
  int   i,r,g,b;
  ptr = BMP->data + (y*BMP->bytesperline) + (x*3);
  r = (c & 0x0000ff);
  g = (c & 0x00ff00) >> 8;
  b = (c & 0xff0000) >> 16;
  for (i=0;i<(y2-y+1);i++)
  {
   *ptr++ = b;
   *ptr++ = g;
   *ptr   = r;
   ptr += BMP->bytesperline - 2;
  }
 }

 // ---

void     BMP_line(tBitmap *BMP, int x0, int y0, int x1, int y1, Color c)
 {
  int dx,ix,cx,dy,iy,cy,m,i;

  if (x0<x1) { dx=x1-x0; ix= 1; }
        else { dx=x0-x1; ix=-1; }
  if (y0<y1) { dy=y1-y0; iy= 1; }
        else { dy=y0-y1; iy=-1; }
  if (dx>dy) m=dx; else m=dy;
  cx=cy=(m>>1);

  for (i=0;i<m;i++)
  {
   BMP_putpixel(BMP,x0,y0,c);
   if ((cx+=dx)>=m) { cx-=m; x0+=ix; }
   if ((cy+=dy)>=m) { cy-=m; y0+=iy; }
  }
 }

 // ---

void     BMP_rectangle(tBitmap *BMP, int x0, int y0, int x1,int y1, Color c)
 {
  BMP_horizline(BMP,x0,y0,x1,c);
  BMP_horizline(BMP,x0,y1,x1,c);
  BMP_vertline(BMP,x0,y0,y1,c);
  BMP_vertline(BMP,x1,y0,y1,c);
 }

 // ---

void     BMP_filledrectangle(tBitmap *BMP, int x0, int y0, int x1,int y1, Color c)
 {
  int i;
  for (i=y0;i<=y1;i++) BMP_horizline(BMP,x0,i,x1,c);
 }

 // ---

void     BMP_clear(tBitmap *BMP)
 {
  memset(BMP->data,0,BMP->datasize);
 }

 // ---

void     BMP_fill(tBitmap *BMP, Color c)
 {
  byte *p1,*p2;
  int   x,y,r,g,b;
  p1 = BMP->data;
  r = (c & 0x0000ff);
  g = (c & 0x00ff00) >> 8;
  b = (c & 0xff0000) >> 16;
  for (y=0;y<BMP->height;y++)
  {
   p2 = p1;
   for (x=0;x<BMP->width;x++)
   {
    *p2++ = b;
    *p2++ = g;
    *p2++ = r;
   }
   p1 += BMP->bytesperline;
  }
 }

 // ---

 // Lagrange polynomial curve through three points (p0, p1, p2).
void     BMP_spline(tBitmap *BMP, int x0,int y0,int x1, int y1, int x2, int y2, int nsteps, Color c)
 {
  int   j0,j1,j2;
  float j,jinc;       // float ?
  float fd1,fd2;
  float fr0,fr1,fr2;
  float x,y,xx,yy;

  j0  =0;
  j2  =1024;
  jinc=1024/nsteps;
  fd1=sqrt(  (x1-x0) * (x1-x0)  +  (y1-y0) * (y1-y0)  ); // p1->p2
  fd2=sqrt(  (x2-x1) * (x2-x1)  +  (y2-y1) * (y2-y1)  ); // p2->p3
  j1=1024*fd1/(fd1+fd2);
  if ((j1<1)||(j1>1023)) return;  // DIRTY ABORT - something's wrong.
  xx = x0;
  yy = y0;
  //pset(x0,y0,15);
  for (j=j0;j<j2;j+=jinc)    // <= -> < ?
  {
   fr0=(j-j0)*(j-j1) / (   (j2-j0)*(j2-j1)   );
   fr1=(j-j1)*(j-j2) / (   (j0-j1)*(j0-j2)   );
   fr2=(j-j0)*(j-j2) / (   (j1-j0)*(j1-j2)   );
   x = fr0*x2 + fr1*x0 + fr2*x1; // New x
   y = fr0*y2 + fr1*y0 + fr2*y1; // New y
   BMP_line(BMP,xx,yy,x,y,c);
   xx = x;
   yy = y;
   //pset(x,y,15); //Draw lines/dots.
  }
 }

 // ---

 // from d:\axon\spline or something like that (in case of bug-hunting)

 union outcode
 {
  struct
  {
   unsigned code0: 1;   /* x < 0 */
   unsigned code1: 1;   /* y < 0 */
   unsigned code2: 1;   /* x > MAXX */
   unsigned code3: 1;   /* y > MAXY */
  }  ocs;
  short outcodes;
 };

 // ---

void     BMP_clipline(tBitmap *BMP, int x1, int y1, int x2, int y2, Color c)
 {
  register union outcode oc1;
  register union outcode oc2;
  short inside;
  short outside;
  int temp;

  oc1.outcodes = 0; oc2.outcodes = 0;
  oc1.ocs.code0 = ( x1 < BMP->xmin );
  oc1.ocs.code1 = ( y1 < BMP->ymin );
  oc1.ocs.code2 = ( x1 > BMP->xmax );
  oc1.ocs.code3 = ( y1 > BMP->ymax );
  oc2.ocs.code0 = ( x2 < BMP->xmin );
  oc2.ocs.code1 = ( y2 < BMP->ymin );
  oc2.ocs.code2 = ( x2 > BMP->xmax );
  oc2.ocs.code3 = ( y2 > BMP->ymax );
  inside = ((oc1.outcodes | oc2.outcodes) == 0);
  outside = ((oc1.outcodes & oc2.outcodes) != 0);
  while( !outside && !inside )
  {
   if ( oc1.outcodes == 0 )
   {
    temp = x1;
    x1 = x2;
    x2 = temp;
    temp = y1;
    y1 = y2;
    y2 = temp;
    temp = oc1.outcodes;
    oc1.outcodes = oc2.outcodes;
    oc2.outcodes = temp;
   }
   if ( oc1.ocs.code0 ) // left
   {
    y1 += (long)(y2-y1) * (BMP->xmin-x1) / (x2 - x1);
    x1 = BMP->xmin;
   }
   else
   if ( oc1.ocs.code1 ) // right
   {
    x1 += (long)(x2-x1) * (BMP->ymin-y1) / (y2 - y1);
    // x1 += (x2-x1) * ((-y1) / (y2 - y1));
    y1 = BMP->ymin;
   }
   else
   if ( oc1.ocs.code2 ) // above
   {
    y1 += (long)(y2-y1) * (BMP->xmax - x1) / (x2 - x1);
    // y1 += (y2-y1) * ((MAXY - x1) / (x2 - x1));
    x1 = BMP->xmax;
   }
   else
   if ( oc1.ocs.code3 ) // below
   {
    x1 += (long)(x2 - x1) * (BMP->ymax - y1) / (y2 - y1);
    // x1 += (x2 - x1) * ((MAXY - y1) / (y2 - y1));
    y1 = BMP->ymax;
   }
   oc1.outcodes = 0;
   oc1.ocs.code0 = ( x1 < BMP->xmin );
   oc1.ocs.code1 = ( y1 < BMP->ymin );
   oc1.ocs.code2 = ( x1 > BMP->xmax );
   oc1.ocs.code3 = ( y1 > BMP->ymax );
   inside = ((oc1.outcodes | oc2.outcodes) == 0);
   outside = ((oc1.outcodes & oc2.outcodes) != 0);
  }
  if ( inside )
  {
   BMP_line(BMP, x1, y1, x2, y2, c);
  }
 }

 // ---

tBitmap *BMP_getimage(tBitmap *BMP, int x0, int y0, int x1, int y1)
 {
  int width,height;
  int i;
  byte *srcptr,*dstptr;
  tBitmap *B;

  if ((x0>BMP->xmax) || (x1<BMP->xmin) || (y0>BMP->ymax) || (y1<BMP->ymin))
  {
   return NULL;
  }
  if (x0 < BMP->xmin) x0 = BMP->xmin;
  if (y0 < BMP->ymin) y0 = BMP->ymin;
  if (x1 > BMP->xmax) x1 = BMP->xmax;
  if (y1 > BMP->ymax) y1 = BMP->ymax;
  width = x1-x0+1;
  height = y1-y0+1;
  B = BMP_create(width,height);
  srcptr = BMP->data + (y0*BMP->bytesperline) + (x0*3);
  dstptr = B->data;
  for (i=y0;i<=y1;i++)
  {
   memcpy(dstptr,srcptr,width*3);
   dstptr += B->bytesperline;
   srcptr += BMP->bytesperline;
  }
  return B;
 }

 // ---

void     BMP_putimage(tBitmap *dst, tBitmap *src, int x0, int y0)
 {
  int i;
  byte *dstptr,*srcptr;

  dstptr = dst->data + (y0*dst->bytesperline) + (x0*3);
  srcptr = src->data;
  for (i=y0;i<y0+src->height;i++)
  {
   memcpy(dstptr,srcptr,src->width*3);
   dstptr += dst->bytesperline;
   srcptr += src->bytesperline;
  }
 }

 // ---

 // Clip at right & top only (mouse cursor!)
void     BMP_putimageClipRT(tBitmap *dst, tBitmap *src, int x0, int y0)
 {
  int i,w,h;
  byte *dstptr,*srcptr;

  w = src->width;
  h = src->height;
  if ((x0+w) >= dst->xmax) w -= ((x0+w) - dst->xmax);
  if ((y0+h) >= dst->ymax) h -= ((y0+h) - dst->ymax);

  w *= 3;
  dstptr = dst->data + (y0*dst->bytesperline) + (x0*3);
  srcptr = src->data;
  for (i=y0;i<y0+h;i++)
  {
   memcpy(dstptr,srcptr,w);
   dstptr += dst->bytesperline;
   srcptr += src->bytesperline;
  }
 }

 // ---

void     BMP_drawchar(tBitmap *BMP, int x, int y, char c, Color clr, byte *font )
 {
  byte *f,*fp,*ptr;
  int BytesLeft,i,j;
  byte r,g,b;

  // if (c > 127) c = 32;
  ptr = BMP->data + (y*BMP->bytesperline) + (x*3);
  f = font+(c*64);

  r = (clr & 0x0000ff);
  g = (clr & 0x00ff00) >> 8;
  b = (clr & 0xff0000) >> 16;

  for (i=0;i<8;i++)
  {
   for (j=0;j<8;j++)
   {
    if ( *(f++) != 0 )
    {
     *ptr++ = b;
     *ptr++ = g;
     *ptr++ = r;
    }
    else ptr += 3;
   }
   ptr += BMP->bytesperline - 24;
  }
 }

 // ---

void     BMP_drawstring(tBitmap *BMP, int x, int y, char *s, Color c, byte *font )
 {
  int i;

  i=0;
  while (s[i] != 0)
  {
   BMP_drawchar(BMP,x,y,s[i],c,font);
   x+=8;
   i++;
  }
 }

 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

 byte StdFont[] =
 {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  1,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  1,  0,  0,  1,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  1,  0,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  1,
  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  0,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,
  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,
  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,
  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,
  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  1,  1,
  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,
  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,
  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,
  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,
  1,  1,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,
  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  1,  1,
  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,
  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,
  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  1,
  1,  1,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,
  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  0,  0,  0,  0,  1,  0,
  0,  1,  0,  0,  0,  0,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,
  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,
  0,  1,  1,  1,  1,  0,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,
  0,  0,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  0,  1,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,
  1,  1,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,
  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,
  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,
  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,
  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  1,
  1,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  0,  0,  1,  1,  0,  0,  1,  0,
  1,  0,  1,  1,  0,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  1,  1,  1,  0,
  0,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  0,  0,  1,  1,  1,  1,  0,
  0,  0,  1,  0,  1,  1,  0,  1,  0,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,
  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  0,  0,  0,
  1,  1,  1,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  1,  1,
  1,  0,  0,  0,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,
  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,
  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,  1,  1,  0,  1,  1,  1,  1,  0,  1,
  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,
  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,
  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  1,  0,  0,  0,
  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  1,
  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,
  1,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,
  1,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  1,
  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  1,
  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  1,  0,  0,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  1,
  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  0,
  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,
  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,
  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,
  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,
  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,
  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  1,
  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  1,  1,  0,  1,  1,  0,  1,  1,  1,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  1,  1,  1,  1,  1,
  1,  1,  1,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,
  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,
  0,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  1,  0,  1,
  1,  0,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,  1,  1,  1,  0,
  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,
  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,
  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,
  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,
  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,
  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,
  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,
  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,
  1,  1,  1,  1,  0,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  0,
  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,
  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  1,
  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,
  0,  0,  0,  1,  0,  0,  1,  1,  0,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,
  0,  0,  0,  1,  1,  0,  1,  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,  0,  1,
  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  0,  0,  1,  1,  0,  1,  0,  0,
  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  1,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  1,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,  1,
  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  1,  1,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,
  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  1,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  1,  0,
  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,
  0,  1,  1,  0,  1,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  1,  1,  0,  1,  1,  1,  0,  0,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,
  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,  0,  1,  1,  1,  0,  1,  1,  0,
  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,
  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,  1,
  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  1,  0,  0,  0,
  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  1,
  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  1,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,
  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,
  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  1,  1,  1,  1,  0,  0,  1,  0,  1,  1,  0,  1,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,
  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,
  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,
  0,  0,  0,  1,  1,  0,  1,  1,  0,  1,  0,  1,  1,  0,  1,  1,  1,  1,  1,
  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,
  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,
  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  1,  1,  1,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  1,  1,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,
  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,
  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  0,  1,  1,  1,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  1,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  0,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,
  1,  1,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,
  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,
  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,
  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,
  1,  1,  1,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  1,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  1,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,
  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,
  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,
  1,  1,  1,  1,  0,  1,  1,  0,  1,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  1,  1,  0,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  1,  1,  0,  1,  1,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,
  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  1,  1,  1,  0,  0,  0,  1,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,
  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,
  1,  0,  1,  1,  0,  1,  0,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,
  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,
  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  0,  0,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  1,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  1,  1,  0,  1,  1,  0,
  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,
  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,
  0,  0,  0,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  0,
  1,  1,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,
  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,
  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,
  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  0,  1,  1,  0,
  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,
  1,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,
  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,
  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  0,
  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,  1,
  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  0,  0,
  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  1,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,
  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,
  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,
  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,
  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  1,  1,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  1,
  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  0,  0,  1,  1,  1,  1,  0,
  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  1,  1,  0,
  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,
  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  1,
  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0,  1,  0,  1,  1,
  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,
  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,
  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,
  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  0,
  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  1,  1,  1,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,
  0,  1,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,
  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,
  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  1,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,
  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,
  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  1,
  1,  0,  0,  1,  1,  0,  1,  1,  1,  0,  1,  1,  0,  1,  1,  1,  1,  1,  1,
  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  0,
  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  0,  0,  0,  1,  0,  1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  1,  0,  0,  0,
  1,  0,  1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,
  0,  0,  0,  1,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  0,  0,  0,
  1,  0,  0,  0,  0,  1,  0,  1,  0,  1,  0,  1,  1,  0,  1,  0,  1,  0,  1,
  0,  0,  1,  0,  1,  0,  1,  0,  1,  1,  0,  1,  0,  1,  0,  1,  0,  0,  1,
  0,  1,  0,  1,  0,  1,  1,  0,  1,  0,  1,  0,  1,  0,  0,  1,  0,  1,  0,
  1,  0,  1,  1,  0,  1,  0,  1,  0,  1,  0,  1,  1,  0,  1,  1,  0,  1,  1,
  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  0,  1,  1,  0,  1,  1,  1,  1,  1,
  0,  1,  1,  1,  0,  1,  1,  0,  1,  1,  0,  1,  1,  0,  1,  1,  1,  0,  1,
  1,  1,  1,  1,  0,  1,  1,  0,  1,  1,  1,  1,  1,  0,  1,  1,  1,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,
  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  1,  1,  1,  1,  0,
  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,
  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,
  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,
  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,
  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  1,  1,  1,
  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,
  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,
  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,
  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,
  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,
  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,
  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,
  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  1,
  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  1,  1,
  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,
  1,  1,  0,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,
  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,
  0,  0,  1,  1,  0,  1,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,
  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,
  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  1,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  1,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,
  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,
  1,  0,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  1,  1,  1,  0,  0,
  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,
  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,
  0,  0,  1,  1,  0,  1,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  1,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,
  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  1,  1,  1,  1,  0,
  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  1,  1,  1,
  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,
  1,  0,  1,  1,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  1,  1,
  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,
  1,  1,  0,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,
  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,
  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,
  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,
  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,
  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,
  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,
  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,
  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  1,  1,
  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,
  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,
  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,
  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,
  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  1,
  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,
  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,
  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  1,  1,  0,  1,  1,  0,  1,
  1,  1,  0,  0,  1,  1,  0,  0,  1,  0,  0,  0,  1,  1,  0,  1,  1,  1,  0,
  0,  0,  1,  1,  1,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,  1,  1,  0,
  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,
  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,
  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  1,  1,  0,  1,  1,  0,  0,
  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  1,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,
  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  1,  1,  0,  1,
  1,  0,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,
  1,  1,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  0,  0,  0,  1,
  1,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,
  1,  1,  0,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,
  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  1,  1,  1,  0,  1,
  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  1,  1,
  1,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,
  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,
  1,  1,  1,  0,  1,  1,  0,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1,  0,  1,
  1,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  1,  1,  0,  1,  1,  0,  1,  1,
  1,  1,  0,  1,  1,  0,  1,  1,  0,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  1,  1,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,
  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,
  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,
  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,
  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,
  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,
  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  1,  1,  0,  1,  1,  0,
  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  1,
  1,  0,  1,  1,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  1,  1,  0,
  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,
  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  1,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,
  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  1,  1,
  1,  1,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,
  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,
  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,
  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  1,
  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0
 };

//
// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
//

 /*
 void main(void)
 {
  tBitmap *b;
  int i;

  GFX_init(320,200);
  for (i=0;i<15;i++) BMP_horizlineInterp(Screen,0,i,15,0x0000ff,0xff0000);
  b = BMP_getimage(Screen,0,0,15,15);
  for (i=0;i<320;i++) BMP_putimageClipRT(Screen,b,i,i/2);
  getch();
  GFX_exit();
 }
 */
