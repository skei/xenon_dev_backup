DOSSEG
.MODEL MEDIUM
.386
.STACK
JUMPS
LOCALS

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
IPS EQU 50

ScreenX EQU 160
ScreenY EQU 100

RotSpeed1 EQU 1
AmpSpeed1 EQU 2
Amp1Add EQU 512
Amp1Shift EQU 1
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PICTURE1 SEGMENT		; 256*128 - Ekstra kant for ekstra bitplan;
label P1 byte			; ›.l. 32,36 - 32*40 = 1280+9 = 1289
 include pic1.pic		; Skriv ut 32 bytes + 8 marger
ENDS

BUFSEG SEGMENT
buffer label byte
 db 16000 dup(0)
ENDS

MUSIC Segment
 Include Song.inc
ENDS 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
.DATA
    
OldInt		dd	?
TimerCnt	dw	?
TimerSteps	dw	?

include font.inc                             
                                             
Text label byte                              
; Characters; A-Z,0-9, .,!/+-()*"Ûabcd   (*=ş, "=' abcd = circle)
;   1234567890123456789012345678901234567890                     
db '                                        '
db 'AXON / XENON DEVELOPMENT STRIKES AGAIN..'                    
db '                                        '                    
db '  YET ANOTHER SHORT INTRO, AGAIN WITH   '                    
db '     LOUSY CODE AND POOR DESIGN....     '          
db '                                        '     
db 'THIS IS MY KIND OF CONGRATULATION TO... '
db '                                        '     
db '  ÛÛ ÛÛ aÛÛÛb ÛÛÛÛb ÛÛÛÛb ÛÛÛÛÛ aÛÛÛb   '
db '  ÛÛ ÛÛ ÛÛ ÛÛ ÛÛ Ûd ÛÛ Ûd ÛÛ    ÛÛ      '
db '  ÛÛÛÛÛ ÛÛ ÛÛ ÛÛÛÛb ÛÛÛÛb ÛÛÛÛ  cÛÛÛb   '
db '  ÛÛ ÛÛ ÛÛ ÛÛ ÛÛ ÛÛ ÛÛ ÛÛ ÛÛ       ÛÛ   '
db '  ÛÛ ÛÛ cÛÛÛd ÛÛÛÛd ÛÛÛÛd ÛÛÛÛÛ cÛÛÛd   '
db '                                        '
db 'OF XENON DEVELOPMENT - WHO COMPLETES HIS'
db ' EIGHTEENTH YEAR THIS SATURDAY (THE 4TH '
db '           OF FEBRUARY 1994)            '
db '                                        '
DB 'CONGRATULATIONS.... HIPP, HIPP, HURRA !!'
db '                                        '
db '   BTW - HOPE YOU"LL PASS THE DRIVING   '
db '               TEST......               '
db '                                        '
db ' ************************************** '
db '                                        ' 
DB 254                                       
db '                                        '
db '    CREDITS FOR THIS LOUSY INTRO....    '
db '                                        '
db 'FONTS, ADLIB PLAYER AND MUSIC - RIPPED..'
db '              CODE - AXON               '
db '                                        '
db '  WELL, WELL.... NOW CALL ONE OF THESE  '
db '   TWO XENON DEVELOPMENT BOARDS......   '
db '                                        '
db 'BOGUS BOUTIQUE (WHQ) - (+47) 73 93 35 69'
db '             SYSOP - HOBBES             '
db '                                        '
db 'ELDRITCH (23.00-07.00) (+47) 72 58 20 79'
db '              SYSOP - AXON              '
db '                                        '
db '  WATCH OUT FOR OUR DEMO/INTRO - TO BE  '
db 'RELEASED AT MINI-TG"95 (17-19 FEB. 1995)'
db '               (PROBABLY)               '
db '                                        '
db ' C"YA....                               '
db '                    PRESS ESC TO END... '
db '                                        '
db '                                        '
db '                                        '
db '                                        '
db 254,253                                   
                                             
