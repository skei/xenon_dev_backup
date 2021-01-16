 #ifndef vesa_h_included
 #define vesa_h_included
 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

 #pragma pack(push);
 #pragma pack(1);

struct Vesa_Mode_Info
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

struct Vesa_Info
 {
  unsigned char VesaSignature[4];
  short int     VesaVersion;
  char         *OEMStringPtr;
  int           Capabilities;
  //char         *VideoModePtr;
  unsigned int  VideoModePtr;
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

extern int   VesaMode;
extern void *VesaPtr;
extern int   RedMaskSize;
extern int   RedFieldPosition;
extern int   BlueMaskSize;
extern int   BlueFieldPosition;
extern int   GreenMaskSize;
extern int   GreenFieldPosition;
extern int   BytesPerScanLine;

// ---

int  VBE2_Init(int X_Res, int Y_Res, int Depth);
void VBE2_UnInit(void);

void VBE2_SetMode(unsigned int mm);
 #pragma aux VBE2_SetMode=\
  " mov eax,0x4f02",\
  " int 0x10",\
 parm [ebx] modify [eax];

 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
 #endif
