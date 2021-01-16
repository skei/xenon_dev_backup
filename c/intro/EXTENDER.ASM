 ; small DPMI- only DOS extender... Axon - 1998

 LOCALS
 .386p

 ;ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
 ;º Segment prototypes                                                      º
 ;ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

 Kernel          SEGMENT PARA PUBLIC USE16 'KERNEL'
 Kernel          ENDS

 Code32          SEGMENT PARA PUBLIC USE32 'FAR_DATA'
                 ASSUME CS:Code32, DS:Code32, SS:Code32
 Code32          ENDS

 CODE_TEXT segment dword public use32 'CODE'
 CODE_TEXT ENDS


 IFDEF FILEIO
  DiskBuffer      SEGMENT PARA PUBLIC USE16 'BSS'
  DiskBuffer      ENDS
 ENDIF

 IDTSeg          SEGMENT PARA PUBLIC USE16 'BSS'
 IDTSeg          ENDS

 StackSeg        SEGMENT PARA PUBLIC STACK 'STACK'
 StackSeg        ENDS


;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿;
;³ This is required to link properly with watcom object files                ³;
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ;

DATA_GROUP  GROUP   Code32,DATACONST,DATACONST2,DATA_DATA,DATA_BSS,CODE_TEXT

Code32      SEGMENT PARA PUBLIC USE32 'FAR_DATA'
Code32      ENDS

DATACONST   SEGMENT PARA PUBLIC USE32 'FAR_DATA'
DATACONST   ENDS

DATACONST2  SEGMENT PARA PUBLIC USE32 'FAR_DATA'
DATACONST2  ENDS

DATA_DATA   SEGMENT PARA PUBLIC USE32 'FAR_DATA'
DATA_DATA   ENDS

DATA_BSS    SEGMENT PARA PUBLIC USE32 'FAR_DATA'
DATA_BSS    ENDS

;
; --- EQUs ---
;

EXCEPTIONS      EQU             ; include exception handlers?

  ; A few macros to make type casting easier
 q EQU qword ptr
 d EQU dword ptr
 w EQU word ptr
 b EQU byte ptr
 o EQU offset
 m EQU small
 l EQU large
 s EQU short

STACK_SIZE            EQU (4000h)
 ; Number of DPMI selectors
NUMBER_DPMI_SELECTORS EQU (4)
 ; Real Mode Interrupt call vector
rm_INTCALL_VECTOR         EQU     (19h)
 ; PIC Vector Mappings
PIC1_VECTOR               EQU     (08h)
PIC2_VECTOR               EQU     (70h)

dExceptionStack STRUC
  excp_SS       dd ?
  excp_GS       dd ?
  excp_FS       dd ?
  excp_ES       dd ?
  excp_DS       dd ?
  excp_EDI      dd ?
  excp_ESI      dd ?
  excp_EBP      dd ?
  excp_ESP      dd ?
  excp_EBX      dd ?
  excp_EDX      dd ?
  excp_ECX      dd ?
  excp_EAX      dd ?
  excp_EIP      dd ?
  excp_CS       dd ?
  excp_EFLAGS   dd ?
  excp_Number   dd ?
 ENDS


;
; --- globals ---
;

GLOBAL C_Entry:NEAR
GLOBAL __CHP:NEAR


 ; Variables for use with mode switching
GLOBAL rm_Int       :BYTE
GLOBAL rm_DS        :WORD
GLOBAL rm_ES        :WORD

 ; Selector Values
GLOBAL Code32Sel    :WORD       ; Code Selector for Code32 SEGMENT
GLOBAL Data32Sel    :WORD       ; Data Selector for Code32 SEGMENT
GLOBAL Flat32Sel    :WORD       ; Data Selector with base of 00000000h

 ; Linear addresses
GLOBAL Code32Addr   :DWORD      ; linear address of Code32 SEGMENT
GLOBAL PSPAddr      :DWORD      ; linear address of PSP SEGMENT
GLOBAL EnvAddr      :DWORD      ; linear address of Environment SEGMENT

 ; Global Code32 procedures
GLOBAL Start32      :NEAR       ; application entry point
GLOBAL Exit32       :NEAR       ; routine to terminate application
GLOBAL ErrorExit32  :NEAR       ; routine to terminate with error message

GLOBAL HexTAB       :BYTE       ; db '0123456789ABCDEF'
GLOBAL Ret_Proc     :NEAR       ; a procedure that points to a ret instruction
GLOBAL pm_EntryType :BYTE       ; protected mode entry type
                                ;   pm_ENTRY_RAWXMS or pm_ENTRY_DPMI

