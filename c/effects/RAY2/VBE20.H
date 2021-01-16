 //北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
 // File        : VGOVBE20.H
 // Description : Vesa Bios Extention 2.0 header file
 // Notes       : Brought to you by Vertigo. If you use this, or have
 //               learned from this, send us an email and/or Greet us
 //               In your demo =).
 //
 //北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

 #ifndef __VBE20_H
 #define __VBE20_H

 #ifdef __cplusplus
 extern "C" {
 #endif

 // Vesa surface structure

#pragma pack(push);
#pragma pack(1);

typedef struct vbesurface
 {
  long vbeMode; // VBE mode number
  long width;   // width of screen
  long height;  // height of screen
  long bits;    // bits per pixel
  long depth;   // bytes per pixel
  char *lfb;    // pointer to LFB
 } VBESURFACE;

 // Vesa infomation block

typedef _Packed struct // VBEINFO
 {
  char VbeSignature[4];         // 'VESA' 4 byte signature
  short VbeVersion;             // VBE version number
  char *OemStringPtr;           // Pointer to OEM string
  unsigned long Capabilities;   // Capabilities of video card
  unsigned short *VideoModePtr; // Pointer to supported modes
  short TotalMemory;            // Number of 64kb memory blocks

  short OemSoftwareRev;         // OEM Software revision number
  char *OemVendorNamePtr;       // Pointer to Vendor Name string
  char *OemProductNamePtr;      // Pointer to Product Name string
  char *OemProductRevPtr;       // Pointer to Product Revision str
  char reserved[222];           // Pad to 256 byte block size
  char OemDATA[256];            // Scratch pad for OEM data
 } VBEINFO;

 #define VBE_CAPS_8BITDAC (1<<0)     // DAC can be 8 bit
 #define VBE_CAPS_NONVGA (1<<1)      // Non-vga compatible controller
 #define VBE_CAPS_BLANKRAMDAC (1<<2) // Programmed DAC with blank bit

 // Vesa mode infomation block

typedef _Packed struct // VBEMODEINFO
 {
  // Mandatory information for all VBE revisions:

  unsigned short  ModeAttributes;        // mode attributes
  unsigned char	  WinAAttributes;        // window A attributes
  unsigned char   WinBAttributes;        // window B attributes
  unsigned short  WinGranularity;        // window granularity
  unsigned short  WinSize;               // window size
  unsigned short  WinASegment;           // window A start segment
  unsigned short  WinBSegment;           // window B start segment
  unsigned long	  WinFuncPtr;            // pointer to window function
  unsigned short  BytesPerScanLine;      // bytes per scan line

  // Mandatory information for VBE 1.2 and above:

  unsigned short  XResolution;           // horizontal resolution in pixels or chars
  unsigned short  YResolution;           // vertical resolution in pixels or chars
  unsigned char   XCharSize;             // character cell width in pixels
  unsigned char	  YCharSize;             // character cell height in pixels
  unsigned char	  NumberOfPlanes;        // number of memory planes
  unsigned char	  BitsPerPixel;          // bits per pixel
  unsigned char	  NumberOfBanks;         // number of banks
  unsigned char	  MemoryModel;           // memory model type
  unsigned char	  BankSize;              // bank size in KB
  unsigned char	  NumberOfImagePages;    // number of images
  unsigned char	  _Reserved;             // reserved for page function

  // Direct Color fields (required for direct/6 and YUV/7 memory models)

  unsigned char	  RedMaskSize;           // size of direct color red mask in bits
  unsigned char	  RedFieldPosition;      // bit position of lsb of red mask
  unsigned char	  GreenMaskSize;         // size of direct color green mask in bits
  unsigned char	  GreenFieldPosition;    // bit position of lsb of green mask
  unsigned char	  BlueMaskSize;          // size of direct color blue mask in bits
  unsigned char	  BlueFieldPosition;     // bit position of lsb of blue mask
  unsigned char	  RsvdMaskSize;          // size of direct color reserved mask in bits
  unsigned char	  RsvdFieldPosition;     // bit position of lsb of reserved mask
  unsigned char	  DirectColorModeInfo;   // direct color mode attributes

  // Mandatory information for VBE 2.0 and above:

  unsigned long	  PhysBasePtr;           // physical address for flat frame buffer
  unsigned long	  OffScreenMemOffset;    // pointer to start of off screen memory
  unsigned short  OffScreenMemSize;      // amount of off screen memory in 1k units
  char		  __Reserved[206];
 } VBEMODEINFO;

#pragma pack(pop);

 // Flags to combine with mode number

 #define VBE_MODEFLAG_NOCLEAR  0x8000  // Dont clear screen on mode set
 #define VBE_MODEFLAG_LFB  0x4000  // Request LFB

 // Mode attributes flags

 #define VBEMODE_CAPS_AVAILABLE  (1<<0)  // Mode is available
 #define VBEMODE_CAPS_TTYOUTPUT  (1<<2)  // TTY output is available
 #define VBEMODE_CAPS_COLORMODE  (1<<3)  // Color video mode
 #define VBEMODE_CAPS_GRAPHMODE  (1<<4)  // Graphics mode
 #define VBEMODE_CAPS_NONVGA (1<<5)  // Non-VGA mode
 #define VBEMODE_CAPS_NONBANKED  (1<<6)  // Banked frame buffer not supported
 #define VBEMODE_CAPS_LINEAR (1<<7)  // Linear frame buffer is supported

 // Mode memory models

 #define VBEMODE_MODEL_TEXT  (0) // Text mode
 #define VBEMODE_MODEL_CGA (1) // CGA mode
 #define VBEMODE_MODEL_HERCULES  (2) // Hercules graphics mode
 #define VBEMODE_MODEL_PLANAR  (3) // VGA planar style mode
 #define VBEMODE_MODEL_PACKED  (4) // Packed pixel mode
 #define VBEMODE_MODEL_MODEX (5) // ModeX (unchained 4, 256 color)
 #define VBEMODE_MODEL_RGB (6) // Direct color RGB
 #define VBEMODE_MODEL_YUV (7) // Direct color YUV

//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//

 //  vbeDetect()
 //        Detect the VBE bios and get info from it.
 //        Also map the real mode pointers and memory into protected mode.
 //        Returns a pointer to a VBEINFO structure.
VBEINFO * vbeDetect(void);

 //  vbeGetModeInfo(MODE,VBEMODEINFO *)
 //        Get video mode information. The VBEMODEINFO field is filled with
 //        info about the specified VBE mode. Returns 1 on success and 0 on
 //        failure.
int vbeGetModeInfo(long,VBEMODEINFO *);

 //  vbeOpen(X_RES,Y_RES,BPP)
 //        Open the VBE driver with the given screen metrics. Returns a
 //        pointer to a VBESURFACE structure on success, or NULL on failure.
VBESURFACE *vbeOpen(long,long,long);

 //  vbeClose(VBESURFACE)
 //        Close the VBE driver for the given VBE SURFACE. The LFB is
 //        unmapped and the video mode is restored to 0x03.
void vbeClose(VBESURFACE *);

 //  vbeSetScanWidth(PIXELS)
 //        Sets the number of pixels per scan line in screen memory. Returns
 //        1 on success and 0 on failure.
int vbeSetScanWidth(long);

 //  vbeGetScanWidth()
 //        Returns the number of pixels per scan line in screen memory.
 //        Returns 0 on failure.
long vbeGetScanWidth(void);

 //  vbeSetDisplayStart(X,Y)
 //        Set the display start position. Returns 1 on success and 0 on
 //        failure.
int vbeSetDisplayStart(long,long);


 //  vbeGetDisplayStart(X *, Y *)
 //        Get X and Y display start position. The X and Y DWORD pointers are
 //        filled with the X and Y position. Returns 1 on success and 0 on
 //        failure.
int vbeGetDisplayStart(long *,long *);

 //  vbeVR()
 //        Wait for vertical retrace
void vbeVr(void);
 #pragma aux vbeVr =\
  " mov dx, 03dah ",\
  "start:         ",\
  " in al, dx     ",\
  " test al, 8    ",\
  " jnz start     ",\
  " mov dx, 03dah ",\
  "end:           ",\
  " in al, dx     ",\
  " test al, 8    ",\
  " jz end        ",\
  parm;

void vmode(long);
 #pragma aux vmode = "int 10h", parm [eax];

 #ifdef __cplusplus
 }
 #endif

 #endif
