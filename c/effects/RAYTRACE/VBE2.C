#include	<dos.h>
#include	<i86.h>

#include "vbe2.h"

#define CUL(a) ((unsigned int) (a))
#define NULL_ 0L

// --------------------------------------------------------------------------

int VesaMode;
int RedMaskSize;
int RedFieldPosition;
int GreenMaskSize;
int GreenFieldPosition;
int BlueMaskSize;
int BlueFieldPosition;
int BytesPerScanLine;
char *VesaPtr;

// --------------------------------------------------------------------------

void Set_Vesa( unsigned int mm)
{
 union REGS r;

 r.x.eax = 0x4F02;
 r.x.ebx = mm;
 int386(0x10,&r,&r);
}

// --------------------------------------------------------------------------

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

// --------------------------------------------------------------------------

void FreeDosMem(void *MemBlock)
{
 union REGS r;

 unsigned int *MemAdr=(unsigned int*) CUL(CUL(MemBlock)-16);
 r.x.eax = 0x0101;     // DPMI free DOS memory
 r.x.edx = MemAdr[0];  // Fetch selector.
 int386 (0x31, &r, &r);
}

// --------------------------------------------------------------------------

void *MapPhysicalToLinear(void *PhysAddr, unsigned int Length)
{
 union REGS  r;

 r.w.ax  = 0x800;
 r.w.bx  = CUL(PhysAddr)>>16;
 r.w.cx  = CUL(PhysAddr) & 0xFFFF;
 r.w.si  = Length>>16;
 r.w.di  = Length & 0xFFFF;
 int386(0x31,&r,&r);
 if (r.x.cflag) return(NULL_);
 else return((void *)(CUL(r.w.bx<<16)+CUL(r.w.cx)));
}

// --------------------------------------------------------------------------

void UnmapPhysicalToLinear( void *LinAddr)
{
 union REGS  r;

 r.w.ax  = 0x801;
 r.w.bx  = CUL(LinAddr)>>16;
 r.w.cx  = CUL(LinAddr) & 0xFFFF;
 int386(0x31,&r,&r);
}

// --------------------------------------------------------------------------

void VBE2_GetModeInfo(struct VESA_Mode_Info *VMI,unsigned int ModeNr)
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

// --------------------------------------------------------------------------

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

// --------------------------------------------------------------------------

int VesaInit(unsigned short int X_Res,unsigned short int Y_Res,unsigned short int Depth)
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
//        printf("Found screen mode %x\n",ScrModeNr);
        DispAddr=(void *)MapPhysicalToLinear((void *)VMI->PhysBasePtr,0x400000);
//        printf("with DispAddr = %x\n",(int)DispAddr);

        RedMaskSize = VMI->RedMaskSize;
        RedFieldPosition = VMI->RedFieldPosition;
        GreenMaskSize = VMI->GreenMaskSize;
        GreenFieldPosition = VMI->GreenFieldPosition;
        BlueMaskSize = VMI->BlueMaskSize;
        BlueFieldPosition = VMI->BlueFieldPosition;
        BytesPerScanLine = VMI->BytesPerScanLine;
        VesaPtr = DispAddr;
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
  VesaMode = ScrModeNr;
//  Set_Vesa(ScrModeNr);
  return 1;
 }
 return 0;
}

// --------------------------------------------------------------------------

void VesaUnInit(void)
{
// Set_Vesa(3);
 UnmapPhysicalToLinear(VesaPtr);
}

// --------------------------------------------------------------------------

void VesaGetStart(unsigned short int *x, unsigned short *y)
{
 union REGS r;

 r.x.eax = 0x4F07;
 r.w.bx = 1;
 int386(0x10,&r,&r);
 *x = r.w.cx;
 *y = r.w.dx;
}

// --------------------------------------------------------------------------

void VesaSetStart(unsigned short int x, unsigned short y)
{
 union REGS r;

 r.x.eax = 0x4F07;
 r.w.bx = 0;
 r.w.cx = x;
 r.w.dx = y;
 int386(0x10,&r,&r);
}

// --------------------------------------------------------------------------

void VesaGetScanlines(unsigned short int *bytes,
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

// --------------------------------------------------------------------------

void VesaSetScanlines(unsigned short int bytes)
{
 union REGS r;

 r.x.eax = 0x4F06;
 r.w.bx = 2;
 r.w.cx = bytes;
 int386(0x10,&r,&r);
}