CharX dw 0                                   
CharY dw 0
Char dw 0
CharColor db 0
TextIndex dw 0
Sweep db 0      ; Set to 1 if clear screen...
SweepLine dw 0  ; Which line to 'sweep' next...
LineY1 dw 0

align 2
IncX dw 0
IncY dw 0
Y dw 0
PosX dw 0
PosY dw 0
PX dw 0
PY dw 0

align 2
Angle1 db 0
align 2
Amplify1 db 0
align 2
Angle2 db 0
align 2
Amplify2 db 0

align 2
Include SinCos.inc
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
.CODE
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SetPal MACRO Clr,r,g,b
 push ax
 push dx
 mov dx,3c8h
 mov al,Clr
 out dx,al
 inc dx
 mov al,r
 out dx,al
 mov al,g
 out dx,al
 mov al,b
 out dx,al
 pop dx
 pop ax
ENDM

SetPlane Macro BitP
 mov dx,3c4h
 mov ah,BitP
 mov al,2
 out dx,ax
ENDM
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC DrawPic
 push ds
 mov ax,0a000h
 mov es,ax
 mov di,(32*40)+4
 mov ax,seg P1
 mov ds,ax
 mov si,offset P1
 mov dx,128	; Y
PicWholeLoop:
 mov bx,32	; 256/8
PicHorizLoop: 
 xor cl,cl
 lodsb
 cmp al,0
 jz bit7
 add cl,128
bit7: 
 lodsb
 cmp al,0
 jz bit6
 add cl,64
bit6: 
 lodsb
 cmp al,0
 jz bit5
 add cl,32
bit5: 
 lodsb
 cmp al,0
 jz bit4
 add cl,16
bit4: 
 lodsb
 cmp al,0
 jz bit3
 add cl,8
bit3: 
 lodsb
 cmp al,0
 jz bit2
 add cl,4
bit2: 
 lodsb
 cmp al,0
 jz bit1
 add cl,2
bit1: 
 lodsb
 cmp al,0
 jz bit0
 add cl,1
bit0:
 mov [es:di],cl
 inc di
 dec bx
 jnz PicHorizLoop
 add di,8
 dec dx
 jnz PicWholeLoop
 pop ds
 ret
ENDP 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; HorizLine. Draws horizontal line
; Pixel plane must be set...
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC HorizLine
 push di
 mov ax,0a000h
 mov es,ax
 mov ax,[LineY1]
 imul ax,40*2
 mov di,ax
 mov eax,0
 rept 10*2
 stosd
 endm
 pop di
 ret
ENDP
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Draws character Char at CharX, CharY (multiplied by 8)
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC DrawChar
 mov ax,[CharY]
 imul ax,320
; add ax,320*50
 mov di,ax
 add di,[CharX]		; ES:DI -> Screen
 mov si,offset font
 mov ax,[Char]
 shl ax,3               ; *8
 add si,ax              ; DS:SI -> Font-data
 mov cx,8
CharLoop:
 movsb
 add di,39
 loop CharLoop
 ret
ENDP
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Entry CL = char. Exit = CX font char number.
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC ParseChar
 push ax
 cmp cl,' '
 jne a0
 mov ax,0
a0:
 cmp cl,'A'
 jne a1
 mov ax,1
a1:
 cmp cl,'B'
 jne a2
 mov ax,2
a2:
 cmp cl,'C'
 jne a3
 mov ax,3
a3:
 cmp cl,'D'
 jne a4
 mov ax,4
a4:
 cmp cl,'E'
 jne a5
 mov ax,5
a5:
 cmp cl,'F'
 jne a6
 mov ax,6
a6:
 cmp cl,'G'
 jne a7
 mov ax,7
a7:
 cmp cl,'H'
 jne a8
 mov ax,8
a8:
 cmp cl,'I'
 jne a9
 mov ax,9
a9:
 cmp cl,'J'
 jne a10
 mov ax,10
a10:
 cmp cl,'K'
 jne a11
 mov ax,11
a11:
 cmp cl,'L'
 jne a12
 mov ax,12