GLOBAL PSPSeg       :WORD       ; PSP Segment



;
; ±±± Code32 Segment ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;

 ; Contains code and data for 32-bit Protected Mode.
 Code32 SEGMENT
        ASSUME CS:Code32, DS:Code32, SS:Code32

;
; --- data ---
;

ExceptionMSG LABEL
 db 'Exception '
ExceptionNumber db 0
 db 'h has occured.$'

No32ProcMSG db '32-bit processor not detected.',0Ah,24h
NoDPMImsg   db 'No DPMI server present. Install a DPMI host (CWSDPMI,Win95, etc).',0Ah,24h
HexTAB      db '0123456789ABCDEF'

rm_Int      db 0,0          ; Real Mode interrupt to execute

rm_SegRegs  LABEL DWORD     ; Real Mode segment registers
rm_ES       dw 0
rm_DS       dw 0

 ; DPMI Mode switch routine (real mode)
DPMI_ModeSwitch   dd      0
; Stack switching registers
rm_SS       dw 0, 0         ; Real mode SS
v_ESP       dd 0     ; virtual ESP

 ;pm_EntryType db pm_ENTRY_RAWXMS   ; Protected mode entry type
KernelSeg    dw Kernel            ; Segment of kernel
PSPSeg       dw 0                 ; PSP Segment

Code32Addr   dd 0
PSPAddr      dd 0
EnvAddr      dd 0

Selectors LABEL WORD
Code32Sel    dw 8h
Data32Sel    dw 10h
Flat32Sel    dw 18h

GDT LABEL BYTE
GDT_NULL    db 8 DUP (0)
GDT_Code32  db 0FFh, 0FFh,   0h,   0h,   0h,  9Ah, 0CFh,   0h
GDT_Data32  db 0FFh, 0FFh,   0h,   0h,   0h,  92h, 0CFh,   0h
GDT_Flat32  db 0FFh, 0FFh,   0h,   0h,   0h,  92h, 0CFh,   0h
GDT_Kernel  db 0FFh, 0FFh,   0h,   0h,   0h,  9Ah,   0h,   0h
GDT_Stack16 db 0FFh, 0FFh,   0h,   0h,   0h,  92h,   0h,   0h

 ; Reference to the Global Descriptor Table
pm_GDT LABEL FWORD
           dw $-GDT
pm_GDTaddr dd o GDT

 ; Interrupt Descriptor Table
 ; Reference to Interrupt Descriptor Table upon entry
r0_IDT            LABEL   FWORD
r0_IDTsize        dw      256*4h
r0_IDTaddr        dd      0
 ; Reference to Real Mode Interrupt Descriptor Table
rm_IDT            LABEL   FWORD
                  dw      256*4h
rm_IDTaddr        dd      o rmIDT
 ; Reference to Protected Mode Interrupt Descriptor Table
pm_IDT            LABEL   FWORD
                  dw      256*8h
pm_IDTaddr        dd      o pmIDT

;
; --- code ---
;

 ;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 ;³ Initialize system registers and variables then jump to Start32          ³
 ;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
Enter32:
 mov ax,m CS:Data32Sel
 mov ds,ax
 mov es,ax
 mov fs,ax
 mov ss,ax
 mov esp,m v_ESP
 mov gs,m Flat32Sel
 sti
 ;call Init_Heap                ; Initialize heap manager
 IFDEF EXCEPTIONS
	call Init_Exception_Handling	; Initialize exception handler
 ENDIF
 jmp Start32

 ;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 ;³ Return to Real Mode and terminate                                      ³
 ;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
Exit32:
 mov ah,4Ch
 int 21h

 ;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 ;³ Error Exit                                                             ³
 ;³ Prints error message contained in EDX and terminates                   ³
 ;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
ErrorExit32:
 mov ax,0003h
 int 10h
 mov ax,1112h
 xor bl,bl
 int 10h
 call PrintString
 mov ah,4Ch
 int 21h

 ;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 ;³ Print '$' terminating string                                           ³
 ;³ Entry: DS:EDX - Address of string                                      ³
 ;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
PrintString:
 pushad
 mov esi,edx
 mov ah,2
 lodsb
 @@10:
 mov dl,al
 int 21h
 lodsb
 cmp al,'$'
 jnz @@10
 popad
 ret

 ; DPMI Real Mode call back
