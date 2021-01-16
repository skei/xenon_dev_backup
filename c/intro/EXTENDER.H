 // TINY Dos-Extender for Watcom... DPMI only (Win95, CWSDPMI, etc)

 #ifndef ext_h_included
 #define ext_h_included
 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

extern unsigned short int Code32Sel;  // Code Selector for Code32 SEGMENT
extern unsigned short int Data32Sel;  // Data Selector for Code32 SEGMENT
extern unsigned short int Flat32Sel;  // Data Selector with base of 00000000h

extern unsigned char      rm_Int;     // Real Mode interrupt to execute
extern unsigned short int rm_DS;      // Real Mode segment registers
extern unsigned short int rm_ES;      //

extern unsigned int       Code32Addr; // linear address of Code32 SEGMENT
extern unsigned int       PSPAddr;    // linear address of PSP SEGMENT
extern unsigned int       EnvAddr;    // linear address of Environment SEGMENT

extern unsigned short int PSPSeg;     // PSP Segment

extern unsigned char      HexTAB;

// ; Global Code32 procedures
//GLOBAL Start32      :NEAR       ; application entry point
//GLOBAL Exit32       :NEAR       ; routine to terminate application
//GLOBAL ErrorExit32  :NEAR       ; routine to terminate with error message

 // 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
 #endif
