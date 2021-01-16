 #include "extender.h"
 #include "vesa.h"


// ---

void *MapPhysicalToLinear(void *PhysAddr, unsigned int Length);
 #pragma aux MapPhysicalToLinear=\
  " mov ax,0x800",\
  " mov ebx,ecx",\
  " shr ebx,16",\
  " and ecx,0xffff",\
  " mov esi,edi",\
  " shr esi,16",\
  " and edi,0xffff",\
  " int 0x31",\
  " jc L1",\
  " mov ax,bx",\
  " shl eax,16",\
  " mov ax,cx",\
  " jmp L2",\
  "L1:",\
  " mov eax,0",\
  "L2:",\
 parm [ecx][edi] value [eax] modify [ebx esi];

 //r.w.ax  = 0x800;
 //r.w.bx  = CUL(PhysAddr)>>16;
 //r.w.cx  = CUL(PhysAddr) & 0xFFFF;
 //r.w.si  = Length>>16;
 //r.w.di  = Length & 0xFFFF;
 //int386(0x31,&r,&r);
 //if (r.x.cflag) return(NULL_);
 //else return((void *)(CUL(r.w.bx<<16)+CUL(r.w.cx)));

 // ---

void *UnMapPhysicalToLinear(void *PhysAddr);
 #pragma aux UnMapPhysicalToLinear=\
  " mov ax,0x800",\
  " mov ebx,ecx",\
  " shr ebx,16",\
  " and ecx,0xffff",\
  " int 0x31",\
  parm [ecx] modify [eax ebx];

 //void UnmapPhysicalToLinear( void *LinAddr)
 //{
 //union REGS  r;
 //r.w.ax  = 0x801;
 //r.w.bx  = CUL(LinAddr)>>16;
 //r.w.cx  = CUL(LinAddr) & 0xFFFF;
 //int386(0x31,&r,&r);
 //}

void GetVesaInfo(void *p);
 #pragma aux GetVesaInfo=\
  "add edi,Code32Addr",\
  "mov eax,edi",\
  "and di,0x000f",\
  "shr eax,4",\
  "mov rm_ES,ax",\
  "mov ax,0x4f00",\
  "mov rm_Int,0x10",\
  "int 0x19",\
  parm [edi] modify [eax];

  // Input:    AX   = 4F01h   Return VBE mode information
  //           CX   =         Mode number
  //        ES:DI   =         Pointer to ModeInfoBlock structure
  // Output:   AX   =         VBE Return Status

void GetVesaModeInfo( unsigned short int mode, void *p);
 #pragma aux GetVesaModeInfo=\
  "add edi,Code32Addr",\
  "mov eax,edi",\
  "and di,0x000f",\
  "shr eax,4",\
  "mov rm_ES,ax",\
  "mov ax,0x4f01",\
  "mov rm_Int,0x10",\
  "int 0x19",\
  parm [cx][edi] modify [eax];


 // in = edi - seg:ofs (16:16)
 // out = ptr
void *RealToPtr(unsigned int s);
 #pragma aux RealToPtr=\
  " mov eax,edi",\
  " and eax,0xffff0000",\
  " shr eax,12",\
  " and edi,0xffff",\
  " add eax,edi",\
  " sub eax,Code32Addr",\
  parm [edi] value [eax];

void clear(void *,char,int);
 #pragma aux clear=\
  " cmp ecx,0",\
  " je return",\
  "L1:",\
  " mov [edi],al",\
  " inc edi",\
  " dec ecx",\
  " jnz L1",\
  "return:",\
  parm [edi][al][ecx] modify [edi eax ecx];

// ---

struct Vesa_Info      VesaInfo;
struct Vesa_Mode_Info VMI;
//
int   VesaMode;
void *VesaPtr;
int   RedMaskSize;
int   RedFieldPosition;
int   BlueMaskSize;
int   BlueFieldPosition;
int   GreenMaskSize;
int   GreenFieldPosition;
int   BytesPerScanLine;

// ---

int VBE2_Init(int X_Res, int Y_Res, int Depth)
 {
  unsigned short int i = 0;
  unsigned short int *ptr;


  VesaInfo.VesaSignature[0] = 'V';
  VesaInfo.VesaSignature[1] = 'B';
  VesaInfo.VesaSignature[2] = 'E';
  VesaInfo.VesaSignature[3] = '2';
  GetVesaInfo(&VesaInfo);
  //if (VesaInfo.VesaVersion != 0x0200)
  //{
  // DrawString("VBE 2.0 not supported!\n\r$");
  // return -1;
  //}
  ptr = (unsigned short int *)RealToPtr(VesaInfo.VideoModePtr);
  while (ptr[i] != 0xffff)
  {
   clear(&VMI,0,sizeof(struct Vesa_Mode_Info));
   GetVesaModeInfo(ptr[i],&VMI);

   if (VMI.PhysBasePtr)
   {
    if (VMI.BitsPerPixel == Depth)
    {
     if (VMI.NumberOfPlanes == 1)
     {
      if ((VMI.XResolution==X_Res) && (VMI.YResolution==Y_Res))
      {
       VesaMode           = (unsigned int)(ptr[i] | 0x4000);
       VesaPtr            = MapPhysicalToLinear((void *)VMI.PhysBasePtr,0x400000);
       RedMaskSize        = VMI.RedMaskSize;
       RedFieldPosition   = VMI.RedFieldPosition;
       GreenMaskSize      = VMI.GreenMaskSize;
       GreenFieldPosition = VMI.GreenFieldPosition;
       BlueMaskSize       = VMI.BlueMaskSize;
       BlueFieldPosition  = VMI.BlueFieldPosition;
       BytesPerScanLine   = VMI.BytesPerScanLine;
       break;
      }
     }
    }
   }
   i++;
  }
  if (VesaPtr) return 1;
  return 0;
 }

 // ---

void VBE2_UnInit(void)
 {
  UnMapPhysicalToLinear(VesaPtr);
 }











