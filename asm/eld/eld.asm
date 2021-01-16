DOSSEG
.MODEL MEDIUM
.386
.STACK
JUMPS

;BufferSeg SEGMENT       ; Fylles i omgang 1
;Buffer db 64000 dup(0)
;ENDS

PICHEIGHT EQU 200
PICWIDTH EQU 40

SpeedX1 EQU 3
SpeedX2 EQU 2
InPicX1 EQU 3
InPicX2 EQU 2
SINDIV EQU 2
SinWidth EQU 50

GraphSeg SEGMENT
label picture byte
 include eldpic.inc
label palette byte
 include eldpal.inc
ENDS

.DATA

include font.inc

Text label byte
;   1234567890123456789012345678901234567890
db '                                        '
db ' XXXX XX   XXX  XXX  XX XXXX  XX  XX X  '
db ' XX   XX   XX X XX X XX  XX  XX X XX X  '
db ' XXX  XX   XX X XXX  XX  XX  XX   XXXX B'
db ' XX   XX   XX X XX X XX  XX  XX X XX X B'
db ' XXXX XXXX XXX  XX X XX  XX   XX  XX X S'
db '                                        '
db '      CODING/SCENE ORIENTED BOARD       '
db '     SYSOP - AXON/XENON DEVELOPMENT     '
db '     XENON DEVELOPMENT MEMBER BOARD     '
db '    +47 72 58 20 79    (23.00-07.00)    '
db '                  TURNING 24HR SOON.....'
DB 254
DB '                                        '
db 'CODING/SCENE ORIENTED BOARD !!! NO GIFS,'
db '  GAMES, MODS, WINDOWS OR OS/2 STUFF,   '
db '     NO ILLEGAL STUFF, NO CRAP.....     '
db 'BUT - TONS OF SOURCES, TEXT FILES, CODE '
db '      TIPS, UTILITIES, ETC, ETC...      '
db '                                        '
db 'THIS SHITTY BBS AD WAS CODED IN ONE DAY '
db '  ONLY.... NO OPTIMIZING, NO DESIGN...  '
db '                                        '
db '    OK, GIVE ELDRITCH BBS A CALL....    '
db '     +47 72 58 20 79  (23.00-07.00)     '
DB 254,253

CharX dw 0
CharY dw 0
Char dw 0
CharColor db 0
TextIndex dw 0
Sweep db 0      ; Set to 1 if clear screen...
SweepLine dw 0  ; Which line to 'sweep' next...

LineY1 dw 0
LineY2 dw 0
LineColor db 0

Sinus label word        ; 256 Words (0-255)
 include sinus.inc

SinX1Index db 0
SinY1Index db 0
SinX2Index db 0
SinY2Index db 0

.CODE

;------------------------------------------------------------
; HorizLine. Draws horizontal line
;------------------------------------------------------------
PROC HorizLine
 push di
; mov ax,0a000h
; mov es,ax
 mov ax,[LineY1]
 imul ax,320
 mov di,ax
 mov cx,320
 mov al,[LineColor]
 rep stosb
 pop di
 ret
ENDP

;-----------------------------------------------------------------------------
; SetPalette - set the VGA color palette.
;-----------------------------------------------------------------------------
PROC SetPalette
 mov cx,768
 mov ax,seg Palette
 mov ds,ax
 mov si,offset palette
 mov     dx,3C8h
 xor     al,al
 out     dx,al
 inc     dx
 rep     outsb
 ret
ENDP

;-----------------------------------------------------------------------------
; WaitVR - Waits the Next VGA Vertical Retrace Ending.
;-----------------------------------------------------------------------------
proc    WaitVR
        mov     dx,3DAh
WaitStartVR:
        in      al,dx
        test    al,8
        je      WaitStartVR
WaitEndVR:
        in      al,dx
        test    al,8
        jne     WaitEndVR
        ret
endp    WaitVR

;------------------------------------------------------------
; Kalkulerer skjerm, og bilde-pos. DS/ES:SI/DI. CX
;------------------------------------------------------------
PROC DoWobble
 mov cx,0
 add [sinX1Index],SpeedX1
 add [SinX2Index],SpeedX2
 xor bh,bh
 mov bl,[Sinx1Index]
 shl bx,1
 xor dh,dh
 mov dl,[Sinx2Index]
 shl dx,1
 xor si,si              ; Picture startpos
 xor di,di              ; Screen startpos
