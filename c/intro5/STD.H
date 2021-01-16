#ifndef std_h_included
#define std_h_included
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

// ±±±±± Defines ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#define NULL (0)
#define PI (3.1415926535)

 #undef __FILLER

 // dword registers

 struct DWORDREGS
 {
  unsigned int eax;
  unsigned int ebx;
  unsigned int ecx;
  unsigned int edx;
  unsigned int esi;
  unsigned int edi;
  unsigned int cflag;
 };

 #define __FILLER(a) unsigned short a;

 // word registers

 struct WORDREGS
 {
  unsigned short ax; __FILLER(_1)
  unsigned short bx; __FILLER(_2)
  unsigned short cx; __FILLER(_3)
  unsigned short dx; __FILLER(_4)
  unsigned short si; __FILLER(_5)
  unsigned short di; __FILLER(_6)
  unsigned int cflag;
 };

 // byte registers

 struct BYTEREGS
 {
  unsigned char al, ah;  __FILLER(_1)
  unsigned char bl, bh;  __FILLER(_2)
  unsigned char cl, ch;  __FILLER(_3)
  unsigned char dl, dh;  __FILLER(_4)
 };

 // general purpose registers union - overlays the corresponding dword,
 // word, and byte registers.

typedef union REGS
 {
  struct DWORDREGS x;
  struct WORDREGS  w;
  struct BYTEREGS  h;
 } REGS;

 // segment registers

typedef struct SREGS
 {
  unsigned short es, cs, ss, ds;
  unsigned short fs, gs;
 } SREGS;

// ±±±±± Global vars ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

extern short int RandSeed;

// ±±±±± Screen/Strings ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

 // \a = bell
 // \b = backspace
 // \f = formfeed
 // \n = new line (no carriage return);
 // \r = carriage return (no new line)
 // \t = horiz tab
 // \v = vert tab
 // \' \" \? \\ characters
 // \ddd octal value
 // \xdd hex value

 // s = ptr to $-terminated string
void DrawString(char *s);
 #pragma aux DrawString=\
  " mov ah,09h",\
  " int 21h",\
  parm [edx] modify [eax];

 // --------------------------------------------------------------------------

 // set video mode
void SetMode(unsigned short int);
 #pragma aux SetMode =\
  "int 10h",\
  parm [ax] modify [ax];

 // --------------------------------------------------------------------------

void SetRGB (char,char,char,char);
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

 // --------------------------------------------------------------------------

 // Vaits for Vertical retrace to START
void WaitVR (void);
 #pragma aux waitvr =\
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

// ±±±±± Keyboard ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±


char GetKey(void);
 #pragma aux GetKey=\
  " mov ah,7",\
  " int 21h",\
  value [al];

// ±±±±± Memory ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

 // in  : num = number of bytes to allocate
 // out : ptr to memory, or 0 for error
char *malloc(unsigned int num);
 #pragma aux malloc =\
  " add eax,4",\
  " mov cx,ax",\
  " mov ebx,eax",\
  " shr ebx,16",\
  " mov ax,0x0501",\
  " int 0x31",\
  " jc error",\
  " mov ax,bx",\
  " shl eax,16",\
  " mov ax,cx",\
  " mov [eax],si",\
  " mov [eax+2],di",\
  " add eax,4",\
  " jmp return",\
  "error:",\
  " mov eax,0",\
  "return:",\
  parm [eax] value [eax] modify [eax ebx ecx edx esi edi];

 // -------------------------------------------------------------------------

 // in  : ptr to memory to free
void free(void *);
 #pragma aux free=\
  " sub eax,4",\
  " mov si,[eax]",\
  " mov di,[eax+2]",\
  " mov ax,0x0502",\
  " int 0x31",\
  parm [eax] modify [eax esi edi];

 // -------------------------------------------------------------------------


void memset(void *,char,int);
 #pragma aux memset=\
  " cmp ecx,0",\
  " je return",\
  "L1:",\
  " mov [edi],al",\
  " inc edi",\
  " dec ecx",\
  " jnz L1",\
  "return:",\
  parm [edi][al][ecx] modify [edi eax ecx];

 // -------------------------------------------------------------------------

void memcpy(void *,void *,int);
 #pragma aux memcpy=\
  " cmp ecx,0",\
  " je return",\
  "L1:",\
  " mov al,[esi]",\
  " inc esi",\
  " mov [edi],al",\
  " inc edi",\
  " dec ecx",\
  " jnz L1",\
  "return:",\
  parm [edi][esi][ecx] modify [esi edi eax ecx];

// ±±±±± Disk/File/IO ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

typedef unsigned int FILE;

 // -------------------------------------------------------------------------

 // in  : f = ASCIIZ filename
 //       mode = openmode
 // out : file handle. 0 = error