a12:
 cmp cl,'M'
 jne a13
 mov ax,13
a13:
 cmp cl,'N'
 jne a14
 mov ax,14
a14:
 cmp cl,'O'
 jne a15
 mov ax,15
a15:
 cmp cl,'P'
 jne a16
 mov ax,16
a16:
 cmp cl,'Q'
 jne a17
 mov ax,17
a17:
 cmp cl,'R'
 jne a18
 mov ax,18
a18:
 cmp cl,'S'
 jne a19
 mov ax,19
a19:
 cmp cl,'T'
 jne a20
 mov ax,20
a20:
 cmp cl,'U'
 jne a21
 mov ax,21
a21:
 cmp cl,'V'
 jne a22
 mov ax,22
a22:
 cmp cl,'W'
 jne a23
 mov ax,23
a23:
 cmp cl,'X'
 jne a24
 mov ax,24
a24:
 cmp cl,'Y'
 jne a25
 mov ax,25
a25:
 cmp cl,'Z'
 jne a26
 mov ax,26
a26:
 cmp cl,'1'
 jne a27
 mov ax,27
a27:
 cmp cl,'2'
 jne a28
 mov ax,28
a28:
 cmp cl,'3'
 jne a29
 mov ax,29
a29:
 cmp cl,'4'
 jne a30
 mov ax,30
a30:
 cmp cl,'5'
 jne a31
 mov ax,31
a31:
 cmp cl,'6'
 jne a32
 mov ax,32
a32:
 cmp cl,'7'
 jne a33
 mov ax,33
a33:
 cmp cl,'8'
 jne a34
 mov ax,34
a34:
 cmp cl,'9'
 jne a35
 mov ax,35
a35:
 cmp cl,'0'
 jne a36
 mov ax,36
a36:
 cmp cl,'.'
 jne a37
 mov ax,37
a37:
 cmp cl,','
 jne a38
 mov ax,38
a38:
 cmp cl,'!'
 jne a39
 mov ax,39
a39:
 cmp cl,'/'
 jne a40
 mov ax,40
a40:
 cmp cl,'+'
 jne a41
 mov ax,41
a41:
 cmp cl,'-'
 jne a42
 mov ax,42
a42:
 cmp cl,'('
 jne a43
 mov ax,43
a43:
 cmp cl,')'
 jne a44
 mov ax,44
a44:
 cmp cl,'*'
 jne a45
 mov ax,45
a45:
 cmp cl,'"'
 jne a46
 mov ax,46
a46:      
 cmp cl,'Û'
 jne a47   
 mov ax,47 
a47:     
 cmp cl,'a'
 jne a48  
 mov ax,48
a48:      
 cmp cl,'b'
 jne a49  
 mov ax,49
a49:      
 cmp cl,'c'
 jne a50  
 mov ax,50
a50:
 cmp cl,'d'
 jne a51
 mov ax,51
a51:       
 mov cx,ax
 pop ax  
 ret     
ENDP
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; HandleText
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC HandleText
 cmp [Sweep],1
 jne NoSweepInProgress
 mov ax,[SweepLine]
 mov [LineY1],ax
 Call HorizLine
 inc [SweepLine]
 cmp [SweepLine],200/2    ; Last line to sweep
 jb NotSweepFinished
 mov [Sweep],0
NotSweepFinished:
 Jmp EndHandleText
NoSweepInProgress:
 mov bx,[TextIndex]
 inc [TextIndex]
 mov cl,[Text+bx]
 cmp cl,254             ; 254 = Sweep-clear text
 jne NotSweep
 mov [Sweep],1
 mov [SweepLine],0     ; 1st line to sweep
 mov [CharX],0
 mov [CharY],0
 Jmp EndHandleText
NotSweep:
 cmp cl,253             ; 253 = Start over
 jne StartOver
 mov [TextIndex],0
 jmp NoSweepInProgress
StartOver:
 cmp cl,252             ; 252 = Set color = next byte
 jne RegularText
 mov bx,[TextIndex]
 inc [TextIndex]
 mov al,[Text+bx]
 mov [CharColor],al
 Jmp NoSweepInProgress
