;ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป;
;บ A DPMI only protected mode kernel.                                        บ;
;บ Copyright (c) 1997 by Maxwell Sayles.  All rights reserved.               บ;
;ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ;

  LOCALS
  JUMPS
  .386p

extrn c0_entry

; A few macros to make type casting easier
q EQU qword ptr
d EQU dword ptr
w EQU word ptr
b EQU byte ptr
o EQU offset
m EQU small
l EQU large
s EQU short

; defines
STACK32SIZE       EQU (16384)
STACK16SIZE       EQU (96)        ; size of initialization stack
DPMI_DATASIZE     EQU (400h)      ; DPMI mode switch data area
GDTENTRIES        EQU (2)


;ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป;
;บ All code for initializing and entering Protected Mode                     บ;
;ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ;
Kernel    SEGMENT BYTE PUBLIC USE16
          ASSUME CS:Kernel, DS:Code32

;ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ;
;ณ ********** DOS Entrance ************************************************  ณ;
;ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู;
Entry16:

;*****************************************************************************;
  ; test for DPMI
  jmp RealEntry__
  db 'WATCOM'
 RealEntry__:
  mov ax,1687h
  int 2Fh
  test ax,ax
  jnz DPMI_ErrorExit

  ; test for 32-bit Protected Mode support (and 386)
  test bl,1
  jz DPMI_ErrorExit

  ; set DS to Code32 segment
  mov eax,Code32
  mov ds,ax

  ; get linear base address of Code32 segment
  shl eax,4
  mov m Code32addr,eax

  ; fix up Code32 and Data32 descriptors
  or m d GDT_Code32+2,eax
  or m d GDT_Data32+2,eax

  ; save DPMI mode-switch entry point
  mov m w DPMI_ModeSwitch  ,di
  mov m w DPMI_ModeSwitch+2,es

  ; load ES with DPMI mode switch data area
  ; address is relative to Stack so that the number of relocatables
  ; in .EXE header do not exceed two
  mov ax,ss
  add ax,STACK16SIZE shr 4  ; move to end of initialization stack
  mov es,ax

  ; Enter protected mode as a 32-bit program
  mov ax,1
  call m DPMI_ModeSwitch
  jc DPMI_ErrorExit


;*****************************************************************************;
  ; In 16-bit protected mode.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ; Allocate and set descriptors
  push ds
  pop es
  mov edi,o GDT
  mov si,m o Selectors
  mov dx,GDTENTRIES
  SelLoop:
    ; Allocate LDT descriptor
    xor ax,ax
    mov cx,1
    int 31h
    ; Save selector
    mov [si],ax
    inc si
    inc si
    ; Set descriptor
    mov bx,ax
    mov ax,000Ch
    int 31h
    add di,8
    dec dx
    jnz SelLoop


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ; Jump to USE32 segment
  push m d Code32Sel
  push l o Enter32
  db 66h, 0CBh


;ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ;
;ณ Error Exit.                                                               ณ;
;ณ Prints Error Message, and returns to DOS with error code 0FFh             ณ;
;ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู;
DPMI_ErrorExit:
  mov dx,o EndOfKernel+100h
  mov ah,9h
  int 21h
  mov ah,4Ch
  int 21h


EndOfKernel LABEL

Kernel    ENDS
;ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป;
;บ FMPI segment ends.                                                        บ;
;ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ;


;ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป;
;บ Contains code and data for 32-bit Protected Mode.                         บ;
;ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ;
Code32    SEGMENT BYTE PUBLIC USE32
          ASSUME CS:Code32, DS:Code32, SS:Code32

;ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ;
;ณ Error Messages                                                            ณ;
;ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู;
NoDPMI_MSG LABEL
db 'DPMI Error$'

;ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ;
;ณ Global Descriptor Table                                                   ณ;
;ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู;
GDT               LABEL     BYTE
GDT_Code32        db      0FFh, 0FFh,   0h,   0h,   0h, 0FAh, 0CFh,   0h
GDT_Data32        db      0FFh, 0FFh,   0h,   0h,   0h, 0F2h, 0CFh,   0h



;ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ;
;ณ Initialize system registers and variables then jump to Start32            ณ;
;ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู;
Enter32:
  mov ax,m Data32Sel
  mov ds,ax
  mov es,ax
  mov ss,ax
  mov esp,o Stack32+STACK32SIZE


;*****************************************************************************;
;ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ;
;ณ Your program begins here.                                                 ณ;
;ณ You have complete access to all DPMI functions.                           ณ;
;ณ You are running in a USE32 segment under a semi-flat memory model:        ณ;
;ณ   CS = DS = ES = SS = Linear address of Code32 Segment                    ณ;
;ณ You probably want to jump to a different object to keep things tidy.      ณ;
;ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู;

  call c0_entry

;  ; set 320x200x8bpp
;  mov ax,13h
;  int 10h
;
;  ; Draw Circle pattern
;  mov edi,000A0000h
;  sub edi,m Code32addr        ; pointer is relative to base of DS
;  xor esi,esi                 ; y coordinate
;  @@YLoop:
;    xor ebp,ebp               ; x coordinate
;    @@XLoop:
;      mov eax,ebp             ; (x-159)^2
;      sub eax,159
;      imul eax
;      mov ebx,eax
;
;      mov eax,esi             ; (y-99)^2
;      sub eax,99
;      imul eax
;
;      add eax,ebx             ; x+y
;
;      mov ecx,eax             ; calculate square root
;      xor ebx,ebx
;      xor eax,eax
;      inc ebx
;      jmp @@20
;      @@10:
;        inc ebx
;        inc eax
;        inc ebx
;        @@20:
;        sub ecx,ebx
;        jae @@10
;
;      stosb                   ; put distance
;
;      inc ebp
;      cmp bp,320
;      jb @@XLoop
;    inc esi
;    cmp si,200
;    jb @@YLoop
;
;  ; wait for keypress
;  xor eax,eax
;  int 16h
;
;  ; set 80x25 textmode
;  mov ax,03h
;  int 10h


;*****************************************************************************;

;ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ;
;ณ Return to Real Mode and terminate                                         ณ;
;ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู;
Exit32:
  mov ah,4Ch
  int 21h


;ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ;
;ณ Variables                                                                 ณ;
;ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู;
DPMI_ModeSwitch   dd      ?       ; DPMI mode switch routine entry address
Code32addr        dd      ?       ; Linear address of Code32 segment


;ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ;
;ณ Selector Values                                                           ณ;
;ณ (Must follow same order as descriptors)                                   ณ;
;ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู;
Selectors         LABEL   WORD
Code32Sel         dw      ?
Data32Sel         dw      ?


;ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ;
;ณ 32-bit stack                                                              ณ;
;ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู;
Stack32 db STACK32SIZE dup (?)


Code32    ENDS
;ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป;
;บ 32-bit segment ends.                                                      บ;
;ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ;


;ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป;
;บ Temporary entry stack.                                                    บ;
;บ                                                                           บ;
;บ Windows 95 requires a 16 byte stack.                                      บ;
;บ CWSDPMI requires a 32 byte stack.                                         บ;
;บ QDPMI requires a 96 byte stack.                                           บ;
;บ                                                                           บ;
;ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ;
Stack16   SEGMENT BYTE PUBLIC STACK 'STACK'
  db STACK16SIZE dup (?)
Stack16   ENDS
;ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป;
;บ Stack ends.                                                               บ;
;ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ;


  END     Entry16

