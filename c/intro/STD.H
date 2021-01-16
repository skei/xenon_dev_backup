 // Replacement stuff for the Watcom std libs

 #ifndef std_h_included
 #define std_h_included
 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

 #include "extender.h"

//
// ÄÄÄ Defines ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

 #define NULL (0)

 #ifndef PI
  #define PI (3.1415926535)
 #endif

//
// ÄÄÄ Screen/strings ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

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
  " add edx,Code32Addr",\
  " mov eax,edx",\
  " and dx,0x000f",\
  " shr eax,4",\
  " mov rm_DS,ax",\
  " mov ah,09h",\
  " mov rm_Int,0x21",\
  " int 0x19",\
  parm [edx] modify [eax];

 // ---

void DrawDword(unsigned int h);
void DrawWord(unsigned short int h);

 // set video mode
void SetMode(unsigned short int);
 #pragma aux SetMode =\
  "int 10h",\
  parm [ax] modify [ax];

 // ---

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

 // ---

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

//
// ÄÄÄ Keyboard ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

char GetKey(void);
 #pragma aux GetKey=\
  " mov ah,7",\
  " int 21h",\
  value [al];

char kbhit(void);
 #pragma aux kbhit = \
  "mov ah,0Bh" \
  "int 21h" \
  value [al];

char getch(void);
 #pragma aux getch = \
  "mov ah,08h" \
  "int 21h" \
  value [al];

//
// ÄÄÄ IO ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

void  outp(unsigned short int Port, unsigned char Value);
 #pragma aux outp = \
  "out dx,al" \
  parm [dx] [al];

void  outpw(unsigned short int Port, unsigned short int Value);
 #pragma aux outpw = \
  "out dx,ax" \
  parm [dx] [ax];

unsigned char inp(unsigned short int Port);
 #pragma aux inp = \
  "in al,dx" \
  parm [dx] value [al];

unsigned short int inpw(unsigned short int Port);
 #pragma aux inpw = \
  "in ax,dx" \
  parm [dx] value [ax];

//
// ÄÄÄ Memory ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

 //Allocate Memory Block
 //In   : AX = 0501h
 //       BX:CX = Size of memory block to allocate in bytes
 //Out  : If function was successful   : Carry flag is clear
 //                                      BX:CX = Linear address
 //                                      SI:DI = Memory block handle
 //       If function was unsuccessful : Carry flag is set

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
  " sub eax,Code32Addr",\
  " mov [eax],si",\
  " mov [eax+2],di",\
  " add eax,4",\
  " jmp return",\
  "error:",\
  " mov eax,0",\
  "return:",\
  parm [eax] value [eax] modify [eax ebx ecx edx esi edi];

 // ---

 //Free Memory Block
 //In   : AX = 0502h
 //       SI:DI = Handle of memory block to free
 //Out  : If function was successful   : Carry flag is clear
 //       If function was unsuccessful : Carry flag is set

 // in  : ptr to memory to free
void free(void *);
 #pragma aux free=\
  " sub eax,4",\
  " mov si,[eax]",\
  " mov di,[eax+2]",\
  " mov ax,0x0502",\
  " int 0x31",\
  parm [eax] modify [eax esi edi];

 // ---


void memset(void *,unsigned char,int);
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

 // ---

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

//
// ÄÄÄ Disk/File IO ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

typedef unsigned int FILE;

 // ---

 // !!!!!!!!!!
 // FIKS DISSE! s† de kaller int 0x19 og setter opp rm_DS
 // Ellers funker det ikke alltid (pmode DS er ikke alltid lik realmode DS)
 // !!!!!!!!!!

 // in  : f = ASCIIZ filename
 //       mode = openmode
 // out : file handle. 0 = error

   //  mov ah,0x3d
   //  int 0x21
   //  jnc return
   //  mov eax,0
   // return:

FILE fopen(char *f, char mode);
 #pragma aux fopen=\
  " add edx,Code32Addr",\
  " mov ebx,edx",\
  " and dx,0x000f",\
  " shr ebx,4",\
  " mov rm_DS,bx",\
  " mov ah,0x3d",\
  " mov rm_Int,0x21",\
  " int 0x19",\
  " jnc return",\
  " mov eax,0",\
  "return:",\
 parm [edx][al] modify [ebx] value [eax];

 // ---

 // in  : b = buffer
 //       num = number of bytes to read
 //       fp = file handle
 // out : number of bytes actually read, or -1 for error

   //  mov ah,0x3f
   //  int 0x21

int fread(void *b, int num, FILE fp);
 #pragma aux fread=\
  " add edx,Code32Addr",\
  " mov eax,edx",\
  " and dx,0x000f",\
  " shr eax,4",\
  " mov rm_DS,ax",\
  " mov ah,0x3f",\
  " mov rm_Int,0x21",\
  " int 0x19",\
 parm [edx][ecx][ebx] value [eax];

 // ---


int fclose(FILE fp);
 #pragma aux fclose=\
  " mov ah,0x3e",\
  " int 0x21",\
 parm [ebx] value [eax];

//
// ÄÄÄ Misc ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//

 // Denne m† kanskje fikses... Tror ikke vardiene distribueres jevnt utover

#define RAND_MAX 32767
extern unsigned short int RandSeed;
unsigned short int rand(void);

 // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 #endif