DPMI_rm_IntCall:
  mov v_ESP,esp                 ; save ESP
  sub esp,6*2                   ; make room for SS, SP, CS, IP, GS, FS
  push m rm_SegRegs             ; store DS, ES
  pushfw                        ; store Flags
  pushad                        ; store EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI
  ; pointer to real mode call back structure
  mov edi,esp
  ; Calculate real mode stack address
  mov eax,m d rm_SS
  mov [edi+48],ax               ; store SS
  shl eax,4
  sub eax,esp
  neg eax
  add eax,m Code32Addr
  mov [edi+46],ax               ; store SP
  ; Simulate real mode interrupt
  mov ax,0300h
  mov bx,m w rm_Int
  xor ecx,ecx
  int 31h
  popad                         ; restore EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI
  popfw                         ; restore Flags
  pop m rm_SegRegs              ; restore DS, ES
  mov esp,v_ESP                 ; restore ESP
  retf 4

 ; Get protected mode interrupt descriptor
 ; Entry:  BL - Interrupt vector
 ; Exit :  AX:EDX - Selector:offset of interrupt vector
DPMI_Get_pm_Int:
  push ecx
  mov ax,0204h
  int 31h
  mov eax,ecx
  pop ecx
  ret

 ; Set protected mode interrupt descriptor
 ; Entry:  BL - Interrupt Vector
 ;         AX:EDX - Selector:offset of interrupt vector
DPMI_Set_pm_Int:
  push ecx
  mov ecx,eax
  mov ax,0205h
  int 31h
  pop ecx
  ret

Exception_Handler:
  mov ds,m CS:Data32Sel
  mov ebx,[esp.excp_Number]
  mov al,HexTAB+ebx
  mov ExceptionNumber,al
  mov edx,o ExceptionMSG
  call PrintString
  jmp Exit32

Init_Exception_Handling:
  ; DPMI exception handling
 @@DPMI:
  xor bl,bl
  mov esi,o DPMI_Handler_Offsets
 @@DPMI_10:
  mov cx,m Code32Sel
  mov edx,[esi]
  mov ax,0203h
  int 31h
  inc bl
  add esi,4
  cmp bl,10h
  jb @@DPMI_10
  ret

 ; Exception entry handlers for DPMI exceptions
 ; Creates routines labeled DPMI_Exception_0 through DPMI_Exception_F
 ; Creates a table of offsets for the above routines
 ; [SS:ESP] - Exception Number
All_DPMI_Exceptions:
  push d [esp+14h+ 4]     ; 14h = offset of EFLAGS, 4  = Exception_Number
  push d [esp+10h+ 8]     ; 10h = offset of CS    , 8  = 4+EFLAGS
  push d [esp+0Ch+12]     ; 0Ch = offset of EIP   , 12 = 8+CS
  pushad
  push ds es fs gs ss
  jmp Exception_Handler

@Create_DPMI_Exception MACRO N
 DPMI_Exception_&N&:
  push d &N&
  jmp All_DPMI_Exceptions
 ENDM
 I=0
 REPT 10h
  @Create_DPMI_Exception %I
  I=I+1
 ENDM

 @DPMI_Exception_Offset MACRO N
  dd o DPMI_Exception_&N&
 ENDM

 DPMI_Handler_Offsets LABEL DWORD
 I=0
 REPT 10h
  @DPMI_Exception_Offset %I
  I=I+1
 ENDM

Start32:
  ; jump to Watcom C/C++ entry point
  call C_Entry
  ; return to operating system
  jmp Exit32

__CHP:
  frndint
_fltused_:
__8087:
__init_387_emulator:
  ret

 ;__CHP:
 ;  ret


 ; ---

 Code32 ENDS

;
; ±±± Stack segment ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;

 StackSeg  SEGMENT
  db STACK_SIZE dup (?)
 StackSeg  ENDS

;
; ±±± IDT segment ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;

 IDTSeg SEGMENT
  pmIDT  db 256*8    dup (?)
  pmISR  db 256*10   dup (?)
  rmIDT  db 256*4    dup (?)
  rmISR  db 256*8    dup (?)
 IDTSeg    ENDS

;
; ±±± Kernel Segment ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;

 Kernel SEGMENT
        ASSUME CS:Kernel, DS:Code32

;
; --- data ---
;

DPMI_GenErrorMSG db 'DPMI initialization error.',0Ah,24h


;
; --- code ---
;

; ²²² MAIN ²²²