RegularText:
 call ParseChar
 mov [Char],cx
 Call DrawChar
 inc [CharX]
 cmp [CharX],40
 jb NotNextLine
 mov [CharX],0
 inc [CharY]
NotNextLine:
EndHandleText:
 ret          
ENDP          
                                                                                                                                                                                                         
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Proc Retrace  
 mov dx,3dah  
Waitretrace1:
 in al,dx     
 test al,8    
 jnz waitretrace1
WaitRetrace2:
 in al,dx
 test al,8
 jz WaitRetrace2
 ret
ENDP
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC Buffer2Ega
 push ds      
 mov ax,0a000h
 mov es,ax
 xor di,di
 mov ax,SEG Buffer
 mov ds,ax
 mov si,OFFSET Buffer
 mov cx,100
BufLoop:
 mov bx,320/16
HorizLoop:
 xor ax,ax
 cmp byte ptr [ds:si],0
 je Notbit7
 add al,128
notbit7:
 cmp byte ptr [ds:si+1],0
 je Notbit6
 add al,32
notbit6:
 cmp byte ptr [ds:si+2],0
 je Notbit5
 add al,8
notbit5:
 cmp byte ptr [ds:si+3],0
 je Notbit4
 add al,2
notbit4:
 cmp byte ptr [ds:si+4],0
 je Notbit3
 add ah,128
notbit3:
 cmp byte ptr [ds:si+5],0
 je Notbit2
 add ah,32
notbit2:
 cmp byte ptr [ds:si+6],0
 je Notbit1
 add ah,8
notbit1:
 cmp byte ptr [ds:si+7],0
 je Notbit0
 add ah,2
notbit0:
 stosw
 add si,8
 dec bx
 jnz HorizLoop
 add di,320/8
 dec cx
 jnz BufLoop
 pop ds
 ret
ENDP 

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; IncX, IncY - integers, defined
; FS:SI -> Picture
; ES = 0a000h
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC PutTexture
 xor bh,bh
 mov cl,[SinTable+bx]
 mov ch,[CosTable+bx]
 push dx
 mov al,cl
 cbw
 imul dx
 mov al,ah
 mov ah,dl
 mov bx,ax
 pop dx
 mov al,ch
 cbw
 imul dx
 mov al,ah
 mov ah,dl
 mov [IncX],ax
 mov [IncY],bx
 mov ax,seg buffer
 mov es,ax
 mov ax,ScreenX
 shr ax,1
 imul [IncX]
 mov [PosX],ax
 mov ax,ScreenY
 shr ax,1
 neg ax
 imul [IncY]
 mov [PosY],ax
 mov [Y],0      ; For y=0 to ScreenY-1 do
 mov di,offset buffer
 align 2
 PutTextureLoop:
  mov ax,[PosX]
  mov [PX],ax
  mov ax,[PosY]
  mov [PY],ax
  mov ax,seg p1
  mov fs,ax
  mov si,offset p1
  mov cx,screenx; shr 1
  cld
  mov ax,[incx]
  shl eax,16
  mov ax,[incy]
  mov esi,eax
  mov dx,[px]
  shl edx,16
  mov dx,[py]
align 2
@1:
  add edx,esi
  mov ebx,edx
  shr ebx,16
  mov bl,dh
  and bx,32767
  mov al,[fs:p1+bx]
  stosb
  dec cx
  jnz @1
  mov ax,[IncY]
  add [PosX],ax
  mov ax,[IncX]
  neg ax
  add [PosY],ax
  inc [Y]
  cmp [Y],ScreenY
  jb PutTextureLoop
 ret
ENDP

;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
; RAD player stuff
;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

;²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
; Enables and starts the player interrupt.
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC SetInt
 push   ax   
 push   es   
 cli       
 xor    ax,ax
 mov    es,ax
 mov    ax,es:[8*4]
 mov	word ptr OldInt,ax
 mov	ax,es:2[8*4]
 mov	word ptr OldInt+2,ax
 mov	word ptr es:[8*4], offset PlayerInt
 mov	es:2[8*4],cs
 mov	ax,IPS
 call	SetTimer
 sti
 pop	es
 pop    ax
 ret