CalcLoop:
 push dx
 mov ax,320
 mul cx                 ; ax = Y current line   þ FLERN MUL þ
 mov di,ax
 pop dx
 mov ax,[sinus+bx]
 add bx,2*InPicX1
 and bx,01ffh
 shr ax,SINDIV          ; sinus / 8 = 0-31
 add di,ax              ; di = y (ax) + leftsinus
 mov bp,ax              ; bp = Left
 push bx
 mov bx,dx
 mov ax,[sinus+bx]
 add bx,2*InPicX2
 and bx,01ffh
 shr ax,SINDIV          ; Ax = right
 mov dx,bx
 pop bx
 push cx        ; Ypos
 push bx        ; Left
 push dx
 mov bx,bp
 mov cx,ax
 add cx,256
;------------------------------------------------------------
; call DrawOneLine
;
 push ds
 mov ax,seg picture
 mov ds,ax
 mov eax,PicWidth
 shl eax,16
 cdq
 sub cx,bx
 movsx ebx,cx
 idiv ebx       ; EAX = 16.16 # to add to pic-index     þ Array IDIV þ
 mov ebx,0
DrawLoop:
 rol ebx,16
 mov dl,[ds:si+bx]
; mov dh,dl
 rol ebx,16
 add ebx,eax
 mov [es:di],dl
 mov [es:di+320*160],dl
 inc di
 loop DrawLoop
 pop ds
;
;------------------------------------------------------------
 pop dx
 pop bx
 pop cx
 add si,PicWidth
 inc cx
 cmp cx,PicHeight
 jb CalcLoop
 ret
ENDP

;------------------------------------------------------------
; Draws character Char at CharX, CharY (multiplied by 8)
;------------------------------------------------------------
PROC DrawChar
 mov ax,[CharY]
 imul ax,320*8
 add ax,320*50
 mov di,ax
 mov ax,[CharX]
 imul ax,8
 add di,ax
 mov si,offset font
 mov ax,[Char]
 shl ax,3               ; *8
 add si,ax              ; DS:SI -> Font-data
 mov al,[CharColor]
 mov cx,8
CharLoop:
 mov bl,[ds:si]
 test bl,128
 jz d1
 mov [es:di],al
d1:
 test bl,64
 jz d2
 mov [es:di+1],al
d2:
 test bl,32
 jz d3
 mov [es:di+2],al
d3:
 test bl,16
 jz d4
 mov [es:di+3],al
d4:
 test bl,8
 jz d5
 mov [es:di+4],al
d5:
 test bl,4
 jz d6
 mov [es:di+5],al
d6:
 test bl,2
 jz d7
 mov [es:di+6],al
d7:
 test bl,1
 jz d8
 mov [es:di+7],al
d8:
 inc si
 add di,320
 loop CharLoop
 ret
ENDP

;------------------------------------------------------------
; Entry CL = char. Exit = CX font char number.
;------------------------------------------------------------

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
 cmp cl,'þ'
 jne a45
 mov ax,45
a45:
 mov cx,ax
 pop ax
 ret
ENDP

;------------------------------------------------------------
; HandleText
;------------------------------------------------------------
PROC HandleText
 cmp [Sweep],1
 jne NoSweepInProgress
 mov ax,[SweepLine]
 mov [LineY1],ax
 mov [LineColor],0      ; Sweep-color
 Call HorizLine
 inc [SweepLine]
 cmp [SweepLine],150    ; Last line to sweep
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
 mov [SweepLine],50     ; 1st line to sweep
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

;------------------------------------------------------------
; Entry point from DOS
;------------------------------------------------------------
EntryPoint:
 mov ax,13h
 int 10h
 cld
 Call SetPalette
 mov ax,@data
 mov ds,ax
 mov ax,0a000h
 mov es,ax
 assume cs:@code,ds:@data
 mov [sweep],0
 mov [CharColor],24
MainLoop:
 Call WaitVR
 Call DoWobble
 Call HandleText
 in al,60h
 cmp al,1
 jne MainLoop

 mov ax,3h
 int 10h
 mov ax,4c00h
 int 21h
End EntryPoint

