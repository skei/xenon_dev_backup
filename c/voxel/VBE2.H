 #ifndef _vesa_h_incl_
 #define _vesa_h_incl_

//
// --- Global variables ---
//

extern int   Vesa_Mode;
extern int   Vesa_RedMaskSize;
extern int   Vesa_RedFieldPosition;
extern int   Vesa_GreenMaskSize;
extern int   Vesa_GreenFieldPosition;
extern int   Vesa_BlueMaskSize;
extern int   Vesa_BlueFieldPosition;
extern int   Vesa_BytesPerScanLine;
extern char *Vesa_Ptr;

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

//
// --- Function prototypes ---
//

int  Vesa_Init         (unsigned short int X_Res,
                        unsigned short int Y_Res,
                        unsigned short int Depth);
void Vesa_UnInit       (void);
void Vesa_SetMode      (unsigned int mm);
void Vesa_GetStart     (unsigned short int *x, unsigned short *y);
void Vesa_SetStart     (unsigned short int x, unsigned short y);
void Vesa_GetScanlines (unsigned short int *bytes,
                        unsigned short int *pixels,
                        unsigned short int *numscanlines);
void Vesa_SetScanlines (unsigned short int bytes);

 // -------------------------------------------------------------------------
 #endif