ENDP 
    
;²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
; Disables the player interrupt.
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

PROC ResetInt
 push	ax
 push   es
 cli
 xor	ax,ax
 mov	es,ax
 mov	ax,word ptr OldInt
 mov	es:[8*4],ax
 mov	ax,word ptr OldInt+2
 mov	es:2[8*4],ax
 call	ResetTimer
 sti          
 pop 	es
 pop    ax
 ret    
ENDP    

;²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
; The player interrupt.  Called IPS times a second.
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC PlayerInt
 push	ax
 call	PlayMusic
;see if we have passed 18.2/s mark
lx:
 mov	ax,TimerSteps		; this no. of steps per int.
 add	TimerCnt,ax                                  
 jnc	ly			; don't call timer interrupt
 pop	ax                                           
 jmp 	OldInt		; call old interrupt handlers
;standard exit
ly:     
 mov	al,20h
 out	20h,al
 pop	ax
 iret   
ENDP
        
;²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
; Sets the interrupt timer duty cycle.
; IN:   
;	AX	- number of times per second for INT08.
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC SetTimer
 push	ax
 push   bx
 push   dx
 mov	bx,ax
 mov	ax,13532	; 1193180 mod 65536 (TASM craps out)
 mov	dx,18		; 1193180/65536 (TASM can't calculate this)
 div	bx
 mov	bx,ax
 mov	al,36h
 out	43h,al
 mov	al,bl
 out	40h,al
 mov	al,bh
 out	40h,al
 mov	TimerSteps,bx	; for keeping 18.2 timer correct
 mov	TimerCnt,0	; counter
 pop 	dx
 pop    bx
 pop    ax     
 ret    
ENDP    
        
;²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
; Resets the interrupt timer back to 18.2/sec duty cycle.
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC ResetTimer
 push	ax
 mov	al,36h
 out	43h,al
 xor	al,al 
 out	40h,al
 out	40h,al
 pop 	ax
ret 
ENDP
    
include		Player.Asm
    

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Entry From DOS
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
EntryPoint:
 mov ax,@data
 mov ds,ax
 assume cs:@code,ds:@data
 mov ax,0dh
 int 10h
 
 mov ax,Music
 mov es,ax
 call InitPlayer
 call   SetInt

;       C, r, g, b
 SetPal 0, 0, 0,16	; Background 
 SetPal 1, 0, 0,48	; RotoZoomer
 SetPal	2,31,31,31	; Writer
 SetPal 3,31,31,63	; Roto + Writer
 SetPal 4, 0, 0, 0	; BackGround Pic
 SetPal 5, 0, 0,47	; Backgound + rotozoom
 SetPal 6,23,23,23	; Background + write
 SetPal 7,23,23,47	; Background + roto + writer
 mov ax,0a000h
 mov es,ax
 xor di,di
 SetPlane 15
 xor eax,eax
 mov cx,64000/(8*4)
 rep stosd
 SetPlane 4
 Call DrawPic
 Mov [Angle1],192
 mov [Amplify1],0
MainLoop:
 Add [Angle1],RotSpeed1
 Add [Amplify1],AmpSpeed1
 mov bl,[Amplify1]
 xor bh,bh
 mov al,[SinTable+bx]
 cbw
 shl ax,Amp1Shift
 mov dx,ax
 add dx,Amp1Add
 mov bl,[Angle1]
 Call PutTexture
 Call Retrace
 SetPlane 1
 Call Buffer2Ega
 SetPlane 2
 Call HandleText
 in al,60h
 cmp al,1
 jne MainLoop
 
 call   ResetInt
 call   EndPlayer

 mov ax,3
 int 10h
 mov ax,4c00h
 int 21h
End EntryPoint
0,0,1
 DB 1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1
 DB 1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1
 DB 1,