FILE fopen(char *f, char mode);
 #pragma aux fopen=\
  " mov ah,0x3d",\
  " int 0x21",\
  " jnc return",\
  " mov eax,0",\
  "return:",\
  parm [edx][al] value [eax];

 // -------------------------------------------------------------------------

 // in  : b = buffer
 //       num = number of bytes to read
 //       fp = file handle
 // out : number of bytes actually read, or -1 for error
int fread(void *b, int num, FILE fp);
 #pragma aux fread=\
 " mov ah,0x3f",\
 " int 0x21",\
 parm [edx][ecx][ebx] value [eax];

 // -------------------------------------------------------------------------

int fclose(FILE fp);
 #pragma aux fclose=\
 " mov ah,0x3e",\
 " int 0x21",\
 parm [ebx] value [eax];

// ±±±±± IO ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void intDPMI(REGS *r);
 #pragma aux intDPMI=\
 " pushad",\
 " push ebp",\
 " mov ebp,eax",\
 " mov eax,[ebp]",\
 " mov ebx,[ebp+4]",\
 " mov ecx,[ebp+8]",\
 " mov edx,[ebp+12]",\
 " mov esi,[ebp+16]",\
 " mov edi,[ebp+20]",\
 " int 0x31",\
 " pop ebp",\
 " mov [ebp],eax",\
 " mov [ebp+4],ebx",\
 " mov [ebp+8],ecx",\
 " mov [ebp+12],edx",\
 " mov [ebp+16],esi",\
 " mov [ebp+20],edi",\
 " pushfd",\
 " pop eax",\
 " mov [ebp+24],eax",\
 " popad",\
 parm [eax] modify [eax ebx ecx edx esi edi];

 // -------------------------------------------------------------------------

void intDOS(REGS *r);
 #pragma aux intDOS=\
 " pushad",\
 " push ebp",\
 " mov ebp,eax",\
 " mov eax,[ebp]",\
 " mov ebx,[ebp+4]",\
 " mov ecx,[ebp+8]",\
 " mov edx,[ebp+12]",\
 " mov esi,[ebp+16]",\
 " mov edi,[ebp+20]",\
 " int 0x21",\
 " pop ebp",\
 " mov [ebp],eax",\
 " mov [ebp+4],ebx",\
 " mov [ebp+8],ecx",\
 " mov [ebp+12],edx",\
 " mov [ebp+16],esi",\
 " mov [ebp+20],edi",\
 " pushfd",\
 " pop eax",\
 " mov [ebp+24],eax",\
 " popad",\
 parm [eax] modify [eax ebx ecx edx esi edi];

 // -------------------------------------------------------------------------

void intVideo(REGS *r);
 #pragma aux intVideo=\
 " pushad",\
 " push ebp",\
 " mov ebp,eax",\
 " mov eax,[ebp]",\
 " mov ebx,[ebp+4]",\
 " mov ecx,[ebp+8]",\
 " mov edx,[ebp+12]",\
 " mov esi,[ebp+16]",\
 " mov edi,[ebp+20]",\
 " int 0x10",\
 " pop ebp",\
 " mov [ebp],eax",\
 " mov [ebp+4],ebx",\
 " mov [ebp+8],ecx",\
 " mov [ebp+12],edx",\
 " mov [ebp+16],esi",\
 " mov [ebp+20],edi",\
 " pushfd",\
 " pop eax",\
 " mov [ebp+24],eax",\
 " popad",\
 parm [eax] modify [eax ebx ecx edx esi edi];

 // -------------------------------------------------------------------------

extern int int386( int, union REGS *, union REGS * );
extern int int386x( int, union REGS *, union REGS *, struct SREGS * );

 // -------------------------------------------------------------------------

 // macros to break 'far' pointers into segment and offset components
#define  FP_OFF(__p) ((unsigned)(__p))
#define _FP_OFF(__p) ((unsigned)(__p))
 unsigned short FP_SEG( const volatile void __far * );
 #pragma aux  FP_SEG = __parm __caller [eax dx] __value [dx];
#define _FP_SEG FP_SEG

 // make a far pointer from segment and offset
#define MK_FP(__s,__o) (((unsigned short)(__s)):>((void __near *)(__o)))

 // -------------------------------------------------------------------------

void outp(unsigned short int port, char data);
 #pragma aux outp=\
 " out dx,al",\
 parm [dx][al];

 // -------------------------------------------------------------------------

void outpw(unsigned short int port, short int data);
 #pragma aux outpw=\
 " out dx,ax",\
 parm [dx][ax];

 // -------------------------------------------------------------------------

char inp(unsigned short int port);
 #pragma aux inp=\
 " in al,dx",\
 parm [dx] value [al];

 // -------------------------------------------------------------------------

char inpw(unsigned short int port);
 #pragma aux inpw=\
 " in ax,dx",\
 parm [dx] value [ax];

// ±±±±± Misc ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void Int2Hex(unsigned int h, char *S);
void Word2Hex(unsigned short int h, char *S);
short int Random(void);

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#endif