Entry16:

  cli                       ; Disable interrupts
  cld                       ; Clear direction flag
  ; Set DS to Code32 segment
  mov ax,Code32
  mov ds,ax
  ; Save PSP segment
  mov m PSPSeg,es
  ; Resize PSP
  mov bx,StackSeg + ((STACK_SIZE+15)/16) + 1
  sub bx,m PSPSeg
  mov ah,4Ah
  int 21h

Check_Processor:

  pushf
	xor ah,ah
	push ax
	popf
	pushf
	pop ax
	and ah,0F0h
	cmp ah,0F0h
	jz Not386
	mov ah,0F0h
	push ax
	popf
	pushf
	pop ax
	and ah,0F0h
	jnz Is386
	; 32-bit Processor Not Found
 Not386:
  mov dx,m o No32ProcMSG
	jmp ErrorExit
 Is386:
	popf

Fixup_GDT:

  ; fix up Kernel descriptor
	mov eax,Kernel
	shl eax,4
  or  m d GDT_Kernel+2,eax
	; fix up Code32 and Data32 descriptors
	mov eax,Code32
	shl eax,4
  mov m Code32Addr,eax          ; save linear address of Code32 segment
  or  m d GDT_Code32+2,eax
  or  m d GDT_Data32+2,eax
	; make the offset of the GDT linear
  add m pm_GDTaddr,eax
	; get linear base address of PSP segment
  movzx eax,m PSPSeg
	shl eax,4
  mov m PSPAddr,eax
  ; get linear base address of Environment segment
  movzx eax,w ES:[2Ch]
  shl eax,4
  mov m EnvAddr,eax
	; calculate ESP relative to Code32
	xor eax,eax
	mov ax,ss
  mov m rm_SS,ax                ; save Real Mode SS
	shl eax,4
	add eax,esp
  sub eax,m Code32Addr
  mov m v_ESP,eax

  ; Determine protected mode entry type
GetSystem:

  ; Test for DPMI
  mov ax,1687h
  int 2Fh
  test ax,ax
  jnz NoDPMI
  jmp DPMI_System       ; DPMI System
 NoDPMI:
  mov dx,m o NoDPMImsg
  jmp ErrorExit

 ; Error Exit.
 ; Prints Error Message, and returns to DOS with error code 0FFh
ErrorExit:
	mov ah,9h
	int 21h
	mov ax,4CFFh
	int 21h

 ; ---

 ; Generic DPMI Error Exit
DPMI_ErrorExit:
  mov dx,m o DPMI_GenErrorMSG
  jmp ErrorExit

 ; DPMI system initialization
DPMI_System:
  ;mov m pm_EntryType,pm_ENTRY_DPMI      ; DPMI entry
  ; Save DPMI mode-switch entry point
  mov m w DPMI_ModeSwitch  ,di
  mov m w DPMI_ModeSwitch+2,es
  ; Test for 32-bit Protected Mode support
  and bx,1
  jz DPMI_ErrorExit
  ; Allocate memory for use by DPMI if necessary
  test si,si
  jz EnterDPMI_ProtMode
  mov bx, si
  mov ah, 48h
  int 21h
  jc DPMI_ErrorExit
  mov es, ax
  ; Enter protected mode as a 32-bit program
  EnterDPMI_ProtMode:
  mov ax,1
  call m DPMI_ModeSwitch
  jc DPMI_ErrorExit
 ; In 16-bit protected mode.
  ; Allocate and set descriptors
  push ds
  pop es
  mov edi,o GDT + 8
  mov esi,o Selectors
  mov dx,NUMBER_DPMI_SELECTORS
 SelLoop:
  ; Allocate LDT descriptor
  xor ax,ax
  mov cx,1
  int 31h
  ; Save selector
  mov [esi],ax
  add esi,2
  ; Set descriptor
  mov bx,ax
  mov ax,000Ch
  or b [edi+5],01100000b    ; set CPL 3
  int 31h
  add edi,8
  dec dx
  jnz SelLoop

  mov ax,m Data32Sel
  mov ds,ax
  mov es,ax

  ; Hook Real Mode Interrupt Call
  mov ax,205h
  mov bl,rm_INTCALL_VECTOR
  mov cx,m Code32Sel
  mov edx,l o DPMI_rm_IntCall
  int 31h

  ; update pointers to system functions
  ;mov GetPMInt,o DPMI_Get_pm_Int
  ;mov SetPMInt,o DPMI_Set_pm_Int

  ; Jump to USE32 segment
  push m d Code32Sel
  push l o Enter32
  db 66h, 0CBh

 ; ---

 Kernel    ENDS

 ; ---

 END Entry16

